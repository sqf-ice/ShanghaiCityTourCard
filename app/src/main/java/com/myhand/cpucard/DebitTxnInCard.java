package com.myhand.cpucard;

import com.myhand.shtcdatafile.DataBlock;

/**
 * Created by wenha_000 on 2017-09-15.
 * 卡内交易明细记录
 */

public class DebitTxnInCard extends DataBlock{
    public DebitTxnInCard() {
        setFieldsLength(new byte[]{8,4,8,2,8,2});
        setData(String.format("%032s","0"));
    }

    public void setDebit(String posID,int amount,int balanceAfter,String txnTime)
    {
        setFieldData(0,posID);
        setFieldData(1,String.format("%04x",amount));
        setFieldData(2,String.format("%08x",balanceAfter));
        setFieldData(4,txnTime);
    }

}
