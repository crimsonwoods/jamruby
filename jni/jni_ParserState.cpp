#include "jni_ParserState.h"
extern "C" {
#include "mruby.h"
#include "mruby/compile.h"
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_ParserState
 * Method:    n_getNerr
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_ParserState_n_1getNerr
  (JNIEnv *env, jclass clazz, jlong state)
{
	mrb_parser_state *p = reinterpret_cast<mrb_parser_state*>(static_cast<intptr_t>(state));
	return p->nerr;
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_ParserState
 * Method:    n_getTree
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_ParserState_n_1getTree
  (JNIEnv * env, jclass clazz, jlong state)
{
	mrb_parser_state *p = reinterpret_cast<mrb_parser_state*>(static_cast<intptr_t>(state));
	return static_cast<jlong>(reinterpret_cast<intptr_t>(p->tree));
}

/*
 * Class:     crimsonwoods_android_libs_jamruby_mruby_ParserState
 * Method:    n_getPool
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_crimsonwoods_android_libs_jamruby_mruby_ParserState_n_1getPool
  (JNIEnv *env, jclass clazz, jlong state)
{
	mrb_parser_state *p = reinterpret_cast<mrb_parser_state*>(static_cast<intptr_t>(state));
	return static_cast<jlong>(reinterpret_cast<intptr_t>(p->pool));
}

