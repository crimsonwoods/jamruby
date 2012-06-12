#ifndef JAMRUBY_JOBJECT_H
#define JAMRUBY_JOBJECT_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}

extern mrb_value jobject_make(mrb_state *mrb, JNIEnv *env, jobject obj);
extern bool      jobject_is_jobject(mrb_state *mrb, mrb_value obj);
extern jobject   jobject_get_jobject(mrb_state *mrb, mrb_value obj);
extern int       jobject_init_class(mrb_state *mrb);

#endif // end of JAMRUBY_JOBJECT_H

