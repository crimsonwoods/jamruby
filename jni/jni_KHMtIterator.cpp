#include "jni_KHMtIterator.h"
#include "jni_common.hpp"
#include "jni_type_conversion.hpp"
#include "safe_jni.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/khash.h"
#include "mruby/proc.h"
}

typedef uint32_t kh_mt_key;

/*
 * Class:     org_jamruby_mruby_KHashMt_KHMtIterator
 * Method:    n_begin
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashMt_00024KHMtIterator_n_1begin
  (JNIEnv *env, jclass, jlong kh)
{
	kh_mt const * const h = to_ptr<kh_mt>(kh);
	if (NULL == h) {
		return 0;
	}
	return static_cast<jlong>(kh_begin(h));
}

/*
 * Class:     org_jamruby_mruby_KHashMt_KHMtIterator
 * Method:    n_end
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashMt_00024KHMtIterator_n_1end
  (JNIEnv *env, jclass, jlong kh)
{
	kh_mt const * const h = to_ptr<kh_mt>(kh);
	if (NULL == h) {
		return 0;
	}
	return static_cast<jlong>(kh_end(h));
}

/*
 * Class:     org_jamruby_mruby_KHashMt_KHMtIterator
 * Method:    n_exist
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_org_jamruby_mruby_KHashMt_00024KHMtIterator_n_1exist
  (JNIEnv *env, jclass, jlong kh, jlong iter)
{
	kh_mt const * const h = to_ptr<kh_mt>(kh);
	khiter_t const k = static_cast<khiter_t>(iter);
	if (k >= kh_end(h)) {
		return JNI_FALSE;
	}
	return kh_exist(h, k) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     org_jamruby_mruby_KHashMt_KHMtIterator
 * Method:    n_next
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashMt_00024KHMtIterator_n_1next
  (JNIEnv *env, jclass, jlong kh, jlong iter)
{
	kh_mt const * const h = to_ptr<kh_mt>(kh);
	while(kh_end(h) > ++iter) {
		if (kh_exist(h, iter)) {
			return static_cast<jlong>(iter);
		}
	}
	return static_cast<jlong>(kh_end(h));
}

/*
 * Class:     org_jamruby_mruby_KHashMt_KHMtIterator
 * Method:    n_key
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashMt_00024KHMtIterator_n_1key
  (JNIEnv *env, jclass, jlong kh, jlong iter)
{
	kh_mt const * const h = to_ptr<kh_mt>(kh);
	khiter_t const k = static_cast<khiter_t>(iter);
	if (k >= kh_end(h)) {
		return -1;
	}
	kh_mt_key key = kh_key(h, k);
	return static_cast<jlong>(key);
}

/*
 * Class:     org_jamruby_mruby_KHashMt_KHMtIterator
 * Method:    n_value
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashMt_00024KHMtIterator_n_1value
  (JNIEnv *env, jclass, jlong kh, jlong iter)
{
	kh_mt const * const h = to_ptr<kh_mt>(kh);
	khiter_t const k = static_cast<khiter_t>(iter);
	RProc * const r = kh_value(h, k);
	return to_jlong(r);
}

