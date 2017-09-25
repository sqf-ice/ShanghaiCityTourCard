package com.myhand.shanghaicitytourcard;

import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.shtcdatafile.SHTCClient;
import com.myhand.transport.DGCommHead;

public class SendDataActivity extends AppCompatActivity {
    private static final String tag=SendDataActivity.class.getSimpleName();
    private SHTCClient client;
    //private String IP="168.10.5.96";
    //private String IP="192.168.100.109";
    //private String IP="130.233.2.124";
    private String IP="130.233.1.98";
    private int portUp=12581;
    private int portDown=12582;
    private TextView txtViewMessage;
    private Button btnSendData;
    private Button btnServerSet;
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

        txtViewMessage=(TextView)findViewById(R.id.textViewMessage);
        btnSendData=(Button)findViewById(R.id.buttonSend);
        btnSendData.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            long sendSeq=((POSApplication)getApplication()).getAppDatabase().getPosSendSequence("12345") ;
            DGCommHead head=new DGCommHead();
                head.setSendSeq((int)sendSeq);
            String sendStr=head.toString();
            Log.d(tag,"DG:"+sendStr+String.format(" Length:%d",sendStr.length()));
            client.SendStringData(sendStr);
            }
        });

        //client=new SHTCClient("130.251.1.189",12581);
        client=new SHTCClient(IP,portUp);//("192.168.100.109",12581);
        client.setHandler(mHandler);
        //progressDialog=new ProgressDialog(SendDataActivity.this);

        new Thread(runnable).start();
        rcvThread.start();
    }

    public Handler mHandler = new Handler() {
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            txtViewMessage.setText(msg.getData().getString("msg"));
        }
    };

    private ProgressDialog progressDialog;

    Runnable runnable=new Runnable() {
        @Override
        public void run() {
//            progressDialog.setMessage("开始连接服务器......");
//            progressDialog.show();
            if(!client.Connect())
            {
//                progressDialog.setMessage("连接服务器失败");
//                progressDialog.hide();
                return;
            }

//            //发生测试数据
//            progressDialog.setMessage("开始发生测试数据......");
//            client.SendStringData("This is a test data");
//            progressDialog.hide();
        }
    };

    /**
     * 数据接收线程
     */
    private Thread rcvThread=new Thread(new Runnable() {
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
            }
        }
    });
}
