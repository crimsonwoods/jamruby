#include "jamruby_mruby_ext.h"
#include "jamruby_mruby_utils.h"
#include "jamruby_Context.h"
#include "jamruby_jni_method_call.h"
#include "jamruby_jni_functor.h"
#include "jni_Log.h"
#include "safe_jni.hpp"
#include <cstring>
extern "C" {
#include "mruby.h"
#include "mruby/string.h"
#include "mruby/variable.h"
#include "mruby/class.h"
}
#include <string>
#include <vector>
#include <algorithm>

namespace org {
namespace jamruby {

static std::string gen_java_class_name(std::string const &name, int const &len);
static void export_jclass(mrb_state *mrb, JNIEnv *env, jclass cls, std::string const &name);
static void define_class(mrb_state *mrb, JNIEnv *env, RClass *parent, jclass cls, std::string const &name);
static mrb_value java_class_method(mrb_state *mrb, mrb_value self);
static mrb_value java_object_method(mrb_state *mrb, mrb_value self);

};
};

mrb_value jamruby_kernel_require(mrb_state *mrb, mrb_value self)
{
	using namespace org::jamruby;

	LOGD("'Jamruby.Kernel.require' is called instead of '%s.%s'.",
		mrb_class_name(mrb, get_called_mrb_class(mrb)), get_called_mrb_func_name(mrb));

	mrb_value name;
	int const argc = mrb_get_args(mrb, "S", &name);
	if (1 != argc) {
		mrb_raisef(mrb, E_ARGUMENT_ERROR, "wrong number of arguments (%d for 1)", argc);
		return mrb_nil_value(); // don't reach here
	}

	if (mrb_type(name) != MRB_TT_STRING) {
		name = mrb_funcall(mrb, name, "to_s", 0);
	}

	char const * const str = RSTRING_PTR(name);
	int const len = RSTRING_LEN(name);

	if (len > 3) {
		// if 'name' is end with ".rb", loading ruby script file.
		if (0 == strncmp(&str[len-3], ".rb", 3)) {
			// TODO call original 'Kernel.require' method.
			return mrb_nil_value();
		}
	}

	jamruby_context *context = jamruby_context::find_context(mrb);
	if (NULL == context) {
		mrb_raise(mrb, E_RUNTIME_ERROR, "jamruby context cannot found.");
		return mrb_nil_value(); // don't reach here
	}

	std::string const class_name = gen_java_class_name(str, len);

	JNIEnv *env = context->get_jni_env();
	safe_jni::safe_local_ref<jclass> cls(env, env->FindClass(class_name.c_str()));
	if (!cls) {
		env->ExceptionClear();
		// TODO call original 'Kernel.require' method.
		mrb_raisef(mrb, E_NAME_ERROR, "class '%.*s' is not found in JVM.", len, str);
		return mrb_nil_value(); // don't reach here
	}

	export_jclass(mrb, env, cls.get(), class_name);

	return mrb_nil_value();
}

namespace org {
namespace jamruby {

static bool is_dot(char const &c)
{
	return '.' == c ? true : false;
}

static std::string gen_java_class_name(std::string const &name, int const &len)
{
	std::string copied;
	copied.resize(len+1);
	std::replace_copy_if(name.begin(), name.end(), copied.begin(), is_dot, '/');
	copied[len] = '\0';
	return copied;
}

static void export_jclass(mrb_state *mrb, JNIEnv *env, jclass cls, std::string const &name)
{
	std::string::size_type ofst = 0;
	RClass *parent = NULL;
	LOGD("export java class '%s'.", name.c_str());
	for (;;) {
		std::string::size_type const n = name.find_first_of('/', ofst);
		std::string mod_name = name.substr(ofst, n - ofst);
		if (std::islower(mod_name[0])) {
			mod_name[0] = std::toupper(mod_name[0]);
		}
		if (std::string::npos == n) {
			define_class(mrb, env, parent, cls, mod_name);
			break;
		}
		ofst = n + 1;

		RClass *mod;
		mrb_sym const sym = mrb_intern(mrb, mod_name.c_str());
		if (NULL == parent) {
			if (mrb_const_defined_at(mrb, mrb->object_class, sym)) {
				mod = mrb_class_get(mrb, mod_name.c_str());
			} else {
				LOGD("define module (%s)\n", mod_name.c_str());
				mod = mrb_define_module(mrb, mod_name.c_str());
			}
		} else {
			if (mrb_const_defined_at(mrb, parent, sym)) {
				mod = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(parent), sym));
			} else {
				LOGD("define module (%s::%s)\n", mrb_string_value_ptr(mrb, mrb_class_path(mrb, parent)), mod_name.c_str());
				mod = mrb_define_module_under(mrb, parent, mod_name.c_str());
			}
		}
		if (NULL == mod) {
			// TODO error handling
			LOGE("failed to define module (%s)\n", mod_name.c_str());
			return;
		}
		parent = mod;
	}
}

