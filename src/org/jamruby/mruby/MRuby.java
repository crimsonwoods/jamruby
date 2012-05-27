package org.jamruby.mruby;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.jamruby.io.FileDescriptorHelper;


public class MRuby {
	public static State open() {
		return new State(n_open());
	}
	
	public static int loadIrep(State state, File f) throws FileNotFoundException {
		return n_loadIrep(state.nativeObject(), f.getAbsolutePath());
	}
	
	public static ParserState parse(State state, String command) {
		return new ParserState(n_parseString(state.nativeObject(), command));
	}
	
	public static ParserState parse(State state, File f) throws FileNotFoundException {
		return new ParserState(n_parseFile(state.nativeObject(), f.getAbsolutePath()));
	}
	
	public static int generateCode(State state, AstNode node) {
		return n_generateCode(state.nativeObject(), node.nativeObject());
	}
	
	public static Value run(State state, RProc proc, Value value) {
		return n_run(state.nativeObject(), proc.nativeObject(), value);
	}
	
	public static void defineGlobalConst(State state, String name, Value value) {
		n_defineGlobalConst(state.nativeObject(), name, value);
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
	
	public static Value strNew(State state, String str) {
		return n_strNew(state.nativeObject(), str);
	}
	
	public static Value topSelf(State state) {
		return n_topSelf(state.nativeObject());
	}
	
	public static Value p(State state, Value obj) {
		return n_p(state.nativeObject(), obj);
	}
	
	public static String sym2name(State state, Symbol sym) {
		return n_sym2name(state.nativeObject(), sym.nativeObject());
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
	private static native void n_defineGlobalConst(long mrb, String name, Value value);
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
	private static native long n_classFromSym(long mrb, long c, long name);
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
	
	private static native Value n_topSelf(long mrb);
	private static native Value n_run(long mrb, long proc, Value value);
	
	private static native Value n_p(long mrb, Value obj);
	private static native long n_toId(long mrb, Value name);
	
	private static native Value n_anyToS(long mrb, Value value);
	private static native String n_objClassname(long mrb, Value obj);
	private static native long n_objClass(long mrb, Value obj);
	private static native Value n_classPath(long mrb, long cls);
	private static native Value n_objInspect(long mrb, Value self);
	private static native Value n_objClone(long mrb, Value self);
	
	static {
		System.loadLibrary("jamruby");
	}
}
