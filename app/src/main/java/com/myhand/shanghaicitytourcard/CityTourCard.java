package com.myhand.shanghaicitytourcard;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.common.Converter;
import com.myhand.cpucard.SHTCCPUUserCard;

/**
 * Created by wenha_000 on 2017-09-03.
 */

public class CityTourCard extends SHTCCPUUserCard {

    public CityTourCard(){

    }

    public CityTourCard(byte[] atr,int chipType)
    {
        setAtr(atr);
        setChipType(chipType);
    }

    public CPUFCIValidData getFCIValidData()
    {
        CPUFCIValidData result=new CPUFCIValidData();
        result.setFileData(getDataFCI().getValidData());

        return result;
    }

    public String apduCardDebit(int amount,byte[] posCode)
    {
        return String.format("%1$s%2$02X%3$08X%4$s",APDUCARDDEBIT,1,amount, HexUtil.bytesToHexString(posCode));
    }
}
