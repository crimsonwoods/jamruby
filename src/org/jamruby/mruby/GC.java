package org.jamruby.mruby;

public class GC {
	public static void collect(State state) {
		n_garbage_collect(state.nativeObject());
	}
	
	public static void incremental(State state) {
		n_incremental_gc(state.nativeObject());
	}
	
	public static int saveArena(State state) {
		return n_gc_arena_save(state.nativeObject());
	}
	
	public static void restoreArena(State state, int arena) {
		n_gc_arena_restore(state.nativeObject(), arena);
	}
	
	public static void mark(State state, RObject obj) {
		n_gc_mark(state.nativeObject(), obj.nativeObject());
	}
	
	public static void fieldWriteBarrier(State state, RObject obj, RObject value) {
		n_field_write_barrier(state.nativeObject(), obj.nativeObject(), value.nativeObject());
	}
	
	public static void writeBarrier(State state, RObject obj) {
		n_write_barrier(state.nativeObject(), obj.nativeObject());
	}
	
	private static native void n_garbage_collect(long mrb);
	private static native void n_incremental_gc(long mrb);
	private static native int n_gc_arena_save(long mrb);
	private static native void n_gc_arena_restore(long mrb, int arena);
	private static native void n_gc_mark(long mrb, long obj);
	private static native void n_field_write_barrier(long mrb, long obj, long value);
	private static native void n_write_barrier(long mrb, long obj);
}
