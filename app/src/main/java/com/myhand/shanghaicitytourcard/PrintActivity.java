package com.myhand.shanghaicitytourcard;

import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.os.RemoteException;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.centerm.smartpos.aidl.printer.AidlPrinter;
import com.centerm.smartpos.aidl.printer.AidlPrinterStateChangeListener;
import com.centerm.smartpos.aidl.printer.PrintDataObject;
import com.centerm.smartpos.aidl.sys.AidlDeviceManager;
import com.centerm.smartpos.constant.Constant;
import com.centerm.smartpos.constant.DeviceErrorCode;
import com.centerm.smartpos.util.LogUtil;
import com.myhand.cpucard.DebitRecord;
import com.myhand.devices.V8Printer;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

public class PrintActivity extends BaseTourCardActivity {
    public static Dialog createLoadingDialog(Context context, String msg) {
        LayoutInflater inflater = LayoutInflater.from(context);
        View v = inflater.inflate(R.layout.layout_dialog_loading, null);// 得到加载view
        LinearLayout layout = (LinearLayout) v
                .findViewById(R.id.lineLayoutView);// 加载布局
        TextView textViewMessage = (TextView) v.findViewById(R.id.textViewMessage);// 提示文字
        textViewMessage.setText(msg);// 设置加载信息

        Dialog loadingDialog = new Dialog(context, R.style.MyDialogStyle);// 创建自定义样式dialog
        loadingDialog.setCancelable(true); // 是否可以按“返回键”消失
        loadingDialog.setCanceledOnTouchOutside(false); // 点击加载框以外的区域
        loadingDialog.setContentView(layout, new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.MATCH_PARENT));// 设置布局
        /**
         *将显示Dialog的方法封装在这里面
         */
        Window window = loadingDialog.getWindow();
        WindowManager.LayoutParams lp = window.getAttributes();
        lp.width = WindowManager.LayoutParams.MATCH_PARENT;
        lp.height = WindowManager.LayoutParams.WRAP_CONTENT;
        window.setGravity(Gravity.CENTER);
        window.setAttributes(lp);
        window.setWindowAnimations(R.style.PopWindowAnimStyle);
        loadingDialog.show();

