package com.myhand.shanghaicitytourcard;

import android.app.Fragment;

/**
 * Created by vincent on 2017/11/11.
 */

public abstract class PayFragment extends Fragment {
    public abstract void ShowErrorMessage(int type,String errorMessage);
    public CardPayActivity getPayActivity(){
        return (CardPayActivity) getActivity();
    }
}
