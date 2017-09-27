package com.myhand.shtcdatafile;

import java.util.Date;

/**
 * Created by wenha_000 on 2017-09-27.
 */

public class FHFileUploadInfo {
    private String fileName;
    private Date createTime;
    private int uploadTimes;
    private Date lastUploadTime;

    public FHFileUploadInfo() {
    }

    public FHFileUploadInfo(String fileName, Date createTime, int uploadTimes, Date lastUploadTime) {
        this.fileName = fileName;
        this.createTime = createTime;
        this.uploadTimes = uploadTimes;
        this.lastUploadTime = lastUploadTime;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public Date getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }

    public int getUploadTimes() {
        return uploadTimes;
    }

    public void setUploadTimes(int uploadTimes) {
        this.uploadTimes = uploadTimes;
    }

    public Date getLastUploadTime() {
        return lastUploadTime;
    }

    public void setLastUploadTime(Date lastUploadTime) {
        this.lastUploadTime = lastUploadTime;
    }
}
