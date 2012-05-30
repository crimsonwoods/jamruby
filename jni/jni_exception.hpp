#ifndef JNI_EXCEPTION_H
#define JNI_EXCEPTION_H

#include <jni.h>
#include <exception>
#include <string>

namespace safe_jni {

class exception : public std::exception {
private:
	std::string message_;
public:
	exception() : std::exception(), message_("") {
	}
	exception(char const *message) : std::exception(), message_(message) {
	}
	virtual ~exception() throw() {
	}

	char const *message() const {
		return message_.c_str();
	}

	virtual char const *java_exception_name() const {
		return "java/lang/Exception";
	}
};

class runtime_exception : public exception {
public:
	runtime_exception() : exception() {
	}
	runtime_exception(char const *message) : exception(message) {
	}
	virtual char const *java_exception_name() const {
		return "java/lang/RuntimeException";
	}
};

class index_out_of_bounds_exception : public runtime_exception {
public:
	index_out_of_bounds_exception() : runtime_exception() {
	}
	index_out_of_bounds_exception(char const *message) : runtime_exception(message) {
	}
	virtual char const *java_exception_name() const {
		return "java/lang/IndexOutOfBoundsException";
	}
};

class null_pointer_exception : public runtime_exception {
public:
	null_pointer_exception() : runtime_exception() {
	}
	null_pointer_exception(char const *message) : runtime_exception(message) {
	}
	virtual char const *java_exception_name() const {
		return "java/lang/NullPointerException";
	}
};

class io_exception : public exception {
public:
	io_exception() : exception() {
	}
	io_exception(char const *message) : exception(message) {
	}
	virtual char const *java_exception_name() const {
		return "java/io/IOException";
	}
};

class file_not_found_exception : public io_exception {
public:
	file_not_found_exception() : io_exception() {
	}
	file_not_found_exception(char const *message) : io_exception(message) {
	}
	virtual char const *java_exception_name() const {
		return "java/io/FileNotFoundException";
	}
};

class no_class_def_found_error : public exception {
public:
	no_class_def_found_error() : exception() {
	}
	no_class_def_found_error(char const *message) : exception(message) {
	}
	virtual char const *java_exception_name() const {
		return "java/lang/NoClassDefFoundError";
	}
};

}

extern void throw_exception(JNIEnv *env, char const *name, char const *message);

#endif
