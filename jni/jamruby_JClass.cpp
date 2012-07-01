#include "jamruby_JClass.h"
#include "safe_jni.hpp"
#include "jni_Log.h"
#include "jni_type_conversion.hpp"
extern "C" {
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"
}
#include <cstdlib>
#include <cstring>
#include <cstddef>

#include <vector>

#include "jamruby_JObject.h"
#include "jamruby_JMethod.h"
#include "jamruby_jni_functor.h"
#include "jamruby_jni_method_call.h"

#define SUPER_CLASS_NAME "JObject"
#define CLASS_NAME "JClass"

struct jclass_data {
	JNIEnv *env;
	jclass gref_jclass;
};

static void jcls_free(mrb_state *mrb, void *ptr)
{
	if (NULL != ptr) {
		jclass_data *p = static_cast<jclass_data*>(ptr);
		p->env->DeleteGlobalRef(p->gref_jclass);
		p->env = NULL;
		p->gref_jclass = NULL;
	}
	mrb_free(mrb, ptr);
}

static struct mrb_data_type jcls_type = { CLASS_NAME, jcls_free };

mrb_value jcls_make(mrb_state *mrb, JNIEnv *env, jclass cls)
{
	safe_jni::clear_exception ce(env);
	RClass *c = mrb_class_get(mrb, CLASS_NAME);
	if (NULL == c) {
		return mrb_nil_value();
	}
	jclass_data *ptr = static_cast<jclass_data*>(mrb_malloc(mrb, sizeof(jclass_data)));
	if (NULL == ptr) {
		return mrb_nil_value();
	}
	ptr->env = env;
	ptr->gref_jclass = static_cast<jclass>(env->NewGlobalRef(cls));
	return mrb_obj_value(Data_Wrap_Struct(mrb, c, &jcls_type, ptr));
}

mrb_value jcls_make(mrb_state *mrb, JNIEnv *env, char const * const name)
{
	safe_jni::clear_exception ce(env);
	safe_jni::safe_local_ref<jclass> jcls(env, env->FindClass(name));
	if (!jcls) {
		return mrb_nil_value();
	}
	return jcls_make(mrb, env, jcls.get());
}

mrb_value jcls_call_static(mrb_state *mrb, mrb_value self)
{
	mrb_value jmthd;
	int rb_argc;
	mrb_value *rb_argv;
	int const argc = mrb_get_args(mrb, "o*", &jmthd, &rb_argv, &rb_argc);
	if (argc < 1) {
		return mrb_nil_value();
	}
	if (!jmethod_is_jmethod(mrb, jmthd)) {
		return mrb_nil_value();
	}

	jclass_data *data = static_cast<jclass_data*>(mrb_get_datatype(mrb, self, &jcls_type));
	JNIEnv *env = data->env;

	jmethodID jmid = jmethod_get_jmethodID(mrb, jmthd);
	if (NULL == jmid) {
		// 'jmthd' is not 'JMethod' object.
		return mrb_nil_value();
	}

	std::vector<jvalue> jvals(rb_argc);
	if (static_cast<std::size_t>(rb_argc) > jvals.size()) {
		return mrb_nil_value();
	}

	std::vector<jni_type_t> types(rb_argc);
	if (static_cast<std::size_t>(rb_argc) > types.size()) {
		return mrb_nil_value();
	}

	if (!jmethod_get_argument_types(mrb, jmthd, &types[0], rb_argc)) {
		return mrb_nil_value();
	}

	using namespace org::jamruby;

	for (int i = 0; i < rb_argc; ++i) {
		convert_mrb_value_to_jvalue(mrb, env, rb_argv[i], jvals[i], types[i]);
	}

	jni_type_t const type = jmethod_get_return_type(mrb, jmthd);
	jvalue const &ret = call_method(mrb, env, type, data->gref_jclass, jmid, &jvals[0]);
	return convert_jvalue_to_mrb_value(mrb, env, type, ret);
}

