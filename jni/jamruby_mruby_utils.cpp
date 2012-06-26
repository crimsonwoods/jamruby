#include "jamruby_mruby_utils.h"
extern "C" {
#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/class.h"
}

char const *get_called_mrb_func_name(mrb_state *mrb)
{
	return mrb_sym2name(mrb, mrb->ci->mid);
}

RClass *get_called_mrb_class(mrb_state *mrb)
{
	return mrb->ci->target_class;
}

RProc *replace_mrb_func(mrb_state *mrb, RClass *klass, char const * const name, mrb_func_t func)
{
	mrb_sym const mid = mrb_intern(mrb, name);
	khash_t(mt) * const h = klass->mt;
	khiter_t k = kh_get(mt, h, mid);
	if ((k != kh_end(h)) && kh_exist(h, k)) {
		RProc * const proc = kh_value(h, k);
		if (NULL != proc) {
			RProc *newproc = mrb_proc_new_cfunc(mrb, func);
			if (NULL == newproc) {
				return NULL;
			}
			newproc->target_class = klass;
			kh_value(h, k) = newproc;
			return proc;
		}
	}
	return NULL;
}

