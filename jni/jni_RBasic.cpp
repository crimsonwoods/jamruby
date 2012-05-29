#include "jni_RBasic.h"
#include "jni_common.hpp"
extern "C" {
#include "mruby.h"
}

/*
 * Class:     org_jamruby_mruby_RBasic
 * Method:    n_getValueType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_RBasic_n_1getValueType
  (JNIEnv *env, jclass clazz, jlong basic)
{
	RBasic const * const rbasic = to_ptr<RBasic>(basic);
	if (NULL == rbasic) {
		return -1;
	}
	return static_cast<jint>(rbasic->tt);
}

/*
 * Class:     org_jamruby_mruby_RBasic
 * Method:    n_getColor
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_RBasic_n_1getColor
  (JNIEnv *env, jclass clazz, jlong basic)
{
	RBasic const * const rbasic = to_ptr<RBasic>(basic);
	if (NULL == rbasic) {
		return -1;
	}
	return static_cast<jint>(rbasic->color);
}

/*
 * Class:     org_jamruby_mruby_RBasic
 * Method:    n_getFlags
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_RBasic_n_1getFlags
  (JNIEnv *env, jclass clazz, jlong basic)
{
	RBasic const * const rbasic = to_ptr<RBasic>(basic);
	if (NULL == rbasic) {
		return 0;
	}
	return static_cast<jint>(rbasic->flags);
}

/*
 * Class:     org_jamruby_mruby_RBasic
 * Method:    n_getRClass
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RBasic_n_1getRClass
  (JNIEnv *env, jclass clazz, jlong basic)
{
	RBasic const * const rbasic = to_ptr<RBasic>(basic);
	if (NULL == rbasic) {
		return to_jlong(NULL);
	}
	return to_jlong(rbasic->c);
}

/*
 * Class:     org_jamruby_mruby_RBasic
 * Method:    n_getGCNext
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RBasic_n_1getGCNext
  (JNIEnv *env, jclass clazz, jlong basic)
{
	RBasic const * const rbasic = to_ptr<RBasic>(basic);
	if (NULL == rbasic) {
		return to_jlong(NULL);
	}
	return to_jlong(rbasic->gcnext);
}


