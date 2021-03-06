﻿#include "Core/Memory.h"
#include <assert.h>
#include <errno.h>
#include "Core/exception/get_process_error.h"
#include "Core/exception/get_task_error.h"
#include "Core/exception/read_memory_error.h"
#include "Core/util.h"

#ifdef _WIN32
#else
#include <mach/vm_map.h>
#include <mach/mach_vm.h>
#include <mach/task.h>
#include <mach-o/dyld_images.h>
#endif

Memory::Memory() {
	connected = false;
}

Memory::~Memory() {}

uint64_t Memory::readu64(mem_address_t address) {
    uint8_t buff[8];
	read(address, 8, buff, NULL);
	uint64_t ans = *((uint64_t*)buff);
	return ans;
}

uint32_t Memory::readu32(mem_address_t address) {
    uint8_t buff[4];
	read(address, 4, buff, NULL);
	uint32_t ans = *((uint32_t*)buff);
	return ans;
}

uint16_t Memory::readu16(mem_address_t address) {
    uint8_t buff[2];
	read(address, 2, buff, NULL);
	uint16_t ans = *((uint16_t*)buff);
	return ans;
}

uint8_t Memory::readu8(mem_address_t address) {
    uint8_t buff[1];
	read(address, 1, buff, NULL);
	uint8_t  ans = *((uint8_t*)buff);
	return ans;
}
