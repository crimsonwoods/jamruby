#include "jni_KHashMt.h"
#include "jni_common.hpp"
#include "jni_type_conversion.hpp"
#include "safe_jni.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/khash.h"
#include "mruby/proc.h"
}

typedef uint32_t kh_mt_key;

inline kh_mt_key to_key(jlong key) {
	return static_cast<kh_mt_key>(key);
}

/*
 * Class:     org_jamruby_mruby_KHashMt
 * Method:    n_khExist
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_org_jamruby_mruby_KHashMt_n_1khExist
  (JNIEnv *env, jclass, jlong kh, jlong key)
{
	kh_mt * const h = to_ptr<kh_mt>(kh);
	khint_t k = kh_get(mt, h, to_key(key));
	if (k == kh_end(h)) {
		return JNI_FALSE;
	}
	return kh_exist(h, k) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     org_jamruby_mruby_KHashMt
 * Method:    n_khGet
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_KHashMt_n_1khGet
  (JNIEnv *env, jclass, jlong kh, jlong key)
{
	kh_mt * const h = to_ptr<kh_mt>(kh);
	khint_t k = kh_get(mt, h, to_key(key));
	if (k == kh_end(h)) {
		return to_jlong(NULL);
	}
	RProc const * const p = kh_value(h,k);
	return to_jlong(p);
}

/*
 * Class:     org_jamruby_mruby_KHashMt
 * Method:    n_khPut
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_KHashMt_n_1khPut
  (JNIEnv *env, jclass, jlong kh, jlong key, jlong value)
{
	kh_mt * const h = to_ptr<kh_mt>(kh);
	khint_t k = kh_get(mt, h, to_key(key));
	if (k == kh_end(h)) {
		k = kh_put(mt, h, to_key(key));
	}
	kh_value(h, k) = to_ptr<RProc>(value);
}

/*
 * Class:     org_jamruby_mruby_KHashMt
 * Method:    n_khSize
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_KHashMt_n_1khSize
  (JNIEnv *env, jclass, jlong kh)
{
	kh_mt const * const h = to_ptr<kh_mt>(kh);
	if (NULL == h) {
		return -1;
	}
	return kh_size(h);
}


