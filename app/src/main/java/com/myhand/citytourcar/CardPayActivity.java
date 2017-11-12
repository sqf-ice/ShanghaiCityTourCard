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
import android.os.RemoteException;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.centerm.smartpos.aidl.psam.AidlPsam;
import com.centerm.smartpos.aidl.rfcard.AidlRFCard;
import com.centerm.smartpos.aidl.soundplayer.AidlSoundPlayer;
import com.centerm.smartpos.aidl.sys.AidlDeviceManager;
import com.centerm.smartpos.constant.Constant;
import com.myhand.cpucard.DebitRecord;
import com.myhand.devices.POSDevice;
import com.myhand.devices.V8PsamDevice;
import com.myhand.devices.V8RFCPUDevice;
import com.myhand.devices.V8Sounder;
import com.myhand.shanghaicitytourcard.BaseTourCardActivity;
import com.myhand.shanghaicitytourcard.CityTourCard;
import com.myhand.shanghaicitytourcard.POSApplication;
import com.myhand.shanghaicitytourcard.PrintActivity;
import com.myhand.shanghaicitytourcard.R;

public class CardPayActivity extends BaseTourCardActivity
implements Pay_Rdcard_Prompt_Fragment.OnFragmentInteractionListener,
        CardPayFragment.OnFragmentInteractionListener {
    private static final int SEQ_PRINTNOTE=1;
    //本机对应设备
    public POSDevice posDevice;

    public V8Sounder soundPlayer;
    public AidlPsam psam1 = null;
    public AidlPsam psam2 = null;
    public AidlPsam psam3 = null;

    public CityTourCard card=null;
    public int payAmount=0;
    private DebitRecord debitRecord=null;

    private FrameLayout frameLayoutFragment;
    private CardPayFragment cardPayFragment;
    private Pay_Rdcard_Prompt_Fragment rdcard_prompt_fragment;

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

        //开始读卡
        readCard();
    }

    private void initFragment(){
        frameLayoutFragment=(FrameLayout)findViewById(R.id.frameLayoutFragment);
        rdcard_prompt_fragment=new Pay_Rdcard_Prompt_Fragment();
        cardPayFragment=new CardPayFragment();
    }

    @Override
    public void onFragmentInteraction(Uri uri) {
        //Toast.makeText(this,"交流,角楼",Toast.LENGTH_LONG).show();
    }

    @Override
    public void onDeviceConnected(AidlDeviceManager deviceManager) {
        try {
            soundPlayer=new V8Sounder();
            soundPlayer.setSoundPlayer(AidlSoundPlayer.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_SOUNDPLAYER)));
            //soundPlayer.intelligenceAmountReader("3.1415");
            posDevice=((POSApplication)getApplication()).getPosDevice();
            this.psam1 = AidlPsam.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM1));
            if(psam1==null){
               Toast.makeText(this,"PASM1 加载失败",Toast.LENGTH_SHORT).show();
               return;
            }

            this.psam2 = AidlPsam.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM2));
            this.psam3 = AidlPsam.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM3));

            V8PsamDevice psamDevice=new V8PsamDevice();
            psamDevice.setPsam(psam1);
            posDevice.setPsamDevice(psamDevice);

            Toast.makeText(this,"非接设备成功连接，开始进行卡设备操作......",Toast.LENGTH_SHORT).show();
            V8RFCPUDevice v8RFCPUDevice=new V8RFCPUDevice();
            v8RFCPUDevice.setRfCard(AidlRFCard.Stub.asInterface(deviceManager.getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_RFCARD)));
            posDevice.setRfcpuDevice(v8RFCPUDevice);
            Toast.makeText(this,"非接设备操作成功",Toast.LENGTH_SHORT).show();
        } catch (RemoteException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            Toast.makeText(this,"非接设备连接失败："+e.getMessage(),Toast.LENGTH_SHORT).show();
        }
    }

    public void showCardInfo(CityTourCard card)
    {
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
    private void loadFragment(int index){
        FragmentManager fragmentManager=getFragmentManager();
        FragmentTransaction transaction=fragmentManager.beginTransaction();
        Fragment fragment=rdcard_prompt_fragment;
        switch (index){
            case 0:
            {
                fragment=rdcard_prompt_fragment;
                break;
            }
            case 1:{
                fragment=cardPayFragment;
            }
        }
        transaction.replace(R.id.frameLayoutFragment,fragment).commit();
    }

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
    private void readCard(){
        new ThreadReadCard().start();
    }

    private Handler handler=new Handler();
    public class ThreadCardDebit extends Thread{
        private int debitAmount;

        public ThreadCardDebit(int debitAmount){
            this.debitAmount=debitAmount;
        }

        @Override
        public void run() {
            super.run();

            while (true) {
                //卡支付
                debitRecord = posDevice.complexDebit(card, debitAmount);
                if (debitRecord == null) {
                    //检查是否需要重新做交易
                    if(posDevice.getErrorCode().compareTo(POSDevice.EC_NORESPONSE)==0){
                        handler.post(new Runnable() {
                            @Override
                            public void run() {
                                cardPayFragment.ShowErrorMessage(2, "交易重做......");
                            }
                        });
/*
                        try {
                            Thread.sleep(10);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
*/
                        //重新读卡
                        while (true) {
                            card = posDevice.readCard();
                            if(card!=null){
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
        switch (requestCode){
            case SEQ_PRINTNOTE:{
                //重新读卡显示卡余额
                card=posDevice.readCard();
                if(card!=null){
                    cardPayFragment.showBalance(card.getBalance());
                    cardPayFragment.setPayAmount(0);
                }
                break;
            }
        }
    }

    public void showPayPrompt() {
        final AlertDialog.Builder normalDialog =
                new AlertDialog.Builder(CardPayActivity.this);
        //normalDialog.setIcon(R.drawable.);
        normalDialog.setTitle("消费金额确认");
        normalDialog.setMessage(String.format("消费金额为%.2f元，是否继续?", cardPayFragment.getPayAmount()));
        normalDialog.setPositiveButton("确定",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
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

    public void printPayNote(){
        Intent intent=new Intent();
        intent.setClass(this,PrintActivity.class);
        startActivity(intent);
    }
}
