package com.myhand.shanghaicitytourcard;

import android.app.ProgressDialog;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;
import android.widget.RadioButton;
import android.widget.TextView;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.ErrorMessage;
import com.myhand.control.ErrorMessageAdapter;
import com.myhand.shtcdatafile.SHTCClient;
import com.myhand.transport.DGCommHead;

import java.util.ArrayList;
import java.util.List;

public class SendDataActivity extends AppCompatActivity {
    private static final String tag=SendDataActivity.class.getSimpleName();
    private static String IPTEST="168.10.5.96";
    private static String IPLOCAL1="192.168.100.109";
    private static String IPLOCAL2="130.233.2.124";

    private SHTCClient client;
    private String IP=IPLOCAL1;
    private int currPort;

    //private String IP="";
    private List<ErrorMessage> listMessage;
    private ListView listViewMessage;
    private ErrorMessageAdapter errorMessageAdapter;

    private TextView txtViewMessage;

    private Button btnSendData;
    private Button btnServerSet;
    private Button btnConnect;
    private Button btnClose;

    private RadioButton radioButtonServerLocal1;
    private RadioButton radioButtonServerLocal2;
    private RadioButton radioButtonServerTest;

    private RadioButton radioButtonPOSUpload;
    private RadioButton radioButtonDownload;
    private RadioButton radioButtonFileUpload;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_send_data);
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

        client=new SHTCClient(IP,currPort);//("192.168.100.109",12581);

        txtViewMessage=(TextView)findViewById(R.id.textViewMessage);
        btnSendData=(Button)findViewById(R.id.buttonSend);
        //btnSendData.setEnabled(false);
        btnSendData.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(client.runnableSendDebitRecord).start();
            }
        });

        btnConnect=(Button)findViewById(R.id.buttonConnect);
        btnConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //client=new SHTCClient("130.251.1.189",12581);
                client.setServerIP(IP);
                client.setServerPort(currPort);
                client.setHandler(mHandler);
                //progressDialog=new ProgressDialog(SendDataActivity.this);

                new Thread(runnableConnect).start();
                btnConnect.setEnabled(false);
            }
        });

        btnClose=(Button)findViewById(R.id.buttonClose);
        //btnClose.setEnabled(false);
        btnClose.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                client.close();
/*
                btnClose.setEnabled(false);
                btnConnect.setEnabled(true);
                btnSendData.setEnabled(false);
*/
            }
        });
        initSelectButton();
        initSetTransMode();
        initErrorMessageControl();


    }

    private void initSetTransMode(){
        radioButtonPOSUpload=(RadioButton)findViewById(R.id.radioButtonPosUpload);
        radioButtonPOSUpload.setChecked(true);
        currPort=SHTCClient.portUp;
        client.setCurrTransMode(SHTCClient.TRANSMODE_POS);

        radioButtonPOSUpload.setOnClickListener(setTransMode);
        radioButtonFileUpload=(RadioButton)findViewById(R.id.radioButtonFileUpload);
        radioButtonFileUpload.setOnClickListener(setTransMode);
        radioButtonDownload=(RadioButton)findViewById(R.id.radioButtonDownload);
        radioButtonDownload.setOnClickListener(setTransMode);
    }

    private void initErrorMessageControl(){
        listMessage= new ArrayList<ErrorMessage>();
        errorMessageAdapter=new ErrorMessageAdapter(this,listMessage);
        listViewMessage=(ListView)findViewById(R.id.listViewMessage) ;
        listViewMessage.setAdapter(errorMessageAdapter);
    }

    private View.OnClickListener selectServer=new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            if(client!=null&&client.isConnected()){
                client.close();
            }
            if(v==findViewById(R.id.radioButtonServerTest)){
                IP=IPTEST;
            }else if(v==findViewById(R.id.radioButtonServerLocal1)){
                IP=IPLOCAL1;
            }else if(v==findViewById(R.id.radioButtonServerLocal2)){
                IP=IPLOCAL2;
            }
            ShowMessage("Current Select Server is "+IP);
        }
    };

    private View.OnClickListener setTransMode=new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            if(v==findViewById(R.id.radioButtonPosUpload)){
                client.setCurrTransMode(SHTCClient.TRANSMODE_POS);
                client.setServerPort(SHTCClient.portUp);
            }else if(v==findViewById(R.id.radioButtonFileUpload)){
                client.setCurrTransMode(SHTCClient.TRANSMODE_DBFILE);
                client.setServerPort(SHTCClient.portUp);
            }else if(v==findViewById(R.id.radioButtonDownload)){
                client.setCurrTransMode(SHTCClient.TRANSMODE_DOWNLOAD);
                client.setServerPort(SHTCClient.portDown);
            }
        }
    };

    private void initSelectButton(){
        radioButtonDownload=(RadioButton)findViewById(R.id.radioButtonDownload);
        radioButtonDownload.setOnClickListener(selectServer);

        radioButtonServerLocal1=(RadioButton)findViewById(R.id.radioButtonServerLocal1);
        radioButtonServerLocal1.setOnClickListener(selectServer);

        radioButtonServerLocal2=(RadioButton)findViewById(R.id.radioButtonServerLocal2);
        radioButtonServerLocal2.setOnClickListener(selectServer);

        radioButtonServerTest=(RadioButton)findViewById(R.id.radioButtonServerTest);
        radioButtonServerTest.setOnClickListener(selectServer);
        radioButtonServerTest.setChecked(true);
        IP=IPTEST;
    }
    public Handler mHandler = new Handler() {
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            txtViewMessage.setText(msg.getData().getString("msg"));
            switch (msg.arg1)
            {
                case 0:{
                    txtViewMessage.setTextColor(Color.rgb(0,255,0));
                    break;
                }
                default:{
                    txtViewMessage.setTextColor(Color.rgb(255,0,0));
                    break;
                }
            }

            listMessage.add(0,new ErrorMessage(msg.arg1,msg.getData().getString("msg")));
            errorMessageAdapter.notifyDataSetChanged();
        }
    };

    private ProgressDialog progressDialog;

    private void connectOK(){
        btnConnect.setEnabled(false);
        btnSendData.setEnabled(true);
        btnClose.setEnabled(true);
    }
    Runnable runnableConnect=new Runnable() {
        @Override
        public void run() {

//            progressDialog.setMessage("开始连接服务器......");
//            progressDialog.show();
            //ShowMessage("开始连接服务器......");
            if(!client.Connect())
            {
//                progressDialog.setMessage("连接服务器失败");
//                progressDialog.hide();
                btnSendData.setEnabled(false);
                return;
            }
            //connectOK();
//            new Thread(runnableReceive).start();
            //ShowMessage("服务器连接成功。");

         }
    };

    /**
     * 接收数据
     */
/*
    Runnable runnableReceive=new Runnable() {
        @Override
        public void run() {
            while(true){
                byte[] frameData=client.getRcvBuffer().getFrameData();
                if(frameData==null)
                {
                    try {
                        Thread.sleep(1);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    continue;
                }
                Log.d(tag, HexUtil.bytesToHexString(frameData));
                ShowMessage("Receive Data Frame:"+new String(frameData));
            }
        }
    };
*/

     public void ShowMessage(String msgTxt){
        Message msg=mHandler.obtainMessage();
        Bundle bundle=new Bundle();
        bundle.putString("msg",msgTxt);
        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }
}