static bool is_method_defined(mrb_state *mrb, RClass *target, char const * const name)
{
	RClass *c = target;
	RProc *proc = mrb_method_search_vm(mrb, &c, mrb_intern(mrb, name));
	if ((c == target) && (NULL != proc)) {
		return true;
	}
	return false;
}

static bool is_class_method_defined(mrb_state *mrb, RClass *target, char const * const name)
{
	return is_method_defined(mrb, target->c, name);
}

static void define_class(mrb_state *mrb, JNIEnv *env, RClass *parent, jclass cls, std::string const &name)
{
	LOGD("define class (%s::%s)\n", (NULL == parent) ? "" : mrb_string_value_ptr(mrb, mrb_class_path(mrb, parent)), name.c_str());

	// TODO resolve java class inheritance hierarchy and get the super class.
	RClass *jobject_class = mrb_class_get(mrb, "JObject");
	RClass *target;
	if (NULL == parent) {
		target = mrb_define_class(mrb, name.c_str(), jobject_class);
	} else {
		target = mrb_define_class_under(mrb, parent, name.c_str(), jobject_class);
	}
	if (NULL == target) {
		// TODO error handling.
		LOGE("cannot define the class '%s'.", name.c_str());
		return;
	}

	// get public methods
	jobject obj = static_cast<jobject>(cls);
	safe_jni::method<jobjectArray> get_methods(env, obj, "getMethods", "()[Ljava/lang/reflect/Method;");
	if (!get_methods) {
		env->ExceptionClear();
		LOGE("cannot find 'getMethods()' method in JVM.");
		return;
	}

	safe_jni::safe_local_ref<jobjectArray> ret(env, get_methods(obj));
	if (!ret) {
		env->ExceptionClear();
		LOGE("cannot get public methods.");
		return;
	}
	safe_jni::safe_object_array methods(env, ret.get());
	size_t const num_of_methods = methods.size();
	if (0 == num_of_methods) {
		LOGW("'%s' has no methods.", name.c_str());
		return;
	}

	safe_jni::safe_local_ref<jclass> modifier_class(env, env->FindClass("java/lang/reflect/Modifier"));
	if (!modifier_class) {
		LOGE("cannot find class 'java.lang.reflect.Modifier' in JVM.");
		return;
	}
	safe_jni::method<bool> is_static(env, modifier_class.get(), "isStatic", "(I)Z");
	if (!is_static) {
		LOGE("cannot find method 'isStatic'.");
		return;
	}

	safe_jni::safe_local_ref<jclass> method_signature_class(env, env->FindClass("org/jamruby/java/MethodSignature"));
	if (!method_signature_class) {
		LOGE("cannot find class 'org.jamruby.java.MethodSignature' in JVM.");
		return;
	}
	safe_jni::method<jstring> gen_method_signature(env, method_signature_class.get(), "genMethodSignature", "(Ljava/lang/reflect/Method;)Ljava/lang/String;");
	if (!gen_method_signature) {
		LOGE("cannot find method 'genMethodSignature'.");
		return;
	}

	jamruby_context *context = jamruby_context::find_context(mrb);
	if (NULL == context) {
		LOGE("cannot find jamruby context.");
		return;
	}

	jclass gref_cls = static_cast<jclass>(env->NewGlobalRef(cls));
	if (NULL == gref_cls) {
		LOGE("cannot create global reference.");
		return;
	}

	context->register_jclass(target, gref_cls);

	for (size_t i = 0; i < num_of_methods; ++i) {
		safe_jni::safe_local_ref<jobject> m(env, methods.get(i));
		safe_jni::method<jstring> get_name(env, m.get(), "getName", "()Ljava/lang/String;");
		safe_jni::safe_local_ref<jstring> mname(env, get_name(m.get()));
		safe_jni::safe_string mname_str(env, mname.get());

		safe_jni::safe_local_ref<jstring> js_signature(env, gen_method_signature(method_signature_class.get(), m.get()));
		safe_jni::safe_string signature(env, js_signature.get());

		safe_jni::method<int> get_modifiers(env, m.get(), "getModifiers", "()I");
		if (!get_modifiers) {
			LOGW("cannot find 'getModifiers' method.");
			break;
		}
		int const modifiers = get_modifiers(m.get());
		if (is_static(modifier_class.get(), modifiers)) {
			if (is_class_method_defined(mrb, target, mname_str.string())) {
				// already exported.
				continue;
			}

			context->register_method_signature(true, target, mname_str.string(), signature.string());

			LOGD("define class method '%s::%s : %s'.", name.c_str(), mname_str.string(), signature.string());
			mrb_define_class_method(mrb, target, mname_str.string(), java_class_method, ARGS_ANY());
		} else {
			if (is_method_defined(mrb, target, mname_str.string())) {
				// already exported.
				continue;
			}

			context->register_method_signature(false, target, mname_str.string(), signature.string());

			LOGD("define instance method '%s.%s : %s'.", name.c_str(), mname_str.string(), signature.string());
			mrb_define_method(mrb, target, mname_str.string(), java_object_method, ARGS_ANY());
		}
	}
}

