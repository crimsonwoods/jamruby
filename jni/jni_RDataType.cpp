#include "jni_RDataType.h"
#include "jni_common.hpp"
#include "safe_jni.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/data.h"
}

/*
 * Class:     org_jamruby_mruby_RData_RDataType
 * Method:    n_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_jamruby_mruby_RData_00024RDataType_n_1getName
  (JNIEnv *env, jclass, jlong dtype)
{
	mrb_data_type const * const t = to_ptr<mrb_data_type>(dtype);
	if (NULL == t) {
		return NULL;
	}
	safe_jni::safe_local_ref<jstring> name(env, env->NewStringUTF(t->struct_name));
	return name.get();
}


