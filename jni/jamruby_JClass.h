#ifndef JAMRUBY_JCLASS_H
#define JAMRUBY_JCLASS_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}

mrb_value jcls_make(mrb_state *mrb, JNIEnv *env, char const * const name);
mrb_value jcls_call_static(mrb_state *mrb, mrb_value self);
mrb_value jcls_to_s(mrb_state *mrb, mrb_value self);
mrb_value jcls_initialize(mrb_state *mrb, mrb_value self);
int jcls_init_class(mrb_state *mrb);

#endif // end of JAMRUBY_JCLASS_H

