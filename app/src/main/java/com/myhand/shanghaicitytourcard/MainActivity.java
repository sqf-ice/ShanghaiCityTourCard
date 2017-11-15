package com.myhand.shanghaicitytourcard;

import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

import com.centerm.smartpos.aidl.printer.AidlPrinter;
import com.centerm.smartpos.aidl.psam.AidlPsam;
import com.centerm.smartpos.aidl.rfcard.AidlRFCard;
import com.centerm.smartpos.aidl.soundplayer.AidlSoundPlayer;
import com.centerm.smartpos.aidl.sys.AidlDeviceManager;
import com.centerm.smartpos.constant.Constant;
import com.myhand.POS.DatabaseSHCT;
import com.myhand.citytourcar.CardPayActivity;
import com.myhand.cpucard.DebitRecord;
import com.myhand.devices.DataExchangeService;
import com.myhand.devices.POSDevice;
import com.myhand.devices.PSAMDevice;
import com.myhand.devices.Sounder;
import com.myhand.devices.V8Printer;
import com.myhand.devices.V8PsamDevice;
import com.myhand.devices.V8RFCPUDevice;
import com.myhand.devices.V8Sounder;
import com.myhand.manage.SettleActivity;
import com.myhand.manage.SettleFragment;
import com.myhand.shtcdatafile.FHFileRecord;
import com.myhand.shtcdatafile.SHTCClient;

public class MainActivity extends BaseTourCardActivity {
    private String tag=MainActivity.class.getSimpleName();
    public static final int CardQuery=1;
    public static final int REQ_Settle=2;
    public static final int DebitQuery=3;
    public static final int Login=4;
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private int amount=2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
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

        //启动数据交换服务
        Intent dataExchangeService=new Intent(MainActivity.this, DataExchangeService.class);
        ComponentName name=startService(dataExchangeService);
        if(name==null){
            Log.d(tag,"Service start failure");
        }else{
            Log.d(tag,"Service start OK,name:"+name.getShortClassName());
        }

        Button btnLogin=(Button)findViewById(R.id.buttonLogin) ;
        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent();
                intent.setClass(MainActivity.this,LoginActivity.class);
                startActivityForResult(intent,Login);
            }
        });

        Button btnCardQuery=(Button)findViewById(R.id.buttonCardQuery);
        btnCardQuery.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(POSApplication.instance.getUser()==null){
                    final AlertDialog.Builder normalDialog =
                            new AlertDialog.Builder(MainActivity.this);
                    //normalDialog.setIcon(R.drawable.);
                    normalDialog.setTitle("登录提示");
                    normalDialog.setMessage(String.format("目前没有登录，请先登录。如果不知道登录账户，请与管理员联系"));
                    normalDialog.setPositiveButton("确定",
                            new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                }
                            });
                    // 显示
                    normalDialog.show();
                    V8Sounder sounder=(V8Sounder)POSApplication.getPOSApplication().getPosDevice().getSounder();
                    if(sounder!=null){
                        sounder.sign();
                    }
                    return;
                }
                startDebit();
            }
        });

        Button btnSendData=(Button)findViewById(R.id.buttonConnect);
        btnSendData.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent();
                intent.setClass(MainActivity.this,SettleActivity.class);
                startActivityForResult(intent,REQ_Settle);
            }
        });

        Button btnDebitQuery=(Button)findViewById(R.id.buttonDebitQuery) ;
        btnDebitQuery.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent();
                intent.setClass(MainActivity.this,DebitQueryActivity.class);
                startActivityForResult(intent,DebitQuery);
            }
        });

/*
        Button btnFHFileUploadInfo=(Button)findViewById(R.id.buttonUploadFileInfo) ;
        btnFHFileUploadInfo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent();
                intent.setClass(MainActivity.this,FHFileQueryActivity.class);
                startActivityForResult(intent,4);
            }
        });
*/

