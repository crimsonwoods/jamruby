#ifndef SAFE_JNI_H
#define SAFE_JNI_H

#include "jni_exception.hpp"
#include <cstddef>

namespace safe_jni {

template <typename T> class safe_array {
private:
	JNIEnv *env_;
	jarray const array_;
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
	jobjectArray const array_;
	size_t size_;
public:
	safe_object_array(JNIEnv *env, jobjectArray array)
		: env_(env), array_(array) {
		size_ = env->GetArrayLength(array);
	}
	~safe_object_array() {
	}

	jobjectArray get() const {
		return array_;
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

template <typename T=jobject> class safe_local_ref {
private:
	JNIEnv *env_;
	T const ref_;
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
	jstring const ref_;
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

template <typename Ret_> class method {
private:
	JNIEnv *env_;
	jmethodID mid_;
public:
	method(JNIEnv *env, jobject obj, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		safe_local_ref<jclass> cls(env, env->GetObjectClass(obj));
		mid_ = env->GetMethodID(cls.get(), name, sig);
	}
	method(JNIEnv *env, jclass cls, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		mid_ = env->GetStaticMethodID(cls, name, sig);
	}
	~method() {
	}
	bool available() const {
		return NULL == mid_ ? false : true;
	}
	bool operator ! () const {
		return !available();
	}
	Ret_ operator () (jobject obj, ...) const {
		va_list args;
		va_start(args, obj);
		jobject ret = env_->CallObjectMethodV(obj, mid_, args);
		va_end(args);
		return static_cast<Ret_>(ret);
	}
	Ret_ operator () (jclass cls, ...) const {
		va_list args;
		va_start(args, cls);
		jobject ret = env_->CallStaticObjectMethodV(cls, mid_, args);
		va_end(args);
		return static_cast<Ret_>(ret);
	}
};

template <> class method<void> {
private:
	JNIEnv *env_;
	jmethodID mid_;
public:
	method(JNIEnv *env, jobject obj, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		safe_local_ref<jclass> cls(env, env->GetObjectClass(obj));
		mid_ = env->GetMethodID(cls.get(), name, sig);
	}
	method(JNIEnv *env, jclass cls, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		mid_ = env->GetStaticMethodID(cls, name, sig);
	}
	~method() {
	}
	bool available() const {
		return NULL == mid_ ? false : true;
	}
	bool operator ! () const {
		return !available();
	}
	void operator () (jobject obj, ...) const {
		va_list args;
		va_start(args, obj);
		env_->CallVoidMethodV(obj, mid_, args);
		va_end(args);
	}
	void operator () (jclass cls, ...) const {
		va_list args;
		va_start(args, cls);
		env_->CallStaticVoidMethodV(cls, mid_, args);
		va_end(args);
	}
};

template <> class method<int> {
private:
	JNIEnv *env_;
	jmethodID mid_;
public:
	method(JNIEnv *env, jobject obj, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		safe_local_ref<jclass> cls(env, env->GetObjectClass(obj));
		mid_ = env->GetMethodID(cls.get(), name, sig);
	}
	method(JNIEnv *env, jclass cls, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		mid_ = env->GetStaticMethodID(cls, name, sig);
	}
	~method() {
	}
	bool available() const {
		return NULL == mid_ ? false : true;
	}
	bool operator ! () const {
		return !available();
	}
	int operator () (jobject obj, ...) const {
		va_list args;
		va_start(args, obj);
		int ret = env_->CallIntMethodV(obj, mid_, args);
		va_end(args);
		return ret;
	}
	int operator () (jclass cls, ...) const {
		va_list args;
		va_start(args, cls);
		int ret = env_->CallStaticIntMethodV(cls, mid_, args);
		va_end(args);
		return ret;
	}
};

template <> class method<long> {
private:
	JNIEnv *env_;
	jmethodID mid_;
public:
	method(JNIEnv *env, jobject obj, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		safe_local_ref<jclass> cls(env, env->GetObjectClass(obj));
		mid_ = env->GetMethodID(cls.get(), name, sig);
	}
	method(JNIEnv *env, jclass cls, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		mid_ = env->GetStaticMethodID(cls, name, sig);
	}
	~method() {
	}
	bool available() const {
		return NULL == mid_ ? false : true;
	}
	bool operator ! () const {
		return !available();
	}
	long operator () (jobject obj, ...) const {
		va_list args;
		va_start(args, obj);
		long ret = env_->CallLongMethodV(obj, mid_, args);
		va_end(args);
		return ret;
	}
	long operator () (jclass cls, ...) const {
		va_list args;
		va_start(args, cls);
		long ret = env_->CallStaticLongMethodV(cls, mid_, args);
		va_end(args);
		return ret;
	}
};

template <> class method<float> {
private:
	JNIEnv *env_;
	jmethodID mid_;
public:
	method(JNIEnv *env, jobject obj, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		safe_local_ref<jclass> cls(env, env->GetObjectClass(obj));
		mid_ = env->GetMethodID(cls.get(), name, sig);
	}
	method(JNIEnv *env, jclass cls, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		mid_ = env->GetStaticMethodID(cls, name, sig);
	}
	~method() {
	}
	bool available() const {
		return NULL == mid_ ? false : true;
	}
	bool operator ! () const {
		return !available();
	}
	float operator () (jobject obj, ...) const {
		va_list args;
		va_start(args, obj);
		float ret = env_->CallFloatMethodV(obj, mid_, args);
		va_end(args);
		return ret;
	}
	float operator () (jclass cls, ...) const {
		va_list args;
		va_start(args, cls);
		float ret = env_->CallStaticFloatMethodV(cls, mid_, args);
		va_end(args);
		return ret;
	}
};

template <> class method<double> {
private:
	JNIEnv *env_;
	jmethodID mid_;
public:
	method(JNIEnv *env, jobject obj, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		safe_local_ref<jclass> cls(env, env->GetObjectClass(obj));
		mid_ = env->GetMethodID(cls.get(), name, sig);
	}
	method(JNIEnv *env, jclass cls, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		mid_ = env->GetStaticMethodID(cls, name, sig);
	}
	~method() {
	}
	bool available() const {
		return NULL == mid_ ? false : true;
	}
	bool operator ! () const {
		return !available();
	}
	double operator () (jobject obj, ...) const {
		va_list args;
		va_start(args, obj);
		double ret = env_->CallDoubleMethodV(obj, mid_, args);
		va_end(args);
		return ret;
	}
	double operator () (jclass cls, ...) const {
		va_list args;
		va_start(args, cls);
		double ret = env_->CallStaticDoubleMethodV(cls, mid_, args);
		va_end(args);
		return ret;
	}
};

template <> class method<bool> {
private:
	JNIEnv *env_;
	jmethodID mid_;
public:
	method(JNIEnv *env, jobject obj, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		safe_local_ref<jclass> cls(env, env->GetObjectClass(obj));
		mid_ = env->GetMethodID(cls.get(), name, sig);
	}
	method(JNIEnv *env, jclass cls, char const * const name, char const * const sig)
		: env_(env), mid_(NULL) {
		mid_ = env->GetStaticMethodID(cls, name, sig);
	}
	~method() {
	}
	bool available() const {
		return NULL == mid_ ? false : true;
	}
	bool operator ! () const {
		return !available();
	}
	bool operator () (jobject obj, ...) const {
		va_list args;
		va_start(args, obj);
		jboolean ret = env_->CallBooleanMethodV(obj, mid_, args);
		va_end(args);
		return JNI_FALSE == ret ? false : true;
	}
	bool operator () (jclass cls, ...) const {
		va_list args;
		va_start(args, cls);
		jboolean ret = env_->CallStaticBooleanMethodV(cls, mid_, args);
		va_end(args);
		return JNI_FALSE == ret ? false : true;
	}
};


}

#endif // end of SAFE_JNI_H

