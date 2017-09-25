package com.myhand.devices;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public abstract class CPUDevice {
    private String errorMessage;

    public String getErrorMessage() {
        return errorMessage;
    }

    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    public abstract byte[] sendAPDU(byte[] apdu);
    public abstract boolean open();
    public abstract byte[] reset();
    public abstract byte readChipType();
}
