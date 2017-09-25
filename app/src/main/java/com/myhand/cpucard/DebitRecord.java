package com.myhand.cpucard;

/**
 * Created by wenha_000 on 2017-09-13.
 */

import com.myhand.shtcdatafile.FHFileRecord;

import java.util.List;

/**
 CorpId	N11	营运单位代码
 LocalTxnSeq	N8	本地流水号
 TxnAttr	N2	交易性质	00-99
 StationId	N6	采集点编号
 PosOprId	N16	POS操作员编号	缺省值为全0
 PosCarrId	N6	POS机载体编号	公交车线路号
 TxnType	N2	交易类型	88/99/98/87/84/83/48/49/6B/6C/6E/81
 PosSeq	N12	POS机流水号
 CityCode	N4	城市代码
 CardFaceNum	N12	卡面号
 CardKind	N2	卡类型	00-99
 BalBef	N8	消费前卡余额
 TxnAmt	N8	交易金额
 TxnDate 	YYYYMMDD	交易发生日期
 TxnTime	hhmmss	交易发生时间
 TxnCounter	N6	交易计数器
 PosId	N8	POS机号
 TAC 	H8	交易认证码
 CardVerNo	N2	卡内版本号	当作地域区分标志

 */
public class DebitRecord {
    private long localTxnSeq;
    private byte txnAttr;
    private String stationID;
    private String oprID;
    private String busID;
    private String txnType;
    private String posSeq;
    private String cityCode;
    private String cardFaceNum;
    private byte cardKind;
    private long balanceBef;
    private long amount;
    private String txnTime;
    private String txnCounter;
    private String posID;
    private String tac;
    private String cardVerNo;
    private byte status;

    public DebitRecord() {
    }

    public DebitRecord(long localTxnSeq, byte txnAttr, String stationID, String oprID, String busID,String txnType, String posSeq,
                       String cityCode, String cardFaceNum, byte cardKind, long balanceBef, long amount, String txnTime,
                       String txnCounter, String posID, String tac, String cardVerNo,byte status) {
        this.localTxnSeq = localTxnSeq;
        this.txnAttr = txnAttr;
        this.stationID = stationID;
        this.oprID = oprID;
        this.busID = busID;
        this.txnType=txnType;
        this.posSeq = posSeq;
        this.cityCode = cityCode;
        this.cardFaceNum = cardFaceNum;
        this.cardKind = cardKind;
        this.balanceBef = balanceBef;
        this.amount = amount;
        this.txnTime = txnTime;
        this.txnCounter = txnCounter;
        this.posID = posID;
        this.tac = tac;
        this.cardVerNo = cardVerNo;
        this.status=status;
    }

    public byte getStatus() {
        return status;
    }

    public String getTxnType() {
        return txnType;
    }

    public void setTxnType(String txnType) {
        this.txnType = txnType;
    }

    public void setStatus(byte status) {
        this.status = status;
    }

    public long getLocalTxnSeq() {
        return localTxnSeq;
    }

    public void setLocalTxnSeq(long localTxnSeq) {
        this.localTxnSeq = localTxnSeq;
    }

    public byte getTxnAttr() {
        return txnAttr;
    }

    public void setTxnAttr(byte txnAttr) {
        this.txnAttr = txnAttr;
    }

    public String getStationID() {
        return stationID;
    }

    public void setStationID(String stationID) {
        this.stationID = stationID;
    }

    public String getOprID() {
        return oprID;
    }

    public void setOprID(String oprID) {
        this.oprID = oprID;
    }

    public String getBusID() {
        return busID;
    }

    public void setBusID(String busID) {
        this.busID = busID;
    }

    public String getPosSeq() {
        return posSeq;
    }

    public void setPosSeq(String posSeq) {
        this.posSeq = posSeq;
    }

    public String getCityCode() {
        return cityCode;
    }

    public void setCityCode(String cityCode) {
        this.cityCode = cityCode;
    }

    public String getCardFaceNum() {
        return cardFaceNum;
    }

    public void setCardFaceNum(String cardFaceNum) {
        this.cardFaceNum = cardFaceNum;
    }

    public byte getCardKind() {
        return cardKind;
    }

    public void setCardKind(byte cardKind) {
        this.cardKind = cardKind;
    }

    public long getBalanceBef() {
        return balanceBef;
    }

    public void setBalanceBef(long balanceBef) {
        this.balanceBef = balanceBef;
    }

    public long getAmount() {
        return amount;
    }

    public void setAmount(long amount) {
        this.amount = amount;
    }

    public String getTxnTime() {
        return txnTime;
    }

    public void setTxnTime(String txnTime) {
        this.txnTime = txnTime;
    }

    public String getTxnCounter() {
        return txnCounter;
    }

    public void setTxnCounter(String txnCounter) {
        this.txnCounter = txnCounter;
    }

    public String getPosID() {
        return posID;
    }

    public void setPosID(String posID) {
        this.posID = posID;
    }

    public String getTac() {
        return tac;
    }

    public void setTac(String tac) {
        this.tac = tac;
    }

    public String getCardVerNo() {
        return cardVerNo;
    }

    public void setCardVerNo(String cardVerNo) {
        this.cardVerNo = cardVerNo;
    }

    public FHFileRecord toFHFileRecord(String unitCode)
    {
        FHFileRecord result=new FHFileRecord(unitCode,localTxnSeq,stationID,String.format("%02x",txnAttr),
                Integer.parseInt(posSeq,0x10),cityCode,cardFaceNum,String.format("%02x",cardKind),
                balanceBef,amount,txnTime,Integer.parseInt(txnCounter,0x10),posID,tac,Integer.parseInt(cardVerNo,0x10));
        return result;
    }
}
