package org.jamruby.io;

import java.io.FileDescriptor;
import java.lang.reflect.Field;

import org.jamruby.android.Log;
import org.jamruby.exception.UnsupportedImplementationException;


public class FileDescriptorHelper {
	private FileDescriptorHelper() {
	}
	
	public static FileDescriptor newFileDescriptor(int fd) {
		final FileDescriptor desc = new FileDescriptor();
		setFileDescriptor(desc, fd);
		return desc;
	}
	
	/**
	 * Set a descriptor of file to 'FileDescriptor' class instance via reflection.
	 * This implementation is specified for Android platform.
	 * @param desc FileDescriptor class instance.
	 * @param fd descriptor of file that depends on platform.
	 * @throws UnsupportedImplementationException
	 */
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
