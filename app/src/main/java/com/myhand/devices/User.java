package com.myhand.devices;

/**
 * Created by wenha_000 on 2017-09-16.
 */

public class User {
    private String userID;
    private String password;

    public User() {
    }

    public String getUserID() {
        return userID;
    }

    public User(String userID, String password) {
        this.userID = userID;
        this.password = password;
    }

    public void setUserID(String userID) {
        this.userID = userID;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }
}
