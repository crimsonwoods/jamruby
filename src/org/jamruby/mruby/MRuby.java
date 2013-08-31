package org.jamruby.mruby;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.jamruby.core.Jamruby;
import org.jamruby.exception.UnsupportedImplementationException;
import org.jamruby.io.FileDescriptorHelper;


public class MRuby {
	public static int loadIrep(State state, File f) throws FileNotFoundException {
		return n_loadIrep(state.nativeObject(), f.getAbsolutePath());
	}
	
	public static ParserState parse(State state, String command) {
		return new ParserState(n_parseString(state.nativeObject(), command));
	}
	
	public static ParserState parse(State state, File f) throws FileNotFoundException {
		return new ParserState(n_parseFile(state.nativeObject(), f.getAbsolutePath()));
	}
	
	public static int generateCode(State state, ParserState parserState) {
		return n_generateCode(state.nativeObject(), parserState.nativeObject());
	}
	
	public static Value arrayNew(State state) {
		return n_arrayNew(state.nativeObject());
	}
	
	public static void arrayPush(State state, Value array, Value elem) {
		n_arrayPush(state.nativeObject(), array, elem);
	}
	
	public static RProc procNew(State state, Irep irep) {
		return new RProc(n_procNew(state.nativeObject(), irep.nativeObject()));
	}
	
	public static RClass defineClass(State state, String name, RClass superClass) {
		return new RClass(n_defineClass(state.nativeObject(), name, superClass.nativeObject()));
	}
	
	public static RClass defineModule(State state, String name) {
		return new RClass(n_defineModule(state.nativeObject(), name));
	}
	
	public static Value singletonClass(State state, Value value) {
		return n_singletonClass(state.nativeObject(), value);
	}
	
	public static void includeModule(State state, RClass c, RClass m) {
		n_includeModule(state.nativeObject(), c.nativeObject(), m.nativeObject());
	}
	
	public static void defineMethod(State state, RClass c, String name, RFunc func, int aspec) {
		n_defineMethod(state.nativeObject(), c.nativeObject(), name, func, aspec);
	}
	
	public static void defineClassMethod(State state, RClass c, String name, RFunc func, int aspec) {
		n_defineClassMethod(state.nativeObject(), c.nativeObject(), name, func, aspec);
	}
	
	public static void defineSingletonMethod(State state, RObject obj, String name, RFunc func, int aspec) {
		n_defineSingletonMethod(state.nativeObject(), obj.nativeObject(), name, func, aspec);
	}
	
	public static void defineModuleFunction(State state, RClass m, String name, RFunc func, int aspec) {
		n_defineModuleFunction(state.nativeObject(), m.nativeObject(), name, func, aspec);
	}
	
	public static void defineConst(State state, RClass m, String name, Value value) {
		n_defineConst(state.nativeObject(), m.nativeObject(), name, value);
	}
	
	public static Value instanceNew(State state, Value cv) {
		return n_instanceNew(state.nativeObject(), cv);
	}
	
	public static RClass classNew(State state) {
		return classNew(state, null);
	}
	
	public static RClass classNew(State state, RClass superClass) {
		if (null == superClass) {
			return new RClass(n_classNew(state.nativeObject(), Jamruby.UNAVAILABLE_NATIVE_OBJECT));
		} else {
			return new RClass(n_classNew(state.nativeObject(), superClass.nativeObject()));
		}
	}
	
	public static RClass moduleNew(State state) {
		return new RClass(n_moduleNew(state.nativeObject()));
	}
	
	public static RClass classGet(State state, String name) {
		return new RClass(n_classGet(state.nativeObject(), name));
	}
	
	public static RClass classObjGet(State state, String name) {
		return new RClass(n_classObjGet(state.nativeObject(), name));
	}
	
	public static Value objDup(State state, Value obj) {
		return n_objDup(state.nativeObject(), obj);
	}
	
	public static Value checkToInteger(State state, Value val, String method) {
		return n_checkToInteger(state.nativeObject(), val, method);
	}
	
	public static int objRespondTo(RClass c, Symbol mid) {
		return n_objRespondTo(c.nativeObject(), mid.nativeObject());
	}
	
	public static RClass defineClassUnder(State state, RClass outerClass, String name) {
		return defineClassUnder(state, outerClass, name, null);
	}
	
