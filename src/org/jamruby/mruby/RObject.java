package org.jamruby.mruby;

public class RObject extends RBasic {
	public RObject(long ptr) {
		super(ptr);
	}
	
	public KHashIv iv() {
		return new KHashIv(n_getIv(nativeObject()));
	}
	
	public Value get(State state, Symbol sym) {
		return n_objIvGet(state.nativeObject(), nativeObject(), sym.nativeObject());
	}
	
	public void set(State state, Symbol sym, Value value) {
		n_objIvSet(state.nativeObject(), nativeObject(), sym.nativeObject(), value);
	}
	
	private static native long n_getIv(long obj);
	
	private static native Value n_objIvGet(long state, long obj, long sym);
	private static native void n_objIvSet(long state, long obj, long sym, Value value);
}
