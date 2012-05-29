#include "jni_RObject.h"
#include "jni_common.hpp"
#include "jni_type_conversion.h"
#include "safe_jni.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/variable.h"
}

/*
 * Class:     org_jamruby_mruby_RObject
 * Method:    n_getIv
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RObject_n_1getIv
  (JNIEnv *env, jclass, jlong obj)
{
	RObject const * const ptr = to_ptr<RObject>(obj);
	if (NULL == ptr) {
		return to_jlong(NULL);
	}
	return to_jlong(ptr->iv);
}

/*
 * Class:     org_jamruby_mruby_RObject
 * Method:    n_objIvGet
 * Signature: (JJJ)Lorg/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_org_jamruby_mruby_RObject_n_1objIvGet
  (JNIEnv *env, jclass, jlong mrb, jlong obj, jlong sym)
{
	mrb_state * const state = to_ptr<mrb_state>(mrb);
	RObject * const ptr = to_ptr<RObject>(obj);
	mrb_value const &ret = mrb_obj_iv_get(state, ptr, static_cast<mrb_sym>(sym));
	safe_jni::safe_local_ref<jobject> result(env, create_value(env, ret));
	return result.get();
}

/*
 * Class:     org_jamruby_mruby_RObject
 * Method:    n_objIvSet
 * Signature: (JJJLorg/jamruby/mruby/Value;)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_RObject_n_1objIvSet
  (JNIEnv *env, jclass, jlong mrb, jlong obj, jlong sym, jobject value)
{
	mrb_state * const state = to_ptr<mrb_state>(mrb);
	RObject * const ptr = to_ptr<RObject>(obj);
	mrb_value val;
	if (!create_mrb_value(env, value ,val)) {
		return;
	}
	mrb_obj_iv_set(state, ptr, static_cast<mrb_sym>(sym), val);
}

