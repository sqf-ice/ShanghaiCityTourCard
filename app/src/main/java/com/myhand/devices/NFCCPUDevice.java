package com.myhand.devices;

import android.nfc.NfcAdapter;

/**
 * Created by wenha_000 on 2017-09-28.
 */

public class NFCCPUDevice extends RFCPUDevice{
    private NfcAdapter nfcAdapter=null;

    @Override
    public byte[] sendAPDU(byte[] apdu) {
        return new byte[0];
    }

    @Override
    public boolean open() {
        return false;
    }

    @Override
    public byte[] reset() {
        return new byte[0];
    }

    @Override
    public byte readChipType() {
        return 0;
    }
}
