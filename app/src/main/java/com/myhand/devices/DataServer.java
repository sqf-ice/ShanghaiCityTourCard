package com.myhand.devices;

/**
 * Created by wenha_000 on 2017-09-16.
 */

public class DataServer {
    private int classic;
    private String IP;
    private int port;

    public DataServer() {
    }

    public DataServer(int classic, String IP, int port) {
        this.classic = classic;
        this.IP = IP;
        this.port = port;
    }

    public String getIP() {
        return IP;
    }

    public void setIP(String IP) {
        this.IP = IP;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public int getClassic() {
        return classic;
    }

    public void setClassic(int classic) {
        this.classic = classic;
    }
}
