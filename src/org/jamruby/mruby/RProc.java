package org.jamruby.mruby;

public class RProc extends RObject {
	public RProc(long ptr) {
		super(ptr);
	}
	
	public RClass targetClass() {
		return new RClass(n_getTargetClass(nativeObject()));
	}
	
	public REnv env() {
		return new REnv(n_getEnv(nativeObject()));
	}
	
	private static native long n_getTargetClass(long ptr);
	private static native long n_getEnv(long ptr);
}
