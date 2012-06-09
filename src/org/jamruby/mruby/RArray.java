package org.jamruby.mruby;

public class RArray extends RObject {
	public RArray(long ptr) {
		super(ptr);
	}
	
	public int capacity() {
		return n_getCapa(nativeObject());
	}
	
	public int length() {
		return n_getLen(nativeObject());
	}
	
	public Value[] toArray() {
		return n_getPtr(nativeObject());
	}
	
	private static native int n_getLen(long array);
	private static native int n_getCapa(long array);
	private static native Value[] n_getPtr(long array);
}
