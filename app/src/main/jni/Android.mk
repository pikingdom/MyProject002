LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := image
LOCAL_SRC_FILES := image.cpp
LOCAL_LDLIBS += -llog -ljnigraphics

include $(BUILD_SHARED_LIBRARY)
