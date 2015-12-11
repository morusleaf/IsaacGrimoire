#pragma once

class Memory {
private:
	bool connected;
	DWORD getProcessIdByName(std::string name);
	HANDLE gtask;
	HANDLE getTask(DWORD pid);
public:
	bool connect(std::string processName);
	bool disconnect();
	mem_address_t getBase();
	void read(mem_address_t addr, mem_size_t size, uint8_t* buffer, mem_size_t *realsize);
	mem_address_t readAddr(mem_address_t address);
    data_t readData(mem_address_t address);
	uint64_t readu64(mem_address_t address);
	uint32_t readu32(mem_address_t address);
	uint16_t readu16(mem_address_t address);
	uint8_t  readu8 (mem_address_t address);
	Memory();
	~Memory();
};
