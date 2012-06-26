#ifndef JAMRUBY_MRUBY_UTILS_H
#define JAMRUBY_MRUBY_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mruby.h"

struct RProc;
struct RClass;

extern char const    *get_called_mrb_func_name(mrb_state *mrb);
extern struct RClass *get_called_mrb_class(mrb_state *mrb);
extern struct RProc  *replace_mrb_func(mrb_state *mrb, struct RClass *klass, char const * const name, mrb_func_t func);

#ifdef __cplusplus
}
#endif

#endif // end of JAMRUBY_MRUBY_UTILS_H

