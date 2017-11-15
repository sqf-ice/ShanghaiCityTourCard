package com.myhand.transport;

import android.util.Log;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * 下载数据请求报文
 * Created by vincent on 2017/11/14.
 */

public class DGDownloadHead extends DGBase{
    private static final String tag=DGDownloadHead.class.getSimpleName();
    /*
    Flag	N1	上传/下载标志
IsEnc	N1	是否加密标志
IsCompress	N1	是否压缩标志
TradeCode	N2	行业代码
POSID	N8	POS机号
CorpId	N11	营运单位代码
CorpName	ANS16	（营运单位简称）
LocalDateTime	YYYYMMDDhhmmss	传输时间
ROMName	ANS32	ROM程序名称
Reserved	ANS8	保留域
     */
    public DGDownloadHead(){
        setFieldsLength(new byte[]{1,1,1,2,8,11,16,14,32,8,1});
        String data="1";
        for (int i=1;i<getDataFieldLength()-1;i++){
            data+="0";
        }
        data+="\n";
        setData(data);
    }

    public DGDownloadHead(byte tradeCode, String posID, String corpID, String corpName, Date localTime,String romName){
        setFieldsLength(new byte[]{1,1,1,2,8,11,16,14,32,8,1});
        Log.d(tag,String.format("SGDGDownloadHead length=%d",getDataFieldLength()));
        String data="1";
        for (int i=1;i<getDataFieldLength()-1;i++){
            data+="0";
        }
        data+="\n";
        setData(data);
        setFieldData(3,tradeCode);
        setFieldData(4,posID);
        setFieldData(5,corpID);
        setFieldData(6,corpName);

        SimpleDateFormat sdf=new SimpleDateFormat("yyyyMMddHHmmss");
        setFieldData(7,sdf.format(localTime));
/*
        setFieldData(8,romName);
        setFieldData(9,"        ");
*/
        Log.d(tag,String.format("下载数据请求头(%d)：%s",getData().length(),getData()));
    }
}
