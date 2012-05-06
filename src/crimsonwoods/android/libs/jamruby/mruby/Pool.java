package crimsonwoods.android.libs.jamruby.mruby;

import crimsonwoods.android.libs.jamruby.Jamruby;

public class Pool {
	private long nativeObj;
	
	public Pool(long ptr) {
		this.nativeObj = ptr;
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
			nativeObj = Jamruby.UNAVAILABLE_NATIVE_OBJECT;
		}
	}
	
	private static native void n_close(long ptr);
}
