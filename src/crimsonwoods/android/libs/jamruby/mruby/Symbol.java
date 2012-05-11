package crimsonwoods.android.libs.jamruby.mruby;

public class Symbol {
	private long nativeObject;
	
	public Symbol(long sym) {
		this.nativeObject = sym;
	}
	
	public synchronized long nativeObject() {
		return nativeObject;
	}
}
