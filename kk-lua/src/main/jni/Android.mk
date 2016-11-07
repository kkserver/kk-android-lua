LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

KK_SRC_FILES := kk-lua.c kk.c

LUA_SRC := lua-5.3.0/src

LOCAL_MODULE := kk-lua

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(LUA_SRC)

LOCAL_SRC_FILES := $(LUA_SRC)/lapi.c \
$(LUA_SRC)/lcode.c \
$(LUA_SRC)/lctype.c \
$(LUA_SRC)/ldebug.c \
$(LUA_SRC)/ldo.c \
$(LUA_SRC)/ldump.c \
$(LUA_SRC)/lfunc.c \
$(LUA_SRC)/lgc.c \
$(LUA_SRC)/llex.c \
$(LUA_SRC)/lmem.c \
$(LUA_SRC)/lobject.c \
$(LUA_SRC)/lopcodes.c \
$(LUA_SRC)/lparser.c \
$(LUA_SRC)/lstate.c \
$(LUA_SRC)/lstring.c \
$(LUA_SRC)/ltable.c \
$(LUA_SRC)/ltm.c \
$(LUA_SRC)/lundump.c \
$(LUA_SRC)/lvm.c \
$(LUA_SRC)/lzio.c \
$(LUA_SRC)/lauxlib.c \
$(LUA_SRC)/lbaselib.c \
$(LUA_SRC)/lbitlib.c \
$(LUA_SRC)/lcorolib.c \
$(LUA_SRC)/ldblib.c \
$(LUA_SRC)/liolib.c \
$(LUA_SRC)/lmathlib.c \
$(LUA_SRC)/loslib.c \
$(LUA_SRC)/lstrlib.c \
$(LUA_SRC)/ltablib.c \
$(LUA_SRC)/loadlib.c \
$(LUA_SRC)/linit.c \
$(LUA_SRC)/lutf8lib.c \
$(KK_SRC_FILES)

LOCAL_CFLAGS := -O3 -DLUA_USE_LINUX -ldl -std=gnu99 -Wall -Wextra -DLUA_COMPAT_5_2 $(SYSCFLAGS) $(MYCFLAGS)

LOCAL_LDLIBS := -lm -llog

include $(BUILD_SHARED_LIBRARY)