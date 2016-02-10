#include <tchar.h>
#include "Memory.h"
#include <errno.h>
#include "exception/get_process_error.h"
#include "exception/get_task_error.h"
#include "exception/read_memory_error.h"
#include "util.h"

// reference: https://technet.microsoft.com/en-us/windows/ms682623
DWORD Memory::getProcessIdByName(std::string name) {
	wchar_t *wname;
	{
		const size_t newsizew = strlen(name.c_str()) + 1;
		wname = new wchar_t[newsizew];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wname, newsizew, name.c_str(), _TRUNCATE);
	}
	DWORD aProcesses[1024], cbNeeded;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		throw get_process_error("can not use EnumProcesses");
	}
	DWORD cProcesses = cbNeeded / sizeof(DWORD);
	for (unsigned int i = 0; i < cProcesses; i++) {
		if (aProcesses[i] != 0) {
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
				PROCESS_VM_READ,
				FALSE, aProcesses[i]);
			if (NULL != hProcess) {
				HMODULE hMod;
				DWORD cbNeeded;
				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
					GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
				}
				if (_tcscmp(szProcessName, wname) == 0) {
					CloseHandle(hProcess);
					return aProcesses[i];
				}
			}
			CloseHandle(hProcess);
		}
	}
	throw get_process_error("can not find isaac-ng.exe");
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
	return (mem_address_t)(hMods[0]);
}

void Memory::read(mem_address_t addr, mem_size_t size, uint8_t* buffer, mem_size_t *realsize)
{
	SIZE_T numBytesRead;
	BOOL ret = ReadProcessMemory(gtask, (void *)addr, buffer, size, &numBytesRead);
	if (ret == false) {
		throw read_memory_error();
	}
	if (realsize != NULL)
		*realsize = numBytesRead;
}

mem_address_t Memory::readAddr(mem_address_t address) {
	return (mem_address_t)readu64(address);
}

data_t Memory::readData(mem_address_t address) {
    return (data_t)readu32(address);
}
