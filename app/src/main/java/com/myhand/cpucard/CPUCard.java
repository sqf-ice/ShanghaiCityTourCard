package com.myhand.cpucard;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public class CPUCard {
    private String errorMessage;
    private byte[] atr;

    public byte[] getAtr() {
        return atr;
    }

    public void setAtr(byte[] atr) {
        this.atr = atr;
    }

    public String getErrorMessage() {
        return errorMessage;
    }

    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }
}
