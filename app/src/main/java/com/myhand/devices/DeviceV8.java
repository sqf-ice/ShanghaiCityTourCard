package com.myhand.devices;

import android.icu.text.SymbolTable;
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
    private static final String tag=DeviceV8.class.getSimpleName();

    //记录当前余额，用于判断用户卡是否扣款后交易失败
    private int oldBalance=-1;
    private int redebit=0;

    public DeviceV8(){
    }

    public int getOldBalance() {
        return oldBalance;
    }

    public void setOldBalance(int oldBalance) {
        this.oldBalance = oldBalance;
    }

    public int getRedebit() {
        return redebit;
    }

    public void setRedebit(int redebit) {
        this.redebit = redebit;
    }

    public void incRedebit(){
        redebit++;
    }

    public boolean unlockCard(CityTourCard card,boolean lock){
        //取随机数
        String apduRand="0084000004";
        byte[] ret=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apduRand));
        if(ret==null){
            setError(EC_NORESPONSE,String.format("解锁卡取随机数无回应：%s",apduRand));
            return false;
        }
        String errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=1){
            setError(errorCode,String.format("解锁卡取随机数失败：%s",apduRand));
            return false;
        }

        byte[] rand=new byte[4];
        System.arraycopy(ret,0,rand,0,4);

        //MAC第一步
        String apduMac="801A450110"
                +card.getInnerCardNoStr()
                +"2000"
                +"FF0000000000";
        ret=getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduMac));
        if(ret==null){
            setError(EC_NORESPONSE,String.format("解锁MAC第一步无回应：%s",apduMac));
            return false;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=1){
            setError(errorCode,String.format("解锁MAC第一步失败：%s",apduMac));
            return false;
        }
        //MAC第二步
        apduMac="80FA050010"
                +HexUtil.bytesToHexString(rand)
                +"000000008418000004800000";
        ret=getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduMac));
        if(ret==null){
            setError(EC_NORESPONSE,String.format("解锁MAC第二步无回应：%s",apduMac));
            return false;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=1){
            setError(errorCode,String.format("解锁MAC第二步失败：%s",apduMac));
            return false;
        }
        byte[] mac=new byte[4];
        System.arraycopy(ret,0,mac,0,4);
        //用户卡锁定应用
        String apduLock;
        if(!lock) {
            apduLock = "8418000004" + HexUtil.bytesToHexString(mac);
        }else {
            apduLock="841E000104" + HexUtil.bytesToHexString(mac);
        }
        ret=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apduLock));
        if(ret==null){
            setError(EC_NORESPONSE,String.format("应用锁定无回应：%s",apduLock));
            return false;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=1){
            setError(errorCode,String.format("应用锁定失败：%s",apduLock));
            return false;
        }
        return true;
    }

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
            setError(EC_NORESPONSE,"RF Device Failure");
            return null;
        }

        retData = getRfcpuDevice().reset();
        if(retData==null){
            //卡复位失败
            setError(EC_NORESPONSE,"卡复位失败" );
            return null;
        }
        CityTourCard card=new CityTourCard(retData,getRfcpuDevice().readChipType());

        //读取0x05文件
        String apduRd05="00B0850000";
        byte[] result=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apduRd05));
        if(result==null){
            setError(EC_NORESPONSE,String.format("读取0x05文件失败：%s",apduRd05));
            return null;
        }
        Log.d(tag,String.format("05文件：%s卡类型：%02X",HexUtil.bytesToHexString(result),result[8]));
        card.setTypeIn05(result[8]);

        //读取FCI数据，上海交通卡规范
        result=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(card.APDUSelFCIDATA));
        if(result==null)
        {
            setError(EC_NORESPONSE,String.format("读取FCI数据失败:%s",card.APDUSelFCIDATA));
            return null;
        }
        //设置FCI数据
        CPUFileDataFCI dataFCI=new CPUFileDataFCI();
        dataFCI.setFileData(CPUDevice.getResponseData(result));
        card.setDataFCI(dataFCI);

        //取卡余额
        result=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(card.APDUFETCHBALANCE));
        if(result==null){
            setError(EC_NORESPONSE,String.format("获取卡余额失败：%s",card.APDUFETCHBALANCE));
            return null;
        }
        byte[] balance=CPUDevice.getResponseData(result);
        Log.d(tag,String.format("卡余额:apdu:%s Result:%s balance:%d",card.APDUFETCHBALANCE,HexUtil.bytesToHexString(balance),
                Converter.BytesToLong(balance)));

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
                POSApplication.instance.getUser().getUsername(),
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
        setError("0000","OK");
        V8PsamDevice psamDevice=(V8PsamDevice)getPsamDevice();
        psamDevice.setError("9000","OK");
        V8RFCPUDevice userCardDevice=(V8RFCPUDevice)getRfcpuDevice();
        userCardDevice.setError("9000","OK");

        if(!openPSAMCard())
        {
            setError(POSDevice.EC_NORESPONSE,"PSAM卡打开失败");
            return null;
        }
        byte[] cardNum=((CityTourCard)userCard).getFCIValidData().getCardNo();
        byte[] verifiedCode=((CityTourCard)userCard).getVerifyCode();

        //SAM卡选1001目录
        String apduPSAMSel001="00A40000021001";
        byte[] ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPSAMSel001));
        Log.d(tag,"Sel1001:apdu="+apduPSAMSel001+" Result="+HexUtil.bytesToHexString(ret));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("SAM卡选1001目录失败:%s",apduPSAMSel001));
            return null;
        }
        String errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            psamDevice.setError(errorCode,String.format("ErrorCode:%s SAM卡选1001目录失败:APDU:%s Response:%s",errorCode,apduPSAMSel001,HexUtil.bytesToHexString(ret)));
            return null;
        }
        //读取密码版本号
        String apduPSAMSel0017="00B0970001";
        ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPSAMSel0017));
        Log.d(tag,"Sel0017:apdu="+apduPSAMSel0017+" Result="+HexUtil.bytesToHexString(ret));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("读取密码版本号失败:%s",apduPSAMSel0017));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            psamDevice.setError(errorCode,String.format("读取密码版本号失败:APDU:%s Response:%s",apduPSAMSel0017,HexUtil.bytesToHexString(ret)));
            return null;
        }
        //PSAM密钥版本号
        byte psamKeyVer=ret[0];

        //用户卡余额
        String apduUserCard="805C000204";
        ret=userCardDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCard));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("读取用户卡余额失败:%s",apduUserCard));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            userCardDevice.setError(errorCode,String.format("读取读取用户卡余额失败:APDU:%s Response:%s",apduUserCard,HexUtil.bytesToHexString(ret)));
            return null;
        }
        byte[] balance=CPUDevice.getResponseData(ret);
        Log.d(tag,String.format("卡余额:apdu:%s Result:%s balance:%d",apduUserCard,HexUtil.bytesToHexString(balance),
            Converter.BytesToLong(balance)));

        //SAM卡选1001目录
        String apduPSAM="00A40000021001";
        ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPSAM));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("SAM卡选1001目录失败:%s",apduPSAM));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            psamDevice.setError(errorCode,String.format("SAM卡选1001目录失败:APDU:%s Response:%s",apduPSAM,HexUtil.bytesToHexString(ret)));
            return null;
        }
        Log.d(tag,"SAM卡选1001目录:apdu="+apduPSAM+" Result="+HexUtil.bytesToHexString(ret));

        //用户卡取随机数
        String apudUserCardRand="0084000008";
        ret=userCardDevice.sendAPDU(HexUtil.hexStringToByte(apudUserCardRand));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("用户卡取随机数失败:%s",apudUserCardRand));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            userCardDevice.setError(errorCode,String.format("用户卡取随机数失败:APDU:%s Response:%s",apudUserCardRand,HexUtil.bytesToHexString(ret)));
            return null;
        }
        byte[] rand=CPUDevice.getResponseData(ret);
        Log.d(tag,"User Card Rand:apdu="+apudUserCardRand+" Result:"+HexUtil.bytesToHexString(rand));

        //Sam卡 801A分散
        String apdu801A="801A450110"
                +HexUtil.bytesToHexString(Converter.byteReversion(((CityTourCard)userCard).getFCIValidData().getCardNo()))
                +"2000FF0000000000";
        ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(apdu801A));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("Sam卡 801A分散失败:%s",apdu801A));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            psamDevice.setError(errorCode,String.format("Sam卡 801A分散失败:APDU:%s Response:%s",apdu801A,HexUtil.bytesToHexString(ret)));
            return null;
        }
        Log.d(tag,"Sam卡 801A分散 apdu="+apdu801A+"Result:"+HexUtil.bytesToHexString(ret));

        //SAM卡80fa
        String apduPSAM80fa="80FA000008"+HexUtil.bytesToHexString(rand) ;
        ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPSAM80fa));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("SAM卡80fa失败:%s",apduPSAM80fa));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            psamDevice.setError(errorCode,String.format("SAM卡80fa失败:APDU:%s Response:%s",apduPSAM80fa,HexUtil.bytesToHexString(ret)));
            return null;
        }
        byte[] mac=CPUDevice.getResponseData(ret);
        Log.d(tag,"SAM卡80fa adpu:"+apduPSAM80fa+" mac:"+HexUtil.bytesToHexString(mac));

        //用户卡外部认证
        String apduUserCardExterl="0082000208"+HexUtil.bytesToHexString(mac);
        ret=userCardDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardExterl));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("用户卡外部认证失败:%s",apduUserCardExterl));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            psamDevice.setError(errorCode,String.format("用户卡外部认证失败:APDU:%s Response:%s",apduUserCardExterl,HexUtil.bytesToHexString(ret)));
            return null;
        }
        Log.d(tag,"用户卡外部认证 apdu:"+apduUserCardExterl+" Result:"+HexUtil.bytesToHexString(ret));

        SHTCPsamCard psamCard=(SHTCPsamCard)psamDevice.getPsamCard();
        CityTourCard currCard=(CityTourCard)userCard;

        //PSAM卡送认证码
        String apduCardAuth=psamCard.apduUserCode(verifiedCode);
        ret=psamDevice.sendAPDU(HexUtil.hexStringToByte(apduCardAuth));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("PSAM卡送认证码失败:%s",apduCardAuth));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            psamDevice.setError(errorCode,String.format("PSAM卡送认证码失败:APDU:%s Response:%s",apduCardAuth,HexUtil.bytesToHexString(ret)));
            return null;
        }
        Log.d("Debit","认证指令："+apduCardAuth+" Result:"+HexUtil.bytesToHexString(ret));

        //用户卡开始交易
        String apduUserCardTxnBegin="805003020B"+psamCard.getKeyVersion()+HexUtil.bytesToHexString(Converter.IntToBytes(amount))
                +psamCard.getPosNo();
        ret=userCardDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardTxnBegin));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("用户卡开始交易失败:%s",apduUserCardTxnBegin));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            userCardDevice.setError(errorCode,String.format("用户卡开始交易失败:APDU:%s Response:%s",apduUserCardTxnBegin,HexUtil.bytesToHexString(ret)));
            return null;
        }
        Log.d(tag,"用户卡开始交易：apdu:"+apduUserCardTxnBegin+" Result:"+HexUtil.bytesToHexString(ret));
        //解析卡结果
        balance=new byte[4];
        int pos=0;
        System.arraycopy(ret,pos,balance,0,4);
        pos+=4;
        ((CityTourCard) userCard).setByteBalance(balance);
        Log.d(tag,String.format("apdu:%s result:%s 卡余额：%d",apduUserCardTxnBegin,HexUtil.bytesToHexString(ret),
                Converter.BytesToLong(balance)));
        byte[] cardSeq=new byte[2];
        System.arraycopy(ret,pos,cardSeq,0,2);
        pos+=2;
        //透支金额
        byte[] limit=new byte[3];
        System.arraycopy(ret,pos,limit,0,3);
        pos+=3;

        //密钥算法
        byte[] alg=new byte[2];
        System.arraycopy(ret,pos,alg,0,2);
        pos+=2;

        //用户卡随机数
        rand=new byte[4];
        System.arraycopy(ret,pos,rand,0,4);
        //PSAM开始交易

        Date date=new Date();
        SimpleDateFormat formatter = new SimpleDateFormat("yyyyMMddHHmmss");
        String dateString = formatter.format(date);
        Log.d(tag,"交易日期："+dateString);

