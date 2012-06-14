#ifndef JAMRUBY_JTHROWABLE_H
#define JAMRUBY_JTHROWABLE_H

#include <jni.h>
extern "C" {
#include "mruby.h"
}

extern int jthrowable_init_class(mrb_state *mrb);

#endif // end of JAMRUBY_JTHROWABLE_H

