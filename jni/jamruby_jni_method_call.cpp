#include "jamruby_jni_method_call.h"
#include "jamruby_JObject.h"
#include "jni_Log.h"
#include "jni_type_conversion.hpp"
extern "C" {
#include "mruby/string.h"
}
#include <map>

namespace org {
namespace jamruby {

typedef mrb_value (*jvalue_to_mrb_value)(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
typedef std::map<std::string, jvalue_to_mrb_value> j2m_converter_map_t;
static j2m_converter_map_t j2m_converters;

static mrb_value Boolean_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
static mrb_value Character_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
static mrb_value Byte_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
static mrb_value Short_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
static mrb_value Integer_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
static mrb_value Long_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
static mrb_value Double_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
static mrb_value Float_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
static mrb_value Object_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);
static mrb_value String_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name);


void init_converters()
{
	if (0 != j2m_converters.size()) {
		return;
	}
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/Boolean",   Boolean_to_mrb));
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/Character", Character_to_mrb));
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/Byte",      Byte_to_mrb));
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/Short",     Short_to_mrb));
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/Integer",   Integer_to_mrb));
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/Long",      Long_to_mrb));
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/Double",    Double_to_mrb));
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/Float",     Float_to_mrb));
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/Object",    Object_to_mrb));
	j2m_converters.insert(j2m_converter_map_t::value_type("java/lang/String",    String_to_mrb));
}

bool convert_mrb_value_to_jvalue(mrb_state *mrb, JNIEnv *env, mrb_value rval, jvalue &jval, jni_type_t const &type)
{
	switch(mrb_type(rval)) {
	case MRB_TT_FALSE:
		if (type.is_array()) {
			if (mrb_nil_p(rval)) {
				LOGD("MRB_TT_FALSE(NIL) -> JNI_TYPE_ARRAY");
				jval.l = NULL;
			} else {
				return false;
			}
		}
		switch(type.type_id()) {
		case JNI_TYPE_BOOLEAN:
			LOGD("MRB_TT_FALSE -> JNI_TYPE_BOOLEAN");
			jval.z = JNI_FALSE;
			break;
		case JNI_TYPE_OBJECT:
			if (mrb_nil_p(rval)) {
				jval.l = NULL;
			} else {
				return false;
			}
			break;
		default:
			return false;
		}
		break;
	case MRB_TT_TRUE:
		switch(type.type_id()) {
		case JNI_TYPE_BOOLEAN:
			LOGD("MRB_TT_TRUE -> JNI_TYPE_BOOLEAN");
			jval.z = JNI_TRUE;
			break;
		default:
			return false;
		}
		break;
	case MRB_TT_FIXNUM:
		switch(type.type_id()) {
		case JNI_TYPE_BYTE:
			LOGD("MRB_TT_FIXNUM -> JNI_TYPE_BYTE");
			jval.b = static_cast<jbyte>(mrb_fixnum(rval));
			break;
		case JNI_TYPE_CHAR:
			LOGD("MRB_TT_FIXNUM -> JNI_TYPE_CHAR");
			jval.c = static_cast<jchar>(mrb_fixnum(rval));
			break;
		case JNI_TYPE_SHORT:
			LOGD("MRB_TT_FIXNUM -> JNI_TYPE_SHORT");
			jval.s = static_cast<jshort>(mrb_fixnum(rval));
			break;
		case JNI_TYPE_INT:
			LOGD("MRB_TT_FIXNUM -> JNI_TYPE_INT");
			jval.i = mrb_fixnum(rval);
			break;
		case JNI_TYPE_LONG:
			LOGD("MRB_TT_FIXNUM -> JNI_TYPE_LONG");
			jval.j = mrb_fixnum(rval);
			break;
		case JNI_TYPE_FLOAT:
			LOGD("MRB_TT_FIXNUM -> JNI_TYPE_FLOAT");
			jval.f = static_cast<jfloat>(mrb_fixnum(rval));
			break;
		case JNI_TYPE_DOUBLE:
			LOGD("MRB_TT_FIXNUM -> JNI_TYPE_DOUBLE");
			jval.d = static_cast<jdouble>(mrb_fixnum(rval));
			break;
		default:
			return false;
		}
		break;
	case MRB_TT_FLOAT:
		switch(type.type_id()) {
		case JNI_TYPE_FLOAT:
			LOGD("MRB_TT_FLOAT -> JNI_TYPE_FLOAT");
			jval.f = static_cast<jfloat>(mrb_float(rval));
			break;
		case JNI_TYPE_DOUBLE:
			LOGD("MRB_TT_FLOAT -> JNI_TYPE_DOUBLE");
			jval.d = mrb_float(rval);
			break;
		default:
			return false;
		}
		break;
	case MRB_TT_STRING:
		switch(type.type_id()) {
		case JNI_TYPE_OBJECT:
		{
			LOGD("MRB_TT_STRING -> JNI_TYPE_OBJECT");
			// TODO validate Java object type.
			jval.l = env->NewStringUTF(mrb_string_value_ptr(mrb, rval));
			break;
		}
		default:
			return false;
		}
		break;
	case MRB_TT_DATA:
		switch(type.type_id()) {
		case JNI_TYPE_OBJECT:
			LOGD("MRB_TT_DATA -> JNI_TYPE_OBJECT");
			if (!jobject_is_jobject(mrb, rval)) {
				return false;
			}
			jval.l = jobject_get_jobject(mrb, rval);
			break;
		default:
			return false;
		}
		break;
	default:
		{
		LOGD("MRB_TT_XXX -> JNI_TYPE_OBJECT");
		jval.l = create_value(env, rval);
		}
		break;
	}
	return true;
}

mrb_value convert_jvalue_to_mrb_value(mrb_state *mrb, JNIEnv *env, jni_type_t const &type, jvalue const &ret)
{
	if (type.is_array()) {
		safe_jni::safe_local_ref<jobject> retval(env, ret.l);
		return (NULL == ret.l) ? mrb_nil_value() : jobject_make(mrb, env, retval.get());
	} else {
		switch(type.type_id()) {
		case JNI_TYPE_VOID:
			return mrb_nil_value();
		case JNI_TYPE_BOOLEAN:
			return (ret.z != JNI_FALSE) ? mrb_true_value() : mrb_false_value();
		case JNI_TYPE_BYTE:
			return mrb_fixnum_value(ret.b);
		case JNI_TYPE_CHAR:
			return mrb_fixnum_value(ret.c);
		case JNI_TYPE_SHORT:
			return mrb_fixnum_value(ret.s);
		case JNI_TYPE_INT:
			return mrb_fixnum_value(ret.i);
		case JNI_TYPE_LONG:
			return mrb_fixnum_value((int32_t)ret.j); // TODO if 'Bignum' is supported by mruby, modify this code.
		case JNI_TYPE_FLOAT:
			return mrb_float_value(mrb, ret.f);
		case JNI_TYPE_DOUBLE:
#ifdef MRB_USE_FLOAT
			return mrb_float_value((float)ret.d);
#else
			return mrb_float_value(mrb, ret.d);
#endif
		case JNI_TYPE_OBJECT:
		{
			if (NULL == ret.l) {
				return mrb_nil_value();
			}

			std::string const &name = type.name();

			j2m_converter_map_t::const_iterator it = j2m_converters.find(name);
			if (it == j2m_converters.end()) {
				return Object_to_mrb(mrb, env, ret.l, name);
			} else {
				return it->second(mrb, env, ret.l, name);
			}
		}
		default:
			return mrb_nil_value();
		}
	}
}

jni_type_t get_return_type(char const * const signature)
{
	char const *p = strchr(signature, ')');
	if (NULL == p) {
		LOGE("%s: Invalid signature format.", __func__);
		return jni_type_t();
	}

	jni_type_id_t tid = JNI_TYPE_UNKNOWN;
	bool is_array = false;
	std::string name;
	do {
		switch(*p++) {
		case 'L':
			{
				LOGD("%s: JNI_TYPE_OBJECT", __func__);
				char *next = strchr(p, ';');
				if (NULL == next) {
					return jni_type_t();
				}
				name.assign(p, next - p);
				p = ++next;
				tid = JNI_TYPE_OBJECT;
			}
			break;
		case 'V':
			LOGD("%s: JNI_TYPE_VOID", __func__);
			tid = JNI_TYPE_VOID;
			break;
		case 'Z':
			LOGD("%s: JNI_TYPE_BOOLEAN", __func__);
			tid = JNI_TYPE_BOOLEAN;
			break;
		case 'B':
			LOGD("%s: JNI_TYPE_BYTE", __func__);
			tid = JNI_TYPE_BYTE;
			break;
		case 'C':
			LOGD("%s: JNI_TYPE_CHAR", __func__);
			tid = JNI_TYPE_CHAR;
			break;
		case 'S':
			LOGD("%s: JNI_TYPE_SHORT", __func__);
			tid = JNI_TYPE_SHORT;
			break;
		case 'I':
			LOGD("%s: JNI_TYPE_INT", __func__);
			tid = JNI_TYPE_INT;
			break;
		case 'J':
			LOGD("%s: JNI_TYPE_LONG", __func__);
			tid = JNI_TYPE_LONG;
			break;
		case 'F':
			LOGD("%s: JNI_TYPE_FLOAT", __func__);
			tid = JNI_TYPE_FLOAT;
			break;
		case 'D':
			LOGD("%s: JNI_TYPE_DOUBLE", __func__);
			tid = JNI_TYPE_DOUBLE;
			break;
		case '[':
			is_array = true;
			break;
		}
	} while(tid == JNI_TYPE_UNKNOWN);

	return name.empty() ? jni_type_t(tid, is_array) : jni_type_t(is_array, name.c_str());
}

bool get_argument_types(char const * const signature, jni_type_t *types, int num)
{
	char const *p = strchr(signature, '(');
	if (NULL == p) {
		LOGE("%s: Invalid signature format.", __func__);
		return false;
	}

	for (int i = 0; i < num; ++i) {
		jni_type_id_t tid = JNI_TYPE_UNKNOWN;
		bool is_array = false;
		do {
			switch(*p++) {
			case 'L':
				tid = JNI_TYPE_OBJECT;
				p = strchr(p, ';');
				if (NULL == p) {
					return false;
				}
				++p;
				break;
			case 'V':
				tid = JNI_TYPE_VOID;
				break;
			case 'Z':
				tid = JNI_TYPE_BOOLEAN;
				break;
			case 'B':
				tid = JNI_TYPE_BYTE;
				break;
			case 'C':
				tid = JNI_TYPE_CHAR;
				break;
			case 'S':
				tid = JNI_TYPE_SHORT;
				break;
			case 'I':
				tid = JNI_TYPE_INT;
				break;
			case 'J':
				tid = JNI_TYPE_LONG;
				break;
			case 'F':
				tid = JNI_TYPE_FLOAT;
				break;
			case 'D':
				tid = JNI_TYPE_DOUBLE;
				break;
			case '[':
				is_array = true;
				break;
			case '\0':
			case ')':
				return false;
			}
		} while(tid == JNI_TYPE_UNKNOWN);
		types[i].type_id(tid, is_array);
	}

	return true;
}

int get_count_of_arguments(char const * const signature)
{
	char const *p = strchr(signature, '(');
	if (NULL == p) {
		LOGE("%s: Invalid signature format '%s'.", __func__, signature);
		return false;
	}

	int ret = 0;
	bool is_end = false;
	while(!is_end) {
		switch(*++p) {
		case 'L':
			p = strchr(p, ';');
			if (NULL == p) {
				LOGE("%s: Invalid signature format '%s'.", __func__, signature);
				return -1;
			}
			// fall through
		case 'V':
		case 'Z':
		case 'B':
		case 'C':
		case 'S':
		case 'I':
		case 'J':
		case 'F':
		case 'D':
			++ret;
			break;
		case '[':
			break;
		case ')':
			is_end = true;
			break;
		default:
			LOGE("%s: Invalid signature format '%c:%02x' in '%s'.", __func__, *p, *p, signature);
			return -1;
		}
	}

	return ret;
}

static mrb_value Boolean_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	safe_jni::method<bool> boolean_value(env, value, "booleanValue", "()Z");
	return boolean_value(value) ? mrb_true_value() : mrb_false_value();
}

