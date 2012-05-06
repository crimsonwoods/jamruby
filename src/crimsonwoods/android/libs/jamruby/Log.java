package crimsonwoods.android.libs.jamruby;

public class Log {
	private static final String LOG_TAG = "jamruby";
	private static final boolean NO_DEBUG_LOG = false;
	
	public static void d(String format, Object... args) {
		if (NO_DEBUG_LOG) {
			return;
		}
		android.util.Log.d(LOG_TAG, String.format(format, args));
	}
	public static void i(String format, Object... args) {
		android.util.Log.i(LOG_TAG, String.format(format, args));
	}
	public static void v(String format, Object... args) {
		android.util.Log.v(LOG_TAG, String.format(format, args));
	}
	public static void w(String format, Object... args) {
		android.util.Log.w(LOG_TAG, String.format(format, args));
	}
	public static void e(String format, Object... args) {
		android.util.Log.e(LOG_TAG, String.format(format, args));
	}
	public static void e(Throwable t, String format, Object... args) {
		android.util.Log.e(LOG_TAG, String.format(format, args), t);
	}
}
