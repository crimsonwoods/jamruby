#include "jamruby_JMethod.h"
#include "jamruby_jni_method_call.h"
#include "safe_jni.hpp"
#include "jni_Log.h"
extern "C" {
#include "mruby/data.h"
}

#define CLASS_NAME "JMethod"

struct jmethod_data {
	JNIEnv *env;
	jmethodID jmid;
	char *signature;
};

static void jmethod_free(mrb_state *mrb, void *ptr)
{
    if (NULL != ptr) {
        jmethod_data *p = static_cast<jmethod_data*>(ptr);
		free(p->signature);
        p->env = NULL;
        p->jmid = NULL;
		p->signature = NULL;
    }
    mrb_free(mrb, ptr);
}

static struct mrb_data_type jmethod_type = { CLASS_NAME, jmethod_free };

mrb_value jmethod_make(mrb_state *mrb, JNIEnv *env, jmethodID jmid, char const * const sig)
{
	safe_jni::clear_exception ce(env);
	RClass *c = mrb_class_get(mrb, CLASS_NAME);
	if (NULL == c) {
		return mrb_nil_value();
	}
	jmethod_data *ptr = static_cast<jmethod_data*>(mrb_malloc(mrb, sizeof(jmethod_data)));
	if (NULL == ptr) {
		return mrb_nil_value();
	}
	char * const dup = strdup(sig);
	if (NULL == dup) {
		mrb_free(mrb, ptr);
		ptr = NULL;
		return mrb_nil_value();
	}
	ptr->env = env;
	ptr->jmid = jmid;
	ptr->signature = dup;
	return mrb_obj_value(Data_Wrap_Struct(mrb, c, &jmethod_type, ptr));
}

jmethodID jmethod_get_jmethodID(mrb_state *mrb, mrb_value self)
{
	jmethod_data* data = static_cast<jmethod_data*>(mrb_get_datatype(mrb, self, &jmethod_type));
	if (NULL == data) {
		return NULL;
	}
	return data->jmid;
}

bool jmethod_is_jmethod(mrb_state *mrb, mrb_value obj)
{
	jmethod_data* data = static_cast<jmethod_data*>(mrb_get_datatype(mrb, obj, &jmethod_type));
	return NULL == data ? false : true;
}

jni_type_t jmethod_get_return_type(mrb_state *mrb, mrb_value obj)
{
	jmethod_data* data = static_cast<jmethod_data*>(mrb_get_datatype(mrb, obj, &jmethod_type));
	char const * const sig = data->signature;
	if (NULL == sig) {
		LOGE("%s: Signature is null.", __func__);
		return jni_type_t();
	}
	return org::jamruby::get_return_type(sig);
}

bool jmethod_get_argument_types(mrb_state *mrb, mrb_value obj, jni_type_t *types, int num)
{
	jmethod_data* data = static_cast<jmethod_data*>(mrb_get_datatype(mrb, obj, &jmethod_type));
	char const * const sig = data->signature;
	if (NULL == sig) {
		LOGE("%s: Signature is null.", __func__);
		return false;
	}
	return org::jamruby::get_argument_types(sig, types, num);
}

static mrb_value jmethod_initialize(mrb_state *mrb, mrb_value self)
{
	return mrb_nil_value();
}

int jmethod_init_class(mrb_state *mrb)
{
	RClass *cls = mrb_define_class(mrb, CLASS_NAME, NULL);
	if (NULL == cls) {
		return -1;
	}
	mrb_define_method(mrb, cls, "initialize", jmethod_initialize, ARGS_REQ(1));
	return 0;
}

