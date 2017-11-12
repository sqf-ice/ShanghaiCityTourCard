package com.myhand.cpucard;

import android.graphics.Color;
import android.util.Log;

import com.centerm.smartpos.aidl.rfcard.AidlRFCard;
import com.centerm.smartpos.util.HexUtil;
import com.myhand.common.Converter;
import com.myhand.shanghaicitytourcard.CPUFileData0015;
import com.myhand.shanghaicitytourcard.CPUFileDataFCI;

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
}
