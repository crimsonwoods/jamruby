package org.jamruby.mruby;

import org.jamruby.core.Jamruby;

public class RBasic {
	private long nativeObj;
	
	public RBasic(long ptr) {
		nativeObj = ptr;
	}
	
	public ValueType valueType() {
		return ValueType.valueOf(n_getValueType(nativeObject()));
	}
	
	public int color() {
		return n_getColor(nativeObject());
	}
	
	public int flags() {
		return n_getFlags(nativeObject());
	}
	
	public RClass rclass() {
		return new RClass(n_getRClass(nativeObject()));
	}
	
	public RBasic gcNext() {
		return new RBasic(n_getGCNext(nativeObject()));
	}
	
	public synchronized long nativeObject() {
		return nativeObj;
	}
	
	protected synchronized void setNativeObject(long ptr) {
		nativeObj = ptr;
	}
	
	public boolean available() {
		return Jamruby.UNAVAILABLE_NATIVE_OBJECT == nativeObject() ? false : true;
	}
	
	private static native int n_getValueType(long ptr);
	private static native int n_getColor(long ptr);
	private static native int n_getFlags(long ptr);
	private static native long n_getRClass(long ptr);
	private static native long n_getGCNext(long ptr);
}
