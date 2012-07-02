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

template <typename JType> static jvalue call_method(mrb_state *mrb, JNIEnv *env, jni_type_t const &type, JType obj, jmethodID jmid, jvalue* args)
{
	jvalue const &ret = jni_functor<JType>(env)(type, obj, jmid, args);
	safe_jni::safe_local_ref<jthrowable> e(env, env->ExceptionOccurred());
	if (NULL != e) {
		env->ExceptionClear();
		RClass *error_class = NULL;
		{
			safe_jni::safe_local_ref<jclass> jerror_class(env, env->FindClass("java/lang/Error"));
			if (JNI_FALSE != env->IsInstanceOf(e.get(), jerror_class.get())) {
				error_class = mrb_class_obj_get(mrb, "JError");
			}
		}
		if (NULL == error_class) {
			safe_jni::safe_local_ref<jclass> jexception_class(env, env->FindClass("java/lang/Exception"));
			if (JNI_FALSE != env->IsInstanceOf(e.get(), jexception_class.get())) {
				error_class = mrb_class_obj_get(mrb, "JException");
			}
		}
		if (NULL == error_class) {
			error_class = mrb_class_obj_get(mrb, "JThrowable");
		}
		env->ExceptionClear();
		char *msg = NULL;
		{
			safe_jni::safe_local_ref<jclass> cls(env, env->GetObjectClass(e.get()));
			if (cls.get()) {
				jmethodID mid = env->GetMethodID(cls.get(), "getMessage", "()Ljava/lang/String;");
				if (NULL != mid) {
					safe_jni::safe_local_ref<jstring> str(env, static_cast<jstring>(env->CallObjectMethod(e.get(), mid)));
					safe_jni::safe_string msg_str(env, str.get());
					msg = strdup(msg_str.string());
				}
			}
		}
		env->ExceptionClear();
		if (NULL == msg) {
			char const *err_msg = "Exception occurred inside JVM.";
			mrb_value const &exc = mrb_exc_new(mrb, error_class, err_msg, strlen(err_msg));
			mrb_exc_raise(mrb, exc);
		} else {
			mrb_value const &exc = mrb_exc_new(mrb, error_class, msg, strlen(msg));
			free(msg);
			msg = NULL;
			mrb_exc_raise(mrb, exc);
		}
	}
	return ret;
}

extern void       init_converters();
extern bool       convert_mrb_value_to_jvalue(mrb_state *mrb, JNIEnv *env, mrb_value rval, jvalue &jval, jni_type_t const &type);
extern mrb_value  convert_jvalue_to_mrb_value(mrb_state *mrb, JNIEnv *env, jni_type_t const &type, jvalue const &ret);
extern jni_type_t get_return_type(char const * const signature);
extern bool       get_argument_types(char const * const signature, jni_type_t *types, int num);
extern int        get_count_of_arguments(char const * const signature);

}
}

#endif // end of JAMRUBY_JNI_METHOD_CALL_H

