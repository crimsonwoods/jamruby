#include "jni_Value.h"
#include "jni_type_conversion.hpp"
extern "C" {
#include "mruby.h"
}

/*
 * Class:     org_jamruby_mruby_Value
 * Method:    n_obj_id
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_Value_n_1obj_1id
  (JNIEnv *env, jobject obj)
{
	mrb_value obj_val;
	if (!create_mrb_value(env, obj, obj_val)) {
		// MEMO invalid object id is undefined.
		return 0;
	}
	return mrb_obj_id(obj_val);
}

