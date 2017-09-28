package com.myhand.devices;

import android.graphics.Color;
import android.os.RemoteException;

import com.centerm.smartpos.aidl.printer.AidlPrinter;
import com.centerm.smartpos.aidl.printer.AidlPrinterStateChangeListener;
import com.centerm.smartpos.constant.DeviceErrorCode;
import com.myhand.shanghaicitytourcard.POSApplication;
import com.myhand.shanghaicitytourcard.R;

/**
 * Created by wenha_000 on 2017-09-28.
 */

public class V8Printer extends Printer{
    private AidlPrinter printDev = null;
    // 打印机回调对象
    private AidlPrinterStateChangeListener callback = new PrinterCallback(); // 打印机回调
    /** 打印机回调类 */
    private class PrinterCallback extends AidlPrinterStateChangeListener.Stub {

        @Override
        public void onPrintError(int arg0) throws RemoteException {
            // showMessage("打印机异常" + arg0, Color.RED);
            getMessStr(arg0);
        }

        @Override
        public void onPrintFinish() throws RemoteException {
            showMessage(POSApplication.getPOSApplication().getString(R.string.printer_finish), "", Color.BLACK);
        }

        @Override
        public void onPrintOutOfPaper() throws RemoteException {
            showMessage(POSApplication.getPOSApplication().getString(R.string.printer_need_paper), "", Color.RED);
        }

    }

    public void showMessage(String msg,String title,int color){

    }

    public void showMessage(String msg){

    }
    private void getMessStr(int ret) {
        switch (ret) {
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_BUSY:
                showMessage(POSApplication.getPOSApplication().getString(R.string.printer_device_busy));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_OK:
                showMessage(POSApplication.getPOSApplication().getString(R.string.printer_success));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_PRINTER_OUT_OF_PAPER:
                showMessage(POSApplication.getPOSApplication().getString(R.string.printer_lack_paper));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_PRINTER_HEAD_OVER_HEIGH:
                showMessage(POSApplication.getPOSApplication().getString(R.string.printer_over_heigh));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_PRINTER_OVER_HEATER:
                showMessage(POSApplication.getPOSApplication().getString(R.string.printer_over_heat));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_PRINTER_LOW_POWER:
                showMessage(POSApplication.getPOSApplication().getString(R.string.printer_low_power));
                break;
            default:
                showMessage(POSApplication.getPOSApplication().getString(R.string.printer_other_exception_code) + ret);
                break;
        }
    }
}
