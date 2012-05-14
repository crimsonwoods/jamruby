package crimsonwoods.android.libs.jamruby;

import java.io.File;
import java.io.IOException;

import crimsonwoods.android.libs.jamruby.exception.ParseException;
import crimsonwoods.android.libs.jamruby.mruby.AstNode;
import crimsonwoods.android.libs.jamruby.mruby.MRuby;
import crimsonwoods.android.libs.jamruby.mruby.ParserState;
import crimsonwoods.android.libs.jamruby.mruby.Pool;
import crimsonwoods.android.libs.jamruby.mruby.RObject;
import crimsonwoods.android.libs.jamruby.mruby.State;
import crimsonwoods.android.libs.jamruby.mruby.Value;

public class Jamruby {
	private State state;
	
	public static final long UNAVAILABLE_NATIVE_OBJECT = 0;
	
	public Jamruby() {
		state = MRuby.getInstance().open();
	}
	
	public void close() {
		state.close();
		state = null;
	}
	
	@Override
	protected void finalize() throws Throwable {
		try {
			super.finalize();
		} finally {
			close();
		}
	}
	
	public Value run(String command, String...args) {
		final MRuby mrb = MRuby.getInstance();
		final int n = generateCode(mrb, mrb.parse(state, command));
		return run(mrb, n, args);
	}
	
	public Value run(File f, String...args) throws IOException {
		final MRuby mrb = MRuby.getInstance();
		final int n = generateCode(mrb, mrb.parse(state, f));
		return run(mrb, n, args);
	}
	
	private int generateCode(MRuby mrb, ParserState p) {
		if (null == p) {
			throw new ParseException();
		}
		final AstNode node = p.tree();
		if (!node.available() || 0 != p.nerr()) {
			throw new ParseException();
		}
		final int n = mrb.generateCode(state, p.tree());
		final Pool pool = p.pool();
		pool.close();
		return n;
	}
	
	private Value run(MRuby mrb, int n, String...args) {
		final Value ARGV = mrb.arrayNew(state);
		final int argc = args.length;
		for(int i = 0; i < argc; ++i) {
			mrb.arrayPush(state, ARGV, mrb.strNew(state, args[i]));
		}
		mrb.defineGlobalConst(state, "ARGV", ARGV);
		final Value ret = mrb.run(state, mrb.procNew(state, state.irep()[n]), mrb.topSelf(state));
		final RObject exc = state.exc();
		if (exc.available()) {
			mrb.p(state, new Value(exc));
		}
		return ret;
	}
}
