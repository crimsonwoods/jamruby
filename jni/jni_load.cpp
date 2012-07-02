#include <jni.h>
#include "jamruby_jni_method_call.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	org::jamruby::init_converters();
	return JNI_VERSION_1_2;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	// nothing to do
}

