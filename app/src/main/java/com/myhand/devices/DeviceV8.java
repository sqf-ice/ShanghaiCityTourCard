package com.myhand.devices;

import android.util.Log;
import android.widget.Toast;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.POS.DatabaseSHCT;
import com.myhand.common.Converter;
import com.myhand.cpucard.CPUUserCard;
import com.myhand.cpucard.DebitRecord;
import com.myhand.cpucard.SHTCCPUUserCard;
import com.myhand.cpucard.SHTCPsamCard;
import com.myhand.shanghaicitytourcard.CPUFileDataFCI;
import com.myhand.shanghaicitytourcard.CityCardQueryActivity;
import com.myhand.shanghaicitytourcard.CityTourCard;
import com.myhand.shanghaicitytourcard.POSApplication;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by wenha_000 on 2017-09-08.
 */

public class DeviceV8 extends POSDevice {
    @Override
    public boolean openPSAMCard() {
        if(getPsamDevice()==null
                ||!getPsamDevice().open()
                ||getPsamDevice().reset()==null)
        {
            getPsamDevice().setErrorMessage("没有PSAM卡");
            return false;
        }

        SHTCPsamCard psamCard=(SHTCPsamCard) getPsamDevice().getPsamCard();
        if(!psamCard.readPosNO(getPsamDevice()))
        {
            getPsamDevice().setErrorMessage("读取POS号失败:"+getPsamDevice().getErrorMessage());
        }
        setPosID(psamCard.getPosNo());
/*
            if(!psamCard.selFile(posDevice.getPsamDevice())){
                mLogString.add(0,"PSAM卡选择文件失败");
            }
*/
        if(!psamCard.readKeyVersion(getPsamDevice()))
        {
            getPsamDevice().setErrorMessage("读取PSAM卡密钥版本号失败");
        }

        return true;
    }

    @Override
    public CityTourCard readCard() {
        byte[] retData=null;
        int retVal = -1;
        if(!getRfcpuDevice().open())
        {
            return null;
        }

        retData = getRfcpuDevice().reset();
        if(retData==null){
            //卡复位失败
            getRfcpuDevice().setErrorMessage("卡复位失败" );
            return null;
        }
        CityTourCard card=new CityTourCard(retData,getRfcpuDevice().readChipType());

        //读取FCI数据，上海交通卡规范
        byte[] result=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(card.APDUSelFCIDATA));
        if(result==null)
        {
            getRfcpuDevice().setErrorMessage("读取FCI数据失败");
            return null;
        }
        //设置FCI数据
        CPUFileDataFCI dataFCI=new CPUFileDataFCI();
        dataFCI.setFileData(result);
        card.setDataFCI(dataFCI);

