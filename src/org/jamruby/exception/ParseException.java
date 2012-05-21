package org.jamruby.exception;

@SuppressWarnings("serial")
public class ParseException extends RuntimeException {
	public ParseException() {
		super("Parse failed.");
	}
	
	public ParseException(String message) {
		super(message);
	}
}
