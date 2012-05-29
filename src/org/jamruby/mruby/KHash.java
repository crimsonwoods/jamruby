package org.jamruby.mruby;

import java.util.Iterator;

public abstract class KHash<TKey, TVal> {
	private long nativeObj;
	
	public KHash(long ptr) {
		nativeObj = ptr;
	}
	
	public synchronized long nativeObject() {
		return nativeObj;
	}
	
	public abstract boolean contains(TKey key);
	public abstract TVal get(TKey key);
	public abstract void put(TKey key, TVal val);
	public abstract Iterator<Entry<TKey, TVal>> iterator();
	public abstract int size();
	
	public static class Entry<TKey, TVal> {
		private TKey key;
		private TVal val;
		
		Entry(TKey key, TVal val) {
			this.key = key;
			this.val = val;
		}
		
		public TKey key() {
			return key;
		}
		
		public TVal value() {
			return val;
		}
	}
}
