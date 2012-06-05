#ifndef JAMRUBY_JOBJECT_H
#define JAMRUBY_JOBJECT_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}

mrb_value jobject_make(mrb_state *mrb, JNIEnv *env, jobject obj);
mrb_value jobject_get_class(mrb_state *mrb, mrb_value self);
mrb_value jobject_call(mrb_state *mrb, mrb_value self);
mrb_value jobject_initialize(mrb_state *mrb, mrb_value self);
int jobject_init_class(mrb_state *mrb);

#endif // end of JAMRUBY_JOBJECT_H

