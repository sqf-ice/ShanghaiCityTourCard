package com.myhand.cpucard;

import com.myhand.shanghaicitytourcard.CPUFileData0015;
import com.myhand.shanghaicitytourcard.CPUFileDataFCI;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public class CPUUserCard extends CPUCard{
    private int chipType;

    public CPUUserCard() {
    }

    public int getChipType() {
        return chipType;
    }

    public void setChipType(int chipType) {
        this.chipType = chipType;
    }
}
