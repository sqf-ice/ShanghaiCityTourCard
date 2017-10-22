package com.myhand.shtcdatafile;

import android.util.Log;

import com.centerm.smartpos.util.HexUtil;

import java.util.Date;

/**
 * Created by wenha_000 on 2017-09-24.
 * 数据缓存区
 * 未完成
 */

public class DataBuffer {
    private static final String tag= DataBuffer.class.getSimpleName();
    private byte[] buffer;
    private int header;
    private int tail;

    public DataBuffer(byte[] buffer) {
        this.buffer = buffer;
        header=0;
        tail=0;
    }

    public DataBuffer() {
        buffer=new byte[1024];
        header=0;
        tail=0;
    }

    public byte[] getBuffer() {
        return buffer;
    }

    public void setBuffer(byte[] buffer) {
        this.buffer = buffer;
    }

    public int getHeader() {
        return header;
    }

    public void setHeader(int header) {
        this.header = header;
    }

    public int getTail() {
        return tail;
    }

    public void setTail(int tail) {
        this.tail = tail;
    }

    public int size(){
        if(tail>=header)
        {
            return tail-header;
        }else{
            return buffer.length-header+tail;
        }
    }
    public boolean putData(byte[] data)
    {
        if(data==null){
            return  false;
        }
        System.arraycopy(data,0,buffer,tail,data.length);
        tail+=data.length;

        //Log.d(tag,"data buffer:"+ HexUtil.bytesToHexString(buffer));
        return true;
    }

    /**
     *
     * @return
     */
    public byte[] getFrameData(){
        if(size()<=4)
        {
            return null;
        }

        int length=0;
        String tmpMsg=new String(buffer,header,4);
        Log.d(tag,"Msg:"+tmpMsg);
        length=Integer.parseInt(tmpMsg);
        Log.d(tag,String.format("Frame data length is %d",length));
        if(size()<4+length){
            //未接收到足够的字节
            Log.d(tag,String.format("size is %d,but data length is %d",size(),length+4));
            return null;
        }
        byte[] result=new byte[length];
        System.arraycopy(buffer,header+4,result,0,length);
        header=length+4;
        if(header==tail)
        {
            header=0;
            tail=0;
        }
        Log.d(tag,"Frame Data:"+HexUtil.bytesToHexString(result));
        return result;
    }

}
