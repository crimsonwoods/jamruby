#include "jni_KHashIv.h"
#include "safe_jni.hpp"
#include "jni_type_conversion.hpp"
#include "jni_common.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/khash.h"
#include "mruby/variable.h"
}

typedef uint32_t kh_iv_key;

inline kh_iv_key to_key(jlong key) {
	return static_cast<kh_iv_key>(key);
}

/*
 * Class:     org_jamruby_mruby_KHashIv
 * Method:    n_khExist
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_org_jamruby_mruby_KHashIv_n_1khExist
  (JNIEnv *env, jclass, jlong kh, jlong key)
{
	kh_iv * const h = to_ptr<kh_iv>(kh);
	khint_t k = kh_get(iv, h, to_key(key));
	if (k == kh_end(h)) {
		return JNI_FALSE;
	}
	if (kh_exist(h, k)) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}

/*
 * Class:     org_jamruby_mruby_KHashIv
 * Method:    n_khGet
 * Signature: (JJ)Lorg/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_org_jamruby_mruby_KHashIv_n_1khGet
  (JNIEnv *env, jclass, jlong kh, jlong key)
{
	kh_iv *h = to_ptr<kh_iv>(kh);
	khint_t k = kh_get(iv, h, to_key(key));
	if (k == kh_end(h)) {
		return NULL;
	}
	mrb_value const &ret = kh_value(h, k);
	safe_jni::safe_local_ref<jobject> result(env, create_value(env, ret));
	return result.get();
}

/*
 * Class:     org_jamruby_mruby_KHashIv
 * Method:    n_khPut
 * Signature: (JJLorg/jamruby/mruby/Value;)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_KHashIv_n_1khPut
  (JNIEnv *env, jclass, jlong kh, jlong key, jobject value)
{
	kh_iv * const h = to_ptr<kh_iv>(kh);
	mrb_value val;
	if (!create_mrb_value(env, value ,val)) {
		return;
	}
	khint_t k = kh_get(iv, h, to_key(key));
	if (k == kh_end(h)) {
		k = kh_put(iv, h, to_key(key));
	}
	kh_value(h, k) = val;
}

/*
 * Class:     org_jamruby_mruby_KHashIv
 * Method:    n_khSize
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_KHashIv_n_1khSize
  (JNIEnv *env, jclass, jlong kh)
{
	kh_iv const * const h = to_ptr<kh_iv>(kh);
	return kh_size(h);
}

