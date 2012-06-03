#ifndef SAFE_JNI_H
#define SAFE_JNI_H

#include "jni_exception.hpp"
#include <cstddef>

namespace safe_jni {

template <typename T> class safe_array {
private:
	JNIEnv *env_;
	jarray const &array_;
	T* ptr_;
	size_t size_;
	bool is_copy_;
	mutable bool is_aborted_;
public:
	safe_array(JNIEnv *env, jarray const &array)
		: env_(env), array_(array), ptr_(NULL), size_(0), is_copy_(false), is_aborted_(false) {
		jboolean is_copy = JNI_FALSE;
		ptr_  = static_cast<T*>(env->GetPrimitiveArrayCritical(array, &is_copy));
		size_ = env->GetArrayLength(array);
		is_copy_ = (JNI_FALSE == is_copy) ? false : true;
	}
	~safe_array() {
		if (is_aborted_) {
			env_->ReleasePrimitiveArrayCritical(array_, ptr_, JNI_ABORT);
		} else {
			env_->ReleasePrimitiveArrayCritical(array_, ptr_, is_copy_ ? JNI_COMMIT : 0);
		}
	}
	T& operator[] (int const &index) {
		if ((index < 0) || (static_cast<size_t>(index) >= size_)) {
			throw safe_jni::index_out_of_bounds_exception("Index out of bounds");
		}
		return ptr_[index];
	}
	T const& operator[] (int const &index) const {
		if ((index < 0) || (static_cast<size_t>(index) >= size_)) {
			throw safe_jni::index_out_of_bounds_exception("Index out of bounds");
		}
		return ptr_[index];
	}
	T* get() {
		return ptr_;
	}
	T const *get() const {
		return ptr_;
	}
	size_t size() const {
		return size_;
	}
	void abort() const {
		is_aborted_ = true;
	}
	bool is_copy() const {
		return is_copy_;
	}
	bool is_aborted() const {
		return is_aborted_;
	}
};

class safe_object_array {
private:
	JNIEnv *env_;
	jobjectArray const &array_;
	size_t size_;
public:
	safe_object_array(JNIEnv *env, jobjectArray array)
		: env_(env), array_(array) {
		size_ = env->GetArrayLength(array);
	}
	~safe_object_array() {
	}

	jobject get(int const &index) const {
		if ((index < 0) || (static_cast<size_t>(index) > size_)) {
			throw safe_jni::index_out_of_bounds_exception("Index out of bounds.");
		}
		return env_->GetObjectArrayElement(array_, index);
	}
	void set(int const &index, jobject item) {
		env_->SetObjectArrayElement(array_, index, item);
	}
	size_t size() const {
		return size_;
	}
};

template <typename T> class safe_local_ref {
private:
	JNIEnv *env_;
	T ref_;
public:
	safe_local_ref(JNIEnv *env, T obj)
		: env_(env), ref_(obj) {
	}
	~safe_local_ref() {
		env_->DeleteLocalRef(ref_);
	}
	T get() const {
		return ref_;
	}
	bool operator == (T &opr) const {
		return ref_ == opr;
	}
	bool operator != (T &opr) const {
		return ref_ != opr;
	}
	bool operator ! () const {
		return !ref_;
	}
	friend bool operator == (T left, safe_local_ref<T> const &right) {
		return left == right.ref_;
	}
	friend bool operator != (T left, safe_local_ref<T> const &right) {
		return left != right.ref_;
	}
};

class safe_string {
private:
	JNIEnv *env_;
	jstring const &ref_;
	char const *string_;
public:
	safe_string(JNIEnv *env, jstring const &string)
		: env_(env), ref_(string), string_(NULL) {
		string_ = env->GetStringUTFChars(ref_, NULL);
	}
	~safe_string() {
		if (string_) {
			env_->ReleaseStringUTFChars(ref_, string_);
			string_ = NULL;
		}
	}
	char const *string() const {
		return string_;
	}
	size_t length() const {
		return env_->GetStringUTFLength(ref_);
	}
	char const operator [] (int const &index) const {
		return string_[index];
	}
};

class clear_exception {
private:
	JNIEnv *env_;
	bool do_clear_;

	clear_exception(clear_exception const &);
	clear_exception &operator = (clear_exception const &);
public:
	clear_exception(JNIEnv *env)
		: env_(env), do_clear_(true) {
	}
	~clear_exception() {
		if (do_clear_) {
			env_->ExceptionClear();
		}
	}
	void no_clear() {
		do_clear_ = false;
	}
	void remark() {
		do_clear_ = true;
	}
};

}

#endif // end of SAFE_JNI_H

