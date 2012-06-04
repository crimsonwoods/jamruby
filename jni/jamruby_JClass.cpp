#include "jamruby_JClass.h"
#include "safe_jni.hpp"
#include "jni_Log.h"
extern "C" {
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"
}
#include <cstdlib>
#include <cstring>

#include "jamruby_JMethod.h"

#define CLASS_NAME "JClass"

struct jclass_data {
	JNIEnv *env;
	jclass gref_jclass;
	char const *name;
};

static void jcls_free(mrb_state *mrb, void *ptr)
{
	if (NULL != ptr) {
		jclass_data *p = static_cast<jclass_data*>(ptr);
		p->env->DeleteGlobalRef(p->gref_jclass);
		free((void *)p->name);
		p->env = NULL;
		p->gref_jclass = NULL;
		p->name = NULL;
	}
	mrb_free(mrb, ptr);
}

static struct mrb_data_type jcls_type = { CLASS_NAME, jcls_free };

mrb_value jcls_make(mrb_state *mrb, JNIEnv *env, char const * const name)
{
	safe_jni::clear_exception ce(env);
	safe_jni::safe_local_ref<jclass> jcls(env, env->FindClass(name));
	if (!jcls) {
		return mrb_nil_value();
	}
	RClass *c = mrb_class_get(mrb, CLASS_NAME);
	if (NULL == c) {
		return mrb_nil_value();
	}
	jclass_data *ptr = static_cast<jclass_data*>(mrb_malloc(mrb, sizeof(jclass_data)));
	if (NULL == ptr) {
		return mrb_nil_value();
	}
	char const *dup = strdup(name);
	if (NULL == dup) {
		mrb_free(mrb, ptr);
		return mrb_nil_value();
	}
	ptr->env = env;
	ptr->gref_jclass = static_cast<jclass>(env->NewGlobalRef(jcls.get()));
	ptr->name = dup;
	return mrb_obj_value(Data_Wrap_Struct(mrb, c, &jcls_type, ptr));
}

mrb_value jcls_call_static(mrb_state *mrb, mrb_value self)
{
	return mrb_nil_value();
}

mrb_value jcls_to_s(mrb_state *mrb, mrb_value self)
{
	jclass_data *data = static_cast<jclass_data*>(mrb_get_datatype(mrb, self, &jcls_type));
	return mrb_str_new2(mrb, data->name);
}

mrb_value jcls_get_method(mrb_state *mrb, mrb_value self)
{
	mrb_value name, signature;
	int const argc = mrb_get_args(mrb, "SS", &name, &signature);
	if (2 != argc) {
		return mrb_nil_value();
	}
	jclass_data *data = static_cast<jclass_data*>(mrb_get_datatype(mrb, self, &jcls_type));
	JNIEnv *env = data->env;
	safe_jni::clear_exception ce(env);
	jmethodID jmid = env->GetMethodID(data->gref_jclass, mrb_string_value_ptr(mrb, name), mrb_string_value_ptr(mrb, signature));
	if (NULL == jmid) {
		return mrb_nil_value();
	}
	return jmethod_make(mrb, env, jmid);
}

mrb_value jcls_get_static_method(mrb_state *mrb, mrb_value self)
{
	mrb_value name, signature;
	int const argc = mrb_get_args(mrb, "SS", &name, &signature);
	if (2 != argc) {
		return mrb_nil_value();
	}
	jclass_data *data = static_cast<jclass_data*>(mrb_get_datatype(mrb, self, &jcls_type));
	JNIEnv *env = data->env;
	safe_jni::clear_exception ce(env);
	jmethodID jmid = env->GetStaticMethodID(data->gref_jclass, mrb_string_value_ptr(mrb, name), mrb_string_value_ptr(mrb, signature));
	if (NULL == jmid) {
		return mrb_nil_value();
	}
	return jmethod_make(mrb, env, jmid);
}

mrb_value jcls_initialize(mrb_state *mrb, mrb_value self)
{
	return mrb_nil_value();
}

int jcls_init_class(mrb_state *mrb)
{
	RClass *cls_jobj = mrb_class_get(mrb, "JObject");
	RClass *cls_jcls = mrb_define_class(mrb, CLASS_NAME, cls_jobj);
	if (NULL == cls_jcls) {
		return -1;
	}
	MRB_SET_INSTANCE_TT(cls_jcls, MRB_TT_DATA);
	mrb_define_method(mrb, cls_jcls, "get_method", jcls_get_method, ARGS_REQ(2));
	mrb_define_method(mrb, cls_jcls, "get_static_method", jcls_get_static_method, ARGS_REQ(2));
	mrb_define_method(mrb, cls_jcls, "call_static", jcls_call_static, ARGS_REQ(1));
	mrb_define_method(mrb, cls_jcls, "to_s", jcls_to_s, ARGS_NONE());
	mrb_define_method(mrb, cls_jcls, "initialize", jcls_initialize, ARGS_REQ(1));
	return 0;
}

