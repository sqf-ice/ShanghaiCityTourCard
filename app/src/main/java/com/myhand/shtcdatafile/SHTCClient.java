package com.myhand.shtcdatafile;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.transport.DGCommHead;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.zip.CRC32;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class SHTCClient {
    private static final String tag=SHTCClient.class.getSimpleName();

    private String serverIP;
    private int serverPort;
    private int timeout=30000;

    private Socket socket;
    private InputStream readStream;
    private OutputStream writeStream;

    private int status;


    private Handler handler;
    private DataBuffer rcvBuffer;

    public SHTCClient() {
        rcvBuffer=new DataBuffer();
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
            }
        }
    });

    //CRC32校验
    public static String CRC(byte[] data)
    {
        CRC32  crc32=new CRC32();
        crc32.update(data);
        return String.format("%04d",crc32.getValue());
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
}
