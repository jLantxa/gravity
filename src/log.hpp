#include <stdio.h>

// Log definitions
#ifndef LOG_LEVEL
#   define LOG_LEVEL 1
#endif

#define LOGI
#define LOGE
#define LOGD
#define LOGV
#define LOGVV

#if LOG_LEVEL >= 4
#   define LOGVV printf("[VV] "); \
    printf
#endif

#if LOG_LEVEL >= 3
#   define LOGV printf("[V] "); \
    printf
#endif

#if LOG_LEVEL >= 2
#   define LOGD printf("[D] "); \
    printf
#endif

#if LOG_LEVEL >= 1
#   define LOGI printf("[I] "); \
    printf
#endif

#if LOG_LEVEL >= 0
#   define LOGE printf("[E] "); \
    printf
#endif
