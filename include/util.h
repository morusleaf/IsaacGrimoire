#pragma once

#include <iostream>

/* #define DEBUG */
//#define DEBUGUI

/* #ifdef DEBUG */
#include <FL/fl_ask.H>
#define ALERT(str) fl_alert(str)
/* #endif */

#ifdef _WIN32

#define ErrorLog(format, ...) fprintf(stderr, \
        "[Error]" format "\n", __VA_ARGS__)
#define WarningLog(format, ...) fprintf(stderr, \
        "[Warning]" format "\n", __VA_ARGS__)
#ifdef  DEBUG
#define Log(format, ...) fprintf(stdout, \
        "[Log]" format "\n", __VA_ARGS__)
#else
#define Log(format, ...)
#endif
#ifdef  DEBUGUI
#define UILog(format, ...) fprintf(stdout, \
        "[UILog]" format "\n", __VA_ARGS__)
#else
#define UILog(format, ...)
#endif

#else

#define ErrorLog(format, args...) fprintf(stderr, \
        "[Error]" format "\n", ##args)
#define WarningLog(format, args...) fprintf(stderr, \
        "[Warning]" format "\n", ##args)
#ifdef  DEBUG
#define Log(format, args...) fprintf(stdout, \
        "[Log]" format "\n", ##args)
#else
#define Log(format, args...)
#endif
#ifdef  DEBUGUI
#define UILog(format, args...) fprintf(stdout, \
        "[UILog]" format "\n", ##args)
#else
#define UILog(format, args...)
#endif

#endif

#define TOP ""

#define FREE(pointer) \
    if (pointer == NULL) \
        ErrorLog("attempt to free empty pointer"); \
    else { \
        free(pointer); \
        pointer = 0; \
    }

#ifdef _WIN32
#define snprintf sprintf_s
#endif
