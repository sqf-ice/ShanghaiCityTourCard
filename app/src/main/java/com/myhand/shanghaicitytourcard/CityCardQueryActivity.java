package com.myhand.shanghaicitytourcard;

import android.os.Bundle;
import android.os.RemoteException;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.centerm.smartpos.aidl.psam.AidlPsam;
import com.centerm.smartpos.aidl.rfcard.AidlRFCard;
import com.centerm.smartpos.aidl.soundplayer.AidlSoundPlayer;
import com.centerm.smartpos.aidl.sys.AidlDeviceManager;
import com.centerm.smartpos.constant.Constant;
import com.centerm.smartpos.util.HexUtil;
import com.myhand.POS.POSApplication;
import com.myhand.cpucard.SHTCPsamCard;
import com.myhand.devices.POSDevice;
import com.myhand.devices.V8PsamDevice;
import com.myhand.devices.V8RFCPUDevice;
import com.myhand.devices.V8Sounder;

import java.util.ArrayList;
import java.util.List;

public class CityCardQueryActivity extends BaseTourCardActivity {
    //本机对应设备
    private POSDevice posDevice;

    private V8Sounder soundPlayer;
    private AidlPsam psam1 = null;
    private AidlPsam psam2 = null;
    private AidlPsam psam3 = null;

    private CityTourCard card=null;
    //消费相关
    private EditText editTextAmount;

    //卡信息显示控件
    private TextView mCardATR;
    private TextView txtViewCardNO;
    private TextView txtViewVerifyCode;
    private TextView txtViewCardBalance;
    private TextView txtViewStartDate;
    private TextView txtViewValidDate;

    //卡操作信息相关
    private ListView mLogListView;
    private ArrayAdapter<String> mLogAdapter;
    private List<String> mLogString;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_city_card_query);
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
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        initControls();
        initDebit();
    }

    public void initDebit(){
        Bundle data=getIntent().getExtras();
        if(data!=null){
            editTextAmount.setText(String.format("%d",data.getInt("amount")));
        }else{
            editTextAmount.setText("1");
        }
    }

    public void initControls()
    {
        editTextAmount=(EditText)findViewById(R.id.editTextAmount);

        //日志列表
        mLogListView=(ListView)findViewById(R.id.listViewLog) ;
        mLogString=new ArrayList<String>();
        mLogAdapter=new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,mLogString);
        mLogListView.setAdapter(mLogAdapter);

        mCardATR=(TextView)findViewById(R.id.textViewATR) ;
        txtViewCardNO=(TextView)findViewById(R.id.textViewCardNO) ;
        txtViewVerifyCode=(TextView)findViewById(R.id.textViewVerifyCode);
        txtViewCardBalance=(TextView)findViewById(R.id.textViewCardBalance) ;

        txtViewStartDate=(TextView)findViewById(R.id.textViewCardStartTime) ;
        txtViewValidDate=(TextView)findViewById(R.id.textViewCardValidDate) ;
        Button btnDebit=(Button)findViewById(R.id.buttonDebit);
        btnDebit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //检查消费金额
                String txtAmount=editTextAmount.getText().toString();
                if(txtAmount.isEmpty()){
                    editTextAmount.setHint("请输入消费金额");
                    return;
                }
                long debitAmount=Long.parseLong(txtAmount);
                if(debitAmount<=0){
                    editTextAmount.setHint("请输入正确的金额");
                    return;
                }

//                soundPlayer.insertCard();
//                soundPlayer.confirmCardNoAmount();
                soundPlayer.intelligenceAmountReader(txtAmount);
                if(card==null){
                    Toast.makeText(CityCardQueryActivity.this,"请先读卡",Toast.LENGTH_SHORT).show();
                    return;
                }
                if(debitAmount>card.getBalance()){
                    soundPlayer.balance();
                    return;
                }

                //if(posDevice.debit(card,(int)debitAmount)==null)
                if(posDevice.complexDebit(card,(int)debitAmount)==null)
                {
                    mLogString.add(0,posDevice.getErrorMessage());
                }else{
                    card=posDevice.readCard();
                    showCardInfo(card);
                }

                mLogAdapter.notifyDataSetChanged();
            }
        });

        Button btnReadCard=(Button)findViewById(R.id.buttonReadCard);
        btnReadCard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            card=posDevice.readCard();
                if(card==null){
                    soundPlayer.insertCard();
                }
            showCardInfo(card);
            mLogAdapter.notifyDataSetChanged();
            }
        });

        //PSAM卡操作
        Button btnPSAM=(Button)findViewById(R.id.buttonPSAM);
        btnPSAM.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            if(!posDevice.openPSAMCard())
            {
                posDevice.getPsamDevice().setErrorMessage("PSAM卡打开失败:"+posDevice.getPsamDevice().getErrorMessage());
                return;
            }

            //显示PSAM信息
            SHTCPsamCard psamCard=(SHTCPsamCard)posDevice.getPsamDevice().getPsamCard();
            String msg="PSAM ";

            if(psamCard.getAtr()!=null)
            {
                msg+="ATR:"+HexUtil.bytesToHexString(psamCard.getAtr());
            }
            msg += " POSNO:"+psamCard.getPosNo();
            msg+=" Ver:"+psamCard.getKeyVersion();

            mLogString.add(0,msg);
            mLogAdapter.notifyDataSetChanged();
            }
        });
    }

/**
    //发送APDU
    public void sendAPDU() {
        try {
            byte[] ret = curPsam.sendApdu(HexUtil.hexStringToByte("00B0960000"));
            if(null != ret){
                String msg=getString(R.string.psam_send_apdu) + HexUtil.bytesToHexString(ret);
                showMessage(msg);
                mLogString.add(0,msg);
            }else{
                String msg=getString(R.string.psam_send_apdu_null);
                showMessage(msg);
                mLogString.add(0,msg);
            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            showMessage(getString(R.string.psam_send_apdu_exception));
        }
    }
*/
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
                mLogString.add(0,"PASM1 加载失败");
            }else{
                mLogString.add(0,"PASM1 加载成功");
            }
            this.psam2 = AidlPsam.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM2));
            this.psam3 = AidlPsam.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PSAM3));

            V8PsamDevice psamDevice=new V8PsamDevice();
            psamDevice.setPsam(psam1);
            posDevice.setCurrPsamDevice(psamDevice);

            mLogString.add(mLogString.size(),"非接设备成功连接，开始进行卡设备操作......");
            V8RFCPUDevice v8RFCPUDevice=new V8RFCPUDevice();
            v8RFCPUDevice.setRfCard(AidlRFCard.Stub.asInterface(deviceManager.getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_RFCARD)));
            posDevice.setRfcpuDevice(v8RFCPUDevice);
            mLogString.add(mLogString.size(),"非接设备操作成功。");
        } catch (RemoteException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            Toast.makeText(CityCardQueryActivity.this,e.getMessage(),Toast.LENGTH_SHORT).show();
            mLogString.add(mLogString.size(),"非接设备连接失败："+e.getMessage());
        }
    }

    public void showCardInfo(CityTourCard card)
    {
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
    }
}
