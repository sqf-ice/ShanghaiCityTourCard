package com.myhand.manage;

import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.net.Uri;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;

import com.myhand.POS.DatabaseSHCT;
import com.myhand.cpucard.DebitRecord;
import com.myhand.shanghaicitytourcard.POSApplication;
import com.myhand.shanghaicitytourcard.R;
import com.myhand.shtcdatafile.SHTCClient;

import java.util.List;

public class SettleActivity extends AppCompatActivity
implements SettleInitFragment.OnFragmentInteractionListener,
SendDataFragment.OnFragmentInteractionListener{
    //未上场交易
    public List<DebitRecord> listDebitRecord;

    //fragment
    public SettleFragment currFragment;
    public SettleInitFragment settleInitFragment;
    public SendDataFragment sendDataFragment;

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

    public void queryUnSendDebit(){
        DatabaseSHCT db= POSApplication.instance.getAppDatabase();
        listDebitRecord=db.debitQuery("where status=0",0);
    }

    public SettleSum sumDebit(){
        SettleSum result=new SettleSum();
        if(listDebitRecord==null||listDebitRecord.size()<=0){
            return result;
        }

        int count=0;
        int sum=0;
        for (DebitRecord debitRecord:listDebitRecord) {
            count++;
            sum+=debitRecord.getAmount();
        }
        result.setCount(count);
        result.setSum(sum);
        return result;
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
        loadFragment(1);

        sendDataFragment.showMessage(0,1,String.format("正在连接服务器，请等待......"));
        sendDataFragment.showMessage(1,1,String.format("正在连接服务器，请等待......"));
        sendDataFragment.showMessage(2,1,String.format("正在连接服务器，请等待......"));
        //SHTCClient
    }

    @Override
    public void onFragmentInteraction(Uri uri) {

    }
}
