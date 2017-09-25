package com.myhand.devices;

import com.centerm.smartpos.aidl.psam.AidlPsam;
import com.centerm.smartpos.util.HexUtil;
import com.myhand.cpucard.PSAMCard;
import com.myhand.cpucard.SHTCPsamCard;

/**
 * Created by wenha_000 on 2017-09-07.
 */

public class V8PsamDevice extends PSAMDevice {
    private AidlPsam psam;

    public V8PsamDevice() {
    }

    public V8PsamDevice(AidlPsam psam) {
        this.psam = psam;
    }

    public AidlPsam getPsam() {
        return psam;
    }

    public void setPsam(AidlPsam psam) {
        this.psam = psam;
    }

    private byte ETUTime=2;
    @Override
    public boolean open() {
        try {
            if (!psam.open()) {
                setErrorMessage("打开失败");
                return false;
            }

            if (!psam.setETU(ETUTime)) {
                setErrorMessage("设置ETU失败");
                return false;
            }
        }catch(Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            setErrorMessage("打开失败："+e.getLocalizedMessage());
            return false;
        }
        return true;
    }

    @Override
    public byte[] reset() {
        try {
            byte[] ret = psam.reset();
            SHTCPsamCard psamCard=new SHTCPsamCard();
            psamCard.setAtr(ret);
            setPsamCard(psamCard);
            return ret;
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            setErrorMessage(e.getLocalizedMessage());
        }
        return  null;
    }

    @Override
    public byte[] sendAPDU(byte[] apdu) {
        try {
            byte[] ret = psam.sendApdu(apdu);
            if(ret.length<2)
            {
                setErrorMessage("返回值长度异常");
                return null;
            }

            if(ret[ret.length-2]!=0x90&&ret[ret.length-1]!=0x00)
            {
                setErrorMessage("APDU执行错误："+HexUtil.bytesToHexString(ret));
                return null;
            }
            // 如果只为9000，直接返回
            if(ret.length==2)
            {
                return ret;
            }
            //去掉尾部的9000
            byte[] result=new byte[ret.length-2];
            System.arraycopy(ret,0,result,0,ret.length-2);
            return  result;
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            setErrorMessage("APDU("+HexUtil.bytesToHexString(apdu)+")执行异常:"+e.getLocalizedMessage());
        }
        return null;
    }

    @Override
    public byte readChipType() {
        return 0;
    }
}
