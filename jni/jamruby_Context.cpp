#include "jamruby_Context.h"
extern "C" {
#include "mruby/class.h"
}
#include <algorithm>

namespace org {
namespace jamruby {

jamruby_context::map_type jamruby_context::inner_map;

void jamruby_context::destruct_class::operator() (jamruby_context::class_map_t::value_type val)
{
	if (val.second) {
		env_->DeleteGlobalRef(val.second->jcls);
		val.second->jcls = NULL;
		delete val.second->class_methods;
		val.second->class_methods = NULL;
		delete val.second->instance_methods;
		val.second->instance_methods = NULL;
	}
	delete val.second;
	val.second = NULL;
}

jamruby_context::~jamruby_context()
{
	std::for_each(classes.begin(), classes.end(), destruct_class(env_));
	classes.clear();
}

bool jamruby_context::register_method_signature(bool is_class_method, struct RClass *target, 
	char const * const name, char const * const sig)
{
	class_map_t::iterator it = classes.find(target);
	if (it == classes.end()) {
		class_map_entry_t *entry = new class_map_entry_t();
		if (NULL == entry) {
			return false;
		}
		std::pair<class_map_t::iterator, bool> ret = classes.insert(class_map_t::value_type(target, entry));
		it = ret.first;
	}

	signature_map_t *&sig_map = is_class_method ? it->second->class_methods : it->second->instance_methods;
	if (NULL == sig_map) {
		sig_map = new signature_map_t();
		if (NULL == sig_map) {
			return false;
		}
	}

	mrb_sym const sym = mrb_intern(mrb_, name);
	signature_map_t::iterator sig_it = sig_map->find(sym);
	if (sig_it == sig_map->end()) {
		signatures_t signatures;
		signatures.insert(sig);
		sig_map->insert(signature_map_t::value_type(sym, signatures));
	} else {
		sig_it->second.insert(sig);
	}
	return true;
}

void jamruby_context::unregister_method_signatures(bool is_class_method, struct RClass *target,
    char const * const name)
{
	class_map_t::iterator it = classes.find(target);
	if (it == classes.end()) {
		return;
	}
	signature_map_t *&sig_map = is_class_method ? it->second->class_methods : it->second->instance_methods;
	if (NULL == sig_map) {
		return;
	}
	mrb_sym const sym = mrb_intern(mrb_, name);
	signature_map_t::iterator sig_it = sig_map->find(sym);
	if (sig_it == sig_map->end()) {
		return;
	}
	sig_map->erase(sig_it);
	if (sig_map->empty()) {
		delete sig_map;
		sig_map = NULL;
		classes.erase(it);
	}
}

jamruby_context::signatures_t jamruby_context::find_method_signatures(bool is_class_method, struct RClass *target,
    char const * const name)
{
	class_map_t::const_iterator it = classes.find(target);
	if (it == classes.end()) {
		return signatures_t();
	}
	signature_map_t *sig_map = is_class_method ? it->second->class_methods : it->second->instance_methods;
	if (NULL == sig_map) {
		return signatures_t();
	}
	mrb_sym const sym = mrb_intern(mrb_, name);
	signature_map_t::const_iterator sig_it = sig_map->find(sym);
	if (sig_it == sig_map->end()) {
		return signatures_t();
	}
	return sig_it->second;
}

bool jamruby_context::register_ctor_signature(struct RClass *target, char const * const sig)
{
	class_map_t::iterator it = classes.find(target);
	if (it == classes.end()) {
		class_map_entry_t *entry = NULL;
		try {
			entry = new class_map_entry_t();
		} catch (std::bad_alloc &) {
			return false;
		}
		if (NULL == entry) {
			return false;
		}
		std::pair<class_map_t::iterator, bool> ret = classes.insert(class_map_t::value_type(target, entry));
		if (!ret.second) {
			return false;
		}
		it = ret.first;
	}

	signatures_t *&signatures = it->second->ctors;
	if (NULL == signatures) {
		try {
			signatures = new signatures_t();
		} catch (std::bad_alloc &) {
			return false;
		}
		if (NULL == signatures) {
			return false;
		}
	}

	return signatures->insert(sig).second;
}

void jamruby_context::unregister_ctor_signatures(struct RClass *target)
{
	class_map_t::iterator it = classes.find(target);
	if (it == classes.end()) {
		return;
	}
	signatures_t *&signatures = it->second->ctors;
	if (NULL == signatures) {
		return;
	}
	signatures->clear();
}

jamruby_context::signatures_t jamruby_context::find_ctor_signatures(struct RClass *target)
{
	static signatures_t empty_signatures;

	class_map_t::iterator it = classes.find(target);
	if (it == classes.end()) {
		return signatures_t();
	}

	if (NULL == it->second->ctors) {
		return signatures_t();
	}

	return *it->second->ctors;
}

bool jamruby_context::register_jclass(struct RClass *target, jclass jcls)
{
	class_map_t::iterator it = classes.find(target);
	if (it == classes.end()) {
		class_map_entry_t *entry = new class_map_entry_t();
		if (NULL == entry) {
			return false;
		}
		std::pair<class_map_t::iterator, bool> ret = classes.insert(class_map_t::value_type(target, entry));
		it = ret.first;
	}

	it->second->jcls = jcls;

	return true;
}

jclass jamruby_context::unregister_jclass(struct RClass *target)
{
	class_map_t::iterator it = classes.find(target);
	if (it == classes.end()) {
		return NULL;
	}
	if (NULL == it->second) {
		return NULL;
	}
	jclass ret = it->second->jcls;
	it->second->jcls = NULL;
	return ret;
}

jclass jamruby_context::find_jclass(struct RClass *target)
{
	class_map_t::const_iterator it = classes.find(target);
	if (it == classes.end()) {
		return NULL;
	}
	if (NULL == it->second) {
		return NULL;
	}
	return it->second->jcls;
}


};
};