        //取卡余额
        byte[] balance=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(card.APDUFETCHBALANCE));
        card.setByteBalance(balance);

        //读取用户卡认证码
        card.setVerifyCode(((V8RFCPUDevice)getRfcpuDevice()).readVerifyCode());

        return card;
    }

    @Override
    public DebitRecord debit(CPUUserCard userCard, int amount)
    {
        String tag="Debit";
        V8PsamDevice psamDevice=(V8PsamDevice)getPsamDevice();
        if(!openPSAMCard())
        {
            return null;
        }

        SHTCPsamCard psamCard=(SHTCPsamCard)psamDevice.getPsamCard();
        CityTourCard currCard=(CityTourCard)userCard;

        //PSAM卡送认证码
        Log.d("Debit","认证指令："+psamCard.apduUserCode(readCard().getVerifyCode()));
        byte[] ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(psamCard.apduUserCode(readCard().getVerifyCode())));
        if(ret==null)
        {
            return null;
        }else{
            Log.e("Debit","Ret:"+HexUtil.bytesToHexString(ret));
        }

        //用户卡开始交易
        String apduUserCardTxnBegin="805001020B"+psamCard.getKeyVersion()+HexUtil.bytesToHexString(Converter.IntToBytes(amount))
            +psamCard.getPosNo();
        Log.d("Debit","用户卡开始交易："+apduUserCardTxnBegin);
        ret=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apduUserCardTxnBegin));
        if(ret==null||ret.length<15)
        {
            setErrorMessage("用户卡开始交易失败："+HexUtil.bytesToHexString(ret));
            return null;
        }
        Log.d("Debit","结果："+HexUtil.bytesToHexString(ret));
        //解析卡结果
        byte[] balanceBef=new byte[4];
        System.arraycopy(ret,0,balanceBef,0,4);
        int pos=4;
        byte[] cardCounter=new byte[2];
        System.arraycopy(ret,pos,cardCounter,0,2);
        pos+=2;
        byte[] limit=new byte[3];
        System.arraycopy(ret,pos,limit,0,3);
        pos+=3;
        byte[] alg=new byte[2];
        System.arraycopy(ret,pos,alg,0,2);
        pos+=2;
        byte[] rand=new byte[4];
        System.arraycopy(ret,pos,rand,0,4);

        //PSAM开始交易
        Date date=new Date();
        SimpleDateFormat formatter = new SimpleDateFormat("yyyyMMddHHmmss");
        String dateString = formatter.format(date);
        Log.d("Debit",dateString);
        byte txnAttr=0x06;

        String apduPsamCardBegin="8070000024"+HexUtil.bytesToHexString(rand)
                +HexUtil.bytesToHexString(cardCounter)+HexUtil.bytesToHexString(Converter.IntToBytes(amount))
                +String.format("%02x",txnAttr)+ dateString+HexUtil.bytesToHexString(alg)
                +HexUtil.bytesToHexString(((CityTourCard)userCard).getFCIValidData().getCardNo())
                +HexUtil.bytesToHexString(currCard.getFCIValidData().getCityCode())
                +"FF0000000000";
        Log.d("Debit","apdu:"+apduPsamCardBegin);
        ret=getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduPsamCardBegin));
        if(ret==null)
        {
            return null;
        }
        Log.d("Debit","结果："+HexUtil.bytesToHexString(ret));
        byte[] posSeq=new byte[4];
        System.arraycopy(ret,0,posSeq,0,4);
        byte[] mac1=new byte[4];
        System.arraycopy(ret,4,mac1,0,4);

        //用户卡交易完成
        String apduUserCardEnd="805401000F"+HexUtil.bytesToHexString(posSeq)
                +dateString+HexUtil.bytesToHexString(mac1);
        Log.d("Debit",apduUserCardEnd);
        ret=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apduUserCardEnd));
        Log.d(tag,"用户卡交易完成 apdu="+apduUserCardEnd+" Result:"+HexUtil.bytesToHexString(ret));
        if(ret==null)
        {
            String msg="用户卡交易完成失败";
            Log.d("Debit",msg);
            setErrorMessage(msg);
            return  null;
        }
        byte[] tac=new byte[4];
        System.arraycopy(ret,0,tac,0,4);
        byte[] mac2=new byte[4];
        System.arraycopy(ret,4,mac2,0,4);
        //PSAM卡完成交易
        String apduPsamCardEnd="8072000004"+HexUtil.bytesToHexString(mac2);
        ret=getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduPsamCardEnd));
        Log.d("Debit","Result:"+HexUtil.bytesToHexString(ret));

        //保存交易
        DebitRecord debitRecord=new DebitRecord(getCorpCode(),
                0,
                txnAttr,
                getStationID(),
                getUser().getUserID(),
                "0",//无公交车编号
                getTxnType(),
                (int)Converter.BytesToLong(posSeq),
                HexUtil.bytesToHexString(currCard.getFCIValidData().getCityCode()),
                HexUtil.bytesToHexString(currCard.getFCIValidData().getCardNo()),
                currCard.getFCIValidData().getCardType(),Converter.BytesToLong(balanceBef),
                amount,dateString,
                (int)Converter.BytesToLong(cardCounter),psamCard.getPosNo(),
                HexUtil.bytesToHexString(tac),
                (byte)Integer.parseInt(psamCard.getKeyVersion(),0x10),//卡内版本号
                (byte)0);

        DatabaseSHCT db=POSApplication.getPOSApplication().getAppDatabase();
        if(!db.saveDebit(debitRecord))
        {
            Log.d(tag,"保存交易失败："+db.errorMessageString);
        }

        return debitRecord;
    }

    @Override
    public DebitRecord complexDebit(CPUUserCard userCard, int amount) {
        String tag="ComplexDebit";
        V8PsamDevice psamDevice=(V8PsamDevice)getPsamDevice();
        if(!openPSAMCard())
        {
            return null;
        }

        //SAM卡选1001目录
        String apduPSAMSel001="00A40000021001";
        byte[] ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPSAMSel001));
        Log.d(tag,"Sel1001:apdu="+apduPSAMSel001+" Result="+HexUtil.bytesToHexString(ret));
        //用户卡取随机数
        String apudUserCardRand="0084000008";
        byte[] rand=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apudUserCardRand));
        Log.d(tag,"User Card Rand:apdu="+apudUserCardRand+" Result:"+HexUtil.bytesToHexString(rand));

        //Sam卡 801A分散
        String apdu801A="801A450110"
                +HexUtil.bytesToHexString(Converter.byteReversion(((CityTourCard)userCard).getFCIValidData().getCardNo()))
                +"2000FF0000000000";
        ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(apdu801A));
        Log.d(tag,"Sam卡 801A分散 apdu="+apdu801A+"Result:"+HexUtil.bytesToHexString(ret));

        //SAM卡80fa
        String apduPSAM80fa="80FA000008"+HexUtil.bytesToHexString(rand) ;
        byte[] mac=psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPSAM80fa));
        Log.d(tag,"SAM卡80fa adpu:"+apduPSAM80fa+" mac:"+HexUtil.bytesToHexString(mac));
        //用户卡外部认证
        String apduUserCardExterl="0082000208"+HexUtil.bytesToHexString(mac);
        ret=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apduUserCardExterl));
        Log.d(tag,"用户卡外部认证 apdu:"+apduUserCardExterl+" Result:"+HexUtil.bytesToHexString(ret));

        SHTCPsamCard psamCard=(SHTCPsamCard)psamDevice.getPsamCard();
        //PSAM卡送认证码
        String apduCardAuth=psamCard.apduUserCode(readCard().getVerifyCode());
        ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(apduCardAuth));
        Log.d("Debit","认证指令："+apduCardAuth+" Result:"+HexUtil.bytesToHexString(ret));
        if(ret==null)
        {
            return null;
        }

        //用户卡开始交易
        String apduUserCardTxnBegin="805003020B"+psamCard.getKeyVersion()+HexUtil.bytesToHexString(Converter.IntToBytes(amount))
                +psamCard.getPosNo();
        ret=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apduUserCardTxnBegin));
        Log.d(tag,"用户卡开始交易：apdu:"+apduUserCardTxnBegin+" Result:"+HexUtil.bytesToHexString(ret));
        if(ret==null||ret.length<15)
        {
            setErrorMessage("用户卡开始交易失败："+HexUtil.bytesToHexString(ret));
            return null;
        }
        //解析卡结果
        byte[] balance=new byte[4];
        System.arraycopy(ret,0,balance,0,4);
        int pos=4;
        byte[] cardSeq=new byte[2];
        System.arraycopy(ret,pos,cardSeq,0,2);
        pos+=2;
        byte[] limit=new byte[3];
        System.arraycopy(ret,pos,limit,0,3);
        pos+=3;
        byte[] alg=new byte[2];
        System.arraycopy(ret,pos,alg,0,2);
        pos+=2;
        rand=new byte[4];
        System.arraycopy(ret,pos,rand,0,4);

        //PSAM开始交易
        Date date=new Date();
        SimpleDateFormat formatter = new SimpleDateFormat("yyyyMMddHHmmss");
        String dateString = formatter.format(date);
        Log.d(tag,"交易日期："+dateString);
        int posSeq=getPosSequence();
        setPosSequence(posSeq++);

        String apduPsamCardBegin="8070000128"+HexUtil.bytesToHexString(rand)
                +HexUtil.bytesToHexString(cardSeq)
                +HexUtil.bytesToHexString(Converter.IntToBytes(amount))
                +"09"+ dateString+HexUtil.bytesToHexString(alg)
                +Converter.IntToBytes(posSeq)
                +HexUtil.bytesToHexString(((CityTourCard)userCard).getFCIValidData().getCardNo())
                +"2000"+"FF0000000000";
        ret=getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduPsamCardBegin));
        Log.d(tag,"PSAM卡开始交易 apdu:"+apduPsamCardBegin+"Result:"+HexUtil.bytesToHexString(ret));
        if(ret==null)
        {
            Log.d(tag,"PSAM Begin failure.");
            return null;
        }
        byte[] posSeq1=new byte[4];
        System.arraycopy(ret,0,posSeq,0,4);
        byte[] mac1=new byte[4];
        System.arraycopy(ret,4,mac1,0,4);

        //用户卡更新复合消费缓存
        String txnData="140601010000A7C500007E0C2BEC7CB9";
        String apduUserCardUpdateCache="80DC003B10"+txnData;
        ret=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apduUserCardUpdateCache));
        if(ret==null)
        {
            Log.d(tag,"用户卡更新复合消费缓存 apdu:"+apduUserCardUpdateCache+" Cause:"
                    +getRfcpuDevice().getErrorMessage());
            return null;
        }
        Log.d(tag,"用户卡更新复合消费缓存 apdu:"+apduUserCardUpdateCache+" Result:"+HexUtil.bytesToHexString(ret));

        //用户卡交易完成
        String apduUserCardEnd="805401000F"+HexUtil.bytesToHexString(Converter.IntToBytes(posSeq))
                +dateString+HexUtil.bytesToHexString(mac1);
        ret=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apduUserCardEnd));
        Log.d(tag,"用户卡交易完成 apdu:"+apduUserCardEnd+" Result:"+HexUtil.bytesToHexString(ret));
        if(ret==null)
        {
            String msg="用户卡交易完成失败";
            Log.d("Debit",msg);
            setErrorMessage(msg);
            return  null;
        }
        byte[] tac=new byte[4];
        System.arraycopy(ret,0,tac,0,4);
        byte[] mac2=new byte[4];
        System.arraycopy(ret,4,mac2,0,4);
        //PSAM卡完成交易
        String apduPsamCardEnd="8072000004"+HexUtil.bytesToHexString(mac2);
        ret=getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduPsamCardEnd));
        Log.d(tag,"PSAM卡完成交易 apdu:"+apduPsamCardEnd+" Result:"+HexUtil.bytesToHexString(ret));

        return null;
    }
}
