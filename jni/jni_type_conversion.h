#ifndef JNI_TYPE_CONVERSION_H
#define JNI_TYPE_CONVERSION_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "mruby.h"

extern bool create_mrb_value(JNIEnv *env, jobject value, mrb_value &store);
extern jobject create_value(JNIEnv *env, mrb_value const &value);

#ifdef __cplusplus
}
#endif

#endif // end of JNI_TYPE_CONVERSION_H

