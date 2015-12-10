#pragma once

class Memory {
private:
    bool connected;
    mach_port_t gtask;
    static int getProcessList(kinfo_proc **procList, size_t *procCount);
    static pid_t searchPidByName(const char* name);
    static mach_port_t getTask(pid_t pid);
    mach_vm_address_t getBaseAddress(mach_port_t task);
public:
    bool connect(std::string processName);
    bool disconnect();
    mach_vm_address_t getBase();
    void read(mem_address_t addr, size_t size, uint8_t* &buffer, size_t *realsize);
    mem_address_t readAddr(mem_address_t address);
    data_t readData(mem_address_t address);
    uint64_t readu64(mem_address_t address);
    uint32_t readu32(mem_address_t address);
    uint16_t readu16(mem_address_t address);
    uint8_t  readu8 (mem_address_t address);
    Memory();
    ~Memory();
};
