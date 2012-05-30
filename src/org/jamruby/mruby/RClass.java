package org.jamruby.mruby;

public class RClass extends RObject {
	public RClass(long ptr) {
		super(ptr);
	}
	
	public RClass superClass() {
		return new RClass(n_getSuperClass(nativeObject()));
	}
	
	public KHashIv iv() {
		return new KHashIv(n_getIv(nativeObject()));
	}
	
	public KHashMt mt() {
		return new KHashMt(n_getMt(nativeObject()));
	}
	
	public static RProc methodSearch(State state, RClass klass, Symbol mid) {
		return new RProc(n_methodSearch(state.nativeObject(), klass.nativeObject(), mid.nativeObject()));
	}
	
	public static boolean objIsInstanceOf(State state, Value obj, RClass klass) {
		return n_objIsInstanceOf(state.nativeObject(), obj, klass.nativeObject());
	}
	
	private static native long n_getSuperClass(long ptr);
	private static native long n_getIv(long ptr);
	private static native long n_getMt(long ptr);
	
	private static native long n_defineClassId(long mrb, long mid, long c);
	private static native long n_defineModuleId(long mrb, long mid);
	private static native long n_vmDefineClass(long mrb, Value outerClass, Value superClass, long mid);
	private static native long n_vmDefineModule(long mrb, Value outerClass, long mid);
	private static native void n_defineMethodVm(long mrb, long c, long mid, Value body);
	private static native void n_defineMethodRaw(long mrb, long c, long mid, long proc);
	
	private static native long n_classOuterModule(long mrb, long c);
	private static native long n_methodSearch(long mrb, long c, long mid);
	
	private static native boolean n_respondTo(long mrb, Value obj, long mid);
	private static native boolean n_objIsInstanceOf(long mrb, Value obj, long c);
	private static native long n_classReal(long c);
	
	private static native void n_objCallInit(long mrb, Value obj, int argc, Value... argv);
}
