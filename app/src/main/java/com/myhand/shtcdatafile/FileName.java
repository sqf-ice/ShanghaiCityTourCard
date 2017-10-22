package com.myhand.shtcdatafile;

import android.util.Log;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by wenha_000 on 2017-09-17.
 */

public class FileName extends DataBlock {
    private static final String tag=FileName.class.getSimpleName();
/*
    public FileName() {
        setFieldsLength(new byte[]{(byte)2,(byte)6,(byte)2,(byte)11,(byte)3,(byte)1});
    }
*/
    public FileName(String tradeCode,String corpID,int sequence,String type) {
        setFieldsLength(new byte[]{(byte)2,(byte)6,(byte)2,(byte)11,(byte)3,(byte)1});
        SimpleDateFormat sdf=new SimpleDateFormat("yyMMdd");

        setData(String.format("FH%s%s%s%03d%s",sdf.format(new Date()),tradeCode,corpID,sequence,type));
        Log.d(tag,String.format("文件名:%s",getData()));
    }

    public String getFileType()
    {
        return getDataField(0);
    }

    public String getFileDate()
    {
        return getDataField(1);
    }

    public String getFieldCode()
    {
        return getDataField(2);
    }

    public String getUintCode()
    {
        return  getDataField(3);
    }

    public String getSeq()
    {
        return  getDataField(4);
    }

    public String getFileModel()
    {
        return  getDataField(5);
    }
}
