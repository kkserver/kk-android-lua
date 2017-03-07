package cn.kkserver.lua;

import android.os.Handler;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import cn.kkserver.core.Value;

/**
 * Created by zhanghailong on 2016/11/7.
 */

public class LuaState extends Handler {

    public final static int LUA_TNIL = 0;
    public final static int LUA_TBOOLEAN = 1;
    public final static int LUA_TLIGHTUSERDATA = 2;
    public final static int LUA_TNUMBER = 3;
    public final static int LUA_TSTRING = 4;
    public final static int LUA_TTABLE = 5;
    public final static int LUA_TFUNCTION = 6;
    public final static int LUA_TUSERDATA = 7;
    public final static int LUA_TTHREAD = 8;
    public final static int LUA_NUMTAGS = 9;

    public final static int LUA_TOBJECT = LUA_TUSERDATA;

    static {
        System.loadLibrary("kk-lua");
    }

    private static final List<IObjectReflect> _reflects = new ArrayList<>(4);

    public static void addObjectReflect(IObjectReflect reflect) {
        _reflects.add(reflect);
    }

    private final long _ptr;

    public LuaState() {
        _ptr = alloc(this);
    }

    @Override
    protected void finalize() throws Throwable {
        dealloc(_ptr);
        super.finalize();
    }

    public int get(Object object,String name) {

        pushValue(Value.get(object,name));

        return 1;
    }

    public void set(Object object,String name) {


        if(object != null) {

            Object v = toValue(-1);

            Value.set(object,name,v);

        }

    }

    public void openlibs() {
        openlibs(_ptr);
    }

    public void newtable() {
        newtable(_ptr);
    }

    public void rawset(int idx) {
        rawset(_ptr,idx);
    }

    public int rawget(int idx) {
        return rawget(_ptr,idx);
    }

    public void pushValue(Object value) {

        if(value == null) {
            pushnil(_ptr);
        }
        else if(value instanceof String) {
            pushstring(_ptr,(String) value);
        }
        else if(value instanceof Integer) {
            pushinteger(_ptr,(Integer) value);
        }
        else if(value instanceof Long) {
            pushstring(_ptr,String.valueOf(value));
        }
        else if(value instanceof Short) {
            pushinteger(_ptr,((Number) value).intValue());
        }
        else if(value instanceof Float) {
            pushnumber(_ptr,((Number) value).doubleValue());
        }
        else if(value instanceof Double) {
            pushnumber(_ptr,(Double) value);
        }
        else if(value instanceof Boolean) {
            pushboolean(_ptr,(Boolean) value);
        }
        else if(value instanceof LuaFunction) {
            pushfunction(_ptr,(LuaFunction) value);
        }
        else {

            for(IObjectReflect v : _reflects) {
                if(v.canReflectToLuaObject(value)) {
                    value = v.reflectToLuaObject(value);
                    break;
                }
            }

            pushobject(_ptr,value);
        }
    }

    public void pushvalue(int idx) {
        pushvalue(_ptr,idx);
    }

    public void pushnil() {
        pushnil(_ptr);
    }

    public void pushstring(String value) {
        pushstring(_ptr,value);
    }

    public void pushinteger(int value) {
        pushinteger(_ptr,value);
    }

    public void pushnumber(double value) {
        pushnumber(_ptr,value);
    }

    public void pushboolean(boolean value) {
        pushboolean(_ptr,value);
    }

    public void pushfunction(LuaFunction value) {
        pushfunction(_ptr,value);
    }

    public void pushobject(Object value) {
        pushobject(_ptr,value);
    }

    public int gettop() {
        return gettop(_ptr);
    }

    public int type(int idx) {
        return type(_ptr,idx);
    }

    public void pop(int n) {
        pop(_ptr,n);
    }

    public int loadstring(String string) {
        return loadstring(_ptr,string);
    }

    public int pcall(int n,int r) {
        return pcall(_ptr,n,r);
    }

    public void gc() {
        gc(_ptr);
    }

    public String tostring(int idx) {
        return tostring(_ptr,idx);
    }

    public int tointeger(int idx) {
        return tointeger(_ptr,idx);
    }

    public boolean toboolean(int idx) {
        return toboolean(_ptr,idx);
    }

