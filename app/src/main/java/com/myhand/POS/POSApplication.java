package com.myhand.POS;

import android.app.Application;
import android.os.Environment;
import android.util.Log;

import com.myhand.cpucard.DebitRecord;
import com.myhand.cpucard.SHTCCPUUserCard;
import com.myhand.devices.DeviceV8;
import com.myhand.devices.POSDevice;
import com.myhand.devices.V8Printer;
import com.myhand.shtcdatafile.DataFile;

import java.io.File;
import java.io.FilenameFilter;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public class POSApplication extends Application{
    private User user;
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

        Date date=new Date(2000,3,1,9,30,10);
        String dateStr= SHTCCPUUserCard.dateInCache(date);
        Log.d(tag,String.format("Date is %s",dateStr));
        SimpleDateFormat sdf=new SimpleDateFormat("yyyyMMddHHmmss");
        Log.d(tag,String.format("Date string is %s",sdf.format(date)));
        Log.d(tag,String.format("Date is %s",SHTCCPUUserCard.dateInCache(sdf.format(date))));

    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
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

    //获取消费文件保存路径
    public String getDebitFilePath()
    {
        return dataPath+"/"+DIRDEBIT;
    }

    //打印小票
    public boolean printPayNot(DebitRecord debitRecord){
        V8Printer printer=(V8Printer) posDevice.getPrinter();
        if(printer==null){
            posDevice.setErrorMessage("没有找到打印机");
            return false;
        }

        
        return true;
    }

    //解析黑名单文件
    public boolean parseBLFile(){
        //检查相关目录
        String blPpath=String.format("%s/BL",dataPath);
        String blPathBackup=String.format("%s/Backup",blPpath);
        File file=new File(blPathBackup);
        if(!file.exists()){
            if(!file.mkdirs()){
                return false;
            }
        }

        file=new File(blPpath);
        if(!file.exists()){
            if(!file.mkdirs()){
                return false;
            }
        }
        //查找黑名单文件
        File[] blFiles= file.listFiles(new FilenameFilter() {
            @Override
            public boolean accept(File dir, String name) {
                if(name.substring(0,2).compareTo("BL")==0){
                    return true;
                }
                return false;
            }
        });
        Log.d(tag,String.format("找到%d个黑名单文件",blFiles.length));
        //解析文件
        for(File tmpfile:blFiles){
            Log.d(tag,String.format("开始处理黑名单文件%s......",tmpfile.getName()));
            if(DataFile.parseBLFile(blPpath,tmpfile.getName())){
                Log.d(tag,String.format("成功解析黑名单文件%s",file.getName()));
                if(!tmpfile.renameTo(new File(blPathBackup,file.getName()))){
                    Log.d(tag,String.format("备份黑名单文件%s失败",file.getName()));
                }
            }else{
                Log.d(tag,String.format("黑名单文件%s解析失败",file.getName()));
            }
        }

        return true;
    }
}
