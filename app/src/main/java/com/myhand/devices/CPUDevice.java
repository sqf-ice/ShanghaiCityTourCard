package com.myhand.devices;

import com.centerm.smartpos.util.HexUtil;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public abstract class CPUDevice {
    private String errorCode;
    private String errorMessage;

    public abstract byte[] sendAPDU(byte[] apdu);
    public abstract boolean open();
    public abstract byte[] reset();
    public abstract byte readChipType();

    public String getErrorCode() {
        return errorCode;
    }

    public void setErrorCode(String errorCode) {
        this.errorCode = errorCode;
    }

    public String getErrorMessage() {
        return errorMessage;
    }
    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    public void setError(String errorCode,String errorMessage){
        this.errorCode=errorCode;
        this.errorMessage=errorMessage;
    }


    public static String parseResponse(byte[] responseData){
        if(responseData.length<2)
        {
            return null;
        }
        // 如果只为9000，直接返回
        if(responseData.length==2)
        {
            return HexUtil.bytesToHexString(responseData);
        }

        byte[] resultByte=new byte[2];
        System.arraycopy(responseData,responseData.length-2,resultByte,0,2);
        return HexUtil.bytesToHexString(resultByte);
    }

    public static byte[] getResponseData(byte[] responseData){
        if(responseData.length<2)
        {
            return null;
        }
        // 如果只为9000，直接返回null
        if(responseData.length==2)
        {
            return null;
        }

        byte[] resultByte=new byte[responseData.length-2];
        System.arraycopy(responseData,0,resultByte,0,responseData.length-2);
        return resultByte;
    }

}
