package com.myhand.cpucard;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public class PSAMCard extends CPUCard {
    private byte keyVersion;
    private byte[] posID;
    private int sequence;
    private byte[] debitMac1;

    public PSAMCard() {
    }

    public byte getKeyVersion() {
        return keyVersion;
    }

    public void setKeyVersion(byte keyVersion) {
        this.keyVersion = keyVersion;
    }

    public byte[] getPosID() {
        return posID;
    }

    public void setPosID(byte[] posID) {
        this.posID = posID;
    }

    public int getSequence() {
        return sequence;
    }

    public void setSequence(int sequence) {
        this.sequence = sequence;
    }

    public byte[] getDebitMac1() {
        return debitMac1;
    }

    public void setDebitMac1(byte[] debitMac1) {
        this.debitMac1 = debitMac1;
    }
}
