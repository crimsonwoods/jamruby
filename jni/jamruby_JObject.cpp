#include "jamruby_JObject.h"
extern "C" {
#include "mruby/class.h"
}

mrb_value jobject_get_class(mrb_state *mrb, mrb_value self)
{
	return mrb_nil_value();
}

mrb_value jobject_call(mrb_state *mrb, mrb_value self)
{
	return mrb_nil_value();
}

mrb_value jobject_initialize(mrb_state *mrb, mrb_value self)
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
	mrb_define_method(mrb, cls_jobj, "get_jclass", jobject_get_class, ARGS_NONE());
	mrb_define_method(mrb, cls_jobj, "call", jobject_call, ARGS_REQ(2));
	mrb_define_method(mrb, cls_jobj, "initialize", jobject_initialize, ARGS_REQ(1));
	return 0;
}

