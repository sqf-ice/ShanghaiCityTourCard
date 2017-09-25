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
        return data.substring(getFieldPos(index),fieldsLength[index]);
    }

    public void setDataField(int index,String fieldData)
    {
        if(index>0) {
            this.data = this.data.substring(getFieldPos(index));
        }
        this.data+=fieldData;
        if(index<fieldsLength.length-1) {
            this.data += this.data.substring(getFieldPos(index + 1), getDataFieldLength());
        }
    }
}
