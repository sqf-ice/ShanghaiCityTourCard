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
import com.myhand.devices.POSDevice;
import com.myhand.shanghaicitytourcard.POSApplication;
import com.myhand.transport.DGCommHead;
import com.myhand.transport.DGDownloadHead;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.StringReader;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.Date;
import java.util.List;
import java.util.zip.CRC32;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class SHTCClient {
    private static final String tag=SHTCClient.class.getSimpleName();

    public static int portUp=12581;
    public static int portDown=12582;

    private int wailtCount;
    public static final int TRANS_STATUS_IDLE=0;

    public static final int TRANS_STATUS_REQUEST=1;
    public static final int TRANS_STATUS_REQUESTOK=2;

    public static final int TRANS_STATUS_BATCHINFO=3;
    public static final int TRANS_STATUS_BATCHINFOOK=4;

    public static final int TRANS_STATUS_SENDRECORD=5;
    public static final int TRANS_STATUS_SENDRECORDOK=6;

    public static final int TRANS_STATUS_SENDTAIL=7;
    public static final int TRANS_STATUS_SENDTAILOK=8;

    public static final int TRANS_STATUS_SENDDOWNLOADREQUEST=11;
    public static final int TRANS_STATUS_SENDDOWNLOADREQUEST_OK=12;

    private int transStatus=TRANS_STATUS_IDLE;

    private static final int TRANS_RESULT_FAILURE=0;
    private static final int TRANS_RESULT_SUCCESSFUL=1;
    private int transResult=TRANS_RESULT_SUCCESSFUL;

    public static final int TRANSMODE_POS=0;//POS直连方式
    public static final int TRANSMODE_DBFILE=1;//文件上传方式
    public static final int TRANSMODE_DOWNLOAD=2;//下载文件
    private int currTransMode=TRANSMODE_POS;

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

    public int getCurrTransMode() {
        return currTransMode;
    }

    public void setCurrTransMode(int currTransMode) {
        this.currTransMode = currTransMode;
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
            //Log.d(tag,"Send Data:"+ HexUtil.bytesToHexString(sendData));
            writeStream.write(sendData, 0, sendData.length);
/*
            Log.d(tag,"Send OK");
            ShowMessage(0,"数据成功发送");
*/
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
                    String countStr=new String(rcvFrameData);
                    ShowMessage(0, String.format("服务器回应长度：%s",countStr ));
                    ShowMessage(0,String.format("已上传记录数%d",Integer.parseInt(countStr)));
                    transResult=TRANS_RESULT_SUCCESSFUL;
                }
                transStatus=TRANS_STATUS_REQUESTOK;
                break;
            }
            case TRANS_STATUS_BATCHINFO:
            {
                ShowMessage(0,String.format("服务器回应：%s",new String(rcvFrameData)));
                transStatus=TRANS_STATUS_BATCHINFOOK;
                break;
            }
            case TRANS_STATUS_SENDRECORD:
            {
                ShowMessage(0,String.format("服务器回应：%s",new String(rcvFrameData)));
                transStatus=TRANS_STATUS_SENDRECORDOK;
                break;
            }
            case TRANS_STATUS_SENDTAIL:
            {
                ShowMessage(0,String.format("服务器回应：%s",new String(rcvFrameData)));
                transStatus=TRANS_STATUS_SENDTAILOK;
                break;
            }
            case TRANS_STATUS_SENDDOWNLOADREQUEST:{
                ShowMessage(0,String.format("服务器回应：%s",new String(rcvFrameData)));
                transStatus=TRANS_STATUS_SENDDOWNLOADREQUEST_OK;
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

    public Runnable runnableDownloadFile=new Runnable() {
        @Override
        public void run() {
            sendDownloadRequest();
        }
    };

    public Runnable runnableSendDebitRecord=new Runnable() {
        private int uploadcount=0;

        @Override
        public void run() {
            //检查是否存在未上传的交易记录
            POSApplication application=POSApplication.getPOSApplication();
            DatabaseSHCT db=application.getAppDatabase();
            String condition=" where status=0";
            List<DebitRecord> recordList=db.debitQuery(condition,0);
            if(recordList==null||recordList.isEmpty()){
                ShowMessage(1,"没有需上传的交易");
                return;
            }else {
                ShowMessage(0,String.format("共有%d条消费交易需要上传，开始上传......",recordList.size()));
            }

            uploadcount=0;
            //发送交易上传请求头
            if(!sendUploadRequest(recordList.size())){
                ShowMessage(1,"没有需上传的消费记录。");
                return;
            }

            //上传批次信息
            if(!sendBatchInfo(recordList.size())){
                ShowMessage(1,"批次信息发送失败，发送过程结束");
                return;
            }

            int sum=0;
            for(int i=0;i<recordList.size();i++){
                DebitRecord debitRecord=recordList.get(i);
                sum+=debitRecord.getAmount();
                if(!sendDebitRecord(debitRecord))
                {
                    ShowMessage(1,"消费记录数据发送失败，发送过程结束");
                    break;
                }else {
                    db.updateRecordStatus(debitRecord.getLocalTxnSeq(),1);
                    ShowMessage(0, String.format("流水号%d消费交易成功上传",debitRecord.getLocalTxnSeq()));
                    uploadcount++;
                }
            }

            //上传尾报文
            if(!sendTailData(recordList.size(),sum))
            {
                ShowMessage(1,"尾报文发送失败");
                return;
            }

            ShowMessage(10,String.format("共成功上传了%d条消费交易。",uploadcount));
        }
    };

    private boolean sendTailData(int count,int sum){
        FHFileTail fileTail=new FHFileTail(count,sum);

        String dataStr=fileTail.getData();
        //带校验上送批次信息
        String crcStr=CRC(dataStr.getBytes());
        //Log.d(tag,String.format("CRC:%s 长度%d",crcStr,crcStr.length()));
        String sendData=String.format("%s%s",dataStr, crcStr);

        Log.d(tag,String.format("上传尾报文(%d):%s",sendData.length(),sendData));

        transStatus=TRANS_STATUS_SENDTAIL;
        SendStringData(sendData);

        wailtCount=300;
        while (transStatus!=TRANS_STATUS_SENDTAILOK&&wailtCount>=0){
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

    private boolean sendDownloadRequest(){
        POSDevice posDevice=POSApplication.instance.getPosDevice();
        DGDownloadHead head=new DGDownloadHead(posDevice.getTradeCode(),posDevice.getPosID(),posDevice.getCorpCode(),
                posDevice.getCorpName(),new Date(),"");

        Log.d(tag,String.format("上传下载文件请求报文:%s(Length:%d)",head.getData(),head.getDataFieldLength()));
        setStatus(TRANS_STATUS_SENDDOWNLOADREQUEST);
        SendStringData(head.getData());
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
            ShowMessage(1,"等待服务器回应下载文件请求超时，结束下载过程");
            return false;
        }
        if(transResult==TRANS_RESULT_FAILURE){
            ShowMessage(1,"通讯错误，结束下载");
            return false;
        }

        return true;
    }

    private boolean sendUploadRequest(int count){
        POSApplication application=POSApplication.getPOSApplication();
        DatabaseSHCT db=application.getAppDatabase();
        transStatus=TRANS_STATUS_REQUEST;
        long sendSeq=db.getPosSendSequence(application.getPosDevice().getPosID()) ;
        //保存当前批次号
        POSApplication.instance.getPosDevice().setPatchNo((int)sendSeq);

        DGCommHead head=new DGCommHead();
        head.setSendSeq((int)sendSeq);
        head.setFileSize(count);
        String sendStr=head.toString();
        Log.d(tag,String.format("上传请求报文:%s(Length:%d)",sendStr,sendStr.length()));
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

    private boolean sendBatchInfo(int recordCnt)
    {
        FHFileRecord fileRecord=new FHFileRecord();

        FHFileDescription fileDescription=new FHFileDescription(recordCnt,fileRecord.getDataFieldLength(),true);
        String dataStr=fileDescription.getData();
        //带校验上送批次信息
        String crcStr=CRC(dataStr.getBytes());
        //Log.d(tag,String.format("CRC:%s 长度%d",crcStr,crcStr.length()));
        String sendData=String.format("%s%s",dataStr, crcStr);

        Log.d(tag,String.format("上传批次纪录头(%d):'%s'",sendData.length(),sendData));

        transStatus=TRANS_STATUS_BATCHINFO;
        SendStringData(sendData);

        wailtCount=300;
        while (transStatus!=TRANS_STATUS_BATCHINFOOK&&wailtCount>=0){
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
    private boolean sendDebitRecord(DebitRecord record){
        FHFileRecord fileRecord=new FHFileRecord();
        //Log.d(tag,String.format("记录长度：%d",fileRecord.getDataFieldLength()));
        fileRecord.fromDebitRecord(record);
        String dataStr=fileRecord.getData();

        //Log.d(tag,String.format("上传记录数据(%d):'%s'",dataStr.length(),dataStr));
        //ShowMessage(0,dataStr);
        String crcStr=CRC(dataStr.getBytes());
        //Log.d(tag,String.format("CRC:%s 长度%d",crcStr,crcStr.length()));
        String sendData=String.format("%s%s",dataStr, crcStr);
        Log.d(tag,"发送数据："+sendData);
        //ShowMessage(0,sendData);
        transStatus=TRANS_STATUS_SENDRECORD;
        SendStringData(sendData);
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
