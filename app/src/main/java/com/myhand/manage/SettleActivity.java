package com.myhand.manage;

import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;

import com.centerm.smartpos.aidl.sys.AidlDeviceManager;
import com.myhand.POS.DatabaseSHCT;
import com.myhand.cpucard.DebitRecord;
import com.myhand.devices.POSDevice;
import com.myhand.shanghaicitytourcard.BaseTourCardActivity;
import com.myhand.shanghaicitytourcard.POSApplication;
import com.myhand.shanghaicitytourcard.PrintActivity;
import com.myhand.shanghaicitytourcard.R;
import com.myhand.shtcdatafile.SHTCClient;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

public class SettleActivity extends AppCompatActivity
implements SettleInitFragment.OnFragmentInteractionListener,
SendDataFragment.OnFragmentInteractionListener{
    private static final String tag=SettleFragment.class.getSimpleName();
    private static final int REQ_PRINTNOTE=1;

    //未上传交易
    public List<DebitRecord> listDebitRecord;
    public SettleSum currSettleSum=new SettleSum();
    //fragment
    public SettleFragment currFragment;
    public SettleInitFragment settleInitFragment;
    public SendDataFragment sendDataFragment;

    private Handler handler=new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            String messgText=msg.getData().getString("msg");
            int errorcode=msg.arg1;
            switch (errorcode){
                case 0:{
                    sendDataFragment.showMessage(2,0,messgText);
                    break;
                }
                case 10:{
                    sendDataFragment.showMessage(2,0,messgText);
                    //打印结算小票
                    printSettleNote(currSettleSum);
                    break;
                }
                default:{
                    sendDataFragment.showMessage(1,0,messgText);
                    break;
                }
            }
            //Log.d(tag,messgText);
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settle);
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

        intFragment();
        loadFragment(0);

        queryUnSendDebit();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode){
            case REQ_PRINTNOTE:{
                //成功打印结算票据后退出结算
                setResult(1);
                finish();
            }
        }
    }

    public void queryUnSendDebit(){
        DatabaseSHCT db= POSApplication.instance.getAppDatabase();
        listDebitRecord=db.debitQuery("where status=0",0);
        sumDebit();
    }

    public void sumDebit(){
        if(listDebitRecord==null||listDebitRecord.size()<=0){
            return;
        }

        Date startTime=null;
        Date endTime=null;
        int count=0;
        int sum=0;
        SimpleDateFormat sdf=new SimpleDateFormat("yyyyMMddHHmmss");
        for (DebitRecord debitRecord:listDebitRecord) {
            try {
                Date txnTime=sdf.parse(debitRecord.getTxnTime());
                if(startTime==null) {
                    startTime = txnTime;
                }else if(startTime.compareTo(txnTime)>0){
                    startTime=txnTime;
                }
                if(endTime==null){
                    endTime=txnTime;
                }else if(endTime.compareTo(txnTime)<0){
                    endTime=txnTime;
                }
            } catch (ParseException e) {
                e.printStackTrace();
            }
            count++;
            sum+=debitRecord.getAmount();
            currSettleSum.setPosID(debitRecord.getPosID());
        }
        currSettleSum.setStartTime(startTime);
        currSettleSum.setEndTime(endTime);
        currSettleSum.setCount(count);
        currSettleSum.setSum(sum);
    }

    public void intFragment(){
        settleInitFragment=SettleInitFragment.newInstance("","");
        sendDataFragment=SendDataFragment.newInstance("","");
        loadFragment(1);
        loadFragment(0);
    }

    public void loadFragment(int index){
        SettleFragment fragment=null;
        switch (index){
            case 0:{
                fragment=settleInitFragment;
                break;
            }
            case 1:{
                fragment=sendDataFragment;
                break;
            }
            default:{
                return;
            }
        }

        FragmentManager fragmentManager=getFragmentManager();
        FragmentTransaction transaction=fragmentManager.beginTransaction();
        transaction.replace(R.id.frameLayoutFragment,fragment).commit();
    }

    public void sendData(){
/*
        //测试打印
        printSettleNote(currSettleSum);
*/
        if(listDebitRecord.size()<=0){
            return;
        }
        loadFragment(1);

        sendDataFragment.showMessage(0,1,String.format("正在连接服务器，请等待......"));
        new ThreadSendData().start();
    }

    public void printSettleNote(SettleSum sum){
        Intent intent=new Intent();
        intent.putExtra("SettleSum",sum);
        intent.setClass(SettleActivity.this, PrintActivity.class);
        startActivityForResult(intent,REQ_PRINTNOTE);
    }

    public class ThreadSendData extends Thread{
        @Override
        public void run() {
            super.run();
            //初始化连接
            SHTCClient client=new SHTCClient(POSDevice.IPTEST,POSDevice.portUp);
            client.setHandler(handler);
            //连接服务器
            while (true){
                if(client.Connect()){
                    break;
                }
            }
            //发送数据
            new Thread(client.runnableSendDebitRecord).start();
        }
    }

    public class ThreadDownloadData extends Thread{
        @Override
        public void run() {
            super.run();
            //初始化连接
            SHTCClient client=new SHTCClient(POSDevice.IPTEST,POSDevice.portDown);
            client.setHandler(handler);
            //连接服务器
            while (true){
                if(client.Connect()){
                    break;
                }
            }
            //下载数据
            new Thread(client.runnableSendDebitRecord).start();
        }
    }

    @Override
    public void onFragmentInteraction(Uri uri) {

    }
}
