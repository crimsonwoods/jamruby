#include "jamruby_JObject.h"
#include "safe_jni.hpp"
extern "C" {
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"
#include "mruby/array.h"
}

#include "jamruby_JClass.h"
#include "jamruby_Context.h"
#include "jamruby_jni_method_call.h"
#include "jamruby_MethodResolver.h"

#include "jni_Log.h"

#include <vector>

#define CLASS_NAME "JObject"

struct jobject_data {
	JNIEnv *env;
	jobject gref_jobj;
};

static void jobject_free(mrb_state *mrb, void *ptr)
{
    if (NULL != ptr) {
        jobject_data *p = static_cast<jobject_data*>(ptr);
        p->env->DeleteGlobalRef(p->gref_jobj);
        p->env = NULL;
        p->gref_jobj = NULL;
    }
    mrb_free(mrb, ptr);
}

static struct mrb_data_type jobject_type = { CLASS_NAME, jobject_free };

mrb_value jobject_make(mrb_state *mrb, JNIEnv *env, jobject obj)
{
	safe_jni::clear_exception ce(env);
	RClass *c = mrb_class_get(mrb, CLASS_NAME);
	if (NULL == c) {
		return mrb_nil_value();
	}
	jobject_data *ptr = static_cast<jobject_data*>(mrb_malloc(mrb, sizeof(jobject_data)));
	if (NULL == ptr) {
		return mrb_nil_value();
	}
	ptr->env = env;
	ptr->gref_jobj = env->NewGlobalRef(obj);
	return mrb_obj_value(Data_Wrap_Struct(mrb, c, &jobject_type, ptr));
}

bool jobject_is_jobject(mrb_state *mrb, mrb_value obj)
{
	jobject_data *data = static_cast<jobject_data*>(mrb_get_datatype(mrb, obj, &jobject_type));
	return NULL == data ? false : true;
}

jobject jobject_get_jobject(mrb_state *mrb, mrb_value obj)
{
	jobject_data *data = static_cast<jobject_data*>(mrb_get_datatype(mrb, obj, &jobject_type));
	if (NULL == data) {
		return NULL;
	}
	return data->gref_jobj;
}

static mrb_value jobject_jclass(mrb_state *mrb, mrb_value self)
{
	jobject_data *data = static_cast<jobject_data*>(mrb_get_datatype(mrb, self, &jobject_type));
	JNIEnv *env = data->env;
	safe_jni::safe_local_ref<jclass> cls(env, env->GetObjectClass(data->gref_jobj));
	return jcls_make(mrb, env, cls.get());
}

static mrb_value jobject_class(mrb_state *mrb, mrb_value self)
{
	jobject_data *data = static_cast<jobject_data*>(mrb_get_datatype(mrb, self, &jobject_type));
	JNIEnv *env = data->env;

	safe_jni::safe_local_ref<jclass> cls(env, env->GetObjectClass(data->gref_jobj));
	jmethodID jmid = env->GetMethodID(cls.get(), "getClass", "()Ljava/lang/Class;");
	if (NULL == jmid) {
		return mrb_nil_value();
	}
	safe_jni::safe_local_ref<jobject> cls_obj(env, env->CallObjectMethod(data->gref_jobj, jmid));
	return jobject_make(mrb, env, cls_obj.get());
}

static mrb_value jobject_to_s(mrb_state *mrb, mrb_value self)
{
	jobject_data *data = static_cast<jobject_data*>(mrb_get_datatype(mrb, self, &jobject_type));
	JNIEnv *env = data->env;

	safe_jni::safe_local_ref<jclass> cls(env, env->GetObjectClass(data->gref_jobj));
	jmethodID jmid = env->GetMethodID(cls.get(), "toString", "()Ljava/lang/String;");
	if (NULL == jmid) {
		return mrb_nil_value();
	}

	safe_jni::safe_local_ref<jstring> s(env, static_cast<jstring>(env->CallObjectMethod(data->gref_jobj, jmid)));
	safe_jni::safe_string str(env, s.get());

	return mrb_str_new_cstr(mrb, str.string());
}

static mrb_value jobject_initialize(mrb_state *mrb, mrb_value self)
{
	jobject_data *data = static_cast<jobject_data*>(mrb_malloc(mrb, sizeof(jobject_data)));
	if (NULL == data) {
		mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
	}

	org::jamruby::jamruby_context *context = org::jamruby::jamruby_context::find_context(mrb);
	if (NULL == context) {
		mrb_free(mrb, data);
		mrb_raise(mrb, E_RUNTIME_ERROR, "jamruby context cannot found.");
	}

	JNIEnv * const env = context->get_jni_env();
	mrb_int argc;
	mrb_value *argv;
	mrb_get_args(mrb, "*", &argv, &argc);

	org::jamruby::jamruby_context::signatures_t signatures =
		context->find_ctor_signatures(mrb_obj_class(mrb, self));
	if (signatures.empty()) {
		mrb_free(mrb, data);
		mrb_raise(mrb, E_RUNTIME_ERROR, "constructor is not registered.");
	}

	org::jamruby::method_resolver resolver;
	std::string const &sig = resolver.resolve_ambiguous(mrb, signatures, argc, argv);
	if (sig.empty()) {
		mrb_free(mrb, data);
		mrb_raise(mrb, E_RUNTIME_ERROR, "cannot resolve constructor.");
	}

	jclass gref_cls = context->find_jclass(mrb_obj_class(mrb, self));
	jmethodID mid = env->GetMethodID(gref_cls, "<init>", sig.c_str());

	std::vector<jvalue> jvals(argc);
	std::vector<jni_type_t> types(argc);
	if (!org::jamruby::get_argument_types(sig.c_str(), &types[0], argc)) {
		mrb_free(mrb, data);
		mrb_raise(mrb, E_RUNTIME_ERROR, "invalid signature format.");
	}

	for (int i = 0; i < argc; ++i) {
		org::jamruby::convert_mrb_value_to_jvalue(mrb, env, argv[i], jvals[i], types[i]);
	}

	jobject obj = org::jamruby::call_ctor(mrb, env, gref_cls, mid, &jvals[0]);

	data->env = context->get_jni_env();
	data->gref_jobj = env->NewGlobalRef(obj);
	DATA_TYPE(self) = &jobject_type;
	DATA_PTR(self) = data;

	return mrb_nil_value();
}

int jobject_init_class(mrb_state *mrb)
{
	RClass *cls_jobj = mrb_define_class(mrb, "JObject", NULL);
	if (NULL == cls_jobj) {
		return -1;
	}
	MRB_SET_INSTANCE_TT(cls_jobj, MRB_TT_DATA);
	mrb_define_method(mrb, cls_jobj, "jclass",     jobject_jclass,     ARGS_NONE());
	mrb_define_method(mrb, cls_jobj, "class",      jobject_class,      ARGS_NONE());
	mrb_define_method(mrb, cls_jobj, "to_s",       jobject_to_s,       ARGS_NONE());
	mrb_define_method(mrb, cls_jobj, "initialize", jobject_initialize, ARGS_ANY());
	return 0;
}

