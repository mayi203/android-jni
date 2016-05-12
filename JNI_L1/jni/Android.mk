LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_LDLIBS    := -llog -landroid
LOCAL_MODULE    := JNILIB
LOCAL_SRC_FILES := com_example_jni_l1_NativeInterface.cpp
include $(BUILD_SHARED_LIBRARY)