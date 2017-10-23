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
        data="";
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
        //初始化原始数据为全0
        data="";
        for(int i=0;i<getDataFieldLength()-1;i++){
            data+='0';
        }
        data+="\n";
        Log.d(tag,String.format("Orignal data:(%d)%s",data.length(),data));
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
        if(fieldsLength[index]==0){
            return "";
        }
        return data.substring(getFieldPos(index),fieldsLength[index]).trim();
    }

    public void setFieldData(int index,String fieldData)
    {
        if(fieldsLength[index]==0){
            return;
        }
        String fmtStr=String.format("%%-%ds",fieldsLength[index]);
        //Log.d(tag,fmtStr);
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
/*
        Log.d(tag,String.format("Index:%d Field Data:(%d)%s Field length:%d Data:%s Length:%d",
                index,fieldData.length(),fieldData,getFieldsLength()[index],this.data,this.data.length()));
*/
    }

    public void setFieldData(int index,int intData){
        if(fieldsLength[index]==0){
            return;
        }

        String fmtStr=String.format("%%0%dd",fieldsLength[index]);
        //Log.d(tag,fmtStr);
        setFieldData(index,String.format(fmtStr,intData));
    }

    public void setFieldLength(int index,int length){
        //检查是否需改变相应域的原有初始化数据
        int oldLength=fieldsLength[index];
        if(length!=oldLength){
            String data=getData();
            Log.d(tag,String.format("Data befor set field length:(%d)%s",data.length(),data));
            String newData="";
            //复制之前的数据
            if(index!=0) {
                int pos = getFieldPos(index);
                newData = data.substring(0, pos);
                Log.d(tag,String.format("Data head:(%d)%s",newData.length(),newData));
            }
            //初始化当前数据
            for(int i=0;i<length;i++){
                newData+="0";
            }
            //复制尾部
            if(index<fieldsLength.length-1){
                newData+=data.substring(getFieldPos(index+1));
            }

            setData(newData);
            Log.d(tag,String.format("Data after set field length:(%d)%s",newData.length(),newData));
        }
        fieldsLength[index]=(byte)length;
    }
}
