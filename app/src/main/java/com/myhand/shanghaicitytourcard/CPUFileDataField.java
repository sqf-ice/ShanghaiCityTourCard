package com.myhand.shanghaicitytourcard;

/**
 * Created by wenha_000 on 2017-09-06.
 */

public class CPUFileDataField {
    private byte position;
    private byte length;

    public CPUFileDataField() {
    }

    public CPUFileDataField(byte position, byte length) {
        this.position = position;
        this.length = length;
    }

    public byte getPosition() {
        return position;
    }

    public void setPosition(byte position) {
        this.position = position;
    }

    public byte getLength() {
        return length;
    }

    public void setLength(byte length) {
        this.length = length;
    }
}
