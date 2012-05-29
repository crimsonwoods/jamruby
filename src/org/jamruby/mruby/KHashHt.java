package org.jamruby.mruby;

import java.util.Iterator;

import org.jamruby.exception.NotImplementedException;

public class KHashHt extends KHash<Value, Value> {
	public KHashHt(long ptr) {
		super(ptr);
	}

	@Override
	public boolean contains(Value key) {
		throw new NotImplementedException();
	}

	@Override
	public Value get(Value key) {
		throw new NotImplementedException();
	}

	@Override
	public void put(Value key, Value val) {
		throw new NotImplementedException();
	}

	@Override
	public Iterator<org.jamruby.mruby.KHash.Entry<Value, Value>> iterator() {
		throw new NotImplementedException();
	}

	@Override
	public int size() {
		throw new NotImplementedException();
	}
}
