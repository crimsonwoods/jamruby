#include "jni_GC.h"
extern "C" {
#include "mruby.h"
}

template <typename T> inline T* to_ptr(jlong const handle) {
	return reinterpret_cast<T* const>(static_cast<intptr_t const>(handle));
}

/*
 * Class:     org_jamruby_mruby_GC
 * Method:    n_garbage_collect
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_GC_n_1garbage_1collect
  (JNIEnv *, jclass, jlong mrb)
{
	mrb_garbage_collect(to_ptr<mrb_state>(mrb));
}

/*
 * Class:     org_jamruby_mruby_GC
 * Method:    n_incremental_gc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_GC_n_1incremental_1gc
  (JNIEnv *, jclass, jlong mrb)
{
	mrb_incremental_gc(to_ptr<mrb_state>(mrb));
}

/*
 * Class:     org_jamruby_mruby_GC
 * Method:    n_gc_arena_save
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_jamruby_mruby_GC_n_1gc_1arena_1save
  (JNIEnv *, jclass, jlong mrb)
{
	return mrb_gc_arena_save(to_ptr<mrb_state>(mrb));
}

/*
 * Class:     org_jamruby_mruby_GC
 * Method:    n_gc_arena_restore
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_GC_n_1gc_1arena_1restore
  (JNIEnv *, jclass, jlong mrb, jint idx)
{
	mrb_gc_arena_restore(to_ptr<mrb_state>(mrb), static_cast<int>(idx));
}

/*
 * Class:     org_jamruby_mruby_GC
 * Method:    n_gc_mark
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_GC_n_1gc_1mark
  (JNIEnv *, jclass, jlong mrb, jlong obj)
{
	mrb_gc_mark(to_ptr<mrb_state>(mrb), to_ptr<RBasic>(obj));
}

/*
 * Class:     org_jamruby_mruby_GC
 * Method:    n_field_write_barrier
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_GC_n_1field_1write_1barrier
  (JNIEnv *, jclass, jlong mrb, jlong obj, jlong value)
{
	mrb_field_write_barrier(to_ptr<mrb_state>(mrb), to_ptr<RBasic>(obj), to_ptr<RBasic>(value));
}

/*
 * Class:     org_jamruby_mruby_GC
 * Method:    n_write_barrier
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_jamruby_mruby_GC_n_1write_1barrier
  (JNIEnv *, jclass, jlong mrb, jlong obj)
{
	mrb_write_barrier(to_ptr<mrb_state>(mrb), to_ptr<RBasic>(obj));
}

