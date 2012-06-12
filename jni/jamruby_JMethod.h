#ifndef JAMRUBY_JMETHOD_H
#define JAMRUBY_JMETHOD_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}

typedef uint32_t jni_type_t;

#define JNI_TYPE_UNKNOWN 0x0000U
#define JNI_TYPE_VOID    0x0001U
#define JNI_TYPE_BOOLEAN 0x0002U
#define JNI_TYPE_BYTE    0x0003U
#define JNI_TYPE_CHAR    0x0004U
#define JNI_TYPE_SHORT   0x0005U
#define JNI_TYPE_INT     0x0006U
#define JNI_TYPE_LONG    0x0007U
#define JNI_TYPE_FLOAT   0x0008U
#define JNI_TYPE_DOUBLE  0x0009U
#define JNI_TYPE_OBJECT  0x000AU
#define JNI_TYPE_ARRAY   0x0100U

#define JNI_TYPE_MASK    0x00ffU

inline bool jni_type_is_array(jni_type_t t) {
	return (0 != (t & JNI_TYPE_ARRAY)) ? true : false;
}

inline jni_type_t jni_type_get_type(jni_type_t t) {
	return (jni_type_t)(t & JNI_TYPE_MASK);
}

inline jni_type_t jni_type_make(jni_type_t t, bool is_array) {
	return is_array ? (jni_type_t)(t | JNI_TYPE_ARRAY) : t;
}

extern mrb_value  jmethod_make(mrb_state *mrb, JNIEnv *env, jmethodID jmid, char const * const sig);
extern jmethodID  jmethod_get_jmethodID(mrb_state *mrb, mrb_value self);
extern bool       jmethod_is_jmethod(mrb_state *mrb, mrb_value obj);
extern jni_type_t jmethod_get_return_type(mrb_state *mrb, mrb_value obj);
extern bool       jmethod_get_argument_types(mrb_state *mrb, mrb_value obj, jni_type_t *types, int num);
extern int        jmethod_init_class(mrb_state *mrb);

#endif // end of JAMRUBY_JMETHOD_H

