package crimsonwoods.android.libs.jamruby.exception;

@SuppressWarnings("serial")
public class NotImplementedException extends RuntimeException {
	public NotImplementedException() {
		super("Not implemented.");
	}
	
	public NotImplementedException(String message) {
		super(message);
	}
}
