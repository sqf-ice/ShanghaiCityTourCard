package com.myhand.devices;

import com.myhand.common.Converter;
import com.myhand.cpucard.CPUUserCard;
import com.myhand.cpucard.DebitRecord;
import com.myhand.cpucard.PSAMCard;
import com.myhand.cpucard.SHTCPsamCard;
import com.myhand.shanghaicitytourcard.CityTourCard;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by wenha_000 on 2017-09-08.
 */
public abstract class POSDevice {

    public static String IPTEST="168.10.5.96";
    public static int portUp=12581;
    public static int portDown=12582;

    //错误代码
    public static final String EC_OK="0000";
    public static final String EC_NORESPONSE="1001";

    public static final String EC_PSAMERROR="3001";
    public static final String EC_RFERROR="4001";

    public static final String EC_VALIDATEDATE="9002";
    public static final String EC_BALANCE="9003";

    public static final String EC_TXNFAILURE="2001";
    public static final String EC_TXNBREK="2002";

    //行业代码
    private byte tradeCode=32;
    //公司代码
    private String corpCode="00320000001";
    //公司名称
    private String corpName="SHANDE";
    private String corpChinesename="上海秩城科技有限公司";
    //批次号
    private int patchNo;
    //站点代码
    private String stationID="123456";
    //POS编码，来自于PSAM卡
    private String posID="14060101";
    //数据保存目录
    private String workDataPath;

    //PSAM卡操作设备，共有3各PSAM卡设备
    private PSAMDevice[] psamDevices;
    private PSAMDevice currPsamDevice;

    //用户卡（非接）操作设备
    private RFCPUDevice rfcpuDevice;
    private Sounder sounder;
    private Printer printer;

    private String errorCode;
    private String errorMessage;

    //交易类型，取决于POS机设备
    private byte txnType=88;
    //设备流水号
    private  int posSequence;

    public String getWorkDataPath() {
        return workDataPath;
    }

    public void setWorkDataPath(String workDataPath) {
        this.workDataPath = workDataPath;
    }

    public byte getTradeCode() {
        return tradeCode;
    }

    public void setTradeCode(byte tradeCode) {
        this.tradeCode = tradeCode;
    }

    public String getCorpCode() {
        return corpCode;
    }

    public String getCorpChinesename() {
        return corpChinesename;
    }

    public void setCorpChinesename(String corpChinesename) {
        this.corpChinesename = corpChinesename;
    }

    public int getPatchNo() {
        return patchNo;
    }

    public void setPatchNo(int patchNo) {
        this.patchNo = patchNo;
    }

    public byte getTxnType() {
        return txnType;
    }

    public void setTxnType(byte txnType) {
        this.txnType = txnType;
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

    public String getErrorCode() {
        return errorCode;
    }

    public void setErrorCode(String errorCode) {
        this.errorCode = errorCode;
    }

    public String getErrorMessage() {
        return String.format("设备：(%s)%s PSAM:(%s)%s Card:(%s)%s",errorCode,errorMessage,
                currPsamDevice.getErrorCode(),currPsamDevice.getErrorMessage(),
                rfcpuDevice.getErrorCode(),rfcpuDevice.getErrorMessage());    }

    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    public void setError(String errorCode,String errorMessage){
        this.errorCode=errorCode;
        this.errorMessage=errorMessage;
    }

    public String getStationID() {
        return stationID;
    }

    public void setStationID(String stationID) {
        this.stationID = stationID;
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

    public PSAMDevice[] getPsamDevices() {
        return psamDevices;
    }

    public void setPsamDevices(PSAMDevice[] psamDevices) {
        this.psamDevices = psamDevices;
    }

    public PSAMDevice getCurrPsamDevice() {
        return currPsamDevice;
    }

    public void setCurrPsamDevice(PSAMDevice currPsamDevice) {
        this.currPsamDevice = currPsamDevice;
    }

    public PSAMDevice getPsamDevice(){
        return currPsamDevice;
    }
    //打开PSAM卡
    public abstract boolean openPSAMCard();
    //读取CPU用户卡
    public abstract CityTourCard readCard();
    //用户卡简单消费
    public abstract DebitRecord debit(CPUUserCard userCard, int amount);
    //用户卡复合消费
    public abstract DebitRecord complexDebit(CPUUserCard userCard,int amount);
    public POSDevice() {
        posSequence=0;
    }

    public POSDevice(PSAMDevice[] psamDevices) {
        this.psamDevices=psamDevices;
        posSequence=0;
    }

    public RFCPUDevice getRfcpuDevice() {
        return rfcpuDevice;
    }

    public void setRfcpuDevice(RFCPUDevice rfcpuDevice) {
        this.rfcpuDevice = rfcpuDevice;
    }

    /**
     * 文件标识（2位）+日期（6位）+ 行业代码（2）+ 营运单位代码（11）+ 序列号（3位）+来源标志（1）
     * @param type
     * @return
     */
    public String makeFHFileName(int sequence,char type){
        SimpleDateFormat simpleDateFormat=new SimpleDateFormat("yyMMdd");
        String result=String.format("FH%s%02d%s%03d%c",simpleDateFormat.format(new Date()),
                tradeCode,posID,sequence,type);

        return result;
    }
    /**
     * 形成消费记录文件
     * @return
     */
    public boolean makeFHFile(){
        return false;
    }

    public Sounder getSounder() {
        return sounder;
    }

    public void setSounder(Sounder sounder) {
        this.sounder = sounder;
    }

    public Printer getPrinter() {
        return printer;
    }

    public void setPrinter(Printer printer) {
        this.printer = printer;
    }
}
