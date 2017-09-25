package com.myhand.shtcdatafile;

/**
 * Created by wenha_000 on 2017-09-17.
 */

public class FileName extends DataBlock {
    public FileName() {
        setFieldsLength(new byte[]{(byte)2,(byte)6,(byte)2,(byte)11,(byte)3,(byte)1});
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
