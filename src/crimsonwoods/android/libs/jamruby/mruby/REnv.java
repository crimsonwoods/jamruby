package crimsonwoods.android.libs.jamruby.mruby;

public class REnv extends RObject {
	public REnv(long ptr) {
		super(ptr);
	}
	
	public int cioff() {
		return n_getCioff(nativeObject());
	}
	
	public long mid() {
		return n_getMid(nativeObject());
	}
	
	private static native long n_getMid(long ptr);
	private static native int n_getCioff(long ptr);
}
