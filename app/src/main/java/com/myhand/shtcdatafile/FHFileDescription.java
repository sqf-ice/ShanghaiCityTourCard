package com.myhand.shtcdatafile;

import android.util.Log;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class FHFileDescription extends FileDescription {
    private static final String tag=FHFileDescription.class.getSimpleName();
    //文件描述数据
    private DataBlock dataBlock;
    //数据域长度
    /**
     Version	N2	版本号	16
     TradeCode	N2	行业代码
     FileType	N4	文件类型	8451/8453
     RecNum	N8	记录总数
     RecLength	N8	记录长度
     Reserved	ANS24	保留域	全F
     IsTSUsed	N1	行业特有数据启用标志	0：不用
     1：启用
     SettFlag	AN1	清算标记
     RtnSign	S1	回车符	’\n’
     */
    public FHFileDescription(int recordCount,int recordLength,boolean hasTsUsed) {
        setFieldsLength(new byte[]{(byte)2,(byte)2,(byte)4,(byte)8,(byte)8,(byte)24,(byte)1,(byte)1,(byte)1});
        setData(String.format("16008451%08d%08dFFFFFFFFFFFFFFFFFFFFFFFF%d0\n",
            recordCount,recordLength,hasTsUsed?1:0));
        Log.d(tag,String.format("FH文件头：%s",getData()));
    }

    public DataBlock getDataBlock() {
        return dataBlock;
    }

    public void setDataBlock(DataBlock dataBlock) {
        this.dataBlock = dataBlock;
    }

    public int getRecNum()
    {
        return Integer.parseInt(dataBlock.getDataField(3));
    }
}
