#include "jni_RClass.h"
#include "jni_common.hpp"
#include "jni_type_conversion.hpp"
extern "C" {
#include "mruby.h"
#include "mruby/class.h"
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_getSuperClass
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1getSuperClass
  (JNIEnv *env, jclass clazz, jlong cls)
{
	RClass const * const rcls = to_ptr<RClass>(cls);
	if (NULL == rcls) {
		return to_jlong(NULL);
	}
	return to_jlong(rcls->super);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_getIv
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1getIv
  (JNIEnv *env, jclass, jlong cls)
{
	RClass const * const c = to_ptr<RClass>(cls);
	if (NULL == c) {
		return to_jlong(NULL);
	}
	return to_jlong(c->iv);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_getMt
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1getMt
  (JNIEnv *env, jclass, jlong cls)
{
	RClass const * const c = to_ptr<RClass>(cls);
	if (NULL == c) {
		return to_jlong(NULL);
	}
	return to_jlong(c->mt);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_defineClassId
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1defineClassId
  (JNIEnv *env, jclass, jlong mrb, jlong mid, jlong c)
{
	RClass * const cls = mrb_define_class_id(to_ptr<mrb_state>(mrb), static_cast<mrb_sym>(mid), to_ptr<RClass>(c));
	return to_jlong(cls);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_defineModuleId
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1defineModuleId
  (JNIEnv *env, jclass, jlong mrb, jlong mid)
{
	RClass * const c = mrb_define_module_id(to_ptr<mrb_state>(mrb), static_cast<mrb_sym>(mid));
	return to_jlong(c);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_vmDefineClass
 * Signature: (JLorg/jamruby/mruby/Value;Lorg/jamruby/mruby/Value;J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1vmDefineClass
  (JNIEnv *env, jclass, jlong mrb, jobject outerClass, jobject superClass, jlong mid)
{
	mrb_value outer_val, super_val;
	if (!create_mrb_value(env, outerClass, outer_val)) {
		return to_jlong(NULL);
	}
	if (!create_mrb_value(env, superClass, super_val)) {
		return to_jlong(NULL);
	}
	RClass * const c = mrb_vm_define_class(to_ptr<mrb_state>(mrb), outer_val, super_val, static_cast<mrb_sym>(mid));
	return to_jlong(c);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_vmDefineModule
 * Signature: (JLorg/jamruby/mruby/Value;J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1vmDefineModule
  (JNIEnv *env, jclass, jlong mrb, jobject outerClass, jlong mid)
{
	mrb_value outer_val;
	if (!create_mrb_value(env, outerClass, outer_val)) {
		return to_jlong(NULL);
	}
	RClass * const c = mrb_vm_define_module(to_ptr<mrb_state>(mrb), outer_val, static_cast<mrb_sym>(mid));
	return to_jlong(c);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_defineMethodVm
 * Signature: (JJJLorg/jamruby/mruby/Value;)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_RClass_n_1defineMethodVm
  (JNIEnv *env, jclass, jlong mrb, jlong c, jlong mid, jobject body)
{
	mrb_value body_val;
	if (!create_mrb_value(env, body, body_val)) {
		return;
	}
	mrb_define_method_vm(to_ptr<mrb_state>(mrb), to_ptr<RClass>(c), static_cast<mrb_sym>(mid), body_val);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_defineMethodRaw
 * Signature: (JJJJ)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_RClass_n_1defineMethodRaw
  (JNIEnv *env, jclass, jlong mrb, jlong c, jlong mid, jlong proc)
{
	mrb_define_method_raw(to_ptr<mrb_state>(mrb), to_ptr<RClass>(c), static_cast<mrb_sym>(mid), to_ptr<RProc>(proc));
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_classOuterModule
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1classOuterModule
  (JNIEnv *env, jclass, jlong mrb, jlong c)
{
	RClass * const cls = mrb_class_outer_module(to_ptr<mrb_state>(mrb), to_ptr<RClass>(c));
	return to_jlong(cls);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_methodSearch
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1methodSearch
  (JNIEnv *env, jclass, jlong mrb, jlong c, jlong mid)
{
	RProc * const proc = mrb_method_search(to_ptr<mrb_state>(mrb), to_ptr<RClass>(c), static_cast<mrb_sym>(mid));
	return to_jlong(proc);
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_respondTo
 * Signature: (JLorg/jamruby/mruby/Value;J)Z
 */
JNIEXPORT jboolean JNICALL Java_org_jamruby_mruby_RClass_n_1respondTo
  (JNIEnv *env, jclass, jlong mrb, jobject obj, jlong mid)
{
	mrb_value obj_val;
	if (!create_mrb_value(env, obj, obj_val)) {
		return JNI_FALSE;
	}
	int const &ret = mrb_respond_to(to_ptr<mrb_state>(mrb), obj_val, static_cast<mrb_sym>(mid));
	return (0 != ret) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_objIsInstanceOf
 * Signature: (JLorg/jamruby/mruby/Value;J)Z
 */
JNIEXPORT jboolean JNICALL Java_org_jamruby_mruby_RClass_n_1objIsInstanceOf
  (JNIEnv *env, jclass, jlong mrb, jobject obj, jlong c)
{
	mrb_value obj_val;
	if (!create_mrb_value(env, obj, obj_val)) {
		return JNI_FALSE;
	}
	int const &ret = mrb_obj_is_instance_of(to_ptr<mrb_state>(mrb), obj_val, to_ptr<RClass>(c));
	return (0 != ret) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     org_jamruby_mruby_RClass
 * Method:    n_classReal
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_RClass_n_1classReal
  (JNIEnv *env, jclass, jlong c)
{
	RClass * const cls = mrb_class_real(to_ptr<RClass>(c));
	return to_jlong(cls);
}

