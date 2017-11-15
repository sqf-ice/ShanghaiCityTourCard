package com.myhand.devices;

import android.graphics.Color;
import android.os.RemoteException;

import com.centerm.smartpos.aidl.soundplayer.AidlSoundPlayer;
import com.centerm.smartpos.aidl.soundplayer.AidlSoundStateChangeListener;
import com.centerm.smartpos.constant.Constant;
import com.centerm.smartpos.constant.MapBean;

import java.util.HashMap;

/**
 * Created by wenha_000 on 2017-10-01.
 */

public class V8Sounder extends Sounder{
    private int readSpeed= 500;
    private boolean readUnit = false;

    private AidlSoundPlayer soundPlayer;
    private AidlSoundStateChangeListener aidlSoundStateListener = new SoundCallback();

    public AidlSoundPlayer getSoundPlayer() {
        return soundPlayer;
    }

    public void setSoundPlayer(AidlSoundPlayer soundPlayer) {
        this.soundPlayer = soundPlayer;
    }

    private class SoundCallback extends AidlSoundStateChangeListener.Stub{

        @Override
        public void onSoundError(int arg0) throws RemoteException {
            // TODO Auto-generated method stub
            showMessage("Sound Error");
        }

        @Override
        public void onSoundFinish() throws RemoteException {
            // TODO Auto-generated method stub
            showMessage("Sound Finish");
        }

        @Override
        public void onSoundPause() throws RemoteException {
            // TODO Auto-generated method stub
            showMessage("Sound pause");
        }

        @Override
        public void onSounding() throws RemoteException {
            // TODO Auto-generated method stub
            showMessage("Sounding... ...");
        }


    }

    private void showMessage(String messageTxt){

    }

    protected void init() {
    }

    //20170518    智能数额朗读   wangwenxun@centerm.com
    //传入的数据如果大于0，那么不能以0开头
    public void intelligenceAmountReader(final String numberStr){
        Thread intellThread = new Thread(new Runnable(){
            @Override
            public void run() {
                // TODO Auto-generated method stub
                if((numberStr!=null)&&(numberStr.length()!=0)){
                    try {
                        setReadParam();
                        //如果不想进行监听，传null也是可以的
                        //soundPlayer.intelligenceSound(numberStr, null);
                        soundPlayer.intelligenceSound(numberStr,aidlSoundStateListener);

                    } catch (RemoteException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                }else{
                    showMessage("Invalid Param");
                }
            }
        });
        intellThread.start();
    }

    // 20170518 普通数额朗读 wangwenxun@centerm.com
    public void commonAmountReader(final String numberStr) {
        Thread t  = new Thread(new Runnable(){
            @Override
            public void run() {
                // TODO Auto-generated method stub
                if ((numberStr != null) && (numberStr.length() != 0)) {
                    try {
                        setReadParam();
                        //设置普通播报时间
                        soundPlayer.commonSound(numberStr, null);

                    } catch (RemoteException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                } else {
                    showMessage("Invalid Param");
                }
            }
        });
        t.start();
    }


    public void swipeCard() {
        play(Constant.VOICE_TYPE.SWIPE_CARD);
    }

    public void insertCard() {
        play(Constant.VOICE_TYPE.INSERT_CARD);
    }

    public void waveCard() {
        play(Constant.VOICE_TYPE.WAVE_CARD);
    }

    public void swipeWaveCard() {
        play(Constant.VOICE_TYPE.SWIPE_WAVE_CARD);
    }

    public void swipeInsertCard() {
        play(Constant.VOICE_TYPE.SWIPE_INSERT_CARD);
    }

    public void swipeInsertWaveCard() {
        play(Constant.VOICE_TYPE.SWIPE_INSERT_WAVE_CARD);
    }

    public void transactionNoLeaveCard() {
        play(Constant.VOICE_TYPE.TRANSACTION_DONT_LEAVE_CARD);
    }

    public void transactionOkLeaveCard() {
        play(Constant.VOICE_TYPE.TRANSACTION_OK_LEAVE_CARD);
    }

    public void inputAmount() {
        play(Constant.VOICE_TYPE.INPUT_AMOUNT);
    }

    public void inputPwd() {
        play(Constant.VOICE_TYPE.INPUT_PASSWORD);
    }

    public void pwdLengthNotEnough() {
        play(Constant.VOICE_TYPE.PASSWORD_LENGTH_NO_ENOUGH);
    }

    public void confirmCardNoAmount() {
        play(Constant.VOICE_TYPE.CONFIRM_CARDNO_AMOUNT);
    }

    public void communicateWait() {
        play(Constant.VOICE_TYPE.TRANSACTION_WAIT_PLEASE);
    }

    public void printWait() {
        play(Constant.VOICE_TYPE.PRINTTING_WAIT_PLEASE);
    }

    public void pwdWrongReTransaction() {
        play(Constant.VOICE_TYPE.PASSWORD_WRONG_RETRANSACTION);
    }

    public void printerLackPaper() {
        play(Constant.VOICE_TYPE.PRINTER_LACK_PAPER);
    }

    public void lowPowerNoPrint() {
        play(Constant.VOICE_TYPE.LOW_POWER_NO_PRINT);
    }

    public void transactionSuccess() {
        play(Constant.VOICE_TYPE.TRANSACTION_SUCCESS);
    }

    public void communicateError() {
        play(Constant.VOICE_TYPE.COMMUNICATION_BREAKDOWN);
    }

    public void sign() {
        play(Constant.VOICE_TYPE.SIGN_PLEASE);
    }

    public void balance() {
        play(Constant.VOICE_TYPE.BALANCE_PLEASE);
    }

    public void automaticSuccess() {
        play(Constant.VOICE_TYPE.AUTOMATIC_SUCCESS);
    }

    public void activationSuccess() {
        play(Constant.VOICE_TYPE.ACTIVATION_SUCCESS);
    }

    public static void sleep(long time){
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public void play(int type) {
        if (soundPlayer != null) {
            try {
                soundPlayer.play(type);
            } catch (Exception e) {
                e.printStackTrace();
                //showMessage(e.getLocalizedMessage(), Color.RED);
            }
        }
    }

    public void setReadParam(){
        MapBean mapbean = new MapBean();
        HashMap<String,Object> hashmap = new HashMap<String,Object>();
        hashmap.put("readfrequence", readSpeed);
        hashmap.put("readUnit",readUnit);
        mapbean.setParamMap(hashmap);
        try {
            soundPlayer.readParam(mapbean);
        } catch (RemoteException e) {
            // TODO Auto-generated catch block
            showMessage("set param fail");
            e.printStackTrace();
        }
    }

/*
    public void onDeviceConnected(AidlDeviceManager deviceManager) {
        try {
            soundPlayer = AidlSoundPlayer.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_SOUNDPLAYER));
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }
*/

}
