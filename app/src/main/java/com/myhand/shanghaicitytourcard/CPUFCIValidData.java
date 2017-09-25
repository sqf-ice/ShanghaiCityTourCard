package com.myhand.shanghaicitytourcard;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by wenha_000 on 2017-09-06.
 * 8698 2000 7590FFFF 0205 2000 B8C646E2AC300CA7 20161021 20211021 18 14
 */

public class CPUFCIValidData extends CPUFileData{
    public CPUFCIValidData() {
        List<CPUFileDataField> fields=new ArrayList<CPUFileDataField>();
        fields.add(0,new CPUFileDataField((byte)0,(byte)2));
        fields.add(1,new CPUFileDataField((byte)2,(byte)2));
        fields.add(2,new CPUFileDataField((byte)4,(byte)4));
        fields.add(3,new CPUFileDataField((byte)8,(byte)2));
        fields.add(4,new CPUFileDataField((byte)10,(byte)2));
        fields.add(5,new CPUFileDataField((byte)12,(byte)8));
        fields.add(6,new CPUFileDataField((byte)20,(byte)4));
        fields.add(7,new CPUFileDataField((byte)24,(byte)4));
        fields.add(8,new CPUFileDataField((byte)28,(byte)1));
        fields.add(9,new CPUFileDataField((byte)29,(byte)1));

        setFileDataFields(fields);
    }

    public byte[] getCityCode()
    {
        return getFieldData(4);
    }

    public byte[] getCardNo()
    {
        return getFieldData(5);
    }

    public byte[] getStartDate()
    {
        return getFieldData(6);
    }

    public byte[] getValidDate()
    {
        return getFieldData(7);
    }

    public byte getCardType()
    {
        return getFieldData(8)[0];
    }

    public byte getCardPrice()
    {
        return getFieldData(9)[0];
    }
}
