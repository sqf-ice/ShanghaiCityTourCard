package com.myhand.common;

import android.util.Log;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.SimpleFormatter;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public class Converter {
    private static final String tag=Converter.class.getSimpleName();
    public static long BytesToLong(byte[] src)
    {
        if(src==null||src.length==0)
        {
            return 0;
        }

        long result=0;
        for(int i=0;i<src.length;i++)
        {
            result=(result<<8)+(src[i]&0xFF);
        }

        return result;
    }

    public static byte[] IntToBytes(int amount)
    {
        byte[] result=new byte[4];
        int temp=amount;
        for(int i=3;i>=0;i--)
        {
            result[i]=(byte)(temp&0xFF);
            temp=amount>>8;
        }
        return result;
    }

    public static byte[] incBytes(byte[] src){
        long tmpLong=0;
        for(int i=0;i<src.length;i++){
            Log.d(tag,String.format("src[%d]=%02X",i,(src[i]&0xFF)));
            tmpLong=tmpLong*256+(src[i]&0xFF);
        }
        tmpLong++;
        Log.d(tag,String.format("incBytes(byte[] src),tmpLong=%d",tmpLong));
        byte[] result=new byte[src.length];
        for(int i=src.length-1;i>=0;i--){
            result[i]=(byte) (((byte)tmpLong)&0xFF);
            tmpLong=tmpLong>>8;
        }
        return result;
    }

    public static byte[] LongToBytes(long amount)
    {
        byte[] result=new byte[8];
        long temp=amount;
        for(int i=7;i>=0;i--)
        {
            result[i]=(byte)(temp&0xFF);
            temp=temp>>8;
        }
        return result;
    }

    public static byte[] byteReversion(byte[] src)
    {
        if(src==null)
        {
            return null;
        }

        byte[] result=new byte[src.length];
        for(int i=0;i<src.length;i++)
        {
            result[i]=src[src.length-1-i];
        }

        return result;
    }

    public static String dateFormatConvert(String dateStr){
        SimpleDateFormat sdf1=new SimpleDateFormat("yyyyMMddHHmmss");
        SimpleDateFormat sdf2=new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        try {
            Date date=sdf1.parse(dateStr);
            return sdf2.format(date);
        } catch (ParseException e) {
            e.printStackTrace();
            return dateStr;
        }
    }
}
