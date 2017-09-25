package com.myhand.devices;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.util.Log;

/**
 * Created by wenha_000 on 2017-09-22.
 */

public class DataExchangeService extends Service {
    private static final String tag="数据交换";
    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(tag,"服务创建成功");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }
}
