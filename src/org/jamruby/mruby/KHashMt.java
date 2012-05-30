package org.jamruby.mruby;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class KHashMt extends KHash<Long, RProc> {
	public KHashMt(long ptr) {
		super(ptr);
	}

	@Override
	public boolean contains(Long key) {
		return n_khExist(nativeObject(), key);
	}

	@Override
	public RProc get(Long key) {
		return new RProc(n_khGet(nativeObject(), key));
	}

	@Override
	public void put(Long key, RProc val) {
		n_khPut(nativeObject(), key, val.nativeObject());
	}

	@Override
	public Iterator<org.jamruby.mruby.KHash.Entry<Long, RProc>> iterator() {
		return new KHMtIterator(nativeObject());
	}

	@Override
	public int size() {
		return n_khSize(nativeObject());
	}
	
	private static native boolean n_khExist(long kh, long key);
	private static native long n_khGet(long kh, long key);
	private static native void n_khPut(long kh, long key, long value);
	private static native int n_khSize(long kh);
	
	private final static class KHMtIterator implements Iterator<Entry<Long, RProc>> {
		private long hash;
		private long iterator;
		
		KHMtIterator(long hash) {
			this.hash = hash;
			iterator = n_begin(hash);
		}
		
		@Override
		public boolean hasNext() {
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
		public org.jamruby.mruby.KHash.Entry<Long, RProc> next() {
			if (iterator == n_end(hash)) {
				throw new NoSuchElementException("no more items.");
			}
			final Entry<Long, RProc> entry = new Entry<Long, RProc>(n_key(hash, iterator), new RProc(n_value(hash, iterator)));
			iterator = n_next(hash, iterator);
			return entry;
		}

		@Override
		public void remove() {
			throw new UnsupportedOperationException("'remove' is unsupported.");
		}
		
		private static native long n_begin(long hash);
		private static native long n_end(long hash);
		private static native boolean n_exist(long hash, long iter);
		private static native long n_next(long hash, long iter);
		private static native long n_key(long hash, long iter);
		private static native long n_value(long hahs, long iter);
	}
}
