package com.myhand.devices;

import com.centerm.smartpos.util.HexUtil;
import com.centerm.smartpos.util.LogUtil;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public abstract class RFCPUDevice extends CPUDevice{
    @Override
    public byte[] sendAPDU(byte[] apdu) {
        LogUtil.print(new String(apdu));
        return new byte[0];
    }
}
