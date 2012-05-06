#ifndef JNI_LOG_H
#define JNI_LOG_H

#include <android/log.h>

#define LOG_TAG "jamruby"

#ifdef NO_DEBUG_LOG
#define LOGD(...)
#else
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, ##__VA_ARGS__)
#endif
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, ##__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, ##__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, ##__VA_ARGS__)

#endif // end of JNI_LOG

