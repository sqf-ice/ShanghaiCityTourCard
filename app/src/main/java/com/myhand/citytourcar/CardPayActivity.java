package com.myhand.citytourcar;

import android.app.AlertDialog;
import android.app.Fragment;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.RemoteException;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.centerm.smartpos.aidl.printer.AidlPrinter;
import com.centerm.smartpos.aidl.psam.AidlPsam;
import com.centerm.smartpos.aidl.rfcard.AidlRFCard;
import com.centerm.smartpos.aidl.soundplayer.AidlSoundPlayer;
import com.centerm.smartpos.aidl.sys.AidlDeviceManager;
import com.centerm.smartpos.constant.Constant;
import com.centerm.smartpos.util.HexUtil;
import com.myhand.POS.DatabaseSHCT;
import com.myhand.common.Converter;
import com.myhand.common.SelectActivity;
import com.myhand.cpucard.DebitRecord;
import com.myhand.cpucard.SHTCCPUUserCard;
import com.myhand.cpucard.SHTCPsamCard;
import com.myhand.devices.CPUDevice;
import com.myhand.devices.DeviceV8;
import com.myhand.devices.POSDevice;
import com.myhand.devices.PSAMDevice;
import com.myhand.devices.RFCPUDevice;
import com.myhand.devices.V8Printer;
import com.myhand.devices.V8PsamDevice;
import com.myhand.devices.V8RFCPUDevice;
import com.myhand.devices.V8Sounder;
import com.myhand.shanghaicitytourcard.BaseTourCardActivity;
import com.myhand.shanghaicitytourcard.CPUFileDataFCI;
import com.myhand.shanghaicitytourcard.CityTourCard;
import com.myhand.POS.POSApplication;
import com.myhand.shanghaicitytourcard.PayFragment;
import com.myhand.shanghaicitytourcard.PrintActivity;
import com.myhand.shanghaicitytourcard.R;

import java.text.SimpleDateFormat;
import java.util.Date;