        return loadingDialog;
    }

    private DebitRecord debitRecord;
    private Dialog dialog;
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
            showMessage(getString(R.string.printer_finish), "", Color.BLACK);
        }

        @Override
        public void onPrintOutOfPaper() throws RemoteException {
            showMessage(getString(R.string.printer_need_paper), "", Color.RED);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_print);
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

        getPrintData();
        dialog=createLoadingDialog(this,"打印机初始化......");
    }

    private void getPrintData(){
        Intent intent=getIntent();
        debitRecord=(DebitRecord) intent.getSerializableExtra("debitRecord");
        if(debitRecord==null){
            Toast.makeText(this,"No debit record laod",Toast.LENGTH_SHORT).show();
        }
    }

    private void getMessStr(int ret) {
        switch (ret) {
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_BUSY:
                showMessage(getString(R.string.printer_device_busy));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_OK:
                showMessage(getString(R.string.printer_success));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_PRINTER_OUT_OF_PAPER:
                showMessage(getString(R.string.printer_lack_paper));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_PRINTER_HEAD_OVER_HEIGH:
                showMessage(getString(R.string.printer_over_heigh));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_PRINTER_OVER_HEATER:
                showMessage(getString(R.string.printer_over_heat));
                break;
            case DeviceErrorCode.DEVICE_PRINTER.DEVICE_PRINTER_LOW_POWER:
                showMessage(getString(R.string.printer_low_power));
                break;
            default:
                showMessage(getString(R.string.printer_other_exception_code) + ret);
                break;
        }

    }

    // 设置灰度为一级
    public void setGrayOne(View v) {
        LogUtil.print(getString(R.string.printer_gray1));
        try {
            printDev.setPrinterGray(0x01);
            showMessage(getString(R.string.printer_gray1_success), Color.GREEN);
        } catch (Exception e) {
            e.printStackTrace();
            showMessage(
                    getString(R.string.printer_gray_exception)
                            + e.getLocalizedMessage(), Color.RED);
        }
    }

    // 设置灰度为二级
    public void setGrayTwo(View v) {
        LogUtil.print(getString(R.string.printer_gray2));
        try {
            printDev.setPrinterGray(0x02);
            showMessage(getString(R.string.printer_gray2_success), Color.GREEN);
        } catch (Exception e) {
            e.printStackTrace();
            showMessage(
                    getString(R.string.printer_gray_exception)
                            + e.getLocalizedMessage(), Color.RED);
        }
    }

    // 设置灰度为三级
    public void setGrayThree(View v) {
        LogUtil.print(getString(R.string.printer_gray3));
        try {
            printDev.setPrinterGray(0x03);
            showMessage(getString(R.string.printer_gray3_success), Color.GREEN);
        } catch (Exception e) {
            e.printStackTrace();
            showMessage(
                    getString(R.string.printer_gray_exception)
                            + e.getLocalizedMessage(), Color.RED);
        }
    }

    // 设置灰度为四级
    public void setGrayFour(View v) {
        LogUtil.print(getString(R.string.printer_gray4));
        try {
            printDev.setPrinterGray(0x04);
            showMessage(getString(R.string.printer_gray4_success), Color.GREEN);
        } catch (Exception e) {
            e.printStackTrace();
            showMessage(
                    getString(R.string.printer_gray_exception)
                            + e.getLocalizedMessage(), Color.RED);
        }
    }

    // 打印文本内容
    public void printText(View v) {
        List<PrintDataObject> list = new ArrayList<PrintDataObject>();
        list.add(new PrintDataObject(
                getString(R.string.printer_textsize_bigger), 24));
        list.add(new PrintDataObject(getString(R.string.printer_textsize_blod),
                8, true));
        list.add(new PrintDataObject(getString(R.string.printer_left), 8,
                false, PrintDataObject.ALIGN.LEFT));
        list.add(new PrintDataObject(getString(R.string.printer_center), 8,
                false, PrintDataObject.ALIGN.CENTER));
        list.add(new PrintDataObject(getString(R.string.printer_right), 8,
                false, PrintDataObject.ALIGN.RIGHT));
        list.add(new PrintDataObject(getString(R.string.printer_underline), 8,
                false, PrintDataObject.ALIGN.LEFT, true));
        list.add(new PrintDataObject(getString(R.string.printer_amount), 8,
                true, PrintDataObject.ALIGN.LEFT, false, false));
        list.add(new PrintDataObject("888.66", 24, true, PrintDataObject.ALIGN.LEFT, false,
                true));
        list.add(new PrintDataObject(getString(R.string.printer_newline), 8,
                false, PrintDataObject.ALIGN.LEFT, false, true));
        list.add(new PrintDataObject(
                getString(R.string.printer_acceptorid_name)));
        list.add(new PrintDataObject(getString(R.string.printer_line_distance),
                8, false, PrintDataObject.ALIGN.LEFT, false, true, 40, 28));
        for (int i = 0; i < 38; i += 3) {
            list.add(new PrintDataObject(
                    getString(R.string.printer_left_distance) + (i * 10), 8,
                    false, PrintDataObject.ALIGN.LEFT, false, true, 24, 0, (i * 10)));
        }
        try {
            this.printDev.printText(list, callback);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void printSomthing(List<PrintDataObject> list){
        try {
            this.printDev.printText(list, callback);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void printBmpFast(View v) {
        try {
            InputStream ins = getResources().getAssets().open(
                    "image/smartposTest.bmp");
            Bitmap bmp = BitmapFactory.decodeStream(ins);
            printDev.printBmpFast(bmp, Constant.ALIGN.CENTER, callback); //位图快速打印接口更改为支持打印队列的接口 2017/03/28 wengtao@centerm.com
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // 吐纸
    public void spitPaper(int height) {
        try {
            printDev.spitPaper(height);
        } catch (Exception e) {
            e.printStackTrace();
            showMessage(
                    getString(R.string.printer_spit_exception)
                            + e.getLocalizedMessage(), Color.RED);
        }
    }

    // 获取打印机状态
    public void getPrintState(View v) {
        try {
            int retCode = printDev.getPrinterState();
            getMessStr(retCode);
        } catch (Exception e) {
            e.printStackTrace();
            showMessage(
                    getString(R.string.printer_status_exception)
                            + e.getLocalizedMessage(), Color.RED);
        }
    }

    public void printSync(View view) {
        new Thread() {
            @Override
            public void run() {
                String str = "123456789";
                try {
                    int ret = printDev.printBarCodeSync(str);
                    getMessStr(ret);
                    InputStream ins = getResources().getAssets().open(
                            "image/smartposTest.bmp");

                    Bitmap bmp = BitmapFactory.decodeStream(ins);
                    ret = printDev.printBmpSync(80, 200, 100, bmp);
                    getMessStr(ret);
                    List<PrintDataObject> list = new ArrayList<PrintDataObject>();
                    list.add(new PrintDataObject(
                            getString(R.string.printer_textsize_bigger), 24));
                    list.add(new PrintDataObject(
                            getString(R.string.printer_textsize_blod), 8, true));
                    list.add(new PrintDataObject(
                            getString(R.string.printer_left), 8, false,
                            PrintDataObject.ALIGN.LEFT));
                    list.add(new PrintDataObject(
                            getString(R.string.printer_center), 8, false,
                            PrintDataObject.ALIGN.CENTER));

                    ret = printDev.printTextSync(list);
                    getMessStr(ret);

                    ret = printDev.printBmpFastSync(bmp, Constant.ALIGN.CENTER);
                } catch (RemoteException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }.start();

    }

    // 初始化打印参数
    public void initPrinter(View v) {
        // begin 017/03/09 14:11 子线程，防止有多个打印任务时点击初始化参数出现anr现象
        // wengtao@centerm.com
        new Thread(new Runnable() {

            @Override
            public void run() {
                try {
                    printDev.initPrinter();
                    showMessage(getString(R.string.printer_init_success));
                } catch (Exception e) {
                    e.printStackTrace();
                    showMessage(getString(R.string.printer_init_exception));
                }
            }
        }).start();
        // end 017/03/09 14:11
    }

    // 打印小字体
    public void printLittleSize(View v) {
        PrintDataObject printDataObject = new PrintDataObject(
                getString(R.string.printer_textsize_normal));
        List<PrintDataObject> list = new ArrayList<PrintDataObject>();
        list.add(printDataObject);
        list.add(printDataObject);
        PrintDataObject printDataObject2 = new PrintDataObject(
                getString(R.string.printer_textsize_normal));
        printDataObject2.setText(getString(R.string.printer_textsize_small));
        printDataObject2.setIsLittleSize(true);
        list.add(printDataObject2);
        list.add(printDataObject2);
        try {
            printDev.printText(list, callback);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private class ThreadPrintNote extends Thread{
        private int headHigh=20;
        private List<PrintDataObject> listPrint;
        private int tailHight=180;

        public ThreadPrintNote(int headHigh,List<PrintDataObject> list,int tailHight){
            this.headHigh=headHigh;
            this.listPrint=list;
            this.tailHight=tailHight;
        }
        public ThreadPrintNote(List<PrintDataObject> list){
            this.headHigh=20;
            this.listPrint=list;
            this.tailHight=180;
        }

        @Override
        public void run() {
            super.run();
            while(printDev==null){
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            try {
                printDev.spitPaper(headHigh);
                printDev.printText(listPrint,callback);
                printDev.spitPaper(tailHight);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    };
    // 倍高
    public void printDoubleHigh(View v) {
        PrintDataObject printDataObject = new PrintDataObject(
                getString(R.string.printer_textsize_normal));
        List<PrintDataObject> list = new ArrayList<PrintDataObject>();
        list.add(printDataObject);
        list.add(printDataObject);
        list.add(printDataObject);
        PrintDataObject printDataObject2 = new PrintDataObject(
                getString(R.string.printer_textsize_normal));
        printDataObject2.setText(getString(R.string.printer_texisize_higher));
        printDataObject2.setSpacing(PrintDataObject.SPACING.DOUBLE_HIGH);
        list.add(printDataObject2);
        list.add(printDataObject2);
        list.add(printDataObject2);
        try {
            printDev.printText(list, callback);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // 倍宽
    public void printDoubleWidth(View v) {
        PrintDataObject printDataObject = new PrintDataObject(
                getString(R.string.printer_textsize_normal));
        List<PrintDataObject> list = new ArrayList<PrintDataObject>();
        list.add(printDataObject);
        list.add(printDataObject);
        list.add(printDataObject);
        PrintDataObject printDataObject2 = new PrintDataObject(
                getString(R.string.printer_textsize_normal));
        printDataObject2.setText(getString(R.string.printer_textsize_wider));
        printDataObject2.setSpacing(PrintDataObject.SPACING.DOUBLE_WIDTH);
        list.add(printDataObject2);
        list.add(printDataObject2);
        list.add(printDataObject2);
        try {
            printDev.printText(list, callback);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // 倍高宽
    public void printDoubleHighWidth(View v) {
        PrintDataObject printDataObject = new PrintDataObject(
                getString(R.string.printer_textsize_normal));
        List<PrintDataObject> list = new ArrayList<PrintDataObject>();
        list.add(printDataObject);
        list.add(printDataObject);
        list.add(printDataObject);
        PrintDataObject printDataObject2 = new PrintDataObject(
                getString(R.string.printer_textsize_normal));
        printDataObject2.setText(getString(R.string.printer_higher_wider));
        printDataObject2.setSpacing(PrintDataObject.SPACING.DOUBLE_HIGH_WIDTH);
        list.add(printDataObject2);
        list.add(printDataObject2);
        list.add(printDataObject2);
        try {
            printDev.printText(list, callback);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 开启打印队列测试 author :wengtao@centerm.com time :2017-3-24 16:10
     */
    public void openQueue(View v) {
        try {
            if (printDev.setPrintQueue(true)) {
                showMessage(getString(R.string.open_queue_success));
            } else {
                showMessage(getString(R.string.open_queue_failed));
            }
        } catch (Exception e) {
            e.printStackTrace();
            showMessage(getString(R.string.open_queue_exception)
                    + e.getMessage());
        }
    }

    /**
     * 关闭打印队列测试 author :wengtao@centerm.com time :2017-3-24 16:10
     */
    public void closeQueue(View v) {
        try {
            if (printDev.setPrintQueue(false)) {
                showMessage(getString(R.string.close_queue_success));
            } else {
                showMessage(getString(R.string.close_queue_failed));
            }
        } catch (Exception e) {
            e.printStackTrace();
            showMessage(getString(R.string.close_queue_exception)
                    + e.getMessage());
        }
    }

/*
    public void printOneCodeFast(View v) {
        barStr = barCode.getText().toString();
        if (TextUtils.isEmpty(barStr)) {
            barStr = "1234567890123456789012";
        }
        try {
            printDev.printBarCodeFast(barStr, Constant.ALIGN.LEFT, callback);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
*/

/*
    public void printCodeType(View v) {
        try {
            printDev.printBarCodeExtend(codeStr, typeIndex, 600, 600,
                    Constant.ALIGN.CENTER, callback);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
*/

    @Override
    public void onDeviceConnected(AidlDeviceManager deviceManager) {
        try {
            printDev = AidlPrinter.Stub.asInterface(deviceManager
                    .getDevice(Constant.DEVICE_TYPE.DEVICE_TYPE_PRINTERDEV));
            dialog.dismiss();
            if(debitRecord!=null) {
                new ThreadPrintNote(V8Printer.printPayNote(debitRecord)).start();
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }
}
