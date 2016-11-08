#include <jni.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "kk.h"

static int Java_cn_kkserver_lua_LuaState_Object_gc(lua_State * L) {

    if(lua_type(L,-1) == LUA_TUSERDATA) {

        jobject * v = lua_touserdata(L,-1);

        if( * v && gJavaVm) {

            jboolean isAttach = 0;

            JNIEnv *env = kk_env(&isAttach);

            if(env) {
                (*env)->DeleteGlobalRef(env,* v);
            }

            if(isAttach) {
                (*gJavaVm)->DetachCurrentThread(gJavaVm);
            }
        }

        * v = NULL;

    }

    kk_log("Java_cn_kkserver_lua_LuaState_Object_gc");

    return 0;
}

static int Java_cn_kkserver_lua_LuaState_Object_get(lua_State * L) {

    int n = 0;
    int top = lua_gettop(L);

    if(top > 1 && lua_type(L,- top) == LUA_TUSERDATA && lua_type(L,- top +1) == LUA_TSTRING) {

        jobject * v = lua_touserdata(L,- top);
        const char * name = lua_tostring(L,- top + 1);

        if( * v && gJavaVm) {

            jboolean isAttach = 0;

            JNIEnv *env = kk_env(&isAttach);

            if(env) {

                jweak * state = NULL;

                lua_getglobal(L,"__JavaLuaState");

                if(lua_type(L,-1) == LUA_TUSERDATA) {
                    state = (jweak *) lua_touserdata(L,-1);
                }

                lua_pop(L,1);

                if(state) {

                    jclass clazz = (* env)->GetObjectClass(env,* state);

                    jmethodID method = (* env)->GetMethodID(env,clazz,"get","(Ljava/lang/Object;Ljava/lang/String;)I");

                    jstring jname = (* env)->NewStringUTF(env,name);

                    n = (* env)->CallIntMethod(env,*state, method,*v,jname);

                    (* env)->DeleteLocalRef(env,jname);

                }
            }

            if(isAttach) {
                (*gJavaVm)->DetachCurrentThread(gJavaVm);
            }
        }

    }

    return n;
}

static int Java_cn_kkserver_lua_LuaState_Object_set(lua_State * L) {

    int top = lua_gettop(L);

    if(top > 2 && lua_type(L,- top) == LUA_TUSERDATA && lua_type(L,- top +1) == LUA_TSTRING) {

        jobject * v = lua_touserdata(L,- top);
        const char * name = lua_tostring(L,- top + 1);

        if( * v && gJavaVm) {

            jboolean isAttach = 0;

            JNIEnv *env = kk_env(&isAttach);

            if(env) {

                jweak * state = NULL;

                lua_getglobal(L,"__JavaLuaState");

                if(lua_type(L,-1) == LUA_TUSERDATA) {
                    state = (jweak *) lua_touserdata(L,-1);
                }

                lua_pop(L,1);

                if(state) {

                    jclass clazz = (* env)->GetObjectClass(env,* state);

                    jmethodID method = (* env)->GetMethodID(env,clazz,"set","(Ljava/lang/Object;Ljava/lang/String;)V");

                    jstring jname = (* env)->NewStringUTF(env,name);

                    (* env)->CallVoidMethod(env,*state, method,*v,jname);

                    (* env)->DeleteLocalRef(env,jname);

                }
            }

            if(isAttach) {
                (*gJavaVm)->DetachCurrentThread(gJavaVm);
            }
        }

    }

    return 0;
}

static int Java_cn_kkserver_lua_LuaState_WeakObject_gc(lua_State * L) {

    if(lua_type(L,-1) == LUA_TUSERDATA) {

        jweak * v = lua_touserdata(L,-1);

        if( * v && gJavaVm) {

            jboolean isAttach = 0;

            JNIEnv *env = kk_env(&isAttach);

            if(env) {
                (*env)->DeleteWeakGlobalRef(env,* v);
            }

            if(isAttach) {
                (*gJavaVm)->DetachCurrentThread(gJavaVm);
            }
        }

        * v = NULL;

    }

    kk_log("Java_cn_kkserver_lua_LuaState_WeakObject_gc");

    return 0;
}

static int Java_cn_kkserver_lua_LuaState_Function(lua_State * L) {

    kk_log("Java_cn_kkserver_lua_LuaState_Function");

    int idx = lua_upvalueindex(1);

    if(lua_type(L,idx) == LUA_TUSERDATA ){

        jobject * fn = (jobject *) lua_touserdata(L,idx);

        if(* fn) {

            jweak * v = NULL;

            lua_getglobal(L,"__JavaLuaState");

            if(lua_type(L,-1) == LUA_TUSERDATA) {
                v = (jweak *) lua_touserdata(L,-1);
            }

            lua_pop(L,1);

            if(v != NULL) {

                jint n = 0;
                jboolean isAttach = 0;

                JNIEnv *env = kk_env(&isAttach);

                if(env) {
                    kk_log("Java_cn_kkserver_lua_LuaState_Function 1");
                    jclass clazz = (*env)->FindClass(env,"cn/kkserver/lua/LuaFunction");
                    jmethodID invoke = (*env)->GetMethodID(env,clazz,"invoke","(Lcn/kkserver/lua/LuaState;)I");
                    n = (*env)->CallIntMethod(env,*fn,invoke,*v);
                    kk_log("Java_cn_kkserver_lua_LuaState_Function 2");
                }

                if(isAttach) {
                    (*gJavaVm)->DetachCurrentThread(gJavaVm);
                }

                return n;

            }

        }

    }

    return 0;
}

