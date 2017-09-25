package com.myhand.shtcdatafile;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class FHFileRecord extends FileRecord {
    private DataBlock dataBlock;

    public FHFileRecord(String unitCode,long localSeq,String stationID,String txnType,long posSeq,String cityCode,
                        String cardFaceNum,String cardType,long balBef,long amount,String txnTime,int cardTxnCounter,
                        String posID,String tac,int cardVer) {
        dataBlock=new DataBlock();

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
        dataBlock.setFieldsLength(new byte[]{11,8,2,6,16,6,6,2,12,4,12,2,8,8,8,6,6,8,8,2,0,1});
        dataBlock.setData(String.format("%- 11s%08d00%06s%016s%06s%s%012d%s%s%s%08d%08d%s%06d%08s%08s%02d\n",
                unitCode,localSeq,"0","0","1",txnType,posSeq,cityCode,cardFaceNum,cardType,balBef,amount,txnTime,cardTxnCounter,
                posID,tac,cardVer));
    }
}