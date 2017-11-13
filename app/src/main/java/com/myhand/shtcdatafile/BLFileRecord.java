package com.myhand.shtcdatafile;

/**
 * Created by vincent on 2017/11/13.
 */

public class BLFileRecord extends FileRecord{
    public BLFileRecord(){
        /*
        CardNum	N10	卡号
Level	N2	等级
Reserved	ANS7	保留域	全F
RtnSign	S1	回车符	’\n’

         */
        setFieldsLength(new byte[]{10,2,7,1});
    }

    public String getCardNum(){
        return getDataField(0);
    }
    public int getLevel(){
        return Integer.parseInt(getDataField(1));
    }
}
