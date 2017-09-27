package com.myhand.shtcdatafile;

/**
 * Created by wenha_000 on 2017-09-13.
 */

public class DataBlock {
    private String data;
    private byte[] fieldsLength;



    public DataBlock() {
    }



    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    public byte[] getFieldsLength() {
        return fieldsLength;
    }

    public void setFieldsLength(byte[] fieldsLength) {
        this.fieldsLength = fieldsLength;
        this.data=new String(new byte[getDataFieldLength()]);
    }

    public int getFieldCount()
    {
        return fieldsLength.length;
    }

    public int getFieldPos(int index)
    {
        if(index<0||index>=fieldsLength.length)
        {
            return 0;
        }

        int result=0;
        for(int i=0;i<index;i++)
        {
            result+=fieldsLength[i];
        }
        return result;
    }

    public int getDataFieldLength(){
        int result=0;
        for(int i=0;i<fieldsLength.length;i++)
        {
            result+=fieldsLength[i];
        }
        return result;
    }

    public String getDataField(int index)
    {
        return data.substring(getFieldPos(index),fieldsLength[index]).trim();
    }

    public void setFieldData(int index,String fieldData)
    {
        String fmtStr=String.format("%%-%ds",fieldsLength[index]);
        String tmpStr=String.format(fmtStr,fieldData);

        if(index>0) {
            this.data = this.data.substring(getFieldPos(index));
        }
        this.data+=tmpStr;

        if(index<fieldsLength.length-1) {
            this.data += this.data.substring(getFieldPos(index + 1), getDataFieldLength());
        }
    }

    public void setFieldData(int index,int intData){
        String fmtStr=String.format("%%0%dd",fieldsLength[index]);
        setFieldData(index,String.format(fmtStr,intData));
    }
}
