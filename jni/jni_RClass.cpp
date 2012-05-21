#include "jni_RClass.h"
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
	RClass *rcls = reinterpret_cast<RClass*>(static_cast<intptr_t>(cls));
	return static_cast<jlong>(reinterpret_cast<intptr_t>(rcls->super));
}