    public double tonumber(int idx) {
        return tonumber(_ptr,idx);
    }

    public Object toobject(int idx) {
        return toobject(_ptr,idx);
    }

    public Object toValue(int idx) {

        switch (type(_ptr,idx)) {
            case LUA_TOBJECT:
                Object v = toobject(_ptr,idx);
                for(IObjectReflect r : _reflects) {
                    if(r.canReflectToJavaObject(v)) {
                        v = r.reflectToJavaObject(v);
                        break;
                    }
                }
                return v;
            case LUA_TSTRING:
                return tostring(_ptr,idx);
            case LUA_TNUMBER:
                return tonumber(_ptr,idx);
            case LUA_TBOOLEAN:
                return toboolean(_ptr,idx);
            case LUA_TTABLE:
                pushvalue(_ptr,idx);
                int size = 0;
                int i = 0;

                List<Object> vs = new ArrayList<Object>();
                Map<String,Object> map = new TreeMap<String,Object>();

                pushnil(_ptr);

                while( next(_ptr,-2) != 0) {

                    switch(type(_ptr,-2)) {
                        case LUA_TNUMBER: {
                            if (i + 1 == tointeger(_ptr, -2)) {
                                i++;
                            }
                            Object value = toValue(-1);
                            if (value != null) {
                                vs.add(value);
                            }
                        }
                        break;
                        case LUA_TSTRING: {
                            String key = tostring(_ptr, -2);
                            if (key != null) {
                                Object value = toValue(-1);
                                if (value != null) {
                                    map.put(key, value);
                                }
                            }
                        }
                        break;
                    }

                    size ++;

                    pop(_ptr,1);

                }

                pop(_ptr,1);

                if(size == 0) {
                    return map;
                }

                if(i == size) {
                    return vs;
                }

                return map;
            case LUA_TFUNCTION:
                return "[function]";
            case LUA_TLIGHTUSERDATA:
                return "[lightuserdata]";
            case LUA_TTHREAD:
                return "[thread]";
        }

        return null;
    }

    public int setmetatable(int idx) {
        return setmetatable(_ptr,idx);
    }

    public int getmetatable(int idx) {
        return getmetatable(_ptr,idx);
    }

    public void setglobal(String name ) {
        setglobal(_ptr,name);
    }

    public int getglobal(String name) {
        return getglobal(_ptr,name);
    }


    public int ref() {
        return ref(_ptr);
    }

    public void unref(int ref) {
        unref(_ptr,ref);
    }

    public int getref(int ref) {
        return getref(_ptr,ref);
    }

    public int next(int idx) { return next(_ptr,idx); }

    private final static native long alloc(Object v);

    private final static native void dealloc(long ptr);

    private final static native void openlibs(long ptr);

    private final static native void newtable(long ptr);

    private final static native void rawset(long ptr,int idx);

    private final static native int rawget(long ptr,int idx);

    private final static native void pushstring(long ptr,String value);

    private final static native void pushinteger(long ptr, int value);

    private final static native void pushboolean(long ptr, boolean value);

    private final static native void pushnumber(long ptr, double value);

    private final static native void pushnil(long ptr);

    private final static native int gettop(long ptr);

    private final static native void pop(long ptr, int count);

    private final static native int pcall(long ptr, int n, int r);

    private final static native int type(long ptr,int idx);

    private final static native int loadstring(long ptr, String string);

    private final static native void pushobject(long ptr, Object object);

    private final static native void setglobal(long ptr,String name);

    private final static native int getglobal(long ptr,String name);

    private final static native int setmetatable(long ptr, int idx);

    private final static native int getmetatable(long ptr, int idx);

    private final static native void pushfunction(long ptr, LuaFunction func);

    private final static native void gc(long ptr);

    private final static native String tostring(long ptr,int idx);

    private final static native int tointeger(long ptr, int idx);

    private final static native boolean toboolean(long ptr, int idx);

    private final static native double tonumber(long ptr, int idx);

    private final static native Object toobject(long ptr, int idx);

    private final static native int ref(long ptr);

    private final static native void unref(long ptr, int ref);

    private final static native int getref(long ptr, int ref);

    private final static native void pushvalue(long ptr, int idx);

    private final static native int next(long ptr, int idx);

}
