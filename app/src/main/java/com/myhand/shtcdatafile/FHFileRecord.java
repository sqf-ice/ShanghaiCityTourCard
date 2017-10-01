package com.myhand.shtcdatafile;

import android.util.Log;

import com.myhand.cpucard.DebitRecord;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class FHFileRecord extends FileRecord {
    private static final String tag=FHFileRecord.class.getSimpleName();
    public FHFileRecord() {
        setFieldsLength(new byte[]{11,8,2,6,16,6,2,12,4,12,2,8,8,14,6,8,8,2,0});
        int length=getDataFieldLength();
        Log.d(tag,String.format("Data:%s length:%d",getData(),getData().length()));
        String data="";
        for(int i=0;i<length;i++){
            data+="0";
        }
        setData(data);
    }

    public FHFileRecord(String unitCode, long localSeq, String stationID, byte txnType, long posSeq, String cityCode,
                        String cardFaceNum, byte cardType, long balBef, long amount, String txnTime, int cardTxnCounter,
                        String posID, String tac, byte cardVer) {
        /**
         File Record Area（N）
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
         TradSpec	ANSVAR	行业特有数据	依据IsTSUsed而决定有无该域
         RtnSign	S1	回车符	’\n’
         */
        setFieldsLength(new byte[]{11,8,2,6,16,6,2,12,4,12,2,8,8,14,6,8,8,2,0});
        setData(String.format("%- 11s%08d00%06s%016s%06s%s%012d%s%s%s%08d%08d%s%06d%08s%08s%02d\n",
                unitCode,localSeq,"0","0","1",txnType,posSeq,cityCode,cardFaceNum,cardType,balBef,amount,txnTime,cardTxnCounter,
                posID,tac,cardVer));
    }

    public void fromDebitRecord(DebitRecord record){
        setFieldData(0,record.getCorpID());
        setFieldData(1,(int)record.getLocalTxnSeq());
        setFieldData(2,record.getTxnAttr());
        setFieldData(3,record.getStationID());
        setFieldData(4,record.getOprID());
        setFieldData(5,record.getBusID());
        setFieldData(6,record.getTxnType());
        setFieldData(7,record.getPosSeq());
        setFieldData(8,record.getCityCode());
        setFieldData(9,record.getCardFaceNum());
        setFieldData(10,record.getCardKind());
        setFieldData(11,(int)record.getBalanceBef());
        setFieldData(12,(int)record.getAmount());
        setFieldData(13,record.getTxnTime());
        setFieldData(14,record.getTxnCounter());
        setFieldData(15,record.getPosID());
        setFieldData(16,record.getTac());
        setFieldData(17,record.getCardVerNo());
    }

    public DebitRecord toDebitRecord(){
        DebitRecord result=new DebitRecord();
        result.setCorpID(getDataField(0));

        return result;
    }
}
