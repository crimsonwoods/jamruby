#include "jamruby_JObject.h"
#include "safe_jni.hpp"
extern "C" {
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"
}

#include "jamruby_JClass.h"

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

	return mrb_str_new2(mrb, str.string());
}

static mrb_value jobject_initialize(mrb_state *mrb, mrb_value self)
{
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
	mrb_define_method(mrb, cls_jobj, "initialize", jobject_initialize, ARGS_REQ(1));
	return 0;
}