static mrb_value Character_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	safe_jni::method<uint16_t> char_value(env, value, "charValue", "()C");
	return mrb_fixnum_value(char_value(value));
}

static mrb_value Byte_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	safe_jni::method<int8_t> int_value(env, value, "intValue", "()I");
	return mrb_fixnum_value(int_value(value));
}

static mrb_value Short_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	safe_jni::method<int16_t> int_value(env, value, "intValue", "()I");
	return mrb_fixnum_value(int_value(value));
}

static mrb_value Integer_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	safe_jni::method<int32_t> int_value(env, value, "intValue", "()I");
	return mrb_fixnum_value(int_value(value));
}

static mrb_value Long_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	mrb_raise(mrb, E_RUNTIME_ERROR, "not implemented conversion fromn 'java/lang/Long' to mruby value.");
	return mrb_nil_value();
}

static mrb_value Double_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	safe_jni::method<double> double_value(env, value, "doubleValue", "()D");
#ifdef MRB_USE_FLOAT
	return mrb_float_value(mrb, (float)double_value(value));
#else
	return mrb_float_value(mrb, double_value(value));
#endif
}

static mrb_value Float_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	safe_jni::method<float> float_value(env, value, "floatValue", "()F");
	return mrb_float_value(mrb, float_value(value));
}

static mrb_value Object_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	safe_jni::safe_local_ref<jobject> retval(env, value);
	return (NULL == value) ? mrb_nil_value() : jobject_make(mrb, env, retval.get());
}

static mrb_value String_to_mrb(mrb_state *mrb, JNIEnv *env, jobject value, std::string const &type_name)
{
	safe_jni::safe_string str(env, static_cast<jstring>(value));
	return mrb_str_new_cstr(mrb, str.string());
}


};
};