static int Java_cn_kkserver_lua_LuaState_print(lua_State * L) {

    int top = lua_gettop(L);

    for(int i=0;i<top;i++ ){
        int type = lua_type(L, -top + i);
        switch(type) {
            case LUA_TNIL:
                kk_log("<nil>");
            case LUA_TNUMBER:
                if(lua_isinteger(L,-top + i)) {
                    kk_log("%d",lua_tointeger(L, -top +i));
                }
                else {
                    kk_log("%f",lua_tonumber(L, -top +i));
                }
            case LUA_TBOOLEAN:
                kk_log("%s",lua_toboolean(L, -top +i) ? "true":"false");
            case LUA_TSTRING:
                kk_log("%s",lua_tostring(L,- top + 1));
            case LUA_TUSERDATA:
                kk_log("<userdata>");
            case LUA_TLIGHTUSERDATA:
                kk_log("<lightuserdata>");
            case LUA_TFUNCTION:
                kk_log("<function>");
            case LUA_TTABLE:
                kk_log("<table>");
            case LUA_TTHREAD:
                kk_log("<thread>");
        }
    }

    return 0;
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_dealloc(JNIEnv *env, jclass type, jlong ptr) {
    UNUSED(env);
    UNUSED(type);
    lua_State * L = (lua_State *) (long) ptr;
    lua_close(L);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_openlibs(JNIEnv *env, jclass type, jlong ptr) {
    UNUSED(env);
    UNUSED(type);
    lua_State * L = (lua_State *) (long) ptr;
    luaL_openlibs(L);

    lua_pushcfunction(L,Java_cn_kkserver_lua_LuaState_print);
    lua_setglobal(L,"print");

}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_newtable(JNIEnv *env, jclass type, jlong ptr) {
    UNUSED(env);
    UNUSED(type);
    lua_State * L = (lua_State *) (long) ptr;
    lua_newtable(L);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_rawset(JNIEnv *env, jclass type, jlong ptr, jint idx) {
    UNUSED(env);
    UNUSED(type);
    lua_State * L = (lua_State *) (long) ptr;
    lua_rawset(L,(int) idx);
}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_rawget(JNIEnv *env, jclass type, jlong ptr, jint idx) {
    UNUSED(env);
    UNUSED(type);
    lua_State * L = (lua_State *) (long) ptr;
    return lua_rawget(L,(int) idx);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_pushstring(JNIEnv *env, jclass type, jlong ptr, jstring value_) {

    UNUSED(type);

    const char *value = (*env)->GetStringUTFChars(env, value_, 0);

    lua_State * L = (lua_State *) (long) ptr;
    lua_pushstring(L,value);

    (*env)->ReleaseStringUTFChars(env, value_, value);

}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_pushinteger(JNIEnv *env, jclass type, jlong ptr, jint value) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    lua_pushinteger(L,(lua_Integer) value);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_pushboolean(JNIEnv *env, jclass type, jlong ptr, jboolean value) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    lua_pushboolean(L,value);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_pushnumber(JNIEnv *env, jclass type, jlong ptr, jdouble value) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    lua_pushnumber(L,value);
}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_gettop(JNIEnv *env, jclass type, jlong ptr) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    return (jint) lua_gettop(L);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_pop(JNIEnv *env, jclass type, jlong ptr, jint count) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    lua_pop(L,count);
}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_pcall(JNIEnv *env, jclass type, jlong ptr, jint n, jint r) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    return lua_pcall(L,n,r,0);
}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_type(JNIEnv *env, jclass type, jlong ptr, jint idx) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    return lua_type(L,idx);
}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_loadstring(JNIEnv *env, jclass type, jlong ptr, jstring string_) {

    UNUSED(type);

    const char *string = (*env)->GetStringUTFChars(env, string_, 0);

    lua_State * L = (lua_State *) (long) ptr;

    int r = luaL_loadstring(L,string);

    (*env)->ReleaseStringUTFChars(env, string_, string);

    return r;
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_pushnil(JNIEnv *env, jclass type, jlong ptr) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    lua_pushnil(L);
}


JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_pushobject(JNIEnv *env, jclass type, jlong ptr, jobject object) {

    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;

    jobject * v = lua_newuserdata(L,sizeof(jobject));

    *v = (* env)->NewGlobalRef(env,object);

    lua_newtable(L);
    lua_pushstring(L,"__gc");
    lua_pushcfunction(L,Java_cn_kkserver_lua_LuaState_Object_gc);
    lua_rawset(L,-3);

    lua_pushstring(L,"__index");
    lua_pushcfunction(L,Java_cn_kkserver_lua_LuaState_Object_get);
    lua_rawset(L,-3);

    lua_pushstring(L,"__newindex");
    lua_pushcfunction(L,Java_cn_kkserver_lua_LuaState_Object_set);
    lua_rawset(L,-3);

    lua_setmetatable(L,-2);

}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_setglobal(JNIEnv *env, jclass type, jlong ptr, jstring name_) {

    UNUSED(type);

    const char *name = (*env)->GetStringUTFChars(env, name_, 0);

    lua_State * L = (lua_State *) (long) ptr;

    lua_setglobal(L,name);

    (*env)->ReleaseStringUTFChars(env, name_, name);
}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_getglobal(JNIEnv *env, jclass type, jlong ptr, jstring name_) {

    UNUSED(type);

    const char *name = (*env)->GetStringUTFChars(env, name_, 0);

    lua_State * L = (lua_State *) (long) ptr;

    int r = lua_getglobal(L,name);

    (*env)->ReleaseStringUTFChars(env, name_, name);

    return r;
}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_setmetatable(JNIEnv *env, jclass type, jlong ptr, jint idx) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long)  ptr;
    return lua_setmetatable(L,idx);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_gc(JNIEnv *env, jclass type, jlong ptr) {
    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    lua_gc(L,LUA_GCCOLLECT,0);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_pushfunction(JNIEnv *env, jclass type, jlong ptr, jobject func) {

    lua_State * L = (lua_State *) (long) ptr;

    Java_cn_kkserver_lua_LuaState_pushobject(env,type,ptr,func);

    lua_pushcclosure(L,Java_cn_kkserver_lua_LuaState_Function,1);

}

JNIEXPORT jlong JNICALL
Java_cn_kkserver_lua_LuaState_alloc(JNIEnv *env, jclass type, jobject object) {

    UNUSED(type);

    lua_State * L = luaL_newstate();

    jweak * v = lua_newuserdata(L,sizeof(jweak));

    * v = (* env)->NewWeakGlobalRef(env,object);

    lua_newtable(L);
    lua_pushstring(L,"__gc");
    lua_pushcfunction(L,Java_cn_kkserver_lua_LuaState_WeakObject_gc);
    lua_rawset(L,-3);

    lua_setmetatable(L,-2);

    lua_setglobal(L,"__JavaLuaState");

    return (jlong) (long) L;

}

JNIEXPORT jstring JNICALL
Java_cn_kkserver_lua_LuaState_tostring(JNIEnv *env, jclass type, jlong ptr, jint idx) {

    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;

    if(lua_type(L,idx) == LUA_TSTRING) {
        return (*env)->NewStringUTF(env, lua_tostring(L,idx));
    }

    return NULL;
}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_tointeger(JNIEnv *env, jclass type, jlong ptr, jint idx) {

    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;

    if(lua_type(L,idx) == LUA_TNUMBER) {
        return lua_tointeger(L,idx);
    }

    return 0;
}

JNIEXPORT jboolean JNICALL
Java_cn_kkserver_lua_LuaState_toboolean(JNIEnv *env, jclass type, jlong ptr, jint idx) {

    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;

    if(lua_type(L,idx) == LUA_TBOOLEAN) {
        return lua_toboolean(L,idx);
    }

    return 0;

}

JNIEXPORT jdouble JNICALL
Java_cn_kkserver_lua_LuaState_tonumber(JNIEnv *env, jclass type, jlong ptr, jint idx) {

    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;

    if(lua_type(L,idx) == LUA_TNUMBER) {
        return lua_tonumber(L,idx);
    }

    return 0;

}

JNIEXPORT jobject JNICALL
Java_cn_kkserver_lua_LuaState_toobject(JNIEnv *env, jclass type, jlong ptr, jint idx) {

    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;

    if(lua_type(L,idx) == LUA_TUSERDATA) {
        jobject * v = (jobject *) lua_touserdata(L,idx);
        return * v;
    }

    return NULL;

}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_getmetatable(JNIEnv *env, jclass type, jlong ptr, jint idx) {

    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;

    return lua_getmetatable(L,idx);

}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_ref(JNIEnv *env, jclass type, jlong ptr) {

    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    return luaL_ref(L,LUA_REGISTRYINDEX);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_unref(JNIEnv *env, jclass type, jlong ptr, jint ref) {

    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    luaL_unref(L,LUA_REGISTRYINDEX,ref);

}

JNIEXPORT jint JNICALL
Java_cn_kkserver_lua_LuaState_getref(JNIEnv *env, jclass type, jlong ptr, jint ref) {

    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    return lua_rawgeti(L,LUA_REGISTRYINDEX,ref);
}

JNIEXPORT void JNICALL
Java_cn_kkserver_lua_LuaState_pushvalue(JNIEnv *env, jclass type, jlong ptr, jint idx) {

    UNUSED(env);
    UNUSED(type);

    lua_State * L = (lua_State *) (long) ptr;
    lua_pushvalue(L,idx);
}