LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPPFLAGS := -std=c++11  
LOCAL_LDLIBS    := -llog -landroid -lGLESv2
LOCAL_MODULE    := JNI_L3
LOCAL_SRC_FILES := JNI_L3.cpp

include $(BUILD_SHARED_LIBRARY)
