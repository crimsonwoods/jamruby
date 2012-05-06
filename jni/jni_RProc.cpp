#include "jni_RProc.h"
extern "C" {
#include "mruby.h"
#include "mruby/proc.h"
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_RProc
 * Method:    n_getTargetClass
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_RProc_n_1getTargetClass
  (JNIEnv *env, jclass clazz, jlong proc)
{
	RProc *rproc = reinterpret_cast<RProc*>(static_cast<intptr_t>(proc));
	return static_cast<jlong>(reinterpret_cast<intptr_t>(rproc->target_class));
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_RProc
 * Method:    n_getEnv
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_RProc_n_1getEnv
  (JNIEnv *env, jclass clazz, jlong proc)
{
	RProc *rproc = reinterpret_cast<RProc*>(static_cast<intptr_t>(proc));
	return static_cast<jlong>(reinterpret_cast<intptr_t>(rproc->env));
}

