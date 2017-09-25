package com.myhand.control;

import android.content.Context;
import android.database.DataSetObserver;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.myhand.cpucard.DebitRecord;
import com.myhand.shanghaicitytourcard.R;

import java.util.List;
import java.util.zip.Inflater;

/**
 * Created by wenha_000 on 2017-09-16.
 */

public class DebitQueryAdapter extends BaseAdapter {
    private Context context;
    private LayoutInflater mInflater = null;
    public DebitQueryAdapter(Context context)
    {
        this.context = context;
        this.mInflater = LayoutInflater.from(context);
    }

    private List<DebitRecord> listDebit;

    public List<DebitRecord> getListDebit() {
        return listDebit;
    }

    public void setListDebit(List<DebitRecord> listDebit) {
        this.listDebit = listDebit;
    }

    @Override
    public int getCount() {
        if(listDebit==null)
            return 0;
        return listDebit.size();
    }

    @Override
    public Object getItem(int position) {
        if(listDebit==null)
            return null;
        return listDebit.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    static class ViewHolder
    {
        public TextView textViewLocalSeg;
        public TextView textViewCardNo;
        public TextView textViewBalanceBef;
        public TextView textViewAmount;
        public TextView textViewCardCounter;
        public TextView textViewTxnTime;
        public TextView textViewStatus;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
       ViewHolder holder;
        if(convertView == null)
        {
            holder = new ViewHolder();
            convertView = mInflater.inflate(R.layout.item_debitquery, null);
            holder.textViewLocalSeg = (TextView) convertView.findViewById(R.id.textViewLocalSeq);
            holder.textViewCardNo = (TextView)convertView.findViewById(R.id.textViewFaceNo);
            holder.textViewBalanceBef = (TextView)convertView.findViewById(R.id.textViewBalanceBef);
            holder.textViewAmount = (TextView)convertView.findViewById(R.id.textViewAmount);
            holder.textViewCardCounter=(TextView)convertView.findViewById(R.id.textViewCardCounter) ;
            holder.textViewTxnTime=(TextView)convertView.findViewById(R.id.textViewTxnTime) ;
            holder.textViewStatus = (TextView)convertView.findViewById(R.id.textViewStatus);
            convertView.setTag(holder);
        }else
        {
            holder = (ViewHolder)convertView.getTag();
        }

        DebitRecord record=listDebit.get(position);
        holder.textViewLocalSeg.setText(String.format("本地流水：%d",record.getLocalTxnSeq()));
        holder.textViewCardNo.setText("卡号："+record.getCardFaceNum());
        holder.textViewBalanceBef.setText(String.format("消费前金额：%d",record.getBalanceBef()));
        holder.textViewAmount.setText(String.format("消费金额:%d",record.getAmount()));
        holder.textViewCardCounter.setText(String.format("卡计数器:%s",record.getTxnCounter()));
        holder.textViewTxnTime.setText("消费时间："+record.getTxnTime());
        holder.textViewStatus.setText(String.format("记录状态：%d",record.getStatus()));

        return convertView;
    }
}