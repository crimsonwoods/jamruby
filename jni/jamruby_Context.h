#ifndef JAMRUBY_CONTEXT_H
#define JAMRUBY_CONTEXT_H

#include <jni.h>
#include "mruby.h"
#include <map>

namespace org {
namespace jamruby {

class jamruby_context {
private:
	jamruby_context(jamruby_context const &ctx);
	jamruby_context &operator = (jamruby_context const &ctx);

	jamruby_context(mrb_state *mrb, JNIEnv *env) : mrb_(mrb), env_(env) {
	}
	~jamruby_context() {
	}

public:
	typedef std::map<mrb_state*, jamruby_context*> map_type;

public:
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

private:
	mrb_state *mrb_;
	JNIEnv    *env_;

	static map_type inner_map;
};

};
};

#endif // end of JAMRUBY_CONTEXT_H

