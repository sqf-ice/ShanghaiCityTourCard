package com.myhand.shtcdatafile;

import android.util.Log;

import com.myhand.shanghaicitytourcard.POSApplication;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class DataFile {
    private static final String tag=DataFile.class.getSimpleName();
    private FileName fileName;
    private FileDescription description;
    private List<FileRecord> recordList;

    public DataFile() {
        recordList=new ArrayList<FileRecord>();
    }

    public DataFile(FileName fileName,FileDescription description, List<FileRecord> recordList) {
        this.fileName=fileName;
        this.description = description;
        this.recordList = recordList;
    }

    public FileName getFileName() {
        return fileName;
    }

    public void setFileName(FileName fileName) {
        this.fileName = fileName;
    }

    public FileDescription getDescription() {
        return description;
    }

    public void setDescription(FileDescription description) {
        this.description = description;
    }

    public List<FileRecord> getRecordList() {
        return recordList;
    }

    public void setRecordList(List<FileRecord> recordList) {
        this.recordList = recordList;
    }

    //保存到文件
    public boolean writeToFile(String path)
    {
        try {
            FileOutputStream writeFile=new FileOutputStream(path+"/"+fileName.getData());
            writeFile.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return false;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    public static boolean parseBLFile(String path,String filename){
        File file=new File(path,filename);
        if(!file.exists()){
            Log.d(tag,String.format("BL File %s/%s  not exist.",path,filename));
            return false;
        }

        //读取文件头
        BLFileDescription blFileDescription=new BLFileDescription();
        byte[] buffer=new byte[blFileDescription.getDataFieldLength()];
        try {
            FileInputStream inputStream = new FileInputStream(file);
            if(inputStream.read(buffer)!=buffer.length){
                inputStream.close();
                Log.d(tag,String.format("BL file size error"));
                return false;
            }
            blFileDescription.setData(new String(buffer));
            if(!blFileDescription.isValid()){
                inputStream.close();
                Log.d(tag,String.format("BL file %s not a valid BL file",filename));
                return false;
            }
            int count=blFileDescription.getCount();
            int recordLength=blFileDescription.getRecordLength();

            buffer=new byte[recordLength];
            BLFileRecord blFileRecord=new BLFileRecord();
            for(int i=0;i<count;i++){
                if(inputStream.read(buffer)!=buffer.length){
                    inputStream.close();
                    Log.d(tag,String.format("Read BL record failure"));
                    return  false;
                }
                blFileRecord.setData(new String(buffer));
                if(!POSApplication.instance.getAppDatabase().insertBlkCard(blFileRecord.getCardNum(),blFileRecord.getLevel())){
                    Log.d(tag,String.format("Insert BL card %s level %d failure.",blFileRecord.getCardNum(),blFileRecord.getLevel()));
                }
            }

            inputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }
}
