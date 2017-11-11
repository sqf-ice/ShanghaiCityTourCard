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
        dataFCI.setFileData(CPUDevice.getResponseData(result));
        card.setDataFCI(dataFCI);

        //取卡余额
        result=getRfcpuDevice().sendAPDU(HexUtil.hexStringToByte(card.APDUFETCHBALANCE));
        byte[] balance=CPUDevice.getResponseData(result);
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
        byte[] verifiedCode=readCard().getVerifyCode();

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
        Log.d(tag,"卡余额:apdu="+apduUserCard+" Result="+HexUtil.bytesToHexString(balance));

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

/*
        if(true) {
            //报告错误：
            userCardDevice.setErrorMessage(String.format("卡外部认证结果：%s", HexUtil.bytesToHexString(ret)));
            return null;
        }
*/

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
        //SAM卡交易开始
        String apduPsamCardBegin="8070000024"
                +HexUtil.bytesToHexString(rand)
                +HexUtil.bytesToHexString(cardSeq)
                +HexUtil.bytesToHexString(Converter.IntToBytes(amount))
                +"09"
                +dateString
                +HexUtil.bytesToHexString(alg)
                //+HexUtil.bytesToHexString(Converter.IntToBytes(posSeq))
                +HexUtil.bytesToHexString(cardNum)
                +"2000"
                +"FF0000000000";
        Log.d(tag,String.format("Begin PSAM txn apdu=%s",apduPsamCardBegin));
        ret=getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduPsamCardBegin));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("SAM卡交易开始失败:%s",apduPsamCardBegin));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            psamDevice.setError(errorCode,String.format("SAM卡交易开始失败:APDU:%s Response:%s",apduPsamCardBegin,HexUtil.bytesToHexString(ret)));
            return null;
        }
        Log.d(tag,"PSAM卡开始交易 apdu:"+apduPsamCardBegin+" Result:"+HexUtil.bytesToHexString(ret));
        byte[] posSeq1=new byte[4];
        System.arraycopy(ret,0,posSeq1,0,4);
        byte[] mac1=new byte[4];
        System.arraycopy(ret,4,mac1,0,4);

        //用户卡更新复合消费缓存
        String txnData="01020304050607080910111213141516";
        String apduUserCardUpdateCache="80DC003B10"+txnData;
        ret=userCardDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardUpdateCache));
        if(ret==null){
            setError(POSDevice.EC_NORESPONSE,String.format("用户卡更新复合消费缓存失败:%s",apduUserCardUpdateCache));
            return null;
        }
        errorCode=CPUDevice.parseResponse(ret);
        if(errorCode.compareTo("9000")!=0) {
            psamDevice.setError(errorCode,String.format("用户卡更新复合消费缓存失败:APDU:%s Response:%s",apduUserCardUpdateCache,HexUtil.bytesToHexString(ret)));
            return null;
        }
        Log.d(tag,"用户卡更新复合消费缓存 apdu:"+apduUserCardUpdateCache+" Result:"+HexUtil.bytesToHexString(ret));

        //用户卡交易完成
        String apduUserCardEnd="805401000F"+HexUtil.bytesToHexString(posSeq1)
                +dateString+HexUtil.bytesToHexString(mac1);
        Log.d(tag,String.format("用户卡交易完成，apdu=%s",apduUserCardEnd));
        ret=userCardDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardEnd));
        if(ret==null)
        {
            String msg="用户卡交易完成失败";
            Log.d("Debit",msg);
            setErrorMessage(msg);
            return  null;
        }
        Log.d(tag,"用户卡交易完成 apdu:"+apduUserCardEnd+" Result:"+HexUtil.bytesToHexString(ret));
        byte[] tac=new byte[4];
        System.arraycopy(ret,0,tac,0,4);
        byte[] mac2=new byte[4];
        System.arraycopy(ret,4,mac2,0,4);
        //PSAM卡完成交易
        String apduPsamCardEnd="8072000004"+HexUtil.bytesToHexString(mac2);
        ret=getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduPsamCardEnd));
        Log.d(tag,"PSAM卡完成交易 apdu:"+apduPsamCardEnd+" Result:"+HexUtil.bytesToHexString(ret));


        //保存交易
        DebitRecord debitRecord=new DebitRecord(getCorpCode(),
                0,
                (byte) 0x09,
                getStationID(),
                getUser().getUserID(),
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
