#include "jni_Pool.h"
extern "C" {
#include "pool.h"
}
/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_Pool
 * Method:    n_close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_Pool_n_1close
  (JNIEnv *env, jclass clazz, jlong pool)
{
	mrb_pool *p = reinterpret_cast<mrb_pool*>(static_cast<intptr_t>(pool));
	mrb_pool_close(p);
}

