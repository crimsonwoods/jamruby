package crimsonwoods.android.libs.jamruby.io;

import java.io.FileDescriptor;
import java.lang.reflect.Field;

import crimsonwoods.android.libs.jamruby.Log;
import crimsonwoods.android.libs.jamruby.exception.UnsupportedImplementationException;

public class FileDescriptorHelper {
	private FileDescriptorHelper() {
	}
	
	public static FileDescriptor newFileDescriptor(int fd) {
		final FileDescriptor desc = new FileDescriptor();
		setFileDescriptor(desc, fd);
		return desc;
	}
	
	private static final void setFileDescriptor(FileDescriptor desc, int fd) throws UnsupportedImplementationException {
		try {
			final Field field = desc.getClass().getDeclaredField("descriptor");
			field.setAccessible(true);
			try {
				field.setInt(desc, fd);
			} catch (IllegalArgumentException e) {
				// if reached here, it's a bug.
				Log.e(e, "Illegal argument (fd = %d).", fd);
			} catch (IllegalAccessException e) {
				// if reached here, it's a bug.
				Log.e(e, "Illegal access.");
			}
		} catch (NoSuchFieldException e) {
			throw new UnsupportedImplementationException("Unsupported implementation.", e);
		}
	}
}
