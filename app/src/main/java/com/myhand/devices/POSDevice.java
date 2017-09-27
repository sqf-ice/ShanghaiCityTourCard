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
    //公司代码
    private String corpCode="00320000001";
    //公司名称
    private String corpName="SHANDE";
    //站点代码
    private String stationID="123456";
    //POS编码，来自于PSAM卡
    private String posID="12345678";
    //操作用户
    private User user;
    //数据保存目录
    private String workDataPath;
    //PSAM卡操作设备
    private PSAMDevice psamDevice;
    //用户卡（非接）操作设备
    private RFCPUDevice rfcpuDevice;

    private String errorMessage;
    //设备流水号
    private  int posSequence;

    public String getWorkDataPath() {
        return workDataPath;
    }

    public void setWorkDataPath(String workDataPath) {
        this.workDataPath = workDataPath;
    }

    public String getCorpCode() {
        return corpCode;
    }

    public void setCorpCode(String corpCode) {
        this.corpCode = corpCode;
    }

    public String getCorpName() {
        return corpName;
    }

    public void setCorpName(String corpName) {
        this.corpName = corpName;
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

    public int getPosSequence() {
        return posSequence;
    }

    public String getPosID() {
        return posID;
    }

    public void setPosID(String posID) {
        this.posID = posID;
    }

    public void setPosSequence(int posSequence) {
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
        posSequence=0;
        user=new User("123456","111111");
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
