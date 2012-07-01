#ifndef JAMRUBY_JNI_FUNCTOR_H
#define JAMRUBY_JNI_FUNCTOR_H

#include <jni.h>
#include "jamruby_jni_types.h"

template <typename JType> class jni_functor {
private:
	JNIEnv *env_;
	jni_functor();
	jni_functor(jni_functor const&);
	jni_functor &operator=(jni_functor const&);
public:
	jni_functor(JNIEnv *env) : env_(env) {
	}
		~jni_functor() {
	}
	jvalue operator () (jni_type_t const &type, JType obj, jmethodID jmid, jvalue *args) const {
		JNIEnv *env = env_;
		jvalue ret;
		if (type.is_array()) {
			ret.l = env->CallObjectMethodA(obj, jmid, args);
		} else {
			switch(type.type_id()) {
			case JNI_TYPE_VOID:
				env->CallVoidMethodA(obj, jmid, args);
				break;
			case JNI_TYPE_BOOLEAN:
				ret.z = env->CallBooleanMethodA(obj, jmid, args);
				break;
			case JNI_TYPE_BYTE:
				ret.b = env->CallByteMethodA(obj, jmid, args);
				break;
			case JNI_TYPE_CHAR:
				ret.c = env->CallCharMethodA(obj, jmid, args);
				break;
			case JNI_TYPE_SHORT:
				ret.s = env->CallShortMethodA(obj, jmid, args);
				break;
			case JNI_TYPE_INT:
				ret.i = env->CallIntMethodA(obj, jmid, args);
				break;
			case JNI_TYPE_LONG:
				ret.j = env->CallLongMethodA(obj, jmid, args);
				break;
			case JNI_TYPE_FLOAT:
				ret.f = env->CallFloatMethodA(obj, jmid, args);
				break;
			case JNI_TYPE_DOUBLE:
				ret.d = env->CallDoubleMethodA(obj, jmid, args);
				break;
			case JNI_TYPE_OBJECT:
				ret.l = env->CallObjectMethodA(obj, jmid, args);
				break;
			default:
				// TODO handle error.
				break;
			}
		}
		return ret;
	}
};

template <> class jni_functor<jclass> {
private:
	JNIEnv *env_;
	jni_functor();
	jni_functor(jni_functor const&);
	jni_functor &operator=(jni_functor const&);
public:
	jni_functor(JNIEnv *env) : env_(env) {
	}
	~jni_functor() {
	}
	jvalue operator () (jni_type_t const &type, jclass cls, jmethodID jmid, jvalue *args) const {
		JNIEnv *env = env_;
		jvalue ret;
		if (type.is_array()) {
			ret.l = env->CallStaticObjectMethodA(cls, jmid, args);
		} else {
			switch (type.type_id()) {
			case JNI_TYPE_VOID:
				env->CallStaticVoidMethodA(cls, jmid, args);
				break;
			case JNI_TYPE_BOOLEAN:
				ret.z = env->CallStaticBooleanMethodA(cls, jmid, args);
				break;
			case JNI_TYPE_BYTE:
				ret.b = env->CallStaticByteMethodA(cls, jmid, args);
				break;
			case JNI_TYPE_CHAR:
				ret.c = env->CallStaticCharMethodA(cls, jmid, args);
				break;
			case JNI_TYPE_SHORT:
				ret.s = env->CallStaticShortMethodA(cls, jmid, args);
				break;
			case JNI_TYPE_INT:
				ret.i = env->CallStaticIntMethodA(cls, jmid, args);
				break;
			case JNI_TYPE_LONG:
				ret.j = env->CallStaticLongMethodA(cls, jmid, args);
				break;
			case JNI_TYPE_FLOAT:
				ret.f = env->CallStaticFloatMethodA(cls, jmid, args);
				break;
			case JNI_TYPE_DOUBLE:
				ret.d = env->CallStaticDoubleMethodA(cls, jmid, args);
				break;
			case JNI_TYPE_OBJECT:
				ret.l = env->CallStaticObjectMethodA(cls, jmid, args);
				break;
			default:
				// TODO handle error.
				break;
			}
		}
		return ret;
	}
};

#endif // end of JAMRUBY_JNI_FUNCTOR_H

