#include "jni_RData.h"
#include "jni_common.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/data.h"
}

/*
 * Class:     org_jamruby_mruby_RData
 * Method:    n_getIv
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RData_n_1getIv
  (JNIEnv *env, jclass, jlong data)
{
	RData const * const d = to_ptr<RData>(data);
	if (NULL == d) {
		return to_jlong(NULL);
	}
	return to_jlong(d->iv);
}

/*
 * Class:     org_jamruby_mruby_RData
 * Method:    n_getType
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RData_n_1getType
  (JNIEnv *env, jclass, jlong data)
{
	RData const * const d = to_ptr<RData>(data);
	if (NULL == d) {
		return to_jlong(NULL);
	}
	return to_jlong(d->type);
}

/*
 * Class:     org_jamruby_mruby_RData
 * Method:    n_getData
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RData_n_1getData
  (JNIEnv *env, jclass, jlong data)
{
	RData const * const d = to_ptr<RData>(data);
	if (NULL == d) {
		return to_jlong(NULL);
	}
	return to_jlong(d->data);
}

