package org.jamruby.mruby;

import org.jamruby.exception.NotImplementedException;

public class Value {
	private ValueType type;
	private int i;
	private double f;
	private Symbol sym;
	private RBasic obj;
	
	public Value(int type, int value) {
		this(ValueType.valueOf(type), value);
	}
	
	public Value(int type, long value) {
		this(ValueType.valueOf(type), value);
	}
	
	public Value(ValueType type, int value) {
		this.type = type;
		this.i = value;
	}
	
	public Value(ValueType type, long obj) {
		this.type = type;
		switch(type) {
		case MRB_TT_SYMBOL:
			this.sym = new Symbol(obj);
			break;
		case MRB_TT_ARRAY:
			this.obj = new RArray(obj);
			break;
		case MRB_TT_ENV:
			this.obj = new REnv(obj);
			break;
		case MRB_TT_OBJECT:
			this.obj = new RObject(obj);
			break;
		case MRB_TT_PROC:
			this.obj = new RProc(obj);
			break;
		case MRB_TT_MODULE:
		case MRB_TT_CLASS:
		case MRB_TT_ICLASS:
			this.obj = new RClass(obj);
			break;
		case MRB_TT_STRING:
			this.obj = new RString(obj);
			break;
		case MRB_TT_HASH:
			this.obj = new RHash(obj);
			break;
		default:
			throw new NotImplementedException(String.format("Unknown value type (%s).", type.toString()));
		}
	}
	
	public Value(boolean value) {
		this.type = value ? ValueType.MRB_TT_TRUE : ValueType.MRB_TT_FALSE;
		this.i = 1;
	}
	
	public Value(float value) {
		this.type = ValueType.MRB_TT_FLOAT;
		this.f = value;
	}
	
	public Value(double value) {
		this.type = ValueType.MRB_TT_FLOAT;
		this.f = value;
	}
	
	public Value(int value) {
		this.type = ValueType.MRB_TT_FIXNUM;
		this.i = value;
	}
	
	public Value(RBasic obj) {
		this.type = obj.valueType();
		this.obj = obj;
	}
	
	public ValueType type() {
		return type;
	}
	
	public int asInt() {
		return i;
	}
	
	public double asFloat() {
		return f;
	}
	
	public Symbol asSymbol() {
		return sym;
	}
	
	public RBasic asObject() {
		return obj;
	}
	
	public String asString() {
		if (type != ValueType.MRB_TT_STRING) {
			return null;
		}
		return ((RString)obj).toString();
	}
	
	public Object[] asArray() {
		if (type != ValueType.MRB_TT_ARRAY) {
			return null;
		}
		return ((RArray)obj).toArray();
	}
	
	public int id() {
		return n_obj_id();
	}
	
	@Override
	public String toString() {
		return toString(null);
	}
	
	public String toString(State state) {
		StringBuilder builder = new StringBuilder();
		builder.append(type.toString()).append(": ");
		switch(type) {
		case MRB_TT_TRUE:
			builder.append("True");
			break;
		case MRB_TT_FALSE:
			builder.append("False");
			break;
		case MRB_TT_FIXNUM:
			builder.append(i);
			break;
		case MRB_TT_FLOAT:
			builder.append(f);
			break;
		case MRB_TT_STRING:
			builder.append(((RString)obj).toString());
			break;
		case MRB_TT_ARRAY:
			final Value[] values = ((RArray)obj).toArray();
			if (null != values) {
				builder.append(String.format("<length=%d", values.length));
				for (int i = 0; i < values.length; ++i) {
					builder.append(String.format(", [%d]={%s}", i, values[i].toString(state)));
				}
				builder.append(">");
			} else {
				builder.append("null");
			}
			break;
		case MRB_TT_CLASS:
		case MRB_TT_MODULE:
		case MRB_TT_ICLASS:
			if (null != state) {
				builder.append(MRuby.className(state, (RClass)obj));
			}
			break;
		case MRB_TT_SYMBOL:
			if (null != state) {
				builder.append(MRuby.sym2name(state, sym));
			}
			break;
		case MRB_TT_OBJECT:
			if (null != state) {
				builder.append(MRuby.objClassname(state, new Value(obj)));
			}
			break;
		case MRB_TT_HASH:
			if (null != state) {
				// TODO enumerate all entries.
			}
			break;
		case MRB_TT_PROC:
		case MRB_TT_DATA:
		case MRB_TT_ENV:
		case MRB_TT_EXCEPTION:
		case MRB_TT_FILE:
		case MRB_TT_FREE:
		case MRB_TT_MATCH:
		case MRB_TT_MAXDEFINE:
		case MRB_TT_RANGE:
		case MRB_TT_REGEX:
		case MRB_TT_SCLASS:
		case MRB_TT_STRUCT:
		case MRB_TT_THREAD:
		case MRB_TT_THREADGRP:
			builder.append("Ruby-Object");
			break;
		case MRB_TT_UNDEF:
			builder.append("Undef");
			break;
		}
		return builder.toString();
	}
	
	private native int n_obj_id();
}