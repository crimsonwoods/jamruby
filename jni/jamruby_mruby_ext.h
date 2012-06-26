#ifndef JAMRUBY_MRUBY_EXT_H
#define JAMRUBY_MRUBY_EXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mruby.h"

extern mrb_value jamruby_kernel_require(mrb_state *mrb, mrb_value self);

#ifdef __cplusplus
}
#endif

#endif // end of JAMRUBY_MRUBY_EXT_H

