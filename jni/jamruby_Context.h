#ifndef JAMRUBY_CONTEXT_H
#define JAMRUBY_CONTEXT_H

#include <jni.h>
#include "mruby.h"
#include <map>
#include <set>
#include <string>

namespace org {
namespace jamruby {

class jamruby_context {
private:
	jamruby_context(jamruby_context const &ctx);
	jamruby_context &operator = (jamruby_context const &ctx);

	jamruby_context(mrb_state *mrb, JNIEnv *env) : mrb_(mrb), env_(env) {
	}
	~jamruby_context();

	struct class_map_entry_t;
public:
	typedef std::set<std::string>                  signatures_t;
	typedef std::map<mrb_state*, jamruby_context*> map_type;
	typedef std::map<mrb_sym, signatures_t>        signature_map_t;
	typedef std::map<RClass*, class_map_entry_t*>  class_map_t;

	static jamruby_context *register_context(mrb_state *mrb, JNIEnv *env) {
		map_type::const_iterator it = inner_map.find(mrb);
		if (it != inner_map.end()) {
			return it->second;
		}
		jamruby_context *context = NULL;
		try {
			context = new jamruby_context(mrb, env);
			inner_map.insert(map_type::value_type(mrb, context));
		} catch (std::bad_alloc&) {
			delete context;
			context = NULL;
		}
		return context;
	}

	static bool unregister_context(mrb_state *mrb) {
		map_type::iterator it = inner_map.find(mrb);
		if (it == inner_map.end()) {
			return false;
		}
		jamruby_context *context = it->second;
		inner_map.erase(it);
		delete context;
		context = NULL;
		return true;
	}

	static jamruby_context *find_context(mrb_state *mrb) {
		map_type::const_iterator it = inner_map.find(mrb);
		if (it == inner_map.end()) {
			return NULL;
		}
		return it->second;
	}

	mrb_state *get_mrb_state() const {
		return mrb_;
	}
	JNIEnv *get_jni_env() const {
		return env_;
	}

	bool register_method_signature(bool is_class_method, struct RClass *target,
		char const * const name, char const * const sig);
	void unregister_method_signatures(bool is_class_method, struct RClass *target,
		char const * const name);
	signatures_t find_method_signatures(bool is_class_method, struct RClass *target,
		char const * const name);

	bool register_ctor_signature(struct RClass *target, char const * const sig);
	void unregister_ctor_signatures(struct RClass *target);
	signatures_t find_ctor_signatures(struct RClass *target);

	bool register_jclass(struct RClass *target, jclass jcls);
	jclass unregister_jclass(struct RClass *target);
	jclass find_jclass(struct RClass *target);

private:
	mrb_state *mrb_;
	JNIEnv    *env_;
	class_map_t classes;

	static map_type inner_map;

	struct class_map_entry_t {
		jclass           jcls;
		signatures_t    *ctors;
		signature_map_t *class_methods;
		signature_map_t *instance_methods;

		class_map_entry_t() : jcls(NULL), ctors(NULL), class_methods(NULL), instance_methods(NULL) {}
		~class_map_entry_t() {}
	};

	class destruct_class{
	private:
		JNIEnv *env_;
	public:
		destruct_class(JNIEnv *env) : env_(env) {}
		destruct_class(destruct_class const &src) : env_(src.env_) {}
		~destruct_class() {}
		destruct_class &operator = (destruct_class const &src) { env_ = src.env_; return *this; }
		void operator() (jamruby_context::class_map_t::value_type val);
	};

};

};
};

#endif // end of JAMRUBY_CONTEXT_H

