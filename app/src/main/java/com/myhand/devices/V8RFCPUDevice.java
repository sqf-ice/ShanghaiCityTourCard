package com.myhand.devices;

import android.widget.Toast;

import com.centerm.smartpos.aidl.rfcard.AidlRFCard;
import com.centerm.smartpos.util.HexUtil;
import com.myhand.cpucard.SHTCPsamCard;
import com.myhand.shanghaicitytourcard.CityCardQueryActivity;

/**
 * Created by wenha_000 on 2017-09-08.
 */

public class V8RFCPUDevice extends RFCPUDevice{
    private AidlRFCard rfCard;

    public V8RFCPUDevice() {
    }

    @Override
    public boolean open() {
        try {
            rfCard.open();
        }catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            setErrorMessage("卡打开异常："+e.getLocalizedMessage());
            return false;
        }
        return true;
    }

    @Override
    public byte[] reset() {
        byte[] retData=null;
        int retVal = -1;
        try {
            retData = rfCard.reset();
            if(retData==null){
                //卡复位失败
                setErrorMessage("卡复位失败");
                return null;
            }else{
                return retData;
            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            setErrorMessage("卡复位异常："+e.getLocalizedMessage());
        }
        return null;
    }

    @Override
    public byte readChipType() {
        return 0;
    }

    public V8RFCPUDevice(AidlRFCard rfCard) {
        this.rfCard = rfCard;
    }

    public AidlRFCard getRfCard() {
        return rfCard;
    }

    public void setRfCard(AidlRFCard rfCard) {
        this.rfCard = rfCard;
    }

    @Override
    public byte[] sendAPDU(byte[] apdu) {
//        byte[] ret = null;
        try {
            return rfCard.send(apdu);
/*
            if (null == ret) {
                setErrorMessage("APDU执行失败："+HexUtil.bytesToHexString(apdu));
                return null;
            }
            if(ret.length<2
                    ||(ret[ret.length-2]!=0x90&&ret[ret.length-1]!=0x00))
            {
                setErrorMessage("APDU执行失败："+HexUtil.bytesToHexString(apdu)+" 结果："+HexUtil.bytesToHexString(ret));
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
*/
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            setErrorMessage("APDU:"+HexUtil.bytesToHexString(apdu)+"执行异常："+e.getLocalizedMessage());
        }
        return null;
    }

    public byte[] readVerifyCode()
    {
        return sendAPDU(HexUtil.hexStringToByte(SHTCPsamCard.APDUUSERCODE));
    }
}
