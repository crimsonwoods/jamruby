#include "jni_RString.h"
extern "C" {
#include "mruby.h"
#include "mruby/string.h"
}

#include "safe_jni.hpp"

inline static RString *to_ptr(jlong str) {
	return reinterpret_cast<RString*>(static_cast<intptr_t>(str));
}

/*
 * Class:     org_jamruby_mruby_RString
 * Method:    n_getPtr
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_jamruby_mruby_RString_n_1getPtr
  (JNIEnv *env, jclass clazz, jlong str)
{
	safe_jni::safe_local_ref<jstring> jstr(env, env->NewStringUTF(to_ptr(str)->ptr));
	return jstr.get();
}

