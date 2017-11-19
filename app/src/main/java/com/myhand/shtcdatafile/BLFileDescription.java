package com.myhand.shtcdatafile;

import android.util.Log;

/**
 * Created by vincent on 2017/11/13.
 */

public class BLFileDescription extends FileDescription{
    private static final String tag=BLFileDescription.class.getSimpleName();
    /*
    Version	N2	版本号
TradeCode	N2	行业代码
FileType	N4	文件类型	7652
RecNum	N8	记录总数
RecLength	N8	记录长度
Reserved	ANS7	保留域	全0
RtnSign	S1	回车符	’\n’

     */
    public BLFileDescription(){
        setFieldsLength(new byte[]{2,2,4,8,8,7,1});
        setFieldData(2,"7652");
    }

    public boolean isValid(){
        Log.d(tag,String .format("黑名单文件头长度：%d，文件头：%s，文件类型为：%s，黑名单个数：%d，黑名单记录长度：%d",getDataFieldLength(),
                getData(),getDataField(2),getCount(),getRecordLength()));

        return getDataField(2).compareTo("7652")==0;
    }

    public int getCount(){
        return Integer.parseInt(getDataField(3));
    }

    public int getRecordLength(){
        return Integer.parseInt(getDataField(4));
    }
}
