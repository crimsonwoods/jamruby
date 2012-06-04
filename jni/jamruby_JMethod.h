#ifndef JAMRUBY_JMETHOD_H
#define JAMRUBY_JMETHOD_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}

extern mrb_value jmethod_make(mrb_state *mrb, JNIEnv *env, jmethodID jmid);
extern jmethodID jmethod_get_jmethodID(mrb_state *mrb, mrb_value self);
extern int       jmethod_init_class(mrb_state *mrb);

#endif // end of JAMRUBY_JMETHOD_H

