package com.myhand.manage;

import java.io.Serializable;
import java.util.Date;

/**
 * Created by vincent on 2017/11/12.
 */

public class SettleSum implements Serializable{
    private String username;
    private String corpID;
    private Date settleTime;

    private int patchNo;
    private int count;
    private int sum;
    private String posID;
    private Date startTime;
    private Date endTime;

    public SettleSum() {
    }

    public SettleSum(int count, int sum) {
        this.count = count;
        this.sum = sum;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getCorpID() {
        return corpID;
    }

    public void setCorpID(String corpID) {
        this.corpID = corpID;
    }

    public Date getSettleTime() {
        return settleTime;
    }

    public void setSettleTime(Date settleTime) {
        this.settleTime = settleTime;
    }

    public int getPatchNo() {
        return patchNo;
    }

    public void setPatchNo(int patchNo) {
        this.patchNo = patchNo;
    }

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public int getSum() {
        return sum;
    }

    public void setSum(int sum) {
        this.sum = sum;
    }

    public String getPosID() {
        return posID;
    }

    public void setPosID(String posID) {
        this.posID = posID;
    }

    public Date getStartTime() {
        return startTime;
    }

    public void setStartTime(Date startTime) {
        this.startTime = startTime;
    }

    public Date getEndTime() {
        return endTime;
    }

    public void setEndTime(Date endTime) {
        this.endTime = endTime;
    }
}
