package org.jamruby.mruby;

public class Irep {
	private long nativeObj;
	
	public Irep(long ptr) {
		this.nativeObj = ptr;
	}
	
	public synchronized long nativeObject() {
		return nativeObj;
	}
}
