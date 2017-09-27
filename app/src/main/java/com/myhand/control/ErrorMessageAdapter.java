package com.myhand.control;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.myhand.ErrorMessage;
import com.myhand.shanghaicitytourcard.R;

import java.util.List;

/**
 * Created by wenha_000 on 2017-09-26.
 */

public class ErrorMessageAdapter extends BaseAdapter{
    private Context context;
    private LayoutInflater mInflater = null;

    private List<ErrorMessage> listErrorMessage;

    public ErrorMessageAdapter(Context context, List<ErrorMessage> listErrorMessage) {
        this.context = context;
        this.listErrorMessage = listErrorMessage;
        this.mInflater=LayoutInflater.from(context);
    }

    @Override
    public int getCount() {
        if(listErrorMessage==null){
            return 0;
        }
        return listErrorMessage.size();
    }

    @Override
    public Object getItem(int position) {
        if(listErrorMessage==null) {
            return null;
        }
        return listErrorMessage.get(position);
    }

    @Override
    public long getItemId(int position) {
        if(listErrorMessage==null) {
            return 0;
        }
        return position;
    }

    static class ViewHolder{
        public TextView textViewErrorCode;
        public TextView textViewErrorMessage;
    }
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder viewHolder;
        if(convertView==null){
            viewHolder=new ViewHolder();
            convertView=mInflater.inflate(R.layout.layout_item_errormessage,null);
            viewHolder.textViewErrorCode=(TextView)convertView.findViewById(R.id.textViewErrorCode);
            viewHolder.textViewErrorMessage=(TextView)convertView.findViewById(R.id.textViewErrorMessage);
            convertView.setTag(viewHolder);
        }else {
            viewHolder=(ViewHolder) convertView.getTag();
        }

        ErrorMessage errorMessage=(ErrorMessage) getItem(position);
        viewHolder.textViewErrorCode.setText(String.format("%04d",errorMessage.getErrorCode()));
        viewHolder.textViewErrorMessage.setText(errorMessage.getErrorMessage());
        switch (errorMessage.getErrorCode()){
            case 0:{
                viewHolder.textViewErrorMessage.setTextColor(Color.rgb(0,0,0));
                break;
            }
            default:{
                viewHolder.textViewErrorMessage.setTextColor(Color.rgb(255,0,0));
                break;
            }
        }
        return convertView;
    }
}
