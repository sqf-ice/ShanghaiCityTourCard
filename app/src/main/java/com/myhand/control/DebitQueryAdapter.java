package com.myhand.control;

import android.content.Context;
import android.content.Intent;
import android.database.DataSetObserver;
import android.graphics.Color;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.cpucard.DebitRecord;
import com.myhand.cpucard.SHTCCPUUserCard;
import com.myhand.shanghaicitytourcard.POSApplication;
import com.myhand.shanghaicitytourcard.R;
import com.myhand.shanghaicitytourcard.TextViewActivity;
import com.myhand.shtcdatafile.FHFileRecord;

import java.text.SimpleDateFormat;
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

        public TextView textViewInnerNo;
        public TextView textViewCardCounter;
        public TextView textViewTxnAttr;
        public TextView textViewCorpID;
        public TextView textViewPosID;
        public TextView textViewPosSeq;


        public TextView textViewBalanceBef;
        public TextView textViewAmount;

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
            holder.textViewInnerNo=(TextView)convertView.findViewById(R.id.textViewInnerNo) ;
            holder.textViewTxnAttr=(TextView)convertView.findViewById(R.id.textViewTxnAttr) ;
            holder.textViewCardNo = (TextView)convertView.findViewById(R.id.textViewFaceNo);

            holder.textViewCorpID=(TextView) convertView.findViewById(R.id.textViewCorpID);
            holder.textViewPosID=(TextView)convertView.findViewById(R.id.textViewPosID);
            holder.textViewPosSeq=(TextView) convertView.findViewById(R.id.textViewPosSeq);

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

        final DebitRecord record=listDebit.get(position);
        //根据状态设定流水号字体颜色
        holder.textViewLocalSeg.setText(String.format("本地流水：%d",record.getLocalTxnSeq()));
        if(record.getStatus()==0x00) {
            holder.textViewLocalSeg.setTextColor(POSApplication.instance.getResources().getColor(R.color.red));
        }else{
            holder.textViewLocalSeg.setTextColor(POSApplication.instance.getResources().getColor(R.color.black));
        }
        holder.textViewCardNo.setText("卡面号："+
                SHTCCPUUserCard.CardFaceNum(HexUtil.hexStringToByte((record.getCardFaceNum().substring(8)))));

        holder.textViewInnerNo.setText("卡内号:"+record.getCardFaceNum());
        holder.textViewTxnAttr.setText(String.format("交易类型:%02X",record.getTxnAttr()));
        holder.textViewPosID.setText("Pos机号:"+record.getPosID());
        holder.textViewPosSeq.setText(String.format("POS流水号：%s",record.getPosSeq()));

        holder.textViewCorpID.setText(String.format("营运单位:%s",record.getCorpID()));
        holder.textViewBalanceBef.setText(String.format("消费前金额：%d",record.getBalanceBef()));
        holder.textViewAmount.setText(String.format("消费金额:%d",record.getAmount()));
        holder.textViewCardCounter.setText(String.format("卡计数器:%d",record.getTxnCounter()));
        holder.textViewTxnTime.setText("消费时间："+record.getTxnTime());

        //记录状态
        String caption=String.format("记录状态：%d",record.getStatus());
        if(record.getStatus()!=0&&record.getLastUploadTime()!=null){
            SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            caption+=String.format(" 上传时间:%s",sdf.format(record.getLastUploadTime()));
        }
        holder.textViewStatus.setText(caption);

/*
        //增加条目点击事件处理
        convertView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent();
                intent.setClass(context, TextViewActivity.class);

                FHFileRecord fileRecord=new FHFileRecord();
                fileRecord.fromDebitRecord(record);
                intent.putExtra("SrcData",fileRecord.getData());
                context.startActivity(intent);
            }
        });
*/

        return convertView;
    }
}