	public static RClass defineClassUnder(State state, RClass outerClass, String name, RClass superClass) {
		if (null == superClass) {
			return new RClass(n_defineClassUnder(state.nativeObject(), outerClass.nativeObject(), name, Jamruby.UNAVAILABLE_NATIVE_OBJECT));
		} else {
			return new RClass(n_defineClassUnder(state.nativeObject(), outerClass.nativeObject(), name, superClass.nativeObject()));
		}
	}
	
	public static RClass defineModuleUnder(State state, RClass outerClass, String name) {
		return new RClass(n_defineModuleUnder(state.nativeObject(), outerClass.nativeObject(), name));
	}
	
	public static Value funcall(State state, Value self, String name, int argc, Value...argv) {
		return n_funcall(state.nativeObject(), self, name, argc, argv);
	}
	
	public static Value funcallWithBlock(State state, Value self, String name, int argc, Value[] argv, Value blk) {
		return n_funcallWithBlock(state.nativeObject(), self, name, argc, argv, blk);
	}
	
	public static Symbol intern(State state, String name) {
		return new Symbol(n_intern(state.nativeObject(), name));
	}
	
	public static String sym2name(State state, Symbol sym) {
		return n_sym2name(state.nativeObject(), sym.nativeObject());
	}
	
	public static Value strFormat(State state, int argc, Value[] argv, Value fmt) {
		return n_strFormat(state.nativeObject(), argc, argv, fmt);
	}
	
	public static Pointer malloc(State state, long size) {
		return new Pointer(n_malloc(state.nativeObject(), size));
	}
	
	public static Pointer calloc(State state, long nelem, long len) {
		return new Pointer(n_calloc(state.nativeObject(), nelem, len));
	}
	
	public static Pointer realloc(State state, Pointer ptr, long len) {
		return new Pointer(n_realloc(state.nativeObject(), ptr.nativePointer(), len));
	}
	
	public static RBasic objAlloc(State state, int vtype, RClass cls) {
		return new RBasic(n_objAlloc(state.nativeObject(), vtype, cls.nativeObject()));
	}
	
	public static Pointer free(State state, Pointer ptr) {
		return new Pointer(n_free(state.nativeObject(), ptr.nativePointer()));
	}
	
	public static Value strNew(State state, String str) {
		return n_strNew(state.nativeObject(), str);
	}
	
	public static State open() {
		State s = new State(n_open());
		if (s.available()) {
			n_init_JNI_module(s.nativeObject(), Thread.currentThread().getId());
		}
		return s;
	}
	
	public static int checkstack(State state, int size) {
		return n_checkstack(state.nativeObject(), size);
	}
	
	public static Value topSelf(State state) {
		return n_topSelf(state.nativeObject());
	}
	
	public static Value run(State state, RProc proc, Value value) {
		return n_run(state.nativeObject(), proc.nativeObject(), value);
	}
	
	public static void p(State state, Value obj) {
		n_p(state.nativeObject(), obj);
	}
	
	public static Symbol toId(State state, Value name) {
		return new Symbol(n_toId(state.nativeObject(), name));
	}
	
	public static int objEqual(State state, Value left, Value right) {
		return n_objEqual(state.nativeObject(), left, right);
	}
	
	public static int equal(State state, Value left, Value right) {
		return n_equal(state.nativeObject(), left, right);
	}
	
	public static Value Integer(State state, Value val) {
		return n_Integer(state.nativeObject(), val);
	}
	
	public static Value Float(State state, Value val) {
		return n_Float(state.nativeObject(), val);
	}
	
	public static Value inspect(State state, Value obj) {
		return n_inspect(state.nativeObject(), obj);
	}
	
	public static int eql(State state, Value left, Value right) {
		return n_eql(state.nativeObject(), left, right);
	}
	
	public static Value checkConvertType(State state, Value val, int type, String tname, String method) {
		return n_checkConvertType(state.nativeObject(), val, type, tname, method);
	}
	
	public static Value anyToS(State state, Value value) {
		return n_anyToS(state.nativeObject(), value);
	}
	
	public static String objClassname(State state, Value obj) {
		return n_objClassname(state.nativeObject(), obj);
	}
	
	public static RClass objClass(State state, Value obj) {
		return new RClass(n_objClass(state.nativeObject(), obj));
	}
	
