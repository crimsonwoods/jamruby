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
		state = MRuby.open();
	}
	
	public void close() {
		state.close();
		state = null;
	}
	
	public State state() {
		return state;
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
		final int n = generateCode(MRuby.parse(state, command));
		return run(n, args);
	}
	
	public Value run(File f, String...args) throws IOException {
		final int n = generateCode(MRuby.parse(state, f));
		return run(n, args);
	}
	
	private int generateCode(ParserState p) {
		if (null == p) {
			throw new ParseException();
		}
		final AstNode node = p.tree();
		if (!node.available() || 0 != p.nerr()) {
			throw new ParseException();
		}
		final int n = MRuby.generateCode(state, p.tree());
		final Pool pool = p.pool();
		pool.close();
		return n;
	}
	
	private Value run(int n, String...args) {
		final Value ARGV = MRuby.arrayNew(state);
		final int argc = args.length;
		for(int i = 0; i < argc; ++i) {
			MRuby.arrayPush(state, ARGV, MRuby.strNew(state, args[i]));
		}
		MRuby.defineGlobalConst(state, "ARGV", ARGV);
		final Value ret = MRuby.run(state, MRuby.procNew(state, state.irep()[n]), MRuby.topSelf(state));
		final RObject exc = state.exc();
		if (exc.available()) {
			MRuby.p(state, new Value(exc));
		}
		return ret;
	}
}
