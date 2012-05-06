#include "jni_State.h"
extern "C" {
#include "mruby.h"
}
#include "safe_jni.hpp"
#include <cstddef>

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_State
 * Method:    n_getIreps
 * Signature: (J)[Lcrimsonwoods/android/libs/jamruby/mruby/Irep;
 */
JNIEXPORT jobjectArray JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_State_n_1getIreps
  (JNIEnv *env, jclass clazz, jlong mrb)
{
	mrb_state* s = reinterpret_cast<mrb_state*>(static_cast<intptr_t>(mrb));
	size_t const len = s->irep_len;
	safe_jni::safe_local_ref<jclass> irep_clazz(env, env->FindClass("crimsonwoods/android/libs/jamruby/mruby/Irep"));
	if (!irep_clazz) {
		return NULL;
	}
	safe_jni::safe_local_ref<jobjectArray> array(env, env->NewObjectArray(len, irep_clazz.get(), NULL));
	if (!array) {
		return NULL;
	}
	jmethodID ctor = env->GetMethodID(irep_clazz.get(), "<init>", "(J)V");
	for (size_t i = 0; i < len; ++i) {
		safe_jni::safe_local_ref<jobject> elem(env, env->NewObject(irep_clazz.get(), ctor, static_cast<jlong>(reinterpret_cast<intptr_t>(s->irep[i]))));
		if (!elem) {
			break;
		}
		env->SetObjectArrayElement(array.get(), i, elem.get());
	}
	return array.get();
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_State
 * Method:    n_getExc
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_State_n_1getExc
  (JNIEnv *env, jclass clazz, jlong mrb)
{
	RObject *exc = reinterpret_cast<mrb_state*>(static_cast<intptr_t>(mrb))->exc;
	return static_cast<jlong>(reinterpret_cast<intptr_t>(exc));
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_State
 * Method:    n_close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_State_n_1close
  (JNIEnv *env, jclass clazz, jlong mrb)
{
	mrb_close(reinterpret_cast<mrb_state*>(static_cast<intptr_t>(mrb)));
	fflush(0);
}

