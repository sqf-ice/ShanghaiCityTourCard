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
import java.net.Socket;
import java.net.SocketAddress;

/**
 * Created by wenha_000 on 2017-09-12.
 */

public class SHTCClient {
    private static final String tag=SHTCClient.class.getSimpleName();
    private String serverIP;
    private int serverPort;
    private Socket socket;
    private InputStream readStream;
    private OutputStream writeStream;

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
            socket = new Socket(serverIP, serverPort);
            readStream=socket.getInputStream();
            writeStream=socket.getOutputStream();

            //开启接收线程
            threadReadData.start();

            Bundle data=new Bundle();
            data.putString("msg","Socket Connect OK!");
            Message msg=handler.obtainMessage();
            msg.setData(data);
            handler.sendMessage(msg);
            return true;
        }catch (IOException ex){
            Message msg=handler.obtainMessage();
            Bundle data=new Bundle();
            data.putString("msg","Connect server exception:"+ex.getLocalizedMessage());
            msg.setData(data);
            handler.sendMessage(msg);
            return false;
        }
    }

    public void SendStringData(String dataStr)
    {
        try {
            if(writeStream==null)
            {
                ShowMessage("No Connected.");
                return;
            }
            String realSendStr=String.format("%04d%s",dataStr.length(),dataStr);
            byte[] sendData=realSendStr.getBytes();
            Log.d(tag,"Send Data:"+ HexUtil.bytesToHexString(sendData));
            writeStream.write(sendData, 0, sendData.length);
            ShowMessage("数据成功发送");
        }catch (IOException ex)
        {
            ShowMessage("Send Data Exception:"+ex.getLocalizedMessage());
        }
    }

    void ShowMessage(String msgStr)
    {
        Message msg=handler.obtainMessage();
        Bundle data=new Bundle();
        data.putString("msg",msgStr);
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
                        byte[] buffer=new byte[count];
                        readStream.read(buffer,0,count);
                        rcvBuffer.putData(buffer);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    });

}
