package com.myhand.shtcdatafile;

import android.util.Log;

/**
 * Created by wenha_000 on 2017-09-13.
 */

public class DataBlock {
    private static final String tag=DataBlock.class.getSimpleName();

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
        for(int i=0;i<data.length();i++){
            this.data+="0";
        }
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
        Log.d(tag,fmtStr);
        String tmpStr=String.format(fmtStr,fieldData);

        String head="";
        if(index>0) {
            head = this.data.substring(0,getFieldPos(index));
        }
        String tail="";
        if(index<fieldsLength.length-1) {
            tail= this.data.substring(getFieldPos(index + 1));
        }
        this.data=head+tmpStr+tail;
        Log.d(tag,String.format("Index:%d Data:%s Length:%d",index,this.data,this.data.length()));
    }

    public void setFieldData(int index,int intData){
        String fmtStr=String.format("%%0%dd",fieldsLength[index]);
        Log.d(tag,fmtStr);
        setFieldData(index,String.format(fmtStr,intData));
    }
}
