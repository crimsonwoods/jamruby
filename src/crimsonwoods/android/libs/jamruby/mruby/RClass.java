package crimsonwoods.android.libs.jamruby.mruby;

public class RClass extends RObject {
	public RClass(long ptr) {
		super(ptr);
	}
	
	public RClass superClass() {
		return new RClass(n_getSuperClass(nativeObject()));
	}
	
	private static native long n_getSuperClass(long ptr);
}
