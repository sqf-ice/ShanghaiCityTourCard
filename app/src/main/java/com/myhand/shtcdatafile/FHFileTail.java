package com.myhand.shtcdatafile;

/**
 * Created by wenha_000 on 2017-10-13.
 */

public class FHFileTail extends DataBlock{
    public FHFileTail(int recordCount, int sum) {
        setFieldsLength(new byte[]{9,8,12});
        setFieldData(0,"***EOF***");
        setFieldData(1,recordCount);
        setFieldData(2,sum);
    }
}
