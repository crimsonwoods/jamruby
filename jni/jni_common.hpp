#ifndef JNI_COMMON_H
#define JNI_COMMON_H

#include <jni.h>
#include <cstdlib>

template <typename T> inline T* to_ptr(jlong const &handle) {
	return reinterpret_cast<T*>(static_cast<intptr_t>(handle));
}

inline jlong to_jlong(void const * const ptr) {
	return static_cast<jlong>(reinterpret_cast<intptr_t>(ptr));
}

#endif // end of JNI_COMMON_H