	public static Value classPath(State state, RClass cls) {
		return n_classPath(state.nativeObject(), cls.nativeObject());
	}
	
	public static Value convertType(State state, Value val, int type, String tname, String method) {
		return n_convertType(state.nativeObject(), val, type, tname, method);
	}
	
	public static boolean objIsKindOf(State state, Value obj, RClass c) {
		// for avoiding a error trapping inside 'mruby'.
		switch (c.valueType()) {
		case MRB_TT_MODULE:
		case MRB_TT_CLASS:
		case MRB_TT_ICLASS:
			break;
		default:
			throw new IllegalArgumentException("'c' is not a type of class object.");
		}
		final int ret = n_objIsKindOf(state.nativeObject(), obj, c.nativeObject());
		return 0 != ret ? true : false;
	}
	
	public static Value objInspect(State state, Value self) {
		return n_objInspect(state.nativeObject(), self);
	}
	
	public static Value objClone(State state, Value self) {
		return n_objClone(state.nativeObject(), self);
	}
	
	public static int blockGivenP() {
		return n_blockGivenP();
	}
	
	public static void raise(State state, RClass c, String message) {
		// 'mrb_raise' function call 'longjmp' method inside native code.
		// so, inhibit this method call to avoid calling a method 'longjmp'.
		throw new UnsupportedImplementationException("'raise' is not supported in this version.");
		//n_raise(state.nativeObject(), c.nativeObject(), message);
	}
	
	public static void warn(State state, String format, Object... args) {
		n_warn(state.nativeObject(), String.format(format, args));
	}
	
	public static void bug(State state, String format, Object... args) {
		n_bug(state.nativeObject(), String.format(format, args));
	}
	
	public static Value yield(State state, Value v, Value blk) {
		return n_yield(state.nativeObject(), v, blk);
	}
	
	public static Value yieldArgv(State state, Value b, int argc, Value[] argv) {
		return n_yieldArgv(state.nativeObject(), b, argc, argv);
	}
	
	public static Value classNewInstance(State state, int argc, Value[] argv, RClass c) {
		return n_classNewInstance(state.nativeObject(), argc, argv, c.nativeObject());
	}
	
	public static Value objNew(State state, RClass c, int argc, Value[] argv) {
		return n_objNew(state.nativeObject(), c.nativeObject(), argc, argv);
	}
	
	public static void defineAlias(State state, RClass c, String name1, String name2) {
		n_defineAlias(state.nativeObject(), c.nativeObject(), name1, name2);
	}
	
	public static String className(State state, RClass c) {
		return n_className(state.nativeObject(), c.nativeObject());
	}
	
	public static void defineGlobalConst(State state, String name, Value value) {
		n_defineGlobalConst(state.nativeObject(), name, value);
	}
	
	public static synchronized InputStream stdout() throws IOException {
		if (null == stdout) {
			stdout = new FileInputStream(FileDescriptorHelper.newFileDescriptor(n_redirect_stdout()));
		}
		return stdout;
	}
	
	public static synchronized InputStream stderr() throws IOException {
		if (null == stderr) {
			stderr = new FileInputStream(FileDescriptorHelper.newFileDescriptor(n_redirect_stderr()));
		}
		return stderr;
	}
	
	public static synchronized OutputStream stdin() throws IOException {
		if (null == stdin) {
			stdin = new FileOutputStream(FileDescriptorHelper.newFileDescriptor(n_redirect_stdin()));
		}
		return stdin;
	}
	
	private static FileInputStream stdout;
	private static FileInputStream stderr;
	private static FileOutputStream stdin;
	
	private static native int n_redirect_stdout() throws IOException;
	private static native int n_redirect_stderr() throws IOException;
	private static native int n_redirect_stdin() throws IOException;
	
	// Non-categorized methods
	private static native int n_loadIrep(long mrb, String path) throws FileNotFoundException;
	private static native long n_parseString(long mrb, String command);
	private static native long n_parseFile(long mrb, String path) throws FileNotFoundException;
	private static native int n_generateCode(long mrb, long node);
	private static native Value n_arrayNew(long mrb);
	private static native void n_arrayPush(long mrb, Value array, Value elem);
	private static native long n_procNew(long mrb, long irep);
	
