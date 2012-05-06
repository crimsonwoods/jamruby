package crimsonwoods.android.libs.jamruby.mruby;

public class RString extends RObject {
	public RString(long ptr) {
		super(ptr);
	}
	
	@Override
	public String toString() {
		return n_getBuf(nativeObject());
	}
	
	private static native String n_getBuf(long str);
}
