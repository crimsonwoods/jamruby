#ifndef JAMRUBY_JCLASS_H
#define JAMRUBY_JCLASS_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}

mrb_value jcls_make(mrb_state *mrb, JNIEnv *env, jclass cls);
mrb_value jcls_make(mrb_state *mrb, JNIEnv *env, char const * const name);
int jcls_init_class(mrb_state *mrb);

#endif // end of JAMRUBY_JCLASS_H

