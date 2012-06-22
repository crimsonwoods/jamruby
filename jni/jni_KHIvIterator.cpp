#include "jni_KHIvIterator.h"
#include "jni_common.hpp"
#include "safe_jni.hpp"
#include "jni_type_conversion.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/khash.h"
#include "mruby/variable.h"
}

typedef uint32_t kh_iv_key;

/*
 * Class:     org_jamruby_mruby_KHashIv_KHIvIterator
 * Method:    n_begin
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashIv_00024KHIvIterator_n_1begin
  (JNIEnv *env, jclass, jlong kh)
{
	kh_iv const * const h = to_ptr<kh_iv>(kh);
	if (NULL == h) {
		return 0;
	}
	return static_cast<jlong>(kh_begin(h));
}

/*
 * Class:     org_jamruby_mruby_KHashIv_KHIvIterator
 * Method:    n_end
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashIv_00024KHIvIterator_n_1end
  (JNIEnv *env, jclass, jlong kh)
{
	kh_iv const * const h = to_ptr<kh_iv>(kh);
	if (NULL == h) {
		return 0;
	}
	return static_cast<jlong>(kh_end(h));
}

/*
 * Class:     org_jamruby_mruby_KHashIv_KHIvIterator
 * Method:    n_next
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashIv_00024KHIvIterator_n_1next
  (JNIEnv *env, jclass, jlong kh, jlong iter)
{
	kh_iv const * const h = to_ptr<kh_iv>(kh);
	while (kh_end(h) > ++iter) {
		if (kh_exist(h, iter)) {
			return static_cast<jlong>(iter);
		}
	}
	return static_cast<jlong>(kh_end(h));
}

/*
 * Class:     org_jamruby_mruby_KHashIv_KHIvIterator
 * Method:    n_key
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashIv_00024KHIvIterator_n_1key
  (JNIEnv *env, jclass, jlong kh, jlong iter)
{
	kh_iv const * const h = to_ptr<kh_iv>(kh);
	khiter_t const k = static_cast<khiter_t>(iter);
	if (k >= kh_end(h)) {
		return -1;
	}
	kh_iv_key key = kh_key(h, k);
	return static_cast<jlong>(key);
}

/*
 * Class:     org_jamruby_mruby_KHashIv_KHIvIterator
 * Method:    n_value
 * Signature: (JJ)Lorg/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_org_jamruby_mruby_KHashIv_00024KHIvIterator_n_1value
  (JNIEnv *env, jclass, jlong kh, jlong iter)
{
	kh_iv const * const h = to_ptr<kh_iv>(kh);
	khiter_t const k = static_cast<khiter_t>(iter);
	mrb_value const &ret = kh_value(h, k);
	safe_jni::safe_local_ref<jobject> result(env, create_value(env, ret));
	return result.get();
}

/*
 * Class:     org_jamruby_mruby_KHashIv_KHIvIterator
 * Method:    n_exist
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_org_jamruby_mruby_KHashIv_00024KHIvIterator_n_1exist
  (JNIEnv *env, jclass, jlong kh, jlong iter)
{
	kh_iv const * const h = to_ptr<kh_iv>(kh);
	khiter_t const k = static_cast<khiter_t>(iter);
	if (k >= kh_end(h)) {
		return JNI_FALSE;
	}
	return kh_exist(h, k) ? JNI_TRUE : JNI_FALSE;
}

