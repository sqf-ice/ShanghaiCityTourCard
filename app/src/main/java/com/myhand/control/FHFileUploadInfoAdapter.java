package com.myhand.control;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.myhand.POS.RegistFile;
import com.myhand.shanghaicitytourcard.R;
import com.myhand.shtcdatafile.FHFileUploadInfo;

import java.text.SimpleDateFormat;
import java.util.List;

/**
 * Created by wenha_000 on 2017-10-13.
 */

public class FHFileUploadInfoAdapter extends BaseAdapter{
    private Context context;
    private LayoutInflater inflater;
    private List<FHFileUploadInfo> fhFileUploadInfoList;

    public FHFileUploadInfoAdapter(Context context, List<FHFileUploadInfo> fhFileUploadInfoList) {
        this.context = context;
        inflater=LayoutInflater.from(context);
        this.fhFileUploadInfoList = fhFileUploadInfoList;
    }

    private static class ViewHolder{
        public TextView textViewFilename;
        public TextView textViewCreateTime;
        public TextView textViewUploadTimes;
        public TextView textViewLoasUploadtTime;
    }

    @Override
    public int getCount() {
        return fhFileUploadInfoList.size();
    }

    @Override
    public Object getItem(int position) {
        return fhFileUploadInfoList.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder viewHolder;
        if(convertView==null){
            convertView=inflater.inflate(R.layout.layout_item_registfile,null);
            viewHolder=new ViewHolder();
            viewHolder.textViewFilename=(TextView) convertView.findViewById(R.id.textViewFilename);
            viewHolder.textViewCreateTime=(TextView) convertView.findViewById(R.id.textViewCreateTime);
            viewHolder.textViewLoasUploadtTime=(TextView) convertView.findViewById(R.id.textViewUploadTime);
            viewHolder.textViewUploadTimes=(TextView) convertView.findViewById(R.id.textViewStatus);
            convertView.setTag(viewHolder);
        }else{
            viewHolder=(ViewHolder) convertView.getTag();
        }

        FHFileUploadInfo fhFileUploadInfo=(FHFileUploadInfo) getItem(position);
        viewHolder.textViewFilename.setText(fhFileUploadInfo.getFileName());
        SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        viewHolder.textViewCreateTime.setText(sdf.format(fhFileUploadInfo.getCreateTime()));
        viewHolder.textViewUploadTimes.setText(String.format("%d",fhFileUploadInfo.getUploadTimes()));
        if(fhFileUploadInfo.getLastUploadTime()!=null) {
            viewHolder.textViewLoasUploadtTime.setText(sdf.format(fhFileUploadInfo.getLastUploadTime()));
        }else{
            viewHolder.textViewLoasUploadtTime.setText("");
        }
        return convertView;
    }
}
