package org.jamruby.mruby;

public class ParserState {
	private long nativeObj;
	
	public ParserState(long ptr) {
		this.nativeObj = ptr;
	}
	
	public synchronized long nativeObject() {
		return nativeObj;
	}
	
	public int nerr() {
		return n_getNerr(nativeObject());
	}
	
	public AstNode tree() {
		return new AstNode(n_getTree(nativeObject()));
	}
	
	/**
	 * get the pool object.<BR/><BR/>
	 * ParserState object is allocated on the pool.<BR/>
	 * And it will be released automatically when the pool is closed.<BR/>
	 * This version does not take care of this procedure.<BR/>
	 * We should pay attention to released object and have to manage ourself.<BR/>
	 * 
	 * @return allocated pool object.
	 */
	public Pool pool() {
		return new Pool(n_getPool(nativeObj));
	}
	
	private static native int n_getNerr(long ptr);
	private static native long n_getTree(long ptr);
	private static native long n_getPool(long ptr);
}
