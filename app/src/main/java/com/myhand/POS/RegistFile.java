package com.myhand.POS;

import java.util.Date;

/**
 * Created by wenha_000 on 2017-10-13.
 */

public class RegistFile {
    private String fileName;
    private String typeStr;
    private Date createTime;
    private Date uploadTime;
    private int status;

    public RegistFile() {
    }

    public RegistFile(String fileName, String typeStr, Date createTime, Date uploadTime, int status) {
        this.fileName = fileName;
        this.typeStr = typeStr;
        this.createTime = createTime;
        this.uploadTime = uploadTime;
        this.status = status;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public String getTypeStr() {
        return typeStr;
    }

    public void setTypeStr(String typeStr) {
        this.typeStr = typeStr;
    }

    public Date getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }

    public Date getUploadTime() {
        return uploadTime;
    }

    public void setUploadTime(Date uploadTime) {
        this.uploadTime = uploadTime;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }
}
