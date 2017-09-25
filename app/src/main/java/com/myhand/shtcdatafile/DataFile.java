package com.myhand.shtcdatafile;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class DataFile {
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
}
