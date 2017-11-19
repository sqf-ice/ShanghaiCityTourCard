package com.myhand.POS;

import android.util.Log;

import com.myhand.cpucard.DebitRecord;
import com.myhand.devices.POSDevice;
import com.myhand.shtcdatafile.FHFileDescription;
import com.myhand.shtcdatafile.FHFileRecord;
import com.myhand.shtcdatafile.FileName;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

/**
 * Created by wenha_000 on 2017-10-13.
 */

public class FileMaker {
    private static final String tag= FileMaker.class.getSimpleName();
    /**
     * 创建消费交易文件
     * @return
     */
    public static boolean FHFileMake(){
        DatabaseSHCT db= POSApplication.instance.getAppDatabase();
        List<DebitRecord> debitRecordList=db.debitQuery("",0);
        if(debitRecordList.size()==0){
            Log.d(tag,"FHFileMake:没有需打包的记录");
            return false;
        }

        POSDevice pos= POSApplication.instance.getPosDevice();
        FHFileRecord fileRecord=new FHFileRecord();
        FileName fileName=new FileName(String.format("%02d",pos.getTradeCode()),pos.getPosID(),(int)db.getPosSendSequence(pos.getPosID()),"H");
        FHFileDescription fhFileDescription=new FHFileDescription(debitRecordList.size(),fileRecord.getDataFieldLength(),true);

        String filePath=String.format("%s/%s",POSApplication.instance.getDebitFilePath(),fileName.getData());
        //开始文件写
        try {
            FileOutputStream fileOutputStream=new FileOutputStream(filePath);
            fileOutputStream.write(fhFileDescription.getData().getBytes());
            for(DebitRecord record:debitRecordList){
                FHFileRecord fhFileRecord=new FHFileRecord();
                fhFileRecord.fromDebitRecord(record);
                fileOutputStream.write(fhFileRecord.getData().getBytes());
            }
            fileOutputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Log.d(tag,String.format("打开文件写异常：%s",e.getLocalizedMessage()));
            return false;
        } catch (IOException e) {
            e.printStackTrace();
            Log.d(tag,String.format("文件写异常：%s",e.getLocalizedMessage()));
        }

        db.registFHFile(fileName.getData());

        return true;
    }
}
