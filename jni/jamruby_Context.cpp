#include "jamruby_Context.h"
extern "C" {
#include "mruby/class.h"
}
#include <algorithm>

namespace org {
namespace jamruby {

jamruby_context::map_type jamruby_context::inner_map;

static void destruct_method_map(jamruby_context::method_map_t::value_type val)
{
	delete val.second;
}

jamruby_context::~jamruby_context()
{
	std::for_each(class_methods.begin(), class_methods.end(), destruct_method_map);
	class_methods.clear();
	std::for_each(instance_methods.begin(), instance_methods.end(), destruct_method_map);
	instance_methods.clear();
}

void jamruby_context::register_method_signature(bool is_class_method, struct RClass *target, 
	char const * const name, char const * const sig)
{
	method_map_t &map = is_class_method ? class_methods : instance_methods;
	method_map_t::iterator it = map.find(target);
	if (it == map.end()) {
		std::pair<method_map_t::iterator, bool> ret = map.insert(method_map_t::value_type(target, new signature_map_t()));
		it = ret.first;
	}
	mrb_sym const sym = mrb_intern(mrb_, name);
	signature_map_t::iterator sig_it = it->second->find(sym);
	if (sig_it == it->second->end()) {
		it->second->insert(signature_map_t::value_type(sym, sig));
	} else {
		sig_it->second = sig;
	}
}

void jamruby_context::unregister_method_signature(bool is_class_method, struct RClass *target,
    char const * const name)
{
	method_map_t &map = is_class_method ? class_methods : instance_methods;
	method_map_t::iterator it = map.find(target);
	if (it == map.end()) {
		return;
	}
	mrb_sym const sym = mrb_intern(mrb_, name);
	signature_map_t::iterator sig_it = it->second->find(sym);
	if (sig_it == it->second->end()) {
		return;
	}
	it->second->erase(sig_it);
	if (it->second->empty()) {
		delete it->second;
		map.erase(it);
	}
}

std::string const &jamruby_context::find_method_signature(bool is_class_method, struct RClass *target,
    char const * const name)
{
	static std::string empty_string;

	method_map_t &map = is_class_method ? class_methods : instance_methods;
	method_map_t::const_iterator it = map.find(target);
	if (it == map.end()) {
		return empty_string;
	}
	mrb_sym const sym = mrb_intern(mrb_, name);
	signature_map_t::const_iterator sig_it = it->second->find(sym);
	if (sig_it == it->second->end()) {
		return empty_string;
	}
	return sig_it->second;
}


};
};

