package org.jamruby.mruby;

public class RString extends RBasic {
	public RString(long ptr) {
		super(ptr);
	}
	
	@Override
	public String toString() {
		return n_getPtr(nativeObject());
	}
	
	private static native String n_getPtr(long str);
}
