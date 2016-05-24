LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
# Pull OpenCV into the build.
OPENCV_CAMERA_MODULES:=off
OPENCV_INSTALL_MODULES:=on
#OPENCV_LIB_TYPE:=STATIC
include  ${OPENCV_ROOT}/sdk/native/jni/OpenCV.mk

LOCAL_C_INCLUDES := $(OPENCV_ROOT)/sdk/native/jni/include
LOCAL_CPPFLAGS := -std=c++11  
LOCAL_LDLIBS    := -llog -landroid -lGLESv2
LOCAL_MODULE    := JNI_L3
LOCAL_SRC_FILES := JNI_L3.cpp

include $(BUILD_SHARED_LIBRARY)
