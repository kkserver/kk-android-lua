package cn.kkserver.lua;

/**
 * Created by zhanghailong on 2016/11/8.
 */

public interface IObjectReflect {

    public boolean canReflectToJavaObject(Object object);

    public boolean canReflectToLuaObject(Object object);

    public Object reflectToJavaObject(Object object);

    public Object reflectToLuaObject(Object object);

}
