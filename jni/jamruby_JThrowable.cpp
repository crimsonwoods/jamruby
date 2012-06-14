#include "jamruby_JThrowable.h"
#include "jni_Log.h"

int jthrowable_init_class(mrb_state *mrb)
{
	RClass *exception_class = mrb_class_obj_get(mrb, "Exception");
	if (NULL == exception_class) {
		LOGE("Class 'Exception' is not registered.");
		return -1;
	}
	RClass *java_error_class = mrb_define_class(mrb, "JavaError",  exception_class);
	RClass *jthrowable_class = mrb_define_class(mrb, "JThrowable", java_error_class);
	mrb_define_class(mrb, "JException", jthrowable_class);
	mrb_define_class(mrb, "JError",     jthrowable_class);
	return 0;
}
