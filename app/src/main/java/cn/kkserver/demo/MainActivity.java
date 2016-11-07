package cn.kkserver.demo;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import java.util.Map;
import java.util.TreeMap;

import cn.kkserver.lua.LuaState;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        LuaState L = new LuaState();

        L.openlibs();

        Map<String,Object> object = new TreeMap<>();

        if(0 == L.loadstring("return function(object) object.name='OK'; object.title = object.name; end")) {

            if(0 == L.pcall(0,1)) {

                L.pushValue(object);

                if(0 == L.pcall(1,0)) {
                    Log.d("kk","OK");
                }
                else {
                    Log.d("kk",L.tostring(-1));
                }

            }
            else {
                Log.d("kk",L.tostring(-1));
            }

        }
        else {
            Log.d("kk",L.tostring(-1));
        }

        L.pop(L.gettop());


        L.gc();

    }
}
