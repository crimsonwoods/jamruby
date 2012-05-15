package crimsonwoods.android.libs.jamruby.mruby;

import java.io.File;
import java.io.FileNotFoundException;

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
	
	private static native Value n_strNew(long mrb, String str);
	private static native long n_open();
	private static native int n_loadIrep(long mrb, String path) throws FileNotFoundException;
	private static native long n_parseString(long mrb, String command);
	private static native long n_parseFile(long mrb, String path) throws FileNotFoundException;
	private static native int n_generateCode(long mrb, long node);
	private static native Value n_run(long mrb, long proc, Value value);
	private static native void n_defineGlobalConst(long mrb, String name, Value value);
	private static native Value n_arrayNew(long mrb);
	private static native void n_arrayPush(long mrb, Value array, Value elem);
	private static native long n_procNew(long mrb, long irep);
	private static native Value n_topSelf(long mrb);
	private static native Value n_p(long mrb, Value obj);
	private static native String n_sym2name(long mrb, long sym);
	
	static {
		System.loadLibrary("jamruby");
	}
}
