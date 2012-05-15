#include "jni_MRuby.h"
#include "jni_Log.h"
#include "safe_jni.hpp"
#include "jni_type_conversion.h"
extern "C" {
#include "mruby.h"
#include "mruby/dump.h"
#include "compile.h"
#include "mruby/proc.h"
#include "mruby/array.h"
}
#include <cstdio>
#include <cerrno>
#include <cstring>

static inline void throw_exception(JNIEnv *env, char const *name, char const *message);

#define MRBSTATE(mrb) to_ptr<mrb_state>(mrb)

template <typename T> static inline T* to_ptr(jlong &handle) {
	return reinterpret_cast<T*>(static_cast<intptr_t>(handle));
}

static inline mrb_sym to_sym(jlong &sym) {
	return static_cast<mrb_sym>(sym);
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_strNew
 * Signature: (JLjava/lang/String;)Lcrimsonwoods/android/libs/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1strNew
  (JNIEnv *env, jclass clazz, jlong mrb, jstring str)
{
	safe_jni::safe_string jstr(env, str);
	mrb_value const &value = mrb_str_new(MRBSTATE(mrb), jstr.string(), jstr.length());
	safe_jni::safe_local_ref<jobject> val(env, create_value(env, value));
	return val.get();
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_open
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1open
  (JNIEnv *env, jclass clazz)
{
	return static_cast<jlong>(reinterpret_cast<intptr_t>(mrb_open()));
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_loadIrep
 * Signature: (JLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1loadIrep
  (JNIEnv *env, jclass clazz, jlong mrb, jstring path)
{
	int n = -1;
	try {
		safe_jni::safe_string file_path(env, path);
		FILE *fp = fopen(file_path.string(), "rb");
		if (NULL == fp) {
			throw safe_jni::file_not_found_exception(strerror(errno));
		}
		n = mrb_load_irep(MRBSTATE(mrb), fp);
		fclose(fp);
	} catch (safe_jni::exception &e) {
		throw_exception(env, e.java_exception_name(), e.message());
	}
	return n;
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_parseString
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1parseString
  (JNIEnv *env, jclass clazz, jlong mrb, jstring command)
{
	mrb_parser_state *state = NULL;
	try {
		safe_jni::safe_string command_line(env, command);
		size_t const length = command_line.length();
		char *copy = new char[length + 1];
		if (NULL == copy) {
			throw std::bad_alloc();
		}
		strncpy(copy, command_line.string(), length);
		copy[length] = '\0';
		state = mrb_parse_string(MRBSTATE(mrb), copy);
	} catch (std::bad_alloc &e) {
		throw_exception(env, "java/lang/OutOfMemoryError", "Insufficient memory.");
	}
	return static_cast<jlong>(reinterpret_cast<intptr_t>(state));
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_parseFile
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1parseFile
  (JNIEnv *env, jclass clazz, jlong mrb, jstring path)
{
	mrb_parser_state *state = NULL;
	try {
		safe_jni::safe_string file_path(env, path);
		FILE *fp = fopen(file_path.string(), "r");
		if (NULL == fp) {
			throw safe_jni::file_not_found_exception(strerror(errno));
		}
		state = mrb_parse_file(MRBSTATE(mrb), fp);
	} catch (safe_jni::exception &e) {
		throw_exception(env, e.java_exception_name(), e.message());
	}
	return static_cast<jlong>(reinterpret_cast<intptr_t>(state));
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_generateCode
 * Signature: (JJ)I
 */
JNIEXPORT jint JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1generateCode
  (JNIEnv *env, jclass clazz, jlong mrb, jlong node)
{
	mrb_ast_node *ast_node = to_ptr<mrb_ast_node>(node);
	return mrb_generate_code(MRBSTATE(mrb), ast_node);
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_run
 * Signature: (JJLcrimsonwoods/android/libs/jamruby/mruby/Value;)Lcrimsonwoods/android/libs/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1run
  (JNIEnv *env, jclass clazz, jlong mrb, jlong proc, jobject value)
{
	mrb_value val = { { 0, } };
	if (!create_mrb_value(env, value, val)) {
		return NULL;
	}
	mrb_value ret = mrb_run(MRBSTATE(mrb), to_ptr<RProc>(proc), val);
	safe_jni::safe_local_ref<jobject> vref(env, create_value(env, ret));
	return vref.get();
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_defineGlobalConst
 * Signature: (JLjava/lang/String;Lcrimsonwoods/android/libs/jamruby/mruby/Value;)V
 */
JNIEXPORT void JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1defineGlobalConst
  (JNIEnv *env, jclass clazz, jlong mrb, jstring name, jobject value)
{
	safe_jni::safe_string vname(env, name);
	mrb_value val = { { 0, } };
	if (!create_mrb_value(env, value, val)) {
		return;
	}
	mrb_define_global_const(MRBSTATE(mrb), vname.string(), val);
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_arrayNew
 * Signature: (J)Lcrimsonwoods/android/libs/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1arrayNew
  (JNIEnv *env, jclass clazz, jlong mrb)
{
	mrb_value const &value = mrb_ary_new(MRBSTATE(mrb));
	safe_jni::safe_local_ref<jobject> val(env, create_value(env, value));
	return val.get();
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_arrayPush
 * Signature: (JLcrimsonwoods/android/libs/jamruby/mruby/Value;Lcrimsonwoods/android/libs/jamruby/mruby/Value;)V
 */
JNIEXPORT void JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1arrayPush
  (JNIEnv *env, jclass clazz, jlong mrb, jobject array, jobject elem)
{
	mrb_value varray = { { 0, } };
	if (!create_mrb_value(env, array, varray)) {
		return;
	}
	mrb_value velem = { { 0, } };
	if (!create_mrb_value(env, elem, velem)) {
		return;
	}
	mrb_ary_push(MRBSTATE(mrb),	varray, velem);
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_procNew
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1procNew
  (JNIEnv *env, jclass clazz, jlong mrb, jlong irep)
{
	RProc *rproc = mrb_proc_new(MRBSTATE(mrb), to_ptr<mrb_irep>(irep));
	return static_cast<jlong>(reinterpret_cast<intptr_t>(rproc));
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_topSelf
 * Signature: (J)Lcrimsonwoods/android/libs/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1topSelf
  (JNIEnv *env, jclass clazz, jlong mrb)
{
	safe_jni::safe_local_ref<jclass> vclazz(env, env->FindClass("crimsonwoods/android/libs/jamruby/mruby/Value"));
	if (!vclazz) {
		return NULL;
	}
	jmethodID ctor = env->GetMethodID(vclazz.get(), "<init>", "(IJ)V");
	if (NULL == ctor) {
		return NULL;
	}
	mrb_value const &value = mrb_top_self(MRBSTATE(mrb));
	safe_jni::safe_local_ref<jobject> val(env, create_value(env, value)); 
	return val.get();
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_p
 * Signature: (JLcrimsonwoods/android/libs/jamruby/mruby/Value;)Lcrimsonwoods/android/libs/jamruby/mruby/Value;
 */
JNIEXPORT jobject JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1p
  (JNIEnv *env, jclass clazz, jlong mrb, jobject value)
{
	mrb_value val = { { 0, } };
	if (!create_mrb_value(env, value, val)) {
		return NULL;
	}
	mrb_value const &ret = mrb_p(MRBSTATE(mrb), val);
	fflush(0);
	safe_jni::safe_local_ref<jobject> ref(env, create_value(env, ret));
	return ref.get();
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_sym2name
 * Signature: (JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1sym2name
  (JNIEnv *env, jclass clazz, jlong mrb, jlong sym)
{
	safe_jni::safe_local_ref<jstring> jstr(env, env->NewStringUTF(mrb_sym2name(MRBSTATE(mrb), to_sym(sym))));
	return jstr.get();
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_redirect_stdout
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1redirect_1stdout
  (JNIEnv *env, jclass clazz)
{
	int stdout_pipe[2] = { -1, -1 };
	if (0 > pipe(stdout_pipe)) {
		throw_exception(env, "java/io/IOException", "Can not create pipe.");
		return -1;
	}
	if (0 > dup2(stdout_pipe[1], STDOUT_FILENO)) {
		throw_exception(env, "java/io/IOException", "Can not duplicate file descriptor.");
		return -1;
	}
	(void)close(stdout_pipe[1]);
	return stdout_pipe[0];
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_redirect_stderr
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1redirect_1stderr
  (JNIEnv *env, jclass clazz)
{
	int stderr_pipe[2] = { -1, -1 };
	if (0 > pipe(stderr_pipe)) {
		throw_exception(env, "java/io/IOException", "Can not create pipe.");
		return -1;
	}
	if (0 > dup2(stderr_pipe[1], STDERR_FILENO)) {
		throw_exception(env, "java/io/IOException", "Can not duplicate file descriptor.");
		return -1;
	}
	(void)close(stderr_pipe[1]);
	return stderr_pipe[0];
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_MRuby
 * Method:    n_redirect_stdin
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_MRuby_n_1redirect_1stdin
  (JNIEnv *env, jclass clazz)
{
	int stdin_pipe[2] = { -1, -1 };
	if (0 > pipe(stdin_pipe)) {
		throw_exception(env, "java/io/IOException", "Can not create pipe.");
		return -1;
	}
	if (0 > dup2(stdin_pipe[0], STDIN_FILENO)) {
		throw_exception(env, "java/io/IOException", "Can not duplicate file descriptor.");
		return -1;
	}
	(void)close(stdin_pipe[0]);
	return stdin_pipe[1];
}

static inline void throw_exception(JNIEnv *env, char const *name, char const *message)
{
	safe_jni::safe_local_ref<jclass> clazz(env, env->FindClass(name));
	if (!clazz) {
		return;
	}
	env->ThrowNew(clazz.get(), message);
}

