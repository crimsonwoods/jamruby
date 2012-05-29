#include "jni_RClass.h"
#include "jni_common.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/class.h"
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_getSuperClass
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1getSuperClass
  (JNIEnv *env, jclass clazz, jlong cls)
{
	RClass const * const rcls = to_ptr<RClass>(cls);
	return to_jlong(rcls->super);
}

