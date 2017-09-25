package com.myhand.shanghaicitytourcard;

/**
 * Created by wenha_000 on 2017-09-06.
 */

import com.centerm.smartpos.util.HexUtil;

import java.util.ArrayList;
import java.util.List;

/**
 * 实例：
 6F328409A00000000386980701A5259F0801029F0C1E869820007590FFFF02052000B8C646E2AC300CA720161021202110211814

 解析
 6F
 32（length）
 84
 09A00000000386980701
 A5
 25（length）
 9F08 0102 应用版本号
 9F0C自定义
 1E （length）
 8698 2000 7590FFFF 0205 2000 B8C646E2AC300CA7 20161021 20211021 18 14

 */
public class CPUFileDataFCI extends CPUFileData {
    public CPUFileDataFCI() {
        List<CPUFileDataField> fields=new ArrayList<CPUFileDataField>();
        fields.add(0,new CPUFileDataField((byte)0,(byte)1));
        fields.add(1,new CPUFileDataField((byte)1,(byte)1));
        fields.add(2,new CPUFileDataField((byte)2,(byte)1));
        fields.add(3,new CPUFileDataField((byte)3,(byte)10));
        fields.add(4,new CPUFileDataField((byte)13,(byte)1));
        fields.add(5,new CPUFileDataField((byte)14,(byte)1));
        fields.add(6,new CPUFileDataField((byte)15,(byte)4));
        fields.add(7,new CPUFileDataField((byte)19,(byte)2));
        fields.add(8,new CPUFileDataField((byte)21,(byte)1));
        fields.add(9,new CPUFileDataField((byte)22,(byte)30));

        setFileDataFields(fields);
    }

    public boolean checkData()
    {
        byte[] data=getFileData();
        if(data==null)
        {
            return false;
        }
        if(data[0]!=0x6f)
        {
            return false;
        }

        return true;
    }

    public byte[] getValidData()
    {
        return getFieldData(9);
    }
}
