#ifndef JAMRUBY_METHOD_RESOLVER_H
#define JAMRUBY_METHOD_RESOLVER_H

#include <vector>
#include <algorithm>

extern "C" {
#include "mruby.h"
}

#include "jamruby_jni_method_call.h"
#include "jni_Log.h"

namespace org {
namespace jamruby {

class method_resolver {
private:
	struct matcher {
		mrb_state *mrb_;
		mrb_int    argc_;
		mrb_value *argv_;
		std::set<std::string> result_;

		matcher(mrb_state *mrb, mrb_int argc, mrb_value *argv)
			: mrb_(mrb), argc_(argc), argv_(argv) {}
		~matcher() {}

		void operator () (std::string const &signature) {
			LOGD("resolve signature: %s", signature.c_str());
			int const n = get_count_of_arguments(signature.c_str());
			if (n != argc_) {
				return;
			}
			std::vector<jni_type_t> types(n);
			if (!get_argument_types(signature.c_str(), &types[0], n)) {
				return;
			}
			for (int i = 0; i < n; ++i) {
				if (!is_mrb_value_convertible_to(mrb_, argv_[i], types[i])) {
					LOGW("type mismatch: %s - %s", mrb_obj_classname(mrb_, argv_[i]), types[i].name().c_str());
					return;
				}
			}
			LOGD("signature is compatible: %s", signature.c_str());
			result_.insert(signature);
			return;
		}
	};
public:
	method_resolver() {}
	~method_resolver() {}

	template <typename Signatures>
	std::string resolve_ambiguous(mrb_state *mrb, Signatures const &signatures, mrb_int argc, mrb_value *argv) {
		matcher m(mrb, argc, argv);
		matcher result = std::for_each(signatures.begin(), signatures.end(), m);
		if (result.result_.size() == 0) {
			// no matching signature.
			LOGE("resolved signature is not.");
			return std::string();
		}
		if (result.result_.size() != 1) {
			// cannot reolve only one signature.
			LOGE("too many signatures are matched.");
			return std::string();
		}
		return *result.result_.begin();
	}
};

};
};

#endif // end of JAMRUBY_METHOD_RESOLVER_H

