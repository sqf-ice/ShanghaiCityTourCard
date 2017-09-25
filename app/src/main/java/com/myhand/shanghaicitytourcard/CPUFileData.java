package com.myhand.shanghaicitytourcard;

import com.centerm.smartpos.util.HexUtil;

import java.util.List;

/**
 * Created by wenha_000 on 2017-09-06.
 */

public class CPUFileData {
    private byte[] fileData;
    private List<CPUFileDataField> fileDataFields;

    public CPUFileData() {
    }

    public byte[] getFileData() {
        return fileData;
    }

    public void setFileData(byte[] fileData) {
        this.fileData = fileData;
    }

    public List<CPUFileDataField> getFileDataFields() {
        return fileDataFields;
    }

    public void setFileDataFields(List<CPUFileDataField> fileDataFields) {
        this.fileDataFields = fileDataFields;
    }

    public int getFieldCount()
    {
        if(fileDataFields==null)
        {
            return 0;
        }

        return fileDataFields.size();
    }

    public byte[] getFieldData(int index)
    {
        if(fileData==null
            ||fileDataFields==null
            ||(index<0||index>=fileDataFields.size()))
        {
            return null;
        }

        CPUFileDataField field=fileDataFields.get(index);
        byte[] result=new byte[field.getLength()];
        System.arraycopy(fileData,field.getPosition(),result,0,field.getLength());
        return result;
    }

    //按域显示数据内容
    public String showData()
    {
        if(getFileData()==null)
        {
            return "无数据";
        }
        String result="Data perase:\n";
        for(int i=0;i<getFieldCount();i++)
        {
            result+=String.format("%d:%s\n",i, HexUtil.bytesToHexString(getFieldData(0)));
        }

        return result;
    }

    //获取数据定义长度
    public int getDataDefineLength()
    {
        if(fileDataFields.size()<=0)
        {
            return 0;
        }

        int result=0;
        for(int i=0;i<getFieldCount();i++)
        {
            result+=fileDataFields.get(i).getLength();
        }
        return result;
    }

    //域索引纠错
    public void checkFieldDefine()
    {
        if(fileDataFields.size()<=0)
        {
            return;
        }

        byte pos=0;
        for(int i=0;i<getFieldCount();i++)
        {
            CPUFileDataField fieldDefine=fileDataFields.get(i);
            fieldDefine.setPosition(pos);
            pos+=fieldDefine.getLength();
        }
    }

}
