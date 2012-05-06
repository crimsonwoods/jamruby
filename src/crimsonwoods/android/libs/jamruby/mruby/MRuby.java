package crimsonwoods.android.libs.jamruby.mruby;

import java.io.File;
import java.io.FileNotFoundException;

public class MRuby {
	private static MRuby instance;
	
	public static synchronized MRuby getInstance() {
		if (null == instance) {
			instance = new MRuby();
		}
		return instance;
	}
	
	public State open() {
		return new State(n_open());
	}
	
	public int loadIrep(State state, File f) throws FileNotFoundException {
		return n_loadIrep(state.nativeObject(), f.getAbsolutePath());
	}
	
	public ParserState parse(State state, String command) {
		return new ParserState(n_parseString(state.nativeObject(), command));
	}
	
	public ParserState parse(State state, File f) throws FileNotFoundException {
		return new ParserState(n_parseFile(state.nativeObject(), f.getAbsolutePath()));
	}
	
	public int generateCode(State state, AstNode node) {
		return n_generateCode(state.nativeObject(), node.nativeObject());
	}
	
	public Value run(State state, RProc proc, Value value) {
		return n_run(state.nativeObject(), proc.nativeObject(), value);
	}
	
	public void defineGlobalConst(State state, String name, Value value) {
		n_defineGlobalConst(state.nativeObject(), name, value);
	}
	
	public Value arrayNew(State state) {
		return n_arrayNew(state.nativeObject());
	}
	
	public void arrayPush(State state, Value array, Value elem) {
		n_arrayPush(state.nativeObject(), array, elem);
	}
	
	public RProc procNew(State state, Irep irep) {
		return new RProc(n_procNew(state.nativeObject(), irep.nativeObject()));
	}
	
	public Value strNew(State state, String str) {
		return n_strNew(str);
	}
	
	public Value topSelf(State state) {
		return n_topSelf(state.nativeObject());
	}
	
	public Value p(State state, Value obj) {
		return n_p(state.nativeObject(), obj);
	}
	
	private static native Value n_strNew(String str);
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
	
	static {
		System.loadLibrary("jamruby");
	}
}
