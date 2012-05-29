#ifndef ORG_JAMRUBY_MRUBY_RFUNC_H
#define ORG_JAMRUBY_MRUBY_RFUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mruby.h"

extern mrb_value jamruby_func_dispatch(mrb_state *mrb, mrb_value self);

#ifdef __cplusplus
}
#endif

#endif // end of ORG_JAMRUBY_MRUBY_RFUNC_H

