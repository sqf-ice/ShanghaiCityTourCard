package com.myhand.transport;

import java.text.DateFormat;
import java.text.FieldPosition;
import java.text.ParsePosition;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by wenha_000 on 2017-09-18.
 */

public class DGCommHead extends DGBase{
    public static int sendSEQ=0;
    public static int NewSendSeq(){
        return ++sendSEQ;
    }

    public static final int DGLENGTH=124;
    public static final byte FLAG_UPLOAD='0';
    public static final byte FLAG_DOWNLOAD='1';

    public static final byte FLAG_ENC='1';
    public static final byte FLAG_PLAIT='0';

    private byte transFlag;
    private byte encFlag;
    private byte isCompress;
    private byte tradeCode;
    private String posID;
    private String corpCode;
    private int sendSeq;
    private int fileSize;
    private String corpName;
    private Date transTime;
    private String reserved;
    private byte[] fileAbstract;

    public DGCommHead() {
        fileAbstract=new byte[48];
        for(int i=0;i<fileAbstract.length;i++)
        {
            fileAbstract[i]='0';
        }
        transFlag=0;
        encFlag=0;
        isCompress=0;
        tradeCode=32;
        posID="10000001";
        corpCode="00320000001";
        sendSeq=NewSendSeq();
        reserved="00000000";
        corpName="SHANDE";
        transTime=new Date();
    }

    public DGCommHead(byte transFlag, byte encFlag, byte isCompress, byte tradeCode, String posID, String corpCode, int sendSeq,
                      int fileSize, String corpName, Date transTime, String reserved, byte[] fileAbstract) {
        this.transFlag = transFlag;
        this.encFlag = encFlag;
        this.isCompress = isCompress;
        this.tradeCode = tradeCode;
        this.posID = posID;
        this.corpCode = corpCode;
        this.sendSeq = sendSeq;
        this.fileSize = fileSize;
        this.corpName = corpName;
        this.transTime = transTime;
        this.reserved = reserved;
        this.fileAbstract = fileAbstract;
    }

    public static int getDGLENGTH() {
        return DGLENGTH;
    }

    public static byte getFlagUpload() {
        return FLAG_UPLOAD;
    }

    public static byte getFlagDownload() {
        return FLAG_DOWNLOAD;
    }

    public static byte getFlagEnc() {
        return FLAG_ENC;
    }

    public static byte getFlagPlait() {
        return FLAG_PLAIT;
    }

    public byte getTransFlag() {
        return transFlag;
    }

    public void setTransFlag(byte transFlag) {
        this.transFlag = transFlag;
    }

    public byte getEncFlag() {
        return encFlag;
    }

    public void setEncFlag(byte encFlag) {
        this.encFlag = encFlag;
    }

    public byte getIsCompress() {
        return isCompress;
    }

    public void setIsCompress(byte isCompress) {
        this.isCompress = isCompress;
    }

    public byte getTradeCode() {
        return tradeCode;
    }

    public void setTradeCode(byte tradeCode) {
        this.tradeCode = tradeCode;
    }

    public String getPosID() {
        return posID;
    }

    public void setPosID(String posID) {
        this.posID = posID;
    }

    public String getCorpCode() {
        return corpCode;
    }

    public void setCorpCode(String corpCode) {
        this.corpCode = corpCode;
    }

    public int getSendSeq() {
        return sendSeq;
    }

    public void setSendSeq(int sendSeq) {
        this.sendSeq = sendSeq;
    }

    public int getFileSize() {
        return fileSize;
    }

    public void setFileSize(int fileSize) {
        this.fileSize = fileSize;
    }

    public String getCorpName() {
        return corpName;
    }

    public void setCorpName(String corpName) {
        this.corpName = corpName;
    }

    public Date getTransTime() {
        return transTime;
    }

    public void setTransTime(Date transTime) {
        this.transTime = transTime;
    }

    public String getReserved() {
        return reserved;
    }

    public void setReserved(String reserved) {
        this.reserved = reserved;
    }

    public byte[] getFileAbstract() {
        return fileAbstract;
    }

    public void setFileAbstract(byte[] fileAbstract) {
        this.fileAbstract = fileAbstract;
    }

    public String toString()
    {
        String result=String.format("%d%d%d%02d",transFlag,encFlag,isCompress,tradeCode);
        result+=String.format("%8s",posID);
        result+=String.format("%11s",corpCode);
        result+=String.format("%06d",sendSeq);
        result+=String.format("%08d",fileSize);
        result+=String.format("%16s",corpName);

        DateFormat format=new SimpleDateFormat("yyyyMMddHHmmss");
        result+=format.format(transTime);
        result+=reserved;
        result+=String.format("%48s","0");
        return result;
    }
}
