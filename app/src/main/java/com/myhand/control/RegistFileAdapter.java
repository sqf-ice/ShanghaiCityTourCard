package com.myhand.control;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.myhand.POS.RegistFile;
import com.myhand.shanghaicitytourcard.R;

import java.text.SimpleDateFormat;
import java.util.List;

/**
 * Created by wenha_000 on 2017-10-13.
 */

public class RegistFileAdapter extends BaseAdapter{
    Context context;
    LayoutInflater inflater;
    List<RegistFile> registFileList;

    public RegistFileAdapter(Context context, List<RegistFile> registFileList) {
        this.context = context;
        inflater=LayoutInflater.from(context);

        this.registFileList = registFileList;
    }

    private static class ViewHolder{
        public TextView textViewFilename;
        public TextView textViewCreateTime;
        public TextView textViewUploadTime;
        public TextView textViewStatus;
    }

    @Override
    public int getCount() {
        return registFileList.size();
    }

    @Override
    public Object getItem(int position) {
        return registFileList.get(position);
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
            viewHolder.textViewFilename=(TextView) convertView.findViewById(R.id.textViewFilename);
            viewHolder.textViewFilename=(TextView) convertView.findViewById(R.id.textViewFilename);
            viewHolder.textViewFilename=(TextView) convertView.findViewById(R.id.textViewFilename);
            convertView.setTag(viewHolder);
        }else{
            viewHolder=(ViewHolder) convertView.getTag();
        }

        RegistFile registFile=(RegistFile) getItem(position);
        viewHolder.textViewFilename.setText(registFile.getFileName());
        SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        viewHolder.textViewCreateTime.setText(sdf.format(registFile.getCreateTime()));
        viewHolder.textViewUploadTime.setText(sdf.format(registFile.getUploadTime()));
        viewHolder.textViewStatus.setText(registFile.getStatus()>0?"未上传":"已上传");

        return convertView;
    }
}
