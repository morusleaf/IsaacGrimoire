#pragma once

#include <string>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/sysctl.h>
#include <mach/message.h>
#endif

#ifdef _WIN32
#define mem_address_t int long
#define data_t uint32_t
#define mem_size_t SIZE_T
#define ret_t BOOL
#define KERN_SUCCESS true
#else
#define mem_address_t mach_vm_address_t
#define data_t uint32_t
#define mem_size_t mach_msg_type_number_t
#define ret_t kern_return_t
#endif

#ifdef _WIN32
#include "MemoryWin64.h"
#else
#include "MemoryMach.h"
#endif
