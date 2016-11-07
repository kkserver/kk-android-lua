//
// Created by zhanghailong on 2016/11/7.
//

#ifndef KK_ANDROID_LUA_KK_H
#define KK_ANDROID_LUA_KK_H

#define UNUSED(x) (void)x

extern JavaVM * gJavaVm;

JNIEnv * kk_env(jboolean * isAttated);

void kk_logv(const char * format,va_list va);

void kk_log(const char * format,...);

#endif //KK_ANDROID_LUA_KK_H
