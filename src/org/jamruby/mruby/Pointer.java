package org.jamruby.mruby;

public class Pointer {
	private long nativePtr;
	
	public static long NATIVE_NULL_POINTER = 0;
	
	public Pointer() {
		nativePtr = NATIVE_NULL_POINTER;
	}
	
	public Pointer(long ptr) {
		nativePtr = ptr;
	}
	
	public long nativePointer() {
		return nativePtr;
	}
	
	public boolean isNull() {
		return NATIVE_NULL_POINTER == nativePtr;
	}
}
