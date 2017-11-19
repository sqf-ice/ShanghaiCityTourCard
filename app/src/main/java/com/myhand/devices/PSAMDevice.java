package com.myhand.devices;

import com.myhand.cpucard.PSAMCard;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public abstract class PSAMDevice extends CPUDevice {
    private PSAMCard psamCard;

    public PSAMDevice() {
        psamCard=new PSAMCard();
    }

    public PSAMCard getPsamCard() {
        return psamCard;
    }

    public void setPsamCard(PSAMCard psamCard) {
        this.psamCard = psamCard;
    }

}
