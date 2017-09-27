package com.myhand.shtcdatafile;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.POS.DatabaseSHCT;
import com.myhand.common.Converter;
import com.myhand.cpucard.DebitRecord;
import com.myhand.shanghaicitytourcard.POSApplication;
import com.myhand.transport.DGCommHead;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.List;
import java.util.zip.CRC32;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class SHTCClient {
    private static final String tag=SHTCClient.class.getSimpleName();

    private int wailtCount;
    private static final int TRANS_STATUS_IDLE=0;
    private static final int TRANS_STATUS_REQUEST=1;
    private static final int TRANS_STATUS_REQUESTOK=2;
    private static final int TRANS_STATUS_SENDRECORD=3;
    private static final int TRANS_STATUS_SENDRECORDOK=4;
    private int transStatus=TRANS_STATUS_IDLE;

    private static final int TRANS_RESULT_FAILURE=0;
    private static final int TRANS_RESULT_SUCCESSFUL=1;
    private int transResult=TRANS_RESULT_SUCCESSFUL;

    private byte[] rcvFrameData;

    private String serverIP;
    private int serverPort;
    private int timeout=30000;

    private Socket socket;
    private InputStream readStream;
    private OutputStream writeStream;

    private int status=0;


    private Handler handler;
    private DataBuffer rcvBuffer;

    public SHTCClient() {
        rcvBuffer=new DataBuffer();
    }

    public int getTransStatus() {
        return transStatus;
    }

    public void setTransStatus(int transStatus) {
        this.transStatus = transStatus;
    }

    public int getTimeout() {
        return timeout;
    }

    public void setTimeout(int timeout) {
        this.timeout = timeout;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public SHTCClient(String serverIP, int serverPort) {
        this.serverIP = serverIP;
        this.serverPort = serverPort;
        rcvBuffer=new DataBuffer();
    }

    public DataBuffer getRcvBuffer() {
        return rcvBuffer;
    }

    public void setRcvBuffer(DataBuffer rcvBuffer) {
        this.rcvBuffer = rcvBuffer;
    }

    public String getServerIP() {
        return serverIP;
    }

    public void setServerIP(String serverIP) {
        this.serverIP = serverIP;
    }

    public int getServerPort() {
        return serverPort;
    }

    public void setServerPort(int serverPort) {
        this.serverPort = serverPort;
    }

    public Socket getSocket() {
        return socket;
    }

    public void setSocket(Socket socket) {
        this.socket = socket;
    }

    public Handler getHandler() {
        return handler;
    }

    public void setHandler(Handler handler) {
        this.handler = handler;
    }

    public boolean Connect()
    {
        try {
            ShowMessage(0,String.format("开始连接服务器(%s@%d)......",serverIP,serverPort));
            socket = new Socket();
            SocketAddress socketAddress = new InetSocketAddress(serverIP,serverPort);
            socket.connect(socketAddress, timeout);

            readStream=socket.getInputStream();
            writeStream=socket.getOutputStream();

            //开启接收线程
            threadReadData.start();
            ShowMessage(0,String.format("服务器成功连接。"));
            return true;
        }catch (IOException ex){
            ShowMessage(1,String.format("服务器连接异常：%s.",ex.getLocalizedMessage()));
            return false;
        }
    }

    public void SendStringData(String dataStr)
    {
        try {
            if(writeStream==null)
            {
                ShowMessage(1,"No Connected.");
                return;
            }
            String realSendStr=String.format("%04d%s",dataStr.length(),dataStr);
            Log.d(tag,"realsenddata:"+realSendStr);

            byte[] sendData=realSendStr.getBytes();
            Log.d(tag,"Send Data:"+ HexUtil.bytesToHexString(sendData));
            writeStream.write(sendData, 0, sendData.length);
            Log.d(tag,"Send OK");
            ShowMessage(0,"数据成功发送");
        }catch (Exception ex)
        {
            Log.d(tag,ex.getLocalizedMessage());
            ShowMessage(1,"Send Data Exception:"+ex.getLocalizedMessage());
        }
    }

    void ShowMessage(int errorcode,String msgStr)
    {
        Message msg=handler.obtainMessage();
        Bundle data=new Bundle();
        data.putString("msg",msgStr);
        msg.setData(data);
        msg.arg1=errorcode;
        handler.sendMessage(msg);
    }

    /**
     * 接收数据线程
     */
    private Thread threadReadData=new Thread(new Runnable() {
        @Override
        public void run() {
            while (true)
            {
                if(readStream==null)
                {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    continue;
                }

                try {
                    //接收数据
                    int count=readStream.available();
                    if(count>0)
                    {
                        Log.d(tag,"Have data read.");
                        byte[] buffer=new byte[count];
                        readStream.read(buffer,0,count);
                        Log.d(tag,"Receive data:"+HexUtil.bytesToHexString(buffer));
                        rcvBuffer.putData(buffer);
                    }

                } catch (IOException e) {
                    e.printStackTrace();
                    ShowMessage(1,"Socket receive data exception:"+e.getLocalizedMessage());
                    break;
                }
                //检查是否接收到了完整的报文数据
                rcvFrameData=rcvBuffer.getFrameData();
                if(rcvFrameData==null){
                    continue;
                }

                //解析接收的数据
                parseResult();
            }
        }
    });

    /**
     * 解析服务器回应结果
     */
    private void parseResult(){
        switch (transStatus){
            case TRANS_STATUS_REQUEST:
            {
                if(rcvFrameData.length==2){
                    ShowMessage(1,String.format("服务器回应错误码：%s",new String(rcvFrameData)));
                    transResult=TRANS_RESULT_FAILURE;
                }else {
                    ShowMessage(0, String.format("服务器回应长度：%s", new String(rcvFrameData)));
                    transResult=TRANS_RESULT_SUCCESSFUL;
                }
                transStatus=TRANS_STATUS_REQUESTOK;
                break;
            }
            case TRANS_STATUS_SENDRECORD:
            {
                ShowMessage(0,String.format("服务器回应：%s",new String(rcvFrameData)));
                transStatus=TRANS_STATUS_SENDRECORDOK;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    //CRC32校验
    public static String CRC(byte[] data)
    {
        CRC32  crc32=new CRC32();
        crc32.update(data);
        return String.format("%08X",crc32.getValue());
    }

    public boolean isConnected(){
        if(socket==null){
            return false;
        }
        return socket.isConnected();
    }

    public void close(){
        if(socket==null){
            return;
        }
        try {
            socket.close();

        } catch (IOException e) {
            e.printStackTrace();
            ShowMessage(1,"Socket close exception:"+e.getLocalizedMessage());
        }
    }

    public Runnable runnableSendDebitRecord=new Runnable() {
        @Override
        public void run() {
            //检查是否存在未上传的交易记录
            POSApplication application=POSApplication.getPOSApplication();
            DatabaseSHCT db=application.getAppDatabase();
            String condition="";
            List<DebitRecord> recordList=db.debitQuery(condition);
            if(recordList==null||recordList.isEmpty()){
                ShowMessage(1,"没有需上传的交易");
                return;
            }

            //发送请求头
            if(!sendRequest()){
                return;
            }

            for(int i=0;i<recordList.size();i++){
                DebitRecord debitRecord=recordList.get(i);
                if(!sendDebitRecord(debitRecord))
                {
                    break;
                }
            }
        }
    };
    private boolean sendRequest(){
        POSApplication application=POSApplication.getPOSApplication();
        DatabaseSHCT db=application.getAppDatabase();
        transStatus=TRANS_STATUS_REQUEST;
        long sendSeq=db.getPosSendSequence(application.getPosDevice().getPosID()) ;
        DGCommHead head=new DGCommHead();
        head.setSendSeq((int)sendSeq);
        String sendStr=head.toString();
        Log.d(tag,"DG:"+sendStr+String.format(" Length:%d",sendStr.length()));
        SendStringData(sendStr);
        //等待服务器回应
        wailtCount=300;
        while (transStatus!=TRANS_STATUS_REQUESTOK&&wailtCount>=0)
        {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            wailtCount--;
        }
        if(wailtCount<0){
            ShowMessage(1,"等待服务器回应发送请求超时，结束发送过程");
            return false;
        }
        if(transResult==TRANS_RESULT_FAILURE){
            ShowMessage(1,"通讯错误，结束发送");
            return false;
        }

        return true;
    }

    private boolean sendDebitRecord(DebitRecord record){
        FHFileRecord fileRecord=new FHFileRecord();
        Log.d(tag,String.format("记录长度：%d",fileRecord.getDataFieldLength()));
        fileRecord.fromDebitRecord(record);
        String dataStr=fileRecord.getData();
        Log.d(tag,"记录数据："+dataStr);
        ShowMessage(0,dataStr);
        String crcStr=CRC(dataStr.getBytes());
        Log.d(tag,String.format("CRC:%s 长度%d",crcStr,crcStr.length()));
        String sendData=String.format("%04d%s%s",dataStr.length()+8,dataStr, crcStr);
        Log.d(tag,"发送数据："+sendData);
        ShowMessage(0,sendData);
        transStatus=TRANS_STATUS_SENDRECORD;
        wailtCount=300;
        while (transStatus!=TRANS_STATUS_SENDRECORDOK&&wailtCount>=0){
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            wailtCount--;
        }
        if(wailtCount<0){
            ShowMessage(1,"等待服务器回应记录发送回应超时，结束发送过程");
            return false;
        }

        transStatus=TRANS_STATUS_IDLE;
        return true;
    }
}
