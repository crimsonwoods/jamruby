package org.jamruby.mruby;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class KHashIv extends KHash<Long, Value> {
	public KHashIv(long ptr) {
		super(ptr);
	}

	@Override
	public boolean contains(Long key) {
		return contains((long)key);
	}

	@Override
	public Value get(Long key) {
		return get((long)key);
	}

	@Override
	public void put(Long key, Value val) {
	}

	@Override
	public Iterator<org.jamruby.mruby.KHash.Entry<Long, Value>> iterator() {
		return new KHIvIterator(nativeObject());
	}

	@Override
	public int size() {
		return n_khSize(nativeObject());
	}
	
	public boolean contains(long key) {
		return n_khExist(nativeObject(), key);
	}
	
	public Value get(long key) {
		return n_khGet(nativeObject(), key);
	}
	
	public void put(long key, Value val) {
		n_khPut(nativeObject(), key, val);
	}
	
	private static native boolean n_khExist(long hash, long key);
	private static native Value n_khGet(long hash, long key);
	private static native void n_khPut(long hash, long key, Value val);
	private static native int n_khSize(long hash);
	
	private static final class KHIvIterator implements Iterator<org.jamruby.mruby.KHash.Entry<Long, Value>> {
		private long hash;
		private long iterator;
		
		public KHIvIterator(long hash) {
			this.hash = hash;
			this.iterator = n_begin(hash);
		}
		
		@Override
		public synchronized boolean hasNext() {
			final long end = n_end(hash);
			if (iterator == end) {
				return false;
			}
			if(n_exist(hash, iterator)) {
				return true;
			}
			iterator = n_next(hash, iterator);
			if (end != iterator) {
				return true;
			}
			return false;
		}

		@Override
		public synchronized org.jamruby.mruby.KHash.Entry<Long, Value> next() {
			if (iterator == n_end(hash)) {
				throw new NoSuchElementException("no more items.");
			}
			final Entry<Long, Value> entry = new Entry<Long, Value>(n_key(hash, iterator), n_value(hash, iterator));
			iterator = n_next(hash, iterator);
			return entry;
		}

		@Override
		public void remove() {
			throw new UnsupportedOperationException("'remove' is unsupported.");
		}
		
		private static native long n_begin(long hash);
		private static native long n_end(long hash);
		private static native long n_next(long hash, long iter);
		private static native long n_key(long hash, long iter);
		private static native Value n_value(long hash, long iter);
		private static native boolean n_exist(long hash, long iter);
	}
}
