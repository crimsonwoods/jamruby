package org.jamruby.mruby;

import android.util.SparseArray;

public enum ValueType {
	MRB_TT_FALSE,
	MRB_TT_FREE,
	MRB_TT_TRUE,
	MRB_TT_FIXNUM,
	MRB_TT_SYMBOL,
	MRB_TT_UNDEF,
	MRB_TT_FLOAT,
	MRB_TT_VOIDP,
	MRB_TT_OBJECT,
	MRB_TT_CLASS,
	MRB_TT_MODULE,
	MRB_TT_ICLASS,
	MRB_TT_SCLASS,
	MRB_TT_PROC,
	MRB_TT_ARRAY,
	MRB_TT_HASH,
	MRB_TT_STRING,
	MRB_TT_RANGE,
	MRB_TT_EXCEPTION,
	MRB_TT_FILE,
	MRB_TT_ENV,
	MRB_TT_DATA,
	MRB_TT_FIBER,
	MRB_TT_MAXDEFINE,
	;
	
	private static final SparseArray<ValueType> typeMap;
	
	static {
		typeMap = new SparseArray<ValueType>();
		ValueType[] types = ValueType.values();
		for(int i = 0; i < types.length; ++i) {
			typeMap.put(i, types[i]);
		}
	}
	
	public static synchronized ValueType valueOf(int value) {
		final ValueType ret = typeMap.get(value);
		if (null == ret) {
			throw new IllegalArgumentException(String.format("Undefined value type (%d).", value));
		}
		return ret;
	}
	
	public static synchronized int toInteger(ValueType value) {
		final int index = typeMap.indexOfValue(value);
		if (0 > index) {
			return -1;
		}
		return typeMap.keyAt(index);
	}
}