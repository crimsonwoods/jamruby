#ifndef JAMRUBY_JNI_METHOD_CALL_H
#define JAMRUBY_JNI_METHOD_CALL_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}
#include "jamruby_jni_types.h"
#include "jamruby_jni_functor.h"
#include "safe_jni.hpp"

namespace org{
namespace jamruby {

extern RClass *get_mruby_error_class(mrb_state *mrb, JNIEnv *env, jthrowable e);
extern char *get_message_from_jthrowable(JNIEnv *env, jthrowable e);
extern void raise_mruby_error(mrb_state *mrb, char *msg, RClass *error_class);

template <typename JType> static jvalue call_method(mrb_state *mrb, JNIEnv *env, jni_type_t const &type, JType obj, jmethodID jmid, jvalue* args)
{
	jvalue const &ret = jni_functor<JType>(env)(type, obj, jmid, args);
	safe_jni::safe_local_ref<jthrowable> e(env, env->ExceptionOccurred());
	if (NULL != e) {
		env->ExceptionClear();
		RClass *error_class = get_mruby_error_class(mrb, env, e.get());
		env->ExceptionClear();
		char *msg = get_message_from_jthrowable(env, e.get());
		env->ExceptionClear();
		raise_mruby_error(mrb, msg, error_class);
	}
	return ret;
}

extern jobject    call_ctor(mrb_state *mrb, JNIEnv *env, jclass jcls, jmethodID jmid, jvalue* args);
extern void       init_converters();
extern bool       convert_mrb_value_to_jvalue(mrb_state *mrb, JNIEnv *env, mrb_value rval, jvalue &jval, jni_type_t const &type);
extern mrb_value  convert_jvalue_to_mrb_value(mrb_state *mrb, JNIEnv *env, jni_type_t const &type, jvalue const &ret);
extern jni_type_t get_return_type(char const * const signature);
extern bool       get_argument_types(char const * const signature, jni_type_t *types, int num);
extern int        get_count_of_arguments(char const * const signature);
extern bool       is_mrb_value_convertible_to(mrb_state *mrb, mrb_value value, jni_type_t const &type);

}
}

#endif // end of JAMRUBY_JNI_METHOD_CALL_H

