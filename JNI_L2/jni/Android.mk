LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPPFLAGS := -std=c++11  

LOCAL_LDLIBS    := -llog -landroid -lGLESv2
LOCAL_MODULE    := GL2JNILib
LOCAL_SRC_FILES := gl_code.cpp

include $(BUILD_SHARED_LIBRARY)
