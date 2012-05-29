#include "jni_AstNode.h"
extern "C" {
#include "mruby.h"
#include "mruby/compile.h"
}

#include "safe_jni.hpp"
#include "jni_common.hpp"

/*
 * Class:     org_jamruby_mruby_AstNode
 * Method:    n_getNodeType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_AstNode_n_1getNodeType
  (JNIEnv *env, jclass clazz, jlong ptr)
{
	mrb_ast_node * const node = to_ptr<mrb_ast_node>(ptr);
	if (NULL == node) {
		return -1;
	}
	return static_cast<jint>(reinterpret_cast<intptr_t>(node->car));
}

/*
 * Class:     org_jamruby_mruby_AstNode
 * Method:    n_getCar
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_AstNode_n_1getCar
  (JNIEnv *env, jclass clazz, jlong ptr)
{
	mrb_ast_node * const node = to_ptr<mrb_ast_node>(ptr);
	return to_jlong(node->car); 
}

/*
 * Class:     org_jamruby_mruby_AstNode
 * Method:    n_getCdr
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_jamruby_mruby_AstNode_n_1getCdr
  (JNIEnv *env, jclass clazz, jlong ptr)
{
	mrb_ast_node * const node = to_ptr<mrb_ast_node>(ptr);
	if (NULL == node) {
		return to_jlong(NULL);
	}
	return to_jlong(node->cdr);
}

