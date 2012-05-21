#include "jni_AstNode.h"
extern "C" {
#include "mruby.h"
#include "mruby/compile.h"
}

#include "safe_jni.hpp"

inline static mrb_ast_node *to_ptr(jlong ptr) {
	return reinterpret_cast<mrb_ast_node*>(static_cast<intptr_t>(ptr));
}

/*
 * Class:     org_jamruby_mruby_AstNode
 * Method:    n_getNodeType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_AstNode_n_1getNodeType
  (JNIEnv *env, jclass clazz, jlong ptr)
{
	return static_cast<jint>(reinterpret_cast<intptr_t>(to_ptr(ptr)->car));
}

/*
 * Class:     org_jamruby_mruby_AstNode
 * Method:    n_getCar
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_AstNode_n_1getCar
  (JNIEnv *env, jclass clazz, jlong ptr)
{
	return static_cast<jlong>(reinterpret_cast<intptr_t>(to_ptr(ptr)->car)); 
}

/*
 * Class:     org_jamruby_mruby_AstNode
 * Method:    n_getCdr
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_AstNode_n_1getCdr
  (JNIEnv *env, jclass clazz, jlong ptr)
{
	return static_cast<jlong>(reinterpret_cast<intptr_t>(to_ptr(ptr)->cdr));
}

