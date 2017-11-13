package com.myhand.shtcdatafile;

import android.util.Log;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.cpucard.DebitRecord;
import com.myhand.cpucard.SHTCCPUUserCard;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class FHFileRecord extends FileRecord {
    private static final String tag=FHFileRecord.class.getSimpleName();

    private List<DtlRec> tsDataList;

    public List<DtlRec> getTsDataList() {
        return tsDataList;
    }

    public void setTsDataList(List<DtlRec> tsDataList) {
        this.tsDataList = tsDataList;
    }

    public FHFileRecord() {
        setFieldsLength(new byte[]{11,8,2,6,16,6,2,12,4,12,2,8,8,14,6,8,8,2,0,1});

        Log.d(tag,String.format("Data:%s length:%d",getData(),getData().length()));
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
        setFieldsLength(new byte[]{11,8,2,6,16,6,2,12,4,12,2,8,8,14,6,8,8,2,0,1});
        setData(String.format("%- 11s%08d00%06s%016s%06s%s%012d%s%s%s%08d%08d%s%06d%08s%08s%02d\n",
                unitCode,localSeq,"0","0","1",txnType,posSeq,cityCode,cardFaceNum,cardType,balBef,amount,txnTime,cardTxnCounter,
                posID,tac,cardVer));
        Log.d(tag,String.format("Init data:(%d)'%s'",getData().length(),getData()));
    }

    public void fromDebitRecord(DebitRecord record){
        //设置其他域
        setFieldData(0,record.getCorpID());
        setFieldData(1,(int)record.getLocalTxnSeq());
        setFieldData(2,record.getTxnAttr());
        setFieldData(3,record.getStationID());
        setFieldData(4,"0000000000000000");//record.getOprID());
        setFieldData(5,"000000");//record.getBusID());
        setFieldData(6,record.getTxnType());
        setFieldData(7,record.getPosSeq());
        setFieldData(8,record.getCityCode());
        String cardfaceNo="0"+ SHTCCPUUserCard.CardFaceNum(HexUtil.hexStringToByte(record.getCardFaceNum().substring(8)));
//        Log.d(tag,"Card face num:"+cardfaceNo);
        setFieldData(9,cardfaceNo);
//        Log.d(tag,getData());
        setFieldData(10,record.getCardKind());
        setFieldData(11,(int)record.getBalanceBef());
        setFieldData(12,(int)record.getAmount());
        setFieldData(13,record.getTxnTime());
        setFieldData(14,record.getTxnCounter());
        setFieldData(15,record.getPosID().substring(4));//取SAM卡号后4个字节
        setFieldData(16,record.getTac());
        setFieldData(17,record.getCardVerNo());
        Log.d(tag,String.format("Data without TS data:(%d)%s",getData().length(),getData()));
        //根据交易判断是否为CPU卡，如果是，添加行业信息，
        // 此处现简单增加

        DtlRecCPU dtlRecCPU=new DtlRecCPU(HexUtil.hexStringToByte(record.getCardFaceNum()),
                record.getTxnAttr(),HexUtil.hexStringToByte(record.getPosID()),
                record.getPosSeq());
        addTsData(dtlRecCPU);
    }

    public DebitRecord toDebitRecord(){
        DebitRecord result=new DebitRecord();
        result.setCorpID(getDataField(0));

        return result;
    }

    //

    /**
     * 更新行业特有数据
     * 变更行业数据后，需要调用此方法更新行业数据
     */
    public void updateTSData(){
        //确定之前是否有行业数据并去除之前的行业特有数据
        int fieldTSDataLength=getFieldsLength()[18];
        if(fieldTSDataLength!=0){
            int tsDataPos=getFieldPos(18);
            String newData=getData().substring(0,tsDataPos)
                    +getData().substring(tsDataPos+fieldTSDataLength);
            setData(newData);
        }
        //判断是否有行业数据
        if(tsDataList==null||tsDataList.size()==0){
            //没有行业数据
            return;
        }
        //存在行业数据
        String tsData=String.format("%04d",tsDataList.size());
        for(int i=0;i<tsDataList.size();i++){
            tsData+=tsDataList.get(i).getData();
        }
        //存在行业数据，首先更新行业数据的长度
        Log.d(tag,String.format("Data withour ts data:(%d)%s",getData().length(),getData()));
        setFieldLength(18,(byte)tsData.length());
        //Log.d(tag,String.format("Init data is (%d)%s",getData().length(),getData()));
        setFieldData(18,tsData);
        Log.d(tag,String.format("行业数据(%d):%s,加入行业数据后的数据记录数据(%d):%s",
                tsData.length(),tsData,getData().length(),getData()));
    }

    /**
     * 增加行业特有数据
     * @param dtlRec
     */
    public void addTsData(DtlRec dtlRec){
        if(tsDataList==null){
            tsDataList=new ArrayList<DtlRec>();
        }
        tsDataList.add(dtlRec);
        updateTSData();
    }
    //
}