/*
        //测试黑名单卡数据库
        POSApplication application=(POSApplication)getApplication();

        DatabaseSHCT db=application.getAppDatabase();
        db.insertBlkCard("9209148411",1);
        if(!db.isBlackCard("92091484110"))
        {
            Log.d(tag,"Black Card Find Failure");
        }else{
            Log.d(tag,String.format("Card %s is black card","92091484110"));
        }

        //检查是否进入消费界面
        Bundle data=getIntent().getExtras();
        if(data!=null){
            amount=data.getInt("amount");
            startDebit();
        }
*/

        //testRecord();
    }


    private void testRecord(){
        DebitRecord record=new DebitRecord("00320000001",1,(byte)80,"123456","100001","1234",
                (byte)88,1,"2000","12",(byte)1,100,1,"20170928092400",1,"10001000","01020304",(byte)1,(byte)0);

        FHFileRecord fhFileRecord=new FHFileRecord();
        fhFileRecord.fromDebitRecord(record);

        Log.d(tag,fhFileRecord.getData());
//
        String crcStr=SHTCClient.CRC(fhFileRecord.getData().getBytes());
        String dgStr=String.format("%04d%s%s",fhFileRecord.getData().length()+8,fhFileRecord.getData(),crcStr);
        Log.d(tag,String.format("DG Str:%s Length:%d",dgStr,dgStr.length()));
    }

    public void startDebit()
    {
        Intent intent=new Intent();
        intent.setClass(MainActivity.this,CardPayActivity.class);
        Bundle data=new Bundle();
        data.putInt("amount",amount);
        intent.putExtras(data);

        startActivityForResult(intent,CardQuery);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    @Override
    public void onDeviceConnected(AidlDeviceManager deviceManager) {
        super.onDeviceConnected(deviceManager);
        POSDevice posDevice=POSApplication.instance.getPosDevice();
        try {
            //声音模块
            V8Sounder soundPlayer=(V8Sounder) posDevice.getSounder();
            if(soundPlayer==null) {
                soundPlayer = new V8Sounder();
            }
            soundPlayer.setSoundPlayer(AidlSoundPlayer.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_SOUNDPLAYER)));
            posDevice.setSounder(soundPlayer);

            //打印模块
            V8Printer printer=(V8Printer) posDevice.getPrinter();
            if(printer==null) {
                printer = new V8Printer();
            }
            printer.setPrintDev(AidlPrinter.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PRINTERDEV)));
            posDevice.setPrinter(printer);

            //PSAM卡模块
            AidlPsam psam[]=new AidlPsam[]{
                AidlPsam.Stub.asInterface(deviceManager
                        .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM1)),
                AidlPsam.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM2)),
                AidlPsam.Stub.asInterface(deviceManager
                        .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM3))};

            if(posDevice.getPsamDevices()==null){
                posDevice.setPsamDevices(new V8PsamDevice[]{new V8PsamDevice(psam[0]),new V8PsamDevice(psam[1]),new V8PsamDevice(psam[2])});
            }else{
                //更新POS设备
                for(int i=0;i<3;i++){
                    PSAMDevice psamDevice=posDevice.getPsamDevices()[i];
                    if(psamDevice==null){
                        psamDevice=new V8PsamDevice(psam[i]);
                    }

                }
                //默认第一个PSAM卡设备
                posDevice.setCurrPsamDevice(posDevice.getPsamDevices()[0]);
            }

            //非接设备
            V8RFCPUDevice v8RFCPUDevice=new V8RFCPUDevice();
            v8RFCPUDevice.setRfCard(AidlRFCard.Stub.asInterface(deviceManager.getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_RFCARD)));
            posDevice.setRfcpuDevice(v8RFCPUDevice);

            soundPlayer.activationSuccess();
        } catch (RemoteException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            Toast.makeText(this,"非接设备连接失败："+e.getMessage(),Toast.LENGTH_SHORT).show();
        }

    }
}
