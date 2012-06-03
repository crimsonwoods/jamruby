package org.jamruby.mruby;

import org.jamruby.core.Jamruby;

public class State {
	private long nativeObj;
	private Irep[] ireps;
	
	public State(long ptr) {
		nativeObj = ptr;
	}
	
	@Override
	protected void finalize() throws Throwable {
		try {
			super.finalize();
		} finally {
			close();
		}
	}
	
	public synchronized long nativeObject() {
		return nativeObj;
	}
	
	public synchronized void close() {
		if (Jamruby.UNAVAILABLE_NATIVE_OBJECT != nativeObj) {
			n_close(nativeObj);
			MRuby.n_cleanup_JNI_module(nativeObj, Thread.currentThread().getId());
			nativeObj = Jamruby.UNAVAILABLE_NATIVE_OBJECT;
		}
	}
	
	public boolean available() {
		return Jamruby.UNAVAILABLE_NATIVE_OBJECT == nativeObject() ? false : true;
	}
	
	public RObject exc() {
		return new RObject(n_getExc(nativeObject()));
	}
	
	public Irep[] irep() {
		if (null == ireps) {
			synchronized(this) {
				if (null == ireps) {
					ireps = n_getIreps(nativeObject());
				}
			}
		}
		return ireps;
	}
	
	private static native Irep[] n_getIreps(long ptr);
	private static native long n_getExc(long ptr);
	private static native void n_close(long ptr);
}
