#ifndef JAMRUBY_JNI_METHOD_H
#define JAMRUBY_JNI_METHOD_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}
#include <string>

#define JNI_TYPE_UNKNOWN ((jni_type_id_t)0x0000U)
#define JNI_TYPE_VOID    ((jni_type_id_t)0x0001U)
#define JNI_TYPE_BOOLEAN ((jni_type_id_t)0x0002U)
#define JNI_TYPE_BYTE    ((jni_type_id_t)0x0003U)
#define JNI_TYPE_CHAR    ((jni_type_id_t)0x0004U)
#define JNI_TYPE_SHORT   ((jni_type_id_t)0x0005U)
#define JNI_TYPE_INT     ((jni_type_id_t)0x0006U)
#define JNI_TYPE_LONG    ((jni_type_id_t)0x0007U)
#define JNI_TYPE_FLOAT   ((jni_type_id_t)0x0008U)
#define JNI_TYPE_DOUBLE  ((jni_type_id_t)0x0009U)
#define JNI_TYPE_OBJECT  ((jni_type_id_t)0x000AU)
#define JNI_TYPE_ARRAY   ((jni_type_id_t)0x0100U)

#define JNI_TYPE_MASK    0x00ffU

typedef uint16_t jni_type_id_t;
class jni_type_t {
private:
	jni_type_id_t type_;
	std::string   name_;

public:
	jni_type_t()
		: type_(JNI_TYPE_UNKNOWN), name_()
	{
	}
	jni_type_t(jni_type_t const &t)
		: type_(t.type_), name_(t.name_)
	{
	}
	jni_type_t(jni_type_id_t id, bool is_array)
		: type_(id | (is_array ? JNI_TYPE_ARRAY : 0)), name_()
	{
	}
	jni_type_t(bool is_array, char const * const name)
		: type_(JNI_TYPE_OBJECT | (is_array ? JNI_TYPE_ARRAY : 0)), name_(name)
	{
	}
	~jni_type_t()
	{
	}

	jni_type_t &operator = (jni_type_t const &t)
	{
		if (&t == this) {
			return *this;
		}
		type_ = t.type_;
		name_ = t.name_;
		return *this;
	}

	bool is_array() const
	{
		return (JNI_TYPE_ARRAY == (type_ & JNI_TYPE_ARRAY));
	}
	jni_type_id_t type_id() const
	{
		return (type_ & JNI_TYPE_MASK);
	}
	std::string const &name() const
	{
		return name_;
	}
	jni_type_id_t type_id(jni_type_id_t id, bool is_array = false)
	{
		return type_ = (id | (is_array ? JNI_TYPE_ARRAY : 0));
	}
	void name(std::string const &name)
	{
		name_ = name;
	}
};

#endif
