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
#   define LOGVV printf("VV\t"); \
    printf
#endif

#if LOG_LEVEL >= 3
#   define LOGV printf("V\t"); \
    printf
#endif

#if LOG_LEVEL >= 2
#   define LOGD printf("D\t"); \
    printf
#endif

#if LOG_LEVEL >= 1
#   define LOGE printf("E\t"); \
    printf
#   define LOGI printf("I\t"); \
    printf
#endif
