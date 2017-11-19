package com.myhand.cpucard;

import android.app.Application;
import android.util.Log;

import com.centerm.smartpos.aidl.psam.AidlPsam;
import com.centerm.smartpos.util.AidlErrorCode;
import com.centerm.smartpos.util.HexUtil;
import com.myhand.common.Converter;
import com.myhand.devices.CPUDevice;
import com.myhand.devices.PSAMDevice;

/**
 * Created by wenha_000 on 2017-09-04.
 */

//上海公交IC卡PSAM卡
public class SHTCPsamCard extends PSAMCard{
    private static final String tag=SHTCPsamCard.class.getSimpleName();
    //APDU指令
    public static final String APDUSel0016="00B0960000";
    public static final String APDUSelFile="00A40000021001";
    public static final String APDUSel0017="00B0970001";
    public static final String APDUUSERCODE="80CA000009";

    //认证用户码指令
    public String apduUserCode(byte[] userCode)
    {
        return APDUUSERCODE+HexUtil.bytesToHexString(userCode);
    }


    private String posNo;
    public String getPosNo()
    {
        return posNo;
    }

    public boolean readPosNO(PSAMDevice psamDevice)
    {
        byte[] ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(APDUSel0016));
        if(ret==null||ret.length<2) {
            setErrorMessage("获取PSAM卡POSNO失败："+HexUtil.bytesToHexString(ret));
            posNo="";
            return false;
        }

        posNo=HexUtil.bytesToHexString(CPUDevice.getResponseData(ret));
        setPosID(CPUDevice.getResponseData(ret));
        Log.d(tag,String.format("PosNO:%s",posNo));

        return true;
    }
    public boolean selFile(PSAMDevice psamDevice)
    {
        byte[] ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(APDUSelFile));
        if(ret==null)
        {
            return false;
        }

        return true;
    }

    public boolean psamReadKeyVersion(PSAMDevice psamDevice)
    {
        if(!selFile(psamDevice))
        {
            setErrorMessage("选择文件失败");
            return false;
        }
        byte[] ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(APDUSel0017));
        if(ret==null)
        {
            return false;
        }
        String errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0){
            Log.d(tag,String.format("选择0017文件失败，apud:%s result:%s",APDUSel0017,HexUtil.bytesToHexString(ret)));
            return false;
        }
        psamDevice.getPsamCard().setKeyVersion((byte) Converter.BytesToLong(CPUDevice.getResponseData(ret)));
        //keyVersion=HexUtil.bytesToHexString();
        return true;
    }
}
