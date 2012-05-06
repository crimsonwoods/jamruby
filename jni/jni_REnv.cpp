#include "jni_REnv.h"
extern "C" {
#include "mruby.h"
#include "mruby/proc.h"
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_REnv
 * Method:    n_getMid
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_REnv_n_1getMid
  (JNIEnv *env, jclass clazz, jlong renv)
{
	REnv *re = reinterpret_cast<REnv*>(static_cast<intptr_t>(renv));
	return static_cast<jlong>(re->mid);
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_REnv
 * Method:    n_getCioff
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_REnv_n_1getCioff
  (JNIEnv *env, jclass clazz, jlong renv)
{
	REnv *re = reinterpret_cast<REnv*>(static_cast<intptr_t>(renv));
	return re->cioff;
}

