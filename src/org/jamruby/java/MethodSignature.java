package org.jamruby.java;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

public class MethodSignature {
	@SuppressWarnings("serial")
	private static final Map<Class<?>, String> TYPE_SIGNATURE_MAP = new HashMap<Class<?>, String>() {{
		put(void.class,    "V");
		put(boolean.class, "Z");
		put(byte.class,    "B");
		put(char.class,    "C");
		put(short.class,   "S");
		put(int.class,     "I");
		put(long.class,    "J");
		put(float.class,   "F");
		put(double.class,  "D");
	}};
	
	public static synchronized String genMethodSignature(Method m) {
		Class<?> retType = m.getReturnType();
		final Class<?>[] paramTypes = m.getParameterTypes();
		final StringBuilder builder = new StringBuilder();
		builder.append("(");
		builder.append(genParameters(paramTypes));
		builder.append(")");
		while(retType.isArray()) {
			builder.append("[");
			retType = retType.getComponentType();
		}
		if (TYPE_SIGNATURE_MAP.containsKey(retType)) {
			builder.append(TYPE_SIGNATURE_MAP.get(retType));
		} else {
			builder.append(genSignature(retType.getCanonicalName()));
		}
		return builder.toString();
	}
	
	public static synchronized String genCtorSignature(Constructor<?> c) {
		final Class<?>[] paramTypes = c.getParameterTypes();
		final StringBuilder builder = new StringBuilder();
		builder.append("(");
		builder.append(genParameters(paramTypes));
		builder.append(")V");
		return builder.toString();
	}
	
	private static String genParameters(Class<?>[] paramTypes) {
		final StringBuilder builder = new StringBuilder();
		for (Class<?> pt : paramTypes) {
			while (pt.isArray()) {
				builder.append("[");
				pt = pt.getComponentType();
			}
			if (TYPE_SIGNATURE_MAP.containsKey(pt)) {
				builder.append(TYPE_SIGNATURE_MAP.get(pt));
			} else {
				builder.append(genSignature(pt.getCanonicalName()));
			}
		}
		return builder.toString();
	}
	
	private static String genSignature(String fqn) {
		return "L" + fqn.replace('.', '/') + ";";
	}
}
