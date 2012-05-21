package org.jamruby.exception;

@SuppressWarnings("serial")
public class UnsupportedImplementationException extends RuntimeException {
	public UnsupportedImplementationException() {
		super();
	}
	
	public UnsupportedImplementationException(String message) {
		super(message);
	}
	
	public UnsupportedImplementationException(Throwable cause) {
		super(cause);
	}
	
	public UnsupportedImplementationException(String message, Throwable cause) {
		super(message, cause);
	}
}
