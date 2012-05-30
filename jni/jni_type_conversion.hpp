#ifndef JNI_TYPE_CONVERSION_H
#define JNI_TYPE_CONVERSION_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}

extern bool create_mrb_value(JNIEnv *env, jobject value, mrb_value &store);
extern jobject create_value(JNIEnv *env, mrb_value const &value);
extern mrb_value *create_mrb_value_array(JNIEnv *env, int const &num, jobjectArray array);

#endif // end of JNI_TYPE_CONVERSION_H

