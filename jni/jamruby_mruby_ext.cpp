#include "jamruby_mruby_ext.h"
#include "jamruby_mruby_utils.h"
#include "jni_Log.h"

mrb_value jamruby_kernel_require(mrb_state *mrb, mrb_value self)
{
	LOGD("'Jamruby.Kernel.require' is called instead of '%s.%s'.",
		mrb_class_name(mrb, get_called_mrb_class(mrb)), get_called_mrb_func_name(mrb));

	// TODO implement.

	return mrb_nil_value();
}

