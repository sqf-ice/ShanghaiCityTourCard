package com.myhand.shtcdatafile;

import com.centerm.smartpos.util.HexUtil;

/**
 * Created by vincent on 2017/10/18.
 */

public class DtlRecCPU extends DtlRec {
    private void init(){
        setFieldsLength(new byte[]{4,1,20,2,1,12,8});
        setFieldData(0,1040);
        setFieldData(1,1);
        //setFieldData(3,0x06);
        setFieldData(4,1);
    }

    public DtlRecCPU(){
        init();
    }

    public DtlRecCPU(byte[] cardInnerCode,byte txnAttr,byte[] posID,int posSeq){
        init();
        setFieldData(2, String.format("0000%s",HexUtil.bytesToHexString(cardInnerCode)));
        setFieldData(3,txnAttr);
        setFieldData(5,HexUtil.bytesToHexString(posID));
        String posSeqHexstr=String.format("%08X",posSeq);
        setFieldData(6,posSeqHexstr);
    }
}
