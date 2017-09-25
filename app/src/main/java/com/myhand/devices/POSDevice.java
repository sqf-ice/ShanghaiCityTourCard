package com.myhand.devices;

import com.myhand.common.Converter;
import com.myhand.cpucard.CPUUserCard;
import com.myhand.cpucard.PSAMCard;
import com.myhand.cpucard.SHTCPsamCard;
import com.myhand.shanghaicitytourcard.CityTourCard;

/**
 * Created by wenha_000 on 2017-09-08.
 */

public abstract class POSDevice {
    //
    private String stationID="123456";
    private String posID;
    private User user;
    private String workDataPath;
    private PSAMDevice psamDevice;
    private RFCPUDevice rfcpuDevice;

    private String errorMessage;

    private byte[] posSequence;

    public String getWorkDataPath() {
        return workDataPath;
    }

    public void setWorkDataPath(String workDataPath) {
        this.workDataPath = workDataPath;
    }

    public String getErrorMessage() {
        return String.format("设备：%1$s PSAM:%2$s Card:%3$s",errorMessage,psamDevice.getErrorMessage(),rfcpuDevice.getErrorMessage());    }

    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    public String getStationID() {
        return stationID;
    }

    public void setStationID(String stationID) {
        this.stationID = stationID;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public byte[] getPosSequence() {
        return posSequence;
    }

    public void setPosSequence(byte[] posSequence) {
        this.posSequence = posSequence;
    }

    //打开PSAM卡
    public abstract boolean openPSAMCard();
    //读取CPU用户卡
    public abstract CityTourCard readCard();
    //用户卡简单消费
    public abstract boolean debit(CPUUserCard userCard,int amount);
    //用户卡复合消费
    public abstract boolean complexDebit(CPUUserCard userCard,int amount);
    public POSDevice() {
        posSequence=new byte[4];
        setPosSequence(0);
        user=new User("123456","111111");

        //初始化数据目录
        
    }

    public void setPosSequence(int seq)
    {
        posSequence= Converter.IntToBytes(seq);
    }

    public byte[] incPosSequence()
    {
        byte[] result=posSequence;
        posSequence=Converter.IntToBytes((int)Converter.BytesToLong(result)+1);
        return result;
    }

    public PSAMDevice getPsamDevice() {
        return psamDevice;
    }

    public void setPsamDevice(PSAMDevice psamDevice) {
        this.psamDevice = psamDevice;
    }

    public RFCPUDevice getRfcpuDevice() {
        return rfcpuDevice;
    }

    public void setRfcpuDevice(RFCPUDevice rfcpuDevice) {
        this.rfcpuDevice = rfcpuDevice;
    }

}
