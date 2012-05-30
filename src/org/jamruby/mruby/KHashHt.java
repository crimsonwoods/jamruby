package org.jamruby.mruby;

import java.util.Iterator;

import org.jamruby.exception.UnsupportedImplementationException;

public class KHashHt extends KHash<Value, Value> {
	public KHashHt(long ptr) {
		super(ptr);
	}

	@Override
	public boolean contains(Value key) {
		return n_khExist(nativeObject(), key);
	}

	@Override
	public Value get(Value key) {
		return n_khGet(nativeObject(), key);
	}

	@Override
	public void put(Value key, Value val) {
		n_khPut(nativeObject(), key, val);
	}

	@Override
	public Iterator<org.jamruby.mruby.KHash.Entry<Value, Value>> iterator() {
		throw new UnsupportedImplementationException("Can not create iterator.");
	}

	@Override
	public int size() {
		return n_khSize(nativeObject());
	}
	
	private static native boolean n_khExist(long hask, Value key);
	private static native Value n_khGet(long hash, Value key);
	private static native void n_khPut(long hash, Value key, Value val);
	private static native int n_khSize(long hash);
}
