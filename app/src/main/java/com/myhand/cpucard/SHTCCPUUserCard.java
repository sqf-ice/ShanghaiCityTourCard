package com.myhand.cpucard;

import android.graphics.Color;

import com.centerm.smartpos.aidl.rfcard.AidlRFCard;
import com.centerm.smartpos.util.HexUtil;
import com.myhand.common.Converter;
import com.myhand.shanghaicitytourcard.CPUFileData0015;
import com.myhand.shanghaicitytourcard.CPUFileDataFCI;

/**
 * Created by wenha_000 on 2017-09-06.
 */

public class SHTCCPUUserCard extends CPUUserCard{
    public static final String APDUSelFCIDATA="00A4040009A0000000038698070100";
    public static final String APDUFETCHBALANCE="805C000204";
    public static final String APDUFETCHUSERCODE="80CA000009";
    public static final String APDUCARDDEBIT="805001020B";

    //密钥版本
    private byte keyVersion;
    public byte getKeyVersion() {
        return keyVersion;
    }

    public void setKeyVersion(byte keyVersion) {
        this.keyVersion = keyVersion;
    }

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
}
