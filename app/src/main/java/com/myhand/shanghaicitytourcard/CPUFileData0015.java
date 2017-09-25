package com.myhand.shanghaicitytourcard;

import com.centerm.smartpos.util.HexUtil;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by wenha_000 on 2017-09-06.
 */

public class CPUFileData0015 extends CPUFileData {
    public CPUFileData0015() {
        List<CPUFileDataField> fields=new ArrayList<CPUFileDataField>();
        fields.add(0,new CPUFileDataField((byte)0,(byte)8));
        fields.add(1,new CPUFileDataField((byte)8,(byte)1));
        fields.add(2,new CPUFileDataField((byte)9,(byte)1));
        fields.add(3,new CPUFileDataField((byte)10,(byte)2));
        fields.add(4,new CPUFileDataField((byte)12,(byte)8));
        fields.add(5,new CPUFileDataField((byte)20,(byte)4));
        fields.add(6,new CPUFileDataField((byte)24,(byte)4));
        fields.add(7,new CPUFileDataField((byte)28,(byte)1));
        fields.add(8,new CPUFileDataField((byte)29,(byte)1));
        setFileDataFields(fields);
    }

    public String getCardNO()
    {
        byte[] ret=getFieldData(4);
        if(ret==null)
        {
            return "";
        }
        return HexUtil.bytesToHexString(ret);
    }

    public String getStartDate()
    {
        return HexUtil.bytesToHexString(getFieldData(5));
    }
    public String getValidDate()
    {
        return HexUtil.bytesToHexString(getFieldData(6));
    }
}
