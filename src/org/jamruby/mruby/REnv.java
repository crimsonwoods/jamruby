package org.jamruby.mruby;

public class REnv extends RBasic {
	public REnv(long ptr) {
		super(ptr);
	}
	
	public int cioff() {
		return n_getCioff(nativeObject());
	}
	
	public Symbol mid() {
		return new Symbol(n_getMid(nativeObject()));
	}
	
	private static native long n_getMid(long ptr);
	private static native int n_getCioff(long ptr);
}
