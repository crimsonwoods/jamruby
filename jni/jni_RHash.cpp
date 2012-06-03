#include "jni_RHash.h"
#include "jni_common.hpp"
#include "jni_type_conversion.hpp"
#include "safe_jni.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/hash.h"
}

/*
 * Class:     org_jamruby_mruby_RHash
 * Method:    n_getIv
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RHash_n_1getIv
  (JNIEnv *env, jclass, jlong hash)
{
	RHash * const ptr = to_ptr<RHash>(hash);
	if (NULL == ptr) {
		return to_jlong(NULL);
	}
	return to_jlong(ptr->iv);
}

/*
 * Class:     org_jamruby_mruby_RHash
 * Method:    n_getHt
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RHash_n_1getHt
  (JNIEnv *env, jclass, jlong hash)
{
	RHash * const ptr = to_ptr<RHash>(hash);
	if (NULL == ptr) {
		return to_jlong(NULL);
	}
	return to_jlong(ptr->ht);
}

/*
 * Class:     org_jamruby_mruby_RHash
 * Method:    n_hashSet
 * Signature: (JLorg/jamruby/mruby/Value;Lorg/jamruby/mruby/Value;Lorg/jamruby/mruby/Value;)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_RHash_n_1hashSet
  (JNIEnv *env, jclass, jlong mrb, jobject hash, jobject key, jobject value)
{
	mrb_value hash_val, key_val, val;
	if (!create_mrb_value(env, hash, hash_val)) {
		return;
	}
	if (!create_mrb_value(env, key, key_val)) {
		return;
	}
	if (!create_mrb_value(env, value, val)) {
		return;
	}
	mrb_hash_set(to_ptr<mrb_state>(mrb), hash_val, key_val, val);
}

/*
 * Class:     org_jamruby_mruby_RHash
 * Method:    n_hashGet
 * Signature: (JLorg/jamruby/mruby/Value;Lorg/jamruby/mruby/Value;)Lorg/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_org_jamruby_mruby_RHash_n_1hashGet
  (JNIEnv *env, jclass, jlong mrb, jobject hash, jobject key)
{
	mrb_value hash_val, key_val;
	if (!create_mrb_value(env, hash, hash_val)) {
		return NULL;
	}
	if (!create_mrb_value(env, key, key_val)) {
		return NULL;
	}
	mrb_value const &ret = mrb_hash_get(to_ptr<mrb_state>(mrb), hash_val, key_val);
	safe_jni::safe_local_ref<jobject> result(env, create_value(env, ret));
	return result.get();
}

/*
 * Class:     org_jamruby_mruby_RHash
 * Method:    n_hashDeleteKey
 * Signature: (JLorg/jamruby/mruby/Value;Lorg/jamruby/mruby/Value;)Lorg/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_org_jamruby_mruby_RHash_n_1hashDeleteKey
  (JNIEnv *env, jclass, jlong mrb, jobject hash, jobject key)
{
	mrb_value hash_val, key_val;
	if (!create_mrb_value(env, hash, hash_val)) {
		return NULL;
	}
	if (!create_mrb_value(env, key, key_val)) {
		return NULL;
	}
	mrb_value const &ret = mrb_hash_delete_key(to_ptr<mrb_state>(mrb), hash_val, key_val);
	safe_jni::safe_local_ref<jobject> result(env, create_value(env, ret));
	return result.get();
}