	// Categorized methods: belongs to global
	private static native long n_defineClass(long mrb, String name, long superClass);
	private static native long n_defineModule(long mrb, String name);
	private static native Value n_singletonClass(long mrb, Value value);
	private static native void n_includeModule(long mrb, long c, long m);
	
	private static native void n_defineMethod(long mrb, long c, String name, RFunc func, int aspec);
	private static native void n_defineClassMethod(long mrb, long c, String name, RFunc func, int aspec);
	private static native void n_defineSingletonMethod(long mrb, long obj, String name, RFunc func, int aspec);
	private static native void n_defineModuleFunction(long mrb, long m, String name, RFunc func, int aspec);
	private static native void n_defineConst(long mrb, long m, String name, Value value);
	private static native Value n_instanceNew(long mrb, Value cv);
	private static native long n_classNew(long mrb, long superClass);
	private static native long n_moduleNew(long mrb);
	private static native long n_classGet(long mrb, String name);
	private static native long n_classObjGet(long mrb, String name);
	
	private static native Value n_objDup(long mrb, Value obj);
	private static native Value n_checkToInteger(long mrb, Value val, String method);
	private static native int n_objRespondTo(long c, long mid);
	private static native long n_defineClassUnder(long mrb, long outerClass, String name, long superClass);
	private static native long n_defineModuleUnder(long mrb, long outerClass, String name);
	
	private static native Value n_funcall(long mrb, Value self, String name, int argc, Value...argv);
	private static native Value n_funcallWithBlock(long mrb, Value self, String name, int argc, Value[] argv, Value blk);
	private static native long n_intern(long mrb, String name);
	private static native String n_sym2name(long mrb, long sym);
	private static native Value n_strFormat(long mrb, int argc, Value[] argv, Value fmt);
	
	private static native long n_malloc(long mrb, long size);
	private static native long n_calloc(long mrb, long nelem, long len);
	private static native long n_realloc(long mrb, long ptr, long len);
	private static native long n_objAlloc(long mrb, int vtype, long cls);
	private static native long n_free(long mrb, long ptr);
	
	private static native Value n_strNew(long mrb, String str);
	
	private static native long n_open();
	private static native int n_checkstack(long mrb, int size);
	
	private static native Value n_topSelf(long mrb);
	private static native Value n_run(long mrb, long proc, Value value);
	
	private static native void n_p(long mrb, Value obj);
	private static native long n_toId(long mrb, Value name);
	
	private static native int n_objEqual(long mrb, Value left, Value right);
	private static native int n_equal(long mrb, Value left, Value right);
	private static native Value n_Integer(long mrb, Value val);
	private static native Value n_Float(long mrb, Value val);
	private static native Value n_inspect(long mrb, Value obj);
	private static native int n_eql(long mrb, Value left, Value right);
	
	private static native Value n_checkConvertType(long mrb, Value val, int type, String tname, String method);
	private static native Value n_anyToS(long mrb, Value value);
	private static native String n_objClassname(long mrb, Value obj);
	private static native long n_objClass(long mrb, Value obj);
	private static native Value n_classPath(long mrb, long cls);
	private static native Value n_convertType(long mrb, Value val, int type, String tname, String method);
	private static native int n_objIsKindOf(long mrb, Value obj, long c);
	private static native Value n_objInspect(long mrb, Value self);
	private static native Value n_objClone(long mrb, Value self);
	
	private static native int n_blockGivenP();
	private static native void n_raise(long mrb, long c, String message);
	private static native void n_warn(long mrb, String message);
	private static native void n_bug(long mrb, String message);
	
	private static native Value n_yield(long mrb, Value v, Value blk);
	private static native Value n_yieldArgv(long mrb, Value b, int argc, Value[] argv);
	private static native Value n_classNewInstance(long mrb, int argc, Value[] argv, long c);
	private static native Value n_objNew(long mrb, long c, int argc, Value[] argv);
	
	private static native void n_defineAlias(long mrb, long c, String name1, String name2);
	private static native String n_className(long mrb, long c);
	private static native void n_defineGlobalConst(long mrb, String name, Value value);
	
	private static native void n_init_JNI_module(long mrb, long threadId);
	static native void n_cleanup_JNI_module(long mrb, long threadId);
	
	static {
		System.loadLibrary("jamruby");
	}
}