static mrb_value java_class_method(mrb_state *mrb, mrb_value self)
{
	try {
		if (mrb_type(self) == MRB_TT_CLASS) {
			RClass *cls = mrb_class_ptr(self);
			jamruby_context *context = jamruby_context::find_context(mrb);
			if (NULL == context) {
				mrb_raise(mrb, E_RUNTIME_ERROR, "jamruby context is not found.");
				return mrb_nil_value(); // don't reach here.
			}

			std::string const &sig = context->find_method_signature(true, cls, get_called_mrb_func_name(mrb));
			if (sig.empty()) {
				mrb_raise(mrb, E_RUNTIME_ERROR, "JVM method signature is not found.");
				return mrb_nil_value(); // don't reach here.
			}

			jclass jcls = context->find_jclass(cls);
			if (NULL == jcls) {
				mrb_raise(mrb, E_RUNTIME_ERROR, "class object in JVM is not found.");
				return mrb_nil_value(); // don't reach here.
			}

			JNIEnv *env = context->get_jni_env();
			jmethodID jmid = env->GetStaticMethodID(jcls, get_called_mrb_func_name(mrb), sig.c_str());
			if (NULL == jmid) {
				LOGE("failed to get method %s::%s - %s", mrb_class_name(mrb, cls), get_called_mrb_func_name(mrb), sig.c_str());
				env->ExceptionClear();
				mrb_raise(mrb, E_RUNTIME_ERROR, "method in JVM is not found.");
				return mrb_nil_value(); // don't reach here.
			}

			mrb_value *rb_argv = NULL;
			int rb_argc = 0;
			mrb_get_args(mrb, "*", &rb_argv, &rb_argc);

			int const argc = get_count_of_arguments(sig.c_str());
			if (rb_argc != argc) {
				mrb_raise(mrb, E_ARGUMENT_ERROR, "number of argument is not match.");
				return mrb_nil_value(); // don't reach here.
			}

			jni_type_t ret_type = get_return_type(sig.c_str());
			std::vector<jvalue> jvals(argc);
			std::vector<jni_type_t> types(argc);
			if (!get_argument_types(sig.c_str(), &types[0], argc)) {
				mrb_raise(mrb, E_RUNTIME_ERROR, "invalid signature format.");
				return mrb_nil_value(); // don't reach here.
			}

			for (int i = 0; i < argc; ++i) {
				convert_mrb_value_to_jvalue(mrb, context->get_jni_env(), rb_argv[i], jvals[i], types[i]);
			}

			jvalue const &ret = call_method(mrb, context->get_jni_env(), ret_type, jcls, jmid, &jvals[0]);
			return convert_jvalue_to_mrb_value(mrb, context->get_jni_env(), ret_type, ret);
		} else {
			mrb_raise(mrb, E_ARGUMENT_ERROR, "argument type must be class type.");
		}
	} catch (std::exception& e) {
		mrb_raise(mrb, E_RUNTIME_ERROR, e.what());
	}
	return mrb_nil_value();
}

static mrb_value java_object_method(mrb_state *mrb, mrb_value self)
{
	LOGD("%s:(%u,%s - %s::%s)", __func__, self.tt, mrb_obj_classname(mrb, self), mrb_class_name(mrb, get_called_mrb_class(mrb)), get_called_mrb_func_name(mrb));
	return mrb_nil_value();
}

};
};
