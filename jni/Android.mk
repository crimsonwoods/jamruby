LOCAL_PATH := $(call my-dir)
RUBY_ROOT  := $(LOCAL_PATH)/../../mruby

$(shell mkdir -p $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI))
$(shell cp $(RUBY_ROOT)/android/obj/local/$(TARGET_ARCH_ABI)/libmruby_lib.a $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libmruby.a)
$(shell cp $(RUBY_ROOT)/android/obj/local/$(TARGET_ARCH_ABI)/libmrblib.a $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libmrblib.a)
$(shell mkdir -p $(LOCAL_PATH)/mrblib)
$(shell cp $(RUBY_ROOT)/mrblib/mrblib.c $(LOCAL_PATH)/mrblib/mrblib.c)

include $(CLEAR_VARS)
LOCAL_MODULE := mruby
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE           := jamruby
LOCAL_CPPFLAGS         := -Wall -Werror -O3 -DNO_DEBUG_LOG
LOCAL_SRC_FILES        := $(wildcard *.cpp) $(wildcard *.c) mrblib/mrblib.c
LOCAL_C_INCLUDES       := $(RUBY_ROOT)/include $(RUBY_ROOT)/src
LOCAL_LDLIBS           := -llog 
LOCAL_STATIC_LIBRARIES := mruby

include $(BUILD_SHARED_LIBRARY)
