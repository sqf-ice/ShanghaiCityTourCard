package com.myhand.devices;

import com.centerm.smartpos.aidl.printer.AidlPrinter;
import com.centerm.smartpos.aidl.printer.PrintDataObject;
import com.centerm.smartpos.util.HexUtil;
import com.myhand.common.Converter;
import com.myhand.cpucard.DebitRecord;
import com.myhand.cpucard.SHTCCPUUserCard;
import com.myhand.manage.SettleSum;
import com.myhand.POS.POSApplication;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by wenha_000 on 2017-09-28.
 */

public class V8Printer extends Printer{
    private AidlPrinter printDev = null;

    public AidlPrinter getPrintDev() {
        return printDev;
    }

    public void setPrintDev(AidlPrinter printDev) {
        this.printDev = printDev;
    }

    // 打印机回调对象
    public static List<PrintDataObject> printPayNote(DebitRecord debitRecord){
        POSDevice posDevice=POSApplication.instance.getPosDevice();
        List<PrintDataObject> result=new ArrayList<PrintDataObject>();

        result.add(new PrintDataObject("欢  迎  使  用", 40,true
                , PrintDataObject.ALIGN.CENTER));
        result.add(new PrintDataObject("上海都市旅游卡", 40,true
                , PrintDataObject.ALIGN.CENTER));
        result.add(new PrintDataObject("-------------------------------", 24,
                true, PrintDataObject.ALIGN.CENTER));
        result.add(new PrintDataObject("商户联", 34,
                true, PrintDataObject.ALIGN.RIGHT));
        result.add(new PrintDataObject(posDevice.getCorpChinesename(), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("行业代码：%d",posDevice.getTradeCode()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("终端号：%s",debitRecord.getPosID()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("公司号：%s",debitRecord.getCorpID()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("-------------------------------", 24,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("小额消费", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("卡号：U%s",
                SHTCCPUUserCard.CardFaceNum(HexUtil.hexStringToByte(debitRecord.getCardFaceNum().substring(8)))),
                24,true, PrintDataObject.ALIGN.LEFT));

        result.add(new PrintDataObject(String.format("时间：%s", Converter.dateFormatConvert(debitRecord.getTxnTime())),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("批次：%d流水：%d",debitRecord.getLocalTxnSeq(),debitRecord.getLocalTxnSeq()),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("有效期：%s",Converter.dateFormatConvert(debitRecord.getTxnTime())),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("消费前余额：%.2f",(float)debitRecord.getBalanceBef()/100.0),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("消费金额：%.2f",(float)debitRecord.getAmount()/100.0),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("消费后余额：%.2f",(float)(debitRecord.getBalanceBef()-debitRecord.getAmount())/100.0),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("-------------------------------", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("MO14-GP", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));

        //客户联
        result.add(new PrintDataObject("欢  迎  使  用", 40,true
                , PrintDataObject.ALIGN.CENTER));
        result.add(new PrintDataObject("上海都市旅游卡", 40,true
                , PrintDataObject.ALIGN.CENTER));
        result.add(new PrintDataObject("-------------------------------", 24,
                true, PrintDataObject.ALIGN.CENTER));
        result.add(new PrintDataObject("持卡人联", 34,
                true, PrintDataObject.ALIGN.RIGHT));
        result.add(new PrintDataObject(posDevice.getCorpChinesename(), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("行业代码：%d",posDevice.getTradeCode()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("终端号：%s",debitRecord.getPosID()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("公司号：%s",debitRecord.getCorpID()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("-------------------------------", 24,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("小额消费", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("卡号：U%s",
                SHTCCPUUserCard.CardFaceNum(HexUtil.hexStringToByte(debitRecord.getCardFaceNum().substring(8)))),
                24,true, PrintDataObject.ALIGN.LEFT));

        result.add(new PrintDataObject(String.format("时间：%s", Converter.dateFormatConvert(debitRecord.getTxnTime())),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("批次：%d流水：%d",debitRecord.getLocalTxnSeq(),debitRecord.getLocalTxnSeq()),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("有效期：%s",Converter.dateFormatConvert(debitRecord.getTxnTime())),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("消费前余额：%.2f",(float)debitRecord.getBalanceBef()/100.0),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("消费金额：%.2f",(float)debitRecord.getAmount()/100.0),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("消费后余额：%.2f",(float)(debitRecord.getBalanceBef()-debitRecord.getAmount())/100.0),
                24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("-------------------------------", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("MO14-GP", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        return result;
    }

    public static List<PrintDataObject> printSettleNote(SettleSum settleSum){
        List<PrintDataObject> result=new ArrayList<PrintDataObject>();
        result.add(new PrintDataObject("上海都市旅游卡业务结算单", 40,true
                , PrintDataObject.ALIGN.CENTER));
        result.add(new PrintDataObject("-------------------------------", 24,
                true, PrintDataObject.ALIGN.CENTER));
        POSDevice posDevice=POSApplication.instance.getPosDevice();
        result.add(new PrintDataObject(posDevice.getCorpChinesename(), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("行业代码：%d",posDevice.getTradeCode()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("终端号：%s",settleSum.getPosID()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("公司号：%s",posDevice.getCorpCode()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("批次号：%d",posDevice.getPatchNo()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("消费总笔数：%d",settleSum.getCount()), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("消费总金额：%.2f",(float)settleSum.getSum()/100f), 30,
                true, PrintDataObject.ALIGN.LEFT));
        SimpleDateFormat sdf=new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        //测试用
        if(settleSum.getStartTime()==null){
            settleSum.setStartTime(new Date());
        }
        if(settleSum.getEndTime()==null){
            settleSum.setEndTime(new Date());
        }
        result.add(new PrintDataObject(String.format("起始：%s",sdf.format(settleSum.getStartTime())), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject(String.format("结束：%s",sdf.format(settleSum.getEndTime())), 30,
                true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("-------------------------------", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("MO14-GP", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));
        result.add(new PrintDataObject("", 24,true, PrintDataObject.ALIGN.LEFT));

        return result;
    }
}
