LOCAL_PATH := $(call my-dir)
RUBY_ROOT  := $(LOCAL_PATH)/../../mruby
#EXTRA_FLAGS:=
EXTRA_FLAGS:= -DNO_DEBUG_LOG

$(shell mkdir -p $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI))
$(shell cp $(RUBY_ROOT)/build/android-$(TARGET_ARCH_ABI)/lib/libmruby.a $(LOCAL_PATH)/../obj/local/$(TARGET_ARCH_ABI)/libmruby.a)

include $(CLEAR_VARS)
LOCAL_MODULE := mruby
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE           := jamruby
LOCAL_CPPFLAGS         := -Wall -Werror -O3 $(EXTRA_FLAGS)
LOCAL_SRC_FILES        := $(wildcard *.cpp) $(wildcard *.c)
LOCAL_C_INCLUDES       := $(RUBY_ROOT)/include $(RUBY_ROOT)/src
LOCAL_LDLIBS           := -llog 
LOCAL_STATIC_LIBRARIES := mruby

include $(BUILD_SHARED_LIBRARY)
