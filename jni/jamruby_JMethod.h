#ifndef JAMRUBY_JMETHOD_H
#define JAMRUBY_JMETHOD_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}
#include "jamruby_jni_types.h"

extern mrb_value  jmethod_make(mrb_state *mrb, JNIEnv *env, jmethodID jmid, char const * const sig);
extern jmethodID  jmethod_get_jmethodID(mrb_state *mrb, mrb_value self);
extern bool       jmethod_is_jmethod(mrb_state *mrb, mrb_value obj);
extern jni_type_t jmethod_get_return_type(mrb_state *mrb, mrb_value obj);
extern bool       jmethod_get_argument_types(mrb_state *mrb, mrb_value obj, jni_type_t *types, int num);
extern int        jmethod_init_class(mrb_state *mrb);

#endif // end of JAMRUBY_JMETHOD_H

