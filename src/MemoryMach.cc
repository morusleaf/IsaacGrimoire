#include "Memory.h"
#include <mach/vm_map.h>
#include <mach/mach_vm.h>
#include <mach/task.h>
#include <mach-o/dyld_images.h>
#include <assert.h>
#include <errno.h>
#include "exception/get_process_error.h"
#include "exception/get_task_error.h"
#include "exception/read_memory_error.h"
#include "util.h"

typedef struct kinfo_proc kinfo_proc;

// Returns a list of all BSD processes on the system.  This routine
// allocates the list and puts it in *procList and a count of the
// number of entries in *procCount.  You are responsible for freeing
// this list (use "free" from System framework).
// On success, the function returns 0.
// On error, the function returns a BSD errno value.
// reference: https://developer.apple.com/legacy/library/qa/qa2001/qa1123.html
int Memory::getProcessList(kinfo_proc **procList, size_t *procCount) {
    int err;
    kinfo_proc *result;
    bool done;
    static const int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    size_t length;

    assert( procList != NULL);
    /* assert(*procList == NULL); */
    assert(procCount != NULL);

    *procCount = 0;

    result = NULL;
    done = false;
    do {
        assert(result == NULL);
        length = 0;
        err = sysctl( (int *)name, (sizeof(name)/sizeof(*name))-1,
                NULL, &length, NULL, 0);
        if (err == -1)
            err = errno;

        if (err == 0) {
            result = (kinfo_proc*)malloc(length);
            if (result == NULL)
                err = ENOMEM;
        }

        if (err == 0) {
            err = sysctl( (int *)name, (sizeof(name)/sizeof(*name))-1,
                    result, &length, NULL, 0);
            if (err == -1)
                err = errno;
            if (err == 0)
                done = true;
            else if (err == ENOMEM) {
                FREE(result);
                err = 0;
            }
        }
    } while (err == 0 && ! done);
    if (err != 0 && result != NULL) {
        FREE(result);
        result = NULL;
    }
    *procList = result;
    if (err == 0)
        *procCount = length / sizeof(kinfo_proc);

    assert( (err == 0) == (*procList != NULL) );

    return err;
}

pid_t Memory::searchPidByName(const char* name) {
    pid_t pid = -1;
    kinfo_proc *procList;
    size_t procCount;

    int err = getProcessList(&procList, &procCount);
    if (err != 0) throw get_process_error("can not get process list");

    bool found = false;
    for (int i = 0; i < procCount+1; i++)
        if (strcmp(procList[i].kp_proc.p_comm, name) == 0) {
            pid = procList[i].kp_proc.p_pid;
            found = true;
            // once found a matched name, cease the search
            break;
        }

    FREE(procList);
    
    if (found)
        return pid;
    else
        throw get_process_error("can not find process");
}

mach_port_t Memory::getTask(pid_t pid) {
    mach_port_t task;
    kern_return_t kret = task_for_pid(current_task(), pid, &task);
    if(kret != KERN_SUCCESS) throw get_task_error();
    return task;
}

mem_address_t Memory::getBaseAddress(mach_port_t task) {
    kern_return_t ret;
    mach_vm_address_t addr = 1;
    mach_vm_size_t size;
    vm_region_basic_info_data_t info;
    mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT_64;
    mach_port_t object_name;

    /* task_suspend(task); */
    ret = mach_vm_region(task, &addr, &size, VM_REGION_BASIC_INFO,
            (vm_region_info_t) &info, &count, &object_name);
    if (ret != KERN_SUCCESS) throw read_memory_error();
    /* task_resume(task); */
    return addr + size;
}

bool Memory::connect(std::string name) {
    if (connected) {
        WarningLog("Already connected");
        return true;
    }
    pid_t pid = searchPidByName(name.c_str());
    gtask = getTask(pid);
    connected = true;
    /* Log("connected to process pid=%d (%s)", pid, name.c_str()); */
    return true;
}

bool Memory::disconnect() {
    if (!connected) {
        WarningLog("already disconnected");
        return true;
    }
    kern_return_t kret = mach_port_deallocate(current_task(), gtask);
    if(kret != KERN_SUCCESS)
        {
            ErrorLog("failed to deallocate port");
            return false;
        }
    connected = false;
    /* Log("disconnected"); */
    return true;
}

mem_address_t Memory::getBase() {
    if (!connected)
        throw read_memory_error();
    return getBaseAddress(gtask);
}

void Memory::read(mem_address_t addr, 
        size_t size, uint8_t* &buffer, size_t *realsize) {
    vm_offset_t readMem;
    mach_msg_type_number_t numBytesRead;

    kern_return_t kr = vm_read(gtask,        // vm_map_t target_task,
                 addr,     // mach_vm_address_t address,
                 size,     // mach_vm_size_t size
                 &readMem,     //vm_offset_t *data,
                 &numBytesRead); // mach_msg_type_number_t *dataCnt

    if (kr != KERN_SUCCESS)
        throw read_memory_error();

    memcpy(buffer, (uint8_t*)readMem, numBytesRead);
    
    if (realsize != NULL)
        *realsize = size;
}

mem_address_t Memory::readAddr(mem_address_t address) {
    return (mem_address_t)readu64(address);
}

data_t Memory::readData(mem_address_t address) {
    return (data_t)readu32(address);
}
