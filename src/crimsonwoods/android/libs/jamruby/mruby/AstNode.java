package crimsonwoods.android.libs.jamruby.mruby;

import crimsonwoods.android.libs.jamruby.Jamruby;

public class AstNode {
	private long nativeObj;
	
	public AstNode(long ptr) {
		this.nativeObj = ptr;
	}
	
	public synchronized long nativeObject() {
		return nativeObj;
	}
	
	public synchronized boolean available() {
		return Jamruby.UNAVAILABLE_NATIVE_OBJECT == nativeObj ? false : true;
	}
}
