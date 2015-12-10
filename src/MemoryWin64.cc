#include "Memory.h"
#include <errno.h>
#include "exception/get_process_error.h"
#include "exception/get_task_error.h"
#include "exception/read_memory_error.h"
#include "util.h"

DWORD Memory::getProcessIdByName(std::string name) {
	HWND handle = FindWindow(NULL, (LPCTSTR)name.c_str());
	if (handle == NULL) throw get_process_error("can not get windows handle");
	DWORD PROC_ID;
	GetWindowThreadProcessId(handle, &PROC_ID);
	return PROC_ID;
}

HANDLE Memory::getTask(DWORD pid) {
	HANDLE task = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (task == NULL) throw get_task_error();
	return task;
}

bool Memory::connect(std::string name) {
	if (connected) {
		WarningLog("Already connected");
		return true;
	}

	DWORD pid = getProcessIdByName(name);
	gtask = getTask(pid);

	connected = true;
	Log("connected to process pid=%d (%s)", pid, name.c_str());
	return true;
}

bool Memory::disconnect() {
	if (!connected) {
		WarningLog("already disconnected");
		return true;
	}
	BOOL ret = CloseHandle(gtask);
	if (ret == false) {
		// TODO: throw exception
	}
	connected = false;
	Log("disconnected");
	return true;
}

mem_address_t Memory::getBase() {
	if (!connected) {
		ErrorLog("has not connected");
		return 0;
	}
	HMODULE hMods[1024];
	DWORD cbNeeded;
	BOOL ret = EnumProcessModules(gtask, hMods, sizeof(hMods), &cbNeeded);
	if (ret == false) throw read_memory_error();
	//Log("got base address: %llX", hMods[0]);
	return (mem_address_t)(hMods[0]);
}

void Memory::read(mem_address_t addr, size_t size, uint8_t* buffer, size_t *realsize)
{
	SIZE_T numBytesRead;
	BOOL ret = ReadProcessMemory(gtask, (void *)addr, buffer, size, &numBytesRead);
	if (ret == false) {
		throw read_memory_error();
	}
	/*if (numBytesRead == size)
		Log("# %X %d", addr, size);
	else
		Log("@ %X %d %d", addr, size, numBytesRead);*/
	if (realsize != NULL)
		*realsize = numBytesRead;
}

mem_address_t Memory::readAddr(mem_address_t address) {
	return (mem_address_t)readu64(address);
}
