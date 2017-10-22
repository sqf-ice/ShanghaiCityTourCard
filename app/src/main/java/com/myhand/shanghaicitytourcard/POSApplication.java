package com.myhand.shanghaicitytourcard;

import android.app.Application;
import android.content.Context;
import android.os.Environment;
import android.util.Log;

import com.myhand.POS.DatabaseSHCT;
import com.myhand.common.AppDatabase;
import com.myhand.devices.DeviceV8;
import com.myhand.devices.POSDevice;

import java.io.File;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public class POSApplication extends Application{
    private DatabaseSHCT appDatabase;
    private POSDevice posDevice;
    private static final String tag="V8App";
    //应用数据文件目录
    private String dataPath;

    public static final String DIRDEBIT="Debit";

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(tag,"V8 Application start......");
        appDatabase=new DatabaseSHCT(getApplicationContext(),"DatabaseSHCT",1);
        appDatabase.init();
        Log.d(tag,"Shanghai City Tour Card Application started");
        instance=this;

        posDevice=new DeviceV8();
        initDataPath();
    }

    public String getDataPath() {
        return dataPath;
    }

    public void setDataPath(String dataPath) {
        this.dataPath = dataPath;
    }

    public DatabaseSHCT getAppDatabase()
    {
        return appDatabase;
    }

    public static POSApplication instance;
    // 获取Application
    public static POSApplication getPOSApplication() {
        return instance;
    }

    public POSDevice getPosDevice()
    {
        return posDevice;
    }

    //初始化数据目录
    public void initDataPath()
    {
        //检查是否有外置SD卡
        if(Environment.getExternalStorageState().equals(
                Environment.MEDIA_MOUNTED))
        {
            dataPath=Environment.getExternalStorageDirectory()
                    .getAbsolutePath();
            //dataPath="/sdcard2";
        }else{
            //没有外置SD卡时，保存到内置SD卡的Data目录下
            dataPath= Environment.getDataDirectory().getPath();
        }
        Log.d(tag,"Datapath:"+dataPath);

        dataPath+="/"+POSApplication.class.getSimpleName();
        //创建应用数据目录
        File appDataPath=new File(dataPath);
        if(!appDataPath.exists()) {
            boolean b = appDataPath.mkdirs();
            if (!b) {
                Log.d(tag, "应用数据目录" + appDataPath + "创建失败");
            }
        }else
        {
            Log.d(tag,"应用数据目录" + appDataPath + "成功创建");
        }
        //创建消费文件存放目录
        File debitPath=new File(getDebitFilePath());
        if(!debitPath.exists()) {
            boolean b = debitPath.mkdirs();
            if(!b)
            {
                Log.d(tag,"创建消费记录文件目录"+getDebitFilePath()+"失败");
            }
        }
    }

    public String getDebitFilePath()
    {
        return dataPath+"/"+DIRDEBIT;
    }
}
