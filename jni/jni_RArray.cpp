#include "jni_RArray.h"
extern "C" {
#include "mruby.h"
#include "mruby/array.h"
}
#include <cstddef>

#include "safe_jni.hpp"
#include "jni_type_conversion.hpp"
#include "jni_common.hpp"

/*
 * Class:     org_jamruby_mruby_RArray
 * Method:    n_getLen
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_RArray_n_1getLen
  (JNIEnv *env, jclass clazz, jlong array)
{
	return to_ptr<RArray>(array)->len;
}

/*
 * Class:     org_jamruby_mruby_RArray
 * Method:    n_getCapa
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_RArray_n_1getCapa
  (JNIEnv *env, jclass clazz, jlong array)
{
	return to_ptr<RArray>(array)->aux.capa;
}

/*
 * Class:     org_jamruby_mruby_RArray
 * Method:    n_getBuf
 * Signature: (J)[Lorg/jamruby/mruby/Value;
 */
JNIEXPORT jobjectArray JNICALL Java_org_jamruby_mruby_RArray_n_1getBuf
  (JNIEnv *env, jclass clazz, jlong array)
{
	size_t const len = to_ptr<RArray>(array)->len;
	mrb_value const * const values = to_ptr<RArray>(array)->buf;

	if (NULL == values) {
		return NULL;
	}

	safe_jni::safe_local_ref<jclass> cls(env, env->FindClass("org/jamruby/mruby/Value"));
	safe_jni::safe_local_ref<jobjectArray> valArray(env, env->NewObjectArray(len, cls.get(), NULL));

	for (size_t i = 0; i < len; ++i) {
		safe_jni::safe_local_ref<jobject> val(env, create_value(env, values[i]));
		env->SetObjectArrayElement(valArray.get(), i, val.get());
	}

	return valArray.get();
}

