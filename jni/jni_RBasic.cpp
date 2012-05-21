#include "jni_RBasic.h"
extern "C" {
#include "mruby.h"
}

inline static RBasic *to_ptr(jlong basic) {
	return reinterpret_cast<RBasic*>(static_cast<intptr_t>(basic));
}

/*
 * Class:     org_jamruby_mruby_RBasic
 * Method:    n_getValueType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_RBasic_n_1getValueType
  (JNIEnv *env, jclass clazz, jlong basic)
{
	RBasic *rbasic = to_ptr(basic);
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
	RBasic *rbasic = to_ptr(basic);
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
	RBasic *rbasic = to_ptr(basic);
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
	RBasic *rbasic = to_ptr(basic);
	return static_cast<jlong>(reinterpret_cast<intptr_t>(rbasic->c));
}

/*
 * Class:     org_jamruby_mruby_RBasic
 * Method:    n_getGCNext
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RBasic_n_1getGCNext
  (JNIEnv *env, jclass clazz, jlong basic)
{
	RBasic *rbasic = to_ptr(basic);
	return static_cast<jlong>(reinterpret_cast<intptr_t>(rbasic->gcnext));
}