mrb_value jcls_call(mrb_state *mrb, mrb_value self)
{
	mrb_value jobj;
	mrb_value jmthd;
	int rb_argc;
	mrb_value *rb_argv;
	int const argc = mrb_get_args(mrb, "oo*", &jobj, &jmthd, &rb_argv, &rb_argc);
	if (argc < 2) {
		LOGE("%s: Too few arguments.", __func__);
		return mrb_nil_value();
	}
	if (!jobject_is_jobject(mrb, jobj)) {
		LOGE("%s: First argument is not 'JObject'.", __func__);
		return mrb_nil_value();
	}
	if (!jmethod_is_jmethod(mrb, jmthd)) {
		LOGE("%s: Second argument is not 'JMethod'.", __func__);
		return mrb_nil_value();
	}

	jclass_data *data = static_cast<jclass_data*>(mrb_get_datatype(mrb, self, &jcls_type));
	JNIEnv *env = data->env;

	jobject obj = jobject_get_jobject(mrb, jobj);
	if (NULL == obj) {
		LOGE("%s: 'JObject' is null.", __func__);
		return mrb_nil_value();
	}

	jmethodID jmid = jmethod_get_jmethodID(mrb, jmthd);
	if (NULL == jmid) {
		LOGE("%s: 'JMethod' is null.", __func__);
		return mrb_nil_value();
	}
	
	std::vector<jvalue> jvals(rb_argc);
	if (static_cast<std::size_t>(rb_argc) > jvals.size()) {
		LOGE("%s: Insufficient memory error.", __func__);
		return mrb_nil_value();
	}

	std::vector<jni_type_t> types(rb_argc);
	if (static_cast<std::size_t>(rb_argc) > types.size()) {
		LOGE("%s: Insufficient memory error.", __func__);
		return mrb_nil_value();
	}

	if (!jmethod_get_argument_types(mrb, jmthd, &types[0], rb_argc)) {
		LOGE("%s: Argument type mismatch.", __func__);
		return mrb_nil_value();
	}

	using namespace org::jamruby;

	for (int i = 0; i < rb_argc; ++i) {
		convert_mrb_value_to_jvalue(mrb, env, rb_argv[i], jvals[i], types[i]);
	}

	jni_type_t const type = jmethod_get_return_type(mrb, jmthd);
	jvalue const &ret = call_method(mrb, env, type, obj, jmid, &jvals[0]); 
	return convert_jvalue_to_mrb_value(mrb, env, type, ret);
}

static mrb_value jcls_get_method(mrb_state *mrb, mrb_value self)
{
	mrb_value name, signature;
	int const argc = mrb_get_args(mrb, "SS", &name, &signature);
	if (2 != argc) {
		return mrb_nil_value();
	}
	jclass_data *data = static_cast<jclass_data*>(mrb_get_datatype(mrb, self, &jcls_type));
	JNIEnv *env = data->env;
	safe_jni::clear_exception ce(env);
	char const * const sig = mrb_string_value_ptr(mrb, signature);
	jmethodID jmid = env->GetMethodID(data->gref_jclass, mrb_string_value_ptr(mrb, name), sig);
	if (NULL == jmid) {
		return mrb_nil_value();
	}
	return jmethod_make(mrb, env, jmid, sig);
}

static mrb_value jcls_get_static_method(mrb_state *mrb, mrb_value self)
{
	mrb_value name, signature;
	int const argc = mrb_get_args(mrb, "SS", &name, &signature);
	if (2 != argc) {
		return mrb_nil_value();
	}
	jclass_data *data = static_cast<jclass_data*>(mrb_get_datatype(mrb, self, &jcls_type));
	JNIEnv *env = data->env;
	safe_jni::clear_exception ce(env);
	char const * const sig = mrb_string_value_ptr(mrb, signature);
	jmethodID jmid = env->GetStaticMethodID(data->gref_jclass, mrb_string_value_ptr(mrb, name), sig);
	if (NULL == jmid) {
		return mrb_nil_value();
	}
	return jmethod_make(mrb, env, jmid, sig);
}

static mrb_value jcls_get_class_object(mrb_state *mrb, mrb_value self)
{
	jclass_data *data = static_cast<jclass_data*>(mrb_get_datatype(mrb, self, &jcls_type));
	JNIEnv *env = data->env;
	safe_jni::safe_local_ref<jclass> objCls(env, env->GetObjectClass(data->gref_jclass));
	return jobject_make(mrb, env, objCls.get());
}

static mrb_value jcls_initialize(mrb_state *mrb, mrb_value self)
{
	return mrb_nil_value();
}

int jcls_init_class(mrb_state *mrb)
{
	RClass *cls_jobj = mrb_class_get(mrb, SUPER_CLASS_NAME);
	RClass *cls_jcls = mrb_define_class(mrb, CLASS_NAME, cls_jobj);
	if (NULL == cls_jcls) {
		return -1;
	}
	MRB_SET_INSTANCE_TT(cls_jcls, MRB_TT_DATA);
	mrb_define_method(mrb, cls_jcls, "get_method", jcls_get_method, ARGS_REQ(2));
	mrb_define_method(mrb, cls_jcls, "get_static_method", jcls_get_static_method, ARGS_REQ(2));
	mrb_define_method(mrb, cls_jcls, "get_class_object", jcls_get_class_object, ARGS_NONE());
	mrb_define_method(mrb, cls_jcls, "call_static", jcls_call_static, ARGS_REQ(1));
	mrb_define_method(mrb, cls_jcls, "call", jcls_call, ARGS_REQ(2));
	mrb_define_method(mrb, cls_jcls, "initialize", jcls_initialize, ARGS_REQ(1));
	return 0;
}

