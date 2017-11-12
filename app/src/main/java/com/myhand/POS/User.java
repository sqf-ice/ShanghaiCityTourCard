package com.myhand.POS;

/**
 * Created by vincent on 2017/11/13.
 */

public class User {
    private String username;
    private String password;
    private int reloID;
    private int status;

    public User() {
    }

    public User(String username, String password, int reloID, int status) {
        this.username = username;
        this.password = password;
        this.reloID = reloID;
        this.status = status;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public int getReloID() {
        return reloID;
    }

    public void setReloID(int reloID) {
        this.reloID = reloID;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }
}
