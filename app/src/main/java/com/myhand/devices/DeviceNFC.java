package com.myhand.devices;

import com.myhand.cpucard.CPUUserCard;
import com.myhand.cpucard.DebitRecord;
import com.myhand.shanghaicitytourcard.CityTourCard;

/**
 * Created by wenha_000 on 2017-09-28.
 */

public class DeviceNFC extends POSDevice{
    @Override
    public boolean openPSAMCard() {
        return false;
    }

    @Override
    public CityTourCard readCard() {
        return null;
    }

    @Override
    public DebitRecord debit(CPUUserCard userCard, int amount) {
        return null;
    }

    @Override
    public DebitRecord complexDebit(CPUUserCard userCard, int amount) {
        return null;
    }
}
