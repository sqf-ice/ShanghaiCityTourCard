package com.myhand.cpucard;

import android.graphics.Color;
import android.util.Log;

import com.centerm.smartpos.aidl.rfcard.AidlRFCard;
import com.centerm.smartpos.util.HexUtil;
import com.myhand.common.Converter;
import com.myhand.shanghaicitytourcard.CPUFileData0015;
import com.myhand.shanghaicitytourcard.CPUFileDataFCI;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by wenha_000 on 2017-09-06.
 */

public class SHTCCPUUserCard extends CPUUserCard{
    private static final String tag=SHTCCPUUserCard.class.getSimpleName();
    public static final String APDUSelFCIDATA="00A4040009A0000000038698070100";
    public static final String APDUFETCHBALANCE="805C000204";
    public static final String APDUFETCHUSERCODE="80CA000009";
    public static final String APDUCARDDEBIT="805001020B";

    //卡类型
    private byte cardType;
    private byte typeIn05;
    //卡有效期
    private Date validateDate;
    //密钥版本
    private byte keyVersion;
    //认证码
    private byte[] verifyCode;
    //卡余额4字节HEX
    private byte[] byteBalance;

    //0015文件
    CPUFileData0015 file0015;
    CPUFileDataFCI dataFCI;

    public SHTCCPUUserCard()
    {

    }

    public byte getTypeIn05() {
        return typeIn05;
    }

    public void setTypeIn05(byte typeIn05) {
        this.typeIn05 = typeIn05;
    }

    public byte getKeyVersion() {
        return keyVersion;
    }
    public void setKeyVersion(byte keyVersion) {
        this.keyVersion = keyVersion;
    }
    public byte[] getVerifyCode() {
        return verifyCode;
    }

    public void setVerifyCode(byte[] verifyCode) {
        this.verifyCode = verifyCode;
    }

    public byte[] getByteBalance() {
        return byteBalance;
    }

    public void setByteBalance(byte[] byteBalance) {
        this.byteBalance = byteBalance;
    }

    public CPUFileData0015 getFile0015() {
        return file0015;
    }

    public void setFile0015(CPUFileData0015 file0015) {
        this.file0015 = file0015;
    }

    public CPUFileDataFCI getDataFCI() {
        return dataFCI;
    }

    public void setDataFCI(CPUFileDataFCI dataFCI) {
        this.dataFCI = dataFCI;
    }

    public int getBalance() {
        return (int) Converter.BytesToLong(byteBalance);
    }

    public void setBalance(int balance) {
        this.byteBalance = Converter.IntToBytes(balance);
    }

    public static String CardFaceNum(byte[] innerCardNo){
        Log.d(tag,HexUtil.bytesToHexString(innerCardNo));
        long csn=(long)((innerCardNo[0]&0xff)<<24)+(long)((innerCardNo[1]&0xff)<<16)+(long)((innerCardNo[2]&0xff)<<8)+(long)(innerCardNo[3]&0xff);
        Log.d(tag,String.format("csn=%d",csn));
        byte[] tmp=new byte[16];
        tmp[10]=0x00;
        for(int i=0;i<10;i++){
            tmp[i]=(byte) (csn%10);
            csn=csn/10;
        }

        tmp[10]=0x00;
        byte temp=0x00;
        for(int i=0;i<5;i++){
            temp=(byte) (tmp[i*2]*2);
            tmp[10]=(byte)((tmp[10]+(temp/10)+(temp%10))&0xff);
        }
        for(int i=0;i<5;i++){
            tmp[10]=(byte)(( tmp[10]+tmp[2*i+1])&0xff);
        }

        temp=(byte)(tmp[10]%10);
        if(temp==0){
            tmp[10]=0;
        }else{
            tmp[10]=(byte)(10-temp);
        }

        byte[] result=new byte[11];
        result[0]=(byte)((byte)(tmp[10]&0x0f)+(byte)0x30);
        for(int i=0;i<5;i++){
            result[i*2+1]=(byte)((byte)(tmp[i*2+1]&0x0f)+(byte)0x30);
            result[i*2+2]=(byte)((byte)(tmp[i*2]&0x0f)+(byte)0x30);
        }
        Log.d(tag,HexUtil.bytesToHexString(result)+"Card face num:"+new String(result));
        return new String(result);
    }

    public static String dateInCache(Date date){
        /*
        14．	交易日期记录交易完成时交易设备内的日期和时间，为5字节表示，年用12bitHEX表示（0—4095），
        月用4bitHEX表示（1—12），日用5bitHEX表示（1—31），时用5bit表示（0—24），分用6bit（0—59）表示，
        秒用6bit表示（0—59）,后2bit用0填充。例：2000年3月1日9时30分10秒将表示为7D030A5E28
         */
        int year=date.getYear();
        int month=date.getMonth();
        int day=date.getDate();
        Log.d(tag,String.format("day is %d",day));
        int hour=date.getHours();
        int min=date.getMinutes();
        int second=date.getSeconds();

        //long result=(year<<10)+(month<<10)+(day<<10)+
        int part1=(year<<4)+(month&0x0000000F);
        Log.d(tag,String.format("%04X",part1));

        int part2=(day<<11)+(hour<<6)+(min);
        Log.d(tag,String.format("%04X",part2));
        int part3=second<<2;
        String resultStr=String.format("%04X%04X%02X",part1,part2,part3);

        return resultStr;
    }

    public static String dateInCache(String dateStr){
        SimpleDateFormat sdf=new SimpleDateFormat("yyyyMMddHHmmss");
        try {
            Date date=sdf.parse(dateStr);
            return dateInCache(date);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        return "";
    }

    }
