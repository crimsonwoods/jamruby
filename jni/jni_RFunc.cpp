#include "jni_RFunc.h"
#include <map>
#include <jni.h>

mrb_value jamruby_func_dispatch(mrb_state *mrb, mrb_value self) {
	// TODO lookup 'JNIEnv' object from thread id table.
	// TODO lookup global reference to Java object.
	// TODO lookup method name and method body.
	// TODO dispatch Java object and call method.
	return mrb_nil_value();
}