/*
        int posSeq=getPosSequence();
        setPosSequence(posSeq++);
*/
        Log.d(tag,String.format("Card num is %s",HexUtil.bytesToHexString(cardNum)));

        boolean isBreakTxn=false;
        byte[] mac2=new byte[4];
        byte[] tac=new byte[4];
        byte[] posSeq1 = new byte[4];
        //检查中断交易
        if(oldBalance==-1){
            //记录当前余额
            oldBalance=(int)Converter.BytesToLong(balance);
        }else{
            //检查是否扣款了
            int currBalance=(int)Converter.BytesToLong(balance);
            if(oldBalance!=currBalance){
                String apdu="00A4040009" +
                        "A00000000386980701" +
                        "00";
                //取交易认证码
                apdu="805A000602"+HexUtil.bytesToHexString(cardSeq);
                ret=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(apdu));
                if(ret==null){
                    setError(POSDevice.EC_NORESPONSE,String.format("取交易认证码失败:%s",apdu));
                    return null;
                }
                errorCode=CPUDevice.parseResponse(ret);
                if(errorCode.compareTo("9000")!=0) {
                    psamDevice.setError(errorCode,String.format("取交易认证码失败:APDU:%s Response:%s",apdu,HexUtil.bytesToHexString(ret)));
                    return null;
                }

                System.arraycopy(ret,0,mac2,0,4);
                System.arraycopy(ret,4,tac,0,4);
                isBreakTxn=true;
            }
        }

        if(!isBreakTxn) {
            //SAM卡交易开始
            String apduPsamCardBegin = "8070000024"
                    + HexUtil.bytesToHexString(rand)
                    + HexUtil.bytesToHexString(cardSeq)
                    + HexUtil.bytesToHexString(Converter.IntToBytes(amount))
                    + "09"
                    + dateString
                    + HexUtil.bytesToHexString(alg)
                    //+HexUtil.bytesToHexString(Converter.IntToBytes(posSeq))
                    + HexUtil.bytesToHexString(cardNum)
                    + "2000"
                    + "FF0000000000";
            Log.d(tag, String.format("Begin PSAM txn apdu=%s", apduPsamCardBegin));
            ret = getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduPsamCardBegin));
            if (ret == null) {
                setError(POSDevice.EC_NORESPONSE, String.format("SAM卡交易开始失败:%s", apduPsamCardBegin));
                return null;
            }
            errorCode = CPUDevice.parseResponse(ret);
            if (errorCode.compareTo("9000") != 0) {
                psamDevice.setError(errorCode, String.format("SAM卡交易开始失败:APDU:%s Response:%s", apduPsamCardBegin, HexUtil.bytesToHexString(ret)));
                return null;
            }
            Log.d(tag, "PSAM卡开始交易 apdu:" + apduPsamCardBegin + " Result:" + HexUtil.bytesToHexString(ret));
            System.arraycopy(ret, 0, posSeq1, 0, 4);
            byte[] mac1 = new byte[4];
            System.arraycopy(ret, 4, mac1, 0, 4);
            int balanceafter = (int) Converter.BytesToLong(balance) - amount;
            Log.d(tag, String.format("交易后余额：%06X,%d", balanceafter, balanceafter));
            //用户卡更新复合消费缓存
            String txnData = getPosID().substring(4)
                    + String.format("%04X", amount)
                    + String.format("%06X", balanceafter)
                    + String.format("%02X", Converter.IntToBytes(amount)[0])
                    + SHTCCPUUserCard.dateInCache(date) + "00";
            Log.d(tag, String.format("交易数据(%d)：%s", txnData.length(), txnData));
            String apduUserCardUpdateCache = "80DC003B10" + txnData;
            ret = userCardDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardUpdateCache));
            if (ret == null) {
                setError(POSDevice.EC_NORESPONSE, String.format("用户卡更新复合消费缓存失败:%s", apduUserCardUpdateCache));
                return null;
            }
            errorCode = CPUDevice.parseResponse(ret);
            if (errorCode.compareTo("9000") != 0) {
                psamDevice.setError(errorCode, String.format("用户卡更新复合消费缓存失败:APDU:%s Response:%s", apduUserCardUpdateCache, HexUtil.bytesToHexString(ret)));
                return null;
            }
            Log.d(tag, "用户卡更新复合消费缓存 apdu:" + apduUserCardUpdateCache + " Result:" + HexUtil.bytesToHexString(ret));

            //用户卡交易完成
            String apduUserCardEnd = "805401000F" + HexUtil.bytesToHexString(posSeq1)
                    + dateString + HexUtil.bytesToHexString(mac1);
            Log.d(tag, String.format("用户卡交易完成，apdu=%s", apduUserCardEnd));
            ret = userCardDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardEnd));
            if (ret == null) {
                setError(POSDevice.EC_NORESPONSE, String.format("用户卡交易完成失败:%s", apduUserCardEnd));
                return null;
            }
            errorCode = CPUDevice.parseResponse(ret);
            if (errorCode.compareTo("9000") != 0) {
                setError(POSDevice.EC_TXNFAILURE, String.format("用户卡交易完成失败:%s(%s)", apduUserCardEnd, errorCode));
                getRfcpuDevice().setError(errorCode, String.format("用户卡更新复合消费缓存失败:APDU:%s Response:%s", apduUserCardUpdateCache, HexUtil.bytesToHexString(ret)));
                return null;
            }
            Log.d(tag, "用户卡交易完成 apdu:" + apduUserCardEnd + " Result:" + HexUtil.bytesToHexString(ret));
            System.arraycopy(ret, 0, tac, 0, 4);
            System.arraycopy(ret, 4, mac2, 0, 4);
        }

        //PSAM卡完成交易
        String apduPsamCardEnd="8072000004"+HexUtil.bytesToHexString(mac2);
        ret=getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduPsamCardEnd));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("PSAM卡完成交易失败:%s",apduPsamCardEnd));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            setError(POSDevice.EC_TXNFAILURE,String.format("PSAM卡完成交易失败:%s(%s)",apduPsamCardEnd,errorCode));
            getRfcpuDevice().setError(errorCode,String.format("PSAM卡完成交易失败:APDU:%s Response:%s",apduPsamCardEnd,HexUtil.bytesToHexString(ret)));
            return null;
        }
        Log.d(tag,"PSAM卡完成交易 apdu:"+apduPsamCardEnd+" Result:"+HexUtil.bytesToHexString(ret));
        //保存交易
        DebitRecord debitRecord=new DebitRecord(getCorpCode(),
                0,
                (byte) 0x09,
                getStationID(),
                POSApplication.instance.getUser().getUsername(),
                "0",//无公交车编号
                getTxnType(),
                (int)Converter.BytesToLong(posSeq1),
                HexUtil.bytesToHexString(currCard.getFCIValidData().getCityCode()),
                HexUtil.bytesToHexString(currCard.getFCIValidData().getCardNo()),
                currCard.getFCIValidData().getCardType(),Converter.BytesToLong(balance),
                amount,dateString,
                (int)Converter.BytesToLong(cardSeq),psamCard.getPosNo(),
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
}