public class CardPayActivity extends BaseTourCardActivity
implements ReadCardFragment.OnFragmentInteractionListener,
        CardPayFragment.OnFragmentInteractionListener,
        ConfirmFragment.OnFragmentInteractionListener {
    private static final String tag = CardPayActivity.class.getSimpleName();

    public static final int CONFIRM_OK = 1;
    public static final int CONFIRM_CANCEL = 0;

    public static final int FRAG_PAYINIT = 0;
    public static final int FRAG_PAYAMOUNT = 1;
    public static final int FRAG_PAYCONFIRM = 2;

    private static final int SEQ_PRINTNOTE = 1;
    //本机对应设备
    public POSDevice posDevice;

    public CityTourCard payCard = null;
    public int payAmount = 0;
    private DebitRecord debitRecord = null;

    private FrameLayout frameLayoutFragment;
    private CardPayFragment cardPayFragment;
    private ReadCardFragment readCardFragment;
    private ConfirmFragment confirmFragment;

    private PayFragment currFragment = null;

    //
    private String cardApdu;
    private String psamApdu;

    private byte[] cardRet;
    private byte[] psamRet;

    private static final int STATUS_IDLE = 0;
    private static final int STATUS_WORKING = 1;
    private static final int STATUS_OK = 2;
    private static final int STATUS_FAILURE = 3;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_card_pay);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });

        initFragment();

        POSApplication.instance.getPosDevice().setContext(this);
        posDevice = POSApplication.instance.getPosDevice();
        POSApplication.instance.getPosDevice().setHandler(handler);
        POSApplication.instance.getPosDevice().setActivity(this);
        //开始读卡
        readCard();
    }

    private boolean selectCard3F00() {
        String apdu = "00A40000023F00";
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(apdu));
        if (cardRet != null) {
            Log.d(tag, String.format("选择用户卡3F00%s", HexUtil.bytesToHexString(cardRet)));
        } else {
            Log.d(tag, String.format("选择用户卡3f00文件失败"));
            readCardFragment.ShowErrorMessage(0, "请再刷一次");
            loadFragment(FRAG_PAYINIT);
            return false;
        }
        return true;
    }

    private void initFragment() {
        frameLayoutFragment = (FrameLayout) findViewById(R.id.frameLayoutFragment);
        readCardFragment = ReadCardFragment.newInstance("", "");
        cardPayFragment = CardPayFragment.newInstance("", "");
        confirmFragment = ConfirmFragment.newInstance("", "");
    }

    @Override
    public void onFragmentInteraction(Uri uri) {
        //Toast.makeText(this,"交流,角楼",Toast.LENGTH_LONG).show();
    }

    @Override
    public void onDeviceConnected(AidlDeviceManager deviceManager) {
        POSDevice posDevice = POSApplication.instance.getPosDevice();
        try {
            //声音模块
            V8Sounder soundPlayer = (V8Sounder) posDevice.getSounder();
            if (soundPlayer == null) {
                soundPlayer = new V8Sounder();
            }
            soundPlayer.setSoundPlayer(AidlSoundPlayer.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_SOUNDPLAYER)));
            posDevice.setSounder(soundPlayer);

            //打印模块
            V8Printer printer = (V8Printer) posDevice.getPrinter();
            if (printer == null) {
                printer = new V8Printer();
            }
            printer.setPrintDev(AidlPrinter.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PRINTERDEV)));
            posDevice.setPrinter(printer);

            //PSAM卡模块
            AidlPsam psam[] = new AidlPsam[]{
                    AidlPsam.Stub.asInterface(deviceManager
                            .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM1)),
                    AidlPsam.Stub.asInterface(deviceManager
                            .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM2)),
                    AidlPsam.Stub.asInterface(deviceManager
                            .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM3))};

            if (posDevice.getPsamDevices() == null) {
                posDevice.setPsamDevices(new V8PsamDevice[]{new V8PsamDevice(psam[0]), new V8PsamDevice(psam[1]), new V8PsamDevice(psam[2])});
            } else {
                //更新POS设备
                for (int i = 0; i < 3; i++) {
                    PSAMDevice psamDevice = posDevice.getPsamDevices()[i];
                    if (psamDevice == null) {
                        psamDevice = new V8PsamDevice(psam[i]);
                    }

                }
                //默认第一个PSAM卡设备
                posDevice.setCurrPsamDevice(posDevice.getPsamDevices()[0]);
            }

            //非接设备
            V8RFCPUDevice v8RFCPUDevice = new V8RFCPUDevice();
            v8RFCPUDevice.setRfCard(AidlRFCard.Stub.asInterface(deviceManager.getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_RFCARD)));
            posDevice.setRfcpuDevice(v8RFCPUDevice);

            //soundPlayer.activationSuccess();
        } catch (RemoteException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            Toast.makeText(this, "非接设备连接失败：" + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }

    public void showCardInfo(CityTourCard card) {
/*
        if(card==null)
        {
            mCardATR.setText("无卡");
            return;
        }

        mCardATR.setText(HexUtil.bytesToHexString(card.getAtr()));

        txtViewCardNO.setText(HexUtil.bytesToHexString(card.getFCIValidData().getCardNo()));
        txtViewVerifyCode.setText(HexUtil.bytesToHexString(card.getVerifyCode()));
        txtViewCardBalance.setText(String.format("%1$d分(%2$+.2f元)",card.getBalance(),(float)card.getBalance()/100.0));
        txtViewStartDate.setText(HexUtil.bytesToHexString(card.getFCIValidData().getStartDate()));
        txtViewValidDate.setText(HexUtil.bytesToHexString(card.getFCIValidData().getValidDate()));
*/
    }

    private void loadFragment(int index) {
        FragmentManager fragmentManager = getFragmentManager();
        FragmentTransaction transaction = fragmentManager.beginTransaction();
        //Fragment fragment=readCardFragment;
        switch (index) {
            case FRAG_PAYINIT: {
                if (currFragment != readCardFragment) {
                    currFragment = readCardFragment;
                } else {
                    return;
                }
                break;
            }
            case FRAG_PAYAMOUNT: {
                if (currFragment != cardPayFragment) {
                    currFragment = cardPayFragment;
                } else {
                    return;
                }
                break;
            }
            case FRAG_PAYCONFIRM: {
                if (currFragment != confirmFragment) {
                    currFragment = confirmFragment;
                } else {
                    return;
                }
                break;
            }
        }
        transaction.replace(R.id.frameLayoutFragment, currFragment).commit();
    }

/*
    private class ThreadReadCard extends Thread{
        public ThreadReadCard(){

        }
        @Override
        public void run() {
            super.run();
            loadFragment(0);

            //等待设备连接
            while (true){
                if(posDevice==null
                        ||posDevice.getRfcpuDevice()==null){
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    continue;
                }
                break;
            }

            //开始读卡
            while (true){
                card=posDevice.readCard();
                if(card!=null){
                    break;
                }
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            cardPayFragment.showBalance(card.getBalance());
            cardPayFragment.ShowErrorMessage(0,String.format("卡类型：%02X，有效期：%s",
                    card.getTypeIn05(),card.getFCIValidData().getValidDate()));
            loadFragment(1);
        }
    };
*/

    private void readCard() {
        loadFragment(FRAG_PAYINIT);
/*
        V8Sounder sounder=(V8Sounder) POSApplication.instance.getPosDevice().getSounder();
        if(sounder!=null){
            sounder.swipeCard();
        }
*/
        new Thread(new Runnable() {
            @Override
            public void run() {
                RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();
                rfcpuDevice.open();
                cardRet = rfcpuDevice.reset();
                payCard = new CityTourCard(cardRet, 0);

                //选择用户卡3F00文件
                while (!selectCard3F00()) {
                    //选择3F00失败时重启rf设备
                    rfcpuDevice.close();
                    rfcpuDevice.open();
                    rfcpuDevice.reset();

                    loadFragment(FRAG_PAYINIT);
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    readCardFragment.ShowErrorMessage(1, "无效卡");
                    loadFragment(FRAG_PAYINIT);
                }

                //读取卡类型
                while (!readCard05()) {
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    readCardFragment.ShowErrorMessage(1, "无效卡");
                    loadFragment(FRAG_PAYINIT);
                }
                //读取用户卡CFI数据
                while (!readCardFCIData()) {
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    readCardFragment.ShowErrorMessage(1, "无效卡");
                    loadFragment(FRAG_PAYINIT);
                }

/*
                //读取卡余额
                while (!readBalance()) {
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    readCardFragment.ShowErrorMessage(1, "无效卡");
                    loadFragment(FRAG_PAYINIT);
                }
*/
                loadFragment(FRAG_PAYAMOUNT);
                //new ThreadReadCard().start();
            }
        }).start();
    }

    //读取05文件获取卡类型
    private boolean readCard05() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();
        //读取0x05文件
        String apduRd05 = "00B0850020";
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(apduRd05));
        if (cardRet == null) {
            Log.d(tag, String.format("读取0x05文件没有回应：%s", apduRd05));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(cardRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("读取0x05文件失败 apdu:%s result:%s", apduRd05, cardRet));
            return false;
        }
        Log.d(tag, String.format("05文件：%s卡类型：%02X", HexUtil.bytesToHexString(cardRet), cardRet[8]));
        payCard.setTypeIn05(cardRet[8]);

        return true;
    }

    //读取用户卡FCI
    private boolean readCardFCIData() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();
        //读取FCI数据，上海交通卡规范
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(CityTourCard.APDUSelFCIDATA));
        if (cardRet == null) {
            Log.d(tag, String.format("读取FCI数据没有回应:%s", CityTourCard.APDUSelFCIDATA));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(cardRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("读取FCI数据没有回应:%s", CityTourCard.APDUSelFCIDATA));
            return false;
        }

        //设置FCI数据
        CPUFileDataFCI dataFCI = new CPUFileDataFCI();
        dataFCI.setFileData(CPUDevice.getResponseData(cardRet));
        payCard.setDataFCI(dataFCI);
        return true;
    }

    public Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case 1: {
                    POSApplication.instance.getPosDevice().showPayPrompt("无效卡", "请重新刷卡");
                    break;
                }
            }
        }
    };


    public class ThreadCardDebit extends Thread{
        private int debitAmount;

        public ThreadCardDebit(int debitAmount){
            this.debitAmount=debitAmount;
            //初始化POS
            ((DeviceV8)posDevice).setOldBalance(-1);
            ((DeviceV8)posDevice).setRedebit(0);
        }

        @Override
        public void run() {
            super.run();

            while (true) {
                //卡支付
                debitRecord = posDevice.complexDebit(payCard, debitAmount);
                if (debitRecord == null) {
                    if(posDevice.getErrorCode().compareTo(POSDevice.EC_INVALIDCARD)==0){
                        handler.post(new Runnable() {
                            @Override
                            public void run() {
                                cardPayFragment.ShowErrorMessage(1,"无效卡");
                            }
                        });
                    }else if(posDevice.getErrorCode().compareTo(POSDevice.EC_TXNFAILURE)==0){
                        V8Sounder sounder=(V8Sounder) POSApplication.instance.getPosDevice().getSounder();
                        if(sounder!=null){
                            sounder.communicateError();
                        }
                        handler.post(new Runnable() {
                            @Override
                            public void run() {
                                cardPayFragment.ShowErrorMessage(1, String.format("交易失败（%s）：%s",
                                        posDevice.getErrorCode(),posDevice.getErrorMessage()));
                            }
                        });

                        return;
                    }
                    //检查是否需要重新做交易
                    else if(posDevice.getErrorCode().compareTo(POSDevice.EC_NORESPONSE)==0){
                        handler.post(new Runnable() {
                            @Override
                            public void run() {
                                cardPayFragment.ShowErrorMessage(2, "交易重做......");
                            }
                        });


                        try {
                            Thread.sleep(10);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }


                        //重新读卡
                        while (true) {
                            payCard = posDevice.readCard();
                            if(payCard!=null){
                                break;
                            }
                        }
                        continue;
                    }

                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            cardPayFragment.ShowErrorMessage(1, posDevice.getErrorMessage());
                        }
                    });
                    return;
                }
                break;
            }
            V8Sounder sounder=(V8Sounder)POSApplication.instance.getPosDevice().getSounder();
            if(sounder!=null){
                sounder.transactionSuccess();
            }
            //打印支付凭证
            Intent intent=new Intent();
            intent.setClass(CardPayActivity.this,PrintActivity.class);
            intent.putExtra("debitRecord",debitRecord);
            startActivityForResult(intent,SEQ_PRINTNOTE);
        }
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case SEQ_PRINTNOTE: {
/*
                //重新读卡显示卡余额
                card=posDevice.readCard();
                if(card!=null){
                    cardPayFragment.showBalance(card.getBalance());
                    cardPayFragment.setPayAmount(0);
                }
*/
                break;
            }
            case 101: {
                POSApplication.instance.getPosDevice().setOK(true);
                break;
            }
        }
    }

    public void showPayPrompt() {
        //检查是否为黑卡
        DatabaseSHCT databaseSHCT = POSApplication.instance.getAppDatabase();

        String cardNo = payCard.getFaceNumber();
        Log.d(tag, String.format("检查卡%s是否是黑卡", cardNo));
        if (databaseSHCT.isBlackCard(cardNo)) {
            V8Sounder sounder = (V8Sounder) POSApplication.instance.getPosDevice().getSounder();
            if (sounder != null) {
                sounder.commonAmountReader(cardNo);
                sounder.insertCard();
            }
            cardPayFragment.ShowErrorMessage(1, String.format("黑卡，卡号%s", cardNo));
            return;
        }

        V8Sounder sounder = (V8Sounder) POSApplication.instance.getPosDevice().getSounder();
        if (sounder != null) {
            sounder.intelligenceAmountReader(String.format("%.2f", cardPayFragment.getPayAmount()));
        }


        final AlertDialog.Builder normalDialog =
                new AlertDialog.Builder(CardPayActivity.this);
        //normalDialog.setIcon(R.drawable.);
        normalDialog.setTitle("消费金额确认");
        normalDialog.setMessage(String.format("消费金额为%.2f元，是否继续?", cardPayFragment.getPayAmount()));
        normalDialog.setPositiveButton("确定",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        V8Sounder sounder=(V8Sounder)POSApplication.instance.getPosDevice().getSounder();
                        if(sounder!=null){
                            sounder.swipeWaveCard();
                        }
                        payAmount=(int)(cardPayFragment.getPayAmount()*100);
                        new ThreadCardDebit(payAmount).start();
                    }
                });
        normalDialog.setNegativeButton("取消",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        //...To-do
                    }
                });
        // 显示
        normalDialog.show();

    }

    public void printPayNote() {
        Intent intent = new Intent();
        intent.setClass(this, PrintActivity.class);
        startActivity(intent);
    }

    @Override
    protected void onResume() {
        super.onResume();
        POSApplication.instance.getPosDevice().setContext(this);
    }

    //读取卡余额
    public boolean readBalance() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();

        String apduUserCardTxnBegin = "805003020B01" + HexUtil.bytesToHexString(Converter.IntToBytes(0))
                + posDevice.getPosID();
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardTxnBegin));
        if (cardRet == null) {
            Log.d(tag, String.format("获取卡余额没有回应:%s", apduUserCardTxnBegin));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(cardRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("获取卡余额失败:APDU:%s Response:%s", apduUserCardTxnBegin, HexUtil.bytesToHexString(cardRet)));
            return false;
        }
        Log.d(tag, "获取卡余额：apdu:" + apduUserCardTxnBegin + " Result:" + HexUtil.bytesToHexString(cardRet));
        //解析卡结果
        byte[] balance = new byte[4];
        int pos = 0;
        System.arraycopy(cardRet, pos, balance, 0, 4);
        pos += 4;
        payCard.setByteBalance(balance);
        byte[] cardSeq = new byte[2];
        System.arraycopy(cardRet, pos, cardSeq, 0, 2);
        pos += 2;
        //透支金额
        byte[] limit = new byte[3];
        System.arraycopy(cardRet, pos, limit, 0, 3);
        pos += 3;

        //密钥算法
        byte[] alg = new byte[2];
        System.arraycopy(cardRet, pos, alg, 0, 2);
        pos += 2;

        //用户卡随机数
        byte[] rand = new byte[4];
        System.arraycopy(cardRet, pos, rand, 0, 4);

        return true;
    }

    //取卡认证码
    private boolean getCardVertifyCode() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();

        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(SHTCPsamCard.APDUUSERCODE));
        if (cardRet == null) {
            Log.d(tag, String.format("取认证码没有回应，apdu:%s", SHTCPsamCard.APDUUSERCODE));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(cardRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("取认证码失败，apdu:%s result:%s", SHTCPsamCard.APDUUSERCODE, HexUtil.bytesToHexString(cardRet)));
            return false;
        }

        payCard.setVerifyCode(CPUDevice.getResponseData(cardRet));
        return true;
    }

    public void cardDebit() {
        PSAMDevice psamDevice = POSApplication.instance.getPosDevice().getPsamDevice();
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();



        //用户卡取认证码
        Log.d(tag,"用户卡取认证码");
        while (!getCardVertifyCode()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        //打开PSAM卡设备
        Log.d(tag,"打开PSAM卡设备");
        while (!psamDevice.open()) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        psamDevice.reset();
        SHTCPsamCard psamCard=(SHTCPsamCard)psamDevice.getPsamCard();
        if(!psamCard.readPosNO(psamDevice)
            ||!psamCard.psamReadKeyVersion(psamDevice))
        {
            return;
        }

        Log.d(tag,"psamSelect1001");
        while (!psamSelect1001()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            return;
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"readPsamKeyVersion");
        while (!readPsamKeyVersion()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"psamSelect1001");
        while (!psamSelect1001()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"cardGetRand");
        while (!cardGetRand()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"psam801A");
        while (!psam801A()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"psam80FA");
        while (!psam80FA()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"cardExtAuth");
        while (!cardExtAuth()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"psamSendVerifyCode");
        while (!psamSendVerifyCode()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"cardBeginTxn");
        while (!cardBeginTxn()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            return;
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"psamBeginTxn");
        while (!psamBeginTxn()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"cardTxnEnd");
        while (!cardTxnEnd()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        Log.d(tag,"psamTxnEnd");
        while (!psamTxnEnd()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        while (!saveDebitRecord()) {
            readCardFragment.ShowErrorMessage(1, "交易失败");
            loadFragment(FRAG_PAYINIT);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        loadFragment(FRAG_PAYAMOUNT);

        printPayNote();
    }

    private boolean psamSelect1001() {
        PSAMDevice psamDevice = POSApplication.instance.getPosDevice().getPsamDevice();
        //SAM卡选1001目录
        String apduPSAMSel001 = "00A40000021001";
        psamRet = posDevice.getPsamDevice().sendAPDU(HexUtil.hexStringToByte(apduPSAMSel001));
        if (psamRet == null) {
            Log.d(tag, String.format("SAM卡选1001目录失败:%s", apduPSAMSel001));
            return false;
        }
        Log.d(tag, "Sel1001:apdu=" + apduPSAMSel001 + " Result=" + HexUtil.bytesToHexString(psamRet));
        String errorCode = CPUDevice.parseResponse(psamRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("ErrorCode:%s SAM卡选1001目录失败:APDU:%s Response:%s", errorCode, apduPSAMSel001, HexUtil.bytesToHexString(psamRet)));
            return false;
        }

        return true;
    }

    private boolean readPsamKeyVersion() {
        PSAMDevice psamDevice = POSApplication.instance.getPosDevice().getPsamDevice();
        //读取密码版本号
        String apduPSAMSel0017 = "00B0970001";
        psamRet = psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPSAMSel0017));
        if (psamRet == null) {
            Log.d(tag, String.format("读取密码版本号失败:%s", apduPSAMSel0017));
            return false;
        }
        Log.d(tag, "Sel0017:apdu=" + apduPSAMSel0017 + " Result=" + HexUtil.bytesToHexString(psamRet));
        String errorCode = CPUDevice.parseResponse(psamRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("读取密码版本号失败:APDU:%s Response:%s", apduPSAMSel0017, HexUtil.bytesToHexString(psamRet)));
            return false;
        }
        //PSAM密钥版本号
        byte psamKeyVer = psamRet[0];
        psamDevice.getPsamCard().setKeyVersion(psamKeyVer);
        return true;
    }

    private byte[] rand;

    private boolean cardGetRand() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();

        //用户卡取随机数
        String apudUserCardRand = "0084000008";
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(apudUserCardRand));
        if (cardRet == null) {
            Log.d(tag, String.format("用户卡取随机数失败:%s", apudUserCardRand));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(cardRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("用户卡取随机数失败:APDU:%s Response:%s", apudUserCardRand, HexUtil.bytesToHexString(cardRet)));
            return false;
        }
        rand = CPUDevice.getResponseData(cardRet);
        Log.d(tag, "User Card Rand:apdu=" + apudUserCardRand + " Result:" + HexUtil.bytesToHexString(rand));

        return true;
    }

    private boolean psam801A() {
        PSAMDevice psamDevice = POSApplication.instance.getPosDevice().getPsamDevice();
        //Sam卡 801A分散
        String apdu801A = "801A450110"
                + HexUtil.bytesToHexString(Converter.byteReversion(payCard.getFCIValidData().getCardNo()))
                + "2000FF0000000000";
        psamRet = psamDevice.sendAPDU(HexUtil.hexStringToByte(apdu801A));
        if (psamRet == null) {
            Log.d(tag, String.format("Sam卡 801A分散无回应:%s", apdu801A));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(psamRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("Sam卡 801A分散失败:APDU:%s Response:%s", apdu801A, HexUtil.bytesToHexString(psamRet)));
            return false;
        }
        Log.d(tag, "Sam卡 801A分散 apdu=" + apdu801A + "Result:" + HexUtil.bytesToHexString(psamRet));
        return true;
    }

    private byte[] mac;

    private boolean psam80FA() {
        PSAMDevice psamDevice = POSApplication.instance.getPosDevice().getPsamDevice();
        //SAM卡80fa
        String apduPSAM80fa = "80FA000008" + HexUtil.bytesToHexString(rand);
        psamRet = psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPSAM80fa));
        if (psamRet == null) {
            Log.d(tag, String.format("SAM卡80fa失败:%s", apduPSAM80fa));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(psamRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("SAM卡80fa失败:APDU:%s Response:%s", apduPSAM80fa, HexUtil.bytesToHexString(psamRet)));
            return false;
        }
        mac = CPUDevice.getResponseData(psamRet);
        Log.d(tag, "SAM卡80fa adpu:" + apduPSAM80fa + " mac:" + HexUtil.bytesToHexString(mac));
        return true;
    }

    private boolean cardExtAuth() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();
        //用户卡外部认证
        String apduUserCardExterl = "0082000208" + HexUtil.bytesToHexString(mac);
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardExterl));
        if (cardRet == null) {
            Log.d(tag, String.format("用户卡外部认证失败:%s", apduUserCardExterl));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(cardRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("用户卡外部认证失败:APDU:%s Response:%s", apduUserCardExterl, HexUtil.bytesToHexString(cardRet)));
            return false;
        }
        Log.d(tag, "用户卡外部认证 apdu:" + apduUserCardExterl + " Result:" + HexUtil.bytesToHexString(cardRet));
        return true;
    }

    private boolean psamSendVerifyCode() {
        PSAMDevice psamDevice = POSApplication.instance.getPosDevice().getPsamDevice();
        //PSAM卡送认证码
        String apduCardAuth = ((SHTCPsamCard) psamDevice.getPsamCard()).apduUserCode(payCard.getVerifyCode());
        psamRet = psamDevice.sendAPDU(HexUtil.hexStringToByte(apduCardAuth));
        if (psamRet == null) {
            Log.d(tag, String.format("PSAM卡送认证码失败:%s", apduCardAuth));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(psamRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("PSAM卡送认证码失败:APDU:%s Response:%s", apduCardAuth, HexUtil.bytesToHexString(psamRet)));
            return false;
        }
        Log.d(tag, "认证指令：" + apduCardAuth + " Result:" + HexUtil.bytesToHexString(psamRet));

        return true;
    }

    private boolean cardBeginTxn() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();
        PSAMDevice psamDevice = POSApplication.instance.getPosDevice().getPsamDevice();

        //用户卡开始交易
        String apduUserCardTxnBegin = "805003020B"
                + String.format("%02X",psamDevice.getPsamCard().getKeyVersion())
                + HexUtil.bytesToHexString(Converter.IntToBytes(payAmount))
                + HexUtil.bytesToHexString(psamDevice.getPsamCard().getPosID());
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardTxnBegin));
        if (cardRet == null) {
            Log.d(tag, String.format("用户卡开始交易失败:%s", apduUserCardTxnBegin));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(cardRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("用户卡开始交易失败:APDU:%s Response:%s", apduUserCardTxnBegin, HexUtil.bytesToHexString(cardRet)));
            return false;
        }
        Log.d(tag, "用户卡开始交易：apdu:" + apduUserCardTxnBegin + " Result:" + HexUtil.bytesToHexString(cardRet));
        //解析卡结果
        byte[] balance = new byte[4];
        int pos = 0;
        System.arraycopy(cardRet, pos, balance, 0, 4);
        pos += 4;
        payCard.setByteBalance(balance);
        Log.d(tag, String.format("apdu:%s result:%s 卡余额：%d", apduUserCardTxnBegin, HexUtil.bytesToHexString(cardRet),
                Converter.BytesToLong(balance)));
        byte[] cardSeq = new byte[2];
        System.arraycopy(cardRet, pos, cardSeq, 0, 2);
        payCard.setByteSequence(cardSeq);
        pos += 2;
        //透支金额
        byte[] limit = new byte[3];
        System.arraycopy(cardRet, pos, limit, 0, 3);
        pos += 3;

        //密钥算法
        byte[] alg = new byte[2];
        System.arraycopy(cardRet, pos, alg, 0, 2);
        payCard.setAlgBytes(alg);
        pos += 2;

        //用户卡随机数
        rand = new byte[4];
        System.arraycopy(cardRet, pos, rand, 0, 4);
        //PSAM开始交易

        return true;
    }

    private Date txnDate;
    private String txtDateStr;
    private boolean psamBeginTxn() {
        PSAMDevice psamDevice = POSApplication.instance.getPosDevice().getPsamDevice();
        txnDate = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMddHHmmss");
        txtDateStr = sdf.format(txnDate);

        //SAM卡交易开始
        String apduPsamCardBegin = "8070000024"
                + HexUtil.bytesToHexString(rand)
                + HexUtil.bytesToHexString(payCard.getByteSequence())
                + HexUtil.bytesToHexString(Converter.IntToBytes(payAmount))
                + "09"
                + txtDateStr
                + HexUtil.bytesToHexString(payCard.getAlgBytes())
                //+HexUtil.bytesToHexString(Converter.IntToBytes(posSeq))
                + payCard.getInnerCardNoStr()
                + "2000"
                + "FF0000000000";
        Log.d(tag, String.format("Begin PSAM txn apdu=%s", apduPsamCardBegin));
        psamRet = psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPsamCardBegin));
        if (psamRet == null) {
            Log.d(tag, String.format("SAM卡交易开始失败:%s", apduPsamCardBegin));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(psamRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("SAM卡交易开始失败:APDU:%s Response:%s", apduPsamCardBegin, HexUtil.bytesToHexString(psamRet)));
            return false;
        }
        Log.d(tag, "PSAM卡开始交易 apdu:" + apduPsamCardBegin + " Result:" + HexUtil.bytesToHexString(psamRet));
        byte[] posSeq = new byte[4];
        System.arraycopy(psamRet, 0, posSeq, 0, 4);
        psamDevice.getPsamCard().setSequence((int) Converter.BytesToLong(posSeq));
        byte[] mac1 = new byte[4];
        System.arraycopy(psamRet, 4, mac1, 0, 4);
        psamDevice.getPsamCard().setDebitMac1(mac1);
        return true;
    }

    private boolean cardUpdateTxnRecord() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();

        int balanceafter = (int) Converter.BytesToLong(payCard.getByteBalance()) - payAmount;
        Log.d(tag, String.format("交易后余额：%06X,%d", balanceafter, balanceafter));
        //用户卡更新复合消费缓存
        String txnData = posDevice.getPosID().substring(4)
                + String.format("%04X", payAmount)
                + String.format("%06X", balanceafter)
                + String.format("%02X", Converter.IntToBytes(payAmount)[0])
                + SHTCCPUUserCard.dateInCache(txnDate) + "00";
        Log.d(tag, String.format("交易数据(%d)：%s", txnData.length(), txnData));
        String apduUserCardUpdateCache = "80DC003B10" + txnData;
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardUpdateCache));
        if (cardRet == null) {
            Log.d(tag, String.format("用户卡更新复合消费缓存失败:%s", apduUserCardUpdateCache));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(cardRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("用户卡更新复合消费缓存失败:APDU:%s Response:%s", apduUserCardUpdateCache, HexUtil.bytesToHexString(cardRet)));
            return false;
        }
        Log.d(tag, "用户卡更新复合消费缓存 apdu:" + apduUserCardUpdateCache + " Result:" + HexUtil.bytesToHexString(cardRet));
        return true;
    }

    private byte[] tac;
    private byte[] mac2;

    private boolean cardTxnEnd() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();
        //用户卡交易完成
        String apduUserCardEnd = "805401000F"
                + String.format("%04X", posDevice.getPsamDevice().getPsamCard().getSequence())
                + txtDateStr
                + HexUtil.bytesToHexString(posDevice.getPsamDevice().getPsamCard().getDebitMac1());
        Log.d(tag, String.format("用户卡交易完成，apdu=%s", apduUserCardEnd));
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(apduUserCardEnd));
        if (cardRet == null) {
            //用户卡交易完成无相应，取mactac
            Log.d(tag, String.format("用户卡交易完成失败:%s", apduUserCardEnd));
            while (!readMacTac()) {
                readCardFragment.ShowErrorMessage(1, "交易失败");
                loadFragment(FRAG_PAYINIT);
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            return true;
        } else {
            Log.d(tag, String.format("apdu:%s result:%s", apduUserCardEnd, HexUtil.bytesToHexString(cardRet)));
            String errorCode = CPUDevice.parseResponse(cardRet);
            //错误码不为9000时重取mac和tac
            if (errorCode.compareTo("9000") != 0) {
                //用户卡交易完成回应错误时，取mactac
                Log.d(tag, String.format("用户卡交易完成失败:%s", apduUserCardEnd));
                while (!readMacTac()) {
                    readCardFragment.ShowErrorMessage(1, "交易失败");
                    loadFragment(FRAG_PAYINIT);
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        }
        //正常返回时取mac、tac
        Log.d(tag, "用户卡交易完成 apdu:" + apduUserCardEnd + " Result:" + HexUtil.bytesToHexString(cardRet));
        System.arraycopy(cardRet, 0, tac, 0, 4);
        System.arraycopy(cardRet, 4, mac2, 0, 4);

        return true;
    }

    private boolean isAnotherCard;

    private boolean readMacTac() {
        RFCPUDevice rfcpuDevice = POSApplication.instance.getPosDevice().getRfcpuDevice();
        isAnotherCard = false;
        String apdu = "805A000902"
                + HexUtil.bytesToHexString(Converter.incBytes(payCard.getByteSequence()));
        //取交易认证码
        rfcpuDevice.close();
        rfcpuDevice.open();
        CityTourCard newcard = posDevice.readCard();
        if (newcard == null) {
            return false;
        }
        if (newcard.getInnerCardNoStr().compareTo(payCard.getInnerCardNoStr()) != 0) {
            Log.d(tag, "请放回原卡");
            isAnotherCard = true;
            return false;
        }
        cardRet = rfcpuDevice.sendAPDU(HexUtil.hexStringToByte(apdu));
        if (cardRet == null) {
            Log.d(tag, String.format("取mactac无响应：%s", apdu));
            return false;
        }
        String errorCode = CPUDevice.parseResponse(cardRet);
        if (errorCode.compareTo("9000") != 0) {
            Log.d(tag, String.format("取交易认证码失败:APDU:%s Response:%s", apdu, HexUtil.bytesToHexString(cardRet)));
            return false;
        }
        System.arraycopy(cardRet, 0, mac2, 0, 4);
        System.arraycopy(cardRet, 4, tac, 0, 4);
        Log.d(tag, String.format("取交易认证码成功 apdu：%s result:%s mac2:%s",
                apdu, HexUtil.bytesToHexString(cardRet), HexUtil.bytesToHexString(mac2)));
        return true;
    }

    private boolean isMac2ErrorCard=false;
    private boolean psamTxnEnd() {
        PSAMDevice psamDevice = POSApplication.instance.getPosDevice().getPsamDevice();
        isMac2ErrorCard = false;
        //PSAM卡完成交易
        Log.d(tag, String.format("Mac2:%s", HexUtil.bytesToHexString(mac2)));
        String apduPsamCardEnd = "8072000004"
                + HexUtil.bytesToHexString(mac2);
        psamRet = psamDevice.sendAPDU(HexUtil.hexStringToByte(apduPsamCardEnd));
        if (psamRet == null) {
            Log.d(tag, String.format("PSAM卡完成交易失败:%s", apduPsamCardEnd));
            return false;
        }
        Log.d(tag, String.format("PSAM卡完成交易,apdu:%s result:%s", apduPsamCardEnd, HexUtil.bytesToHexString(psamRet)));
        String errorCode = CPUDevice.parseResponse(psamRet);
        if (errorCode.compareTo("9000") != 0) {
            //MAC2无效，记录该卡
            POSApplication.instance.getAppDatabase().insertMac2ErrCard(payCard.getInnerCardNoStr());
            Log.d(tag, String.format("无效卡，PSAM卡完成交易失败:%s(%s)", apduPsamCardEnd, errorCode));
            return false;
        }
        Log.d(tag, "PSAM卡完成交易 apdu:" + apduPsamCardEnd + " Result:" + HexUtil.bytesToHexString(psamRet));
        //删除MAC2错误卡记录
        DatabaseSHCT db=POSApplication.instance.getAppDatabase();
        db.deleteMac2ErrCard();
        return true;
    }

    private DebitRecord currDebitRecord;
    private boolean saveDebitRecord(){
        PSAMDevice psamDevice=POSApplication.instance.getPosDevice().getPsamDevice();
        DebitRecord debitRecord=new DebitRecord(posDevice.getCorpCode(),
            0,
            (byte) 0x09,
            posDevice.getStationID(),
            POSApplication.instance.getUser().getUsername(),
            "0",//无公交车编号
            posDevice.getTxnType(),
            //(int)Converter.BytesToLong(),
            psamDevice.getPsamCard().getSequence(),
            HexUtil.bytesToHexString(payCard.getFCIValidData().getCityCode()),
            HexUtil.bytesToHexString(payCard.getFCIValidData().getCardNo()),
            payCard.getFCIValidData().getCardType(),
            Converter.BytesToLong(payCard.getByteBalance()),
            payAmount,
            txtDateStr,
            (int)Converter.BytesToLong(payCard.getByteSequence()),
            //psamDevice.getPsamCard().getPosID(),
            HexUtil.bytesToHexString(psamDevice.getPsamCard().getPosID()),
            HexUtil.bytesToHexString(tac),
            //(byte)Integer.parseInt(psamDevice.getPsamCard().getKeyVersion(),0x10),
            psamDevice.getPsamCard().getKeyVersion(),// 卡内版本号
            (byte)0);

        DatabaseSHCT db=POSApplication.getPOSApplication().getAppDatabase();
        if(!db.saveDebit(debitRecord))
        {
            Log.d(tag,"保存交易失败："+db.errorMessageString);
            return false;
        }
        return true;
    }
}
