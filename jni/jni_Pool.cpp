#include "jni_Pool.h"
extern "C" {
#include "mruby/compile.h"
}
/*
 * Class:     org_jamruby_mruby_Pool
 * Method:    n_close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_Pool_n_1close
  (JNIEnv *env, jclass clazz, jlong pool)
{
	mrb_pool *p = reinterpret_cast<mrb_pool*>(static_cast<intptr_t>(pool));
	mrb_pool_close(p);
}

