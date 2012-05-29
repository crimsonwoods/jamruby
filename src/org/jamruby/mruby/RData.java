package org.jamruby.mruby;

public class RData extends RObject {
	public RData(long ptr) {
		super(ptr);
	}
	
	public RDataType dataType() {
		return new RDataType(n_getType(nativeObject()));
	}
	
	public Pointer data() {
		return new Pointer(n_getData(nativeObject()));
	}
	
	private static native long n_getIv(long self);
	private static native long n_getType(long self);
	private static native long n_getData(long self);
	
	public final static class RDataType {
		private long nativeObj;
		
		public RDataType(long ptr) {
			nativeObj = ptr;
		}
		
		public String name() {
			return n_getName(nativeObject());
		}
		
		public synchronized long nativeObject() {
			return nativeObj;
		}
		
		private static native String n_getName(long self);
	}
}
