package com.myhand.citytourcar;

import android.content.Context;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.shanghaicitytourcard.CityTourCard;
import com.myhand.shanghaicitytourcard.PayFragment;
import com.myhand.shanghaicitytourcard.R;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link CardPayFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link CardPayFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class CardPayFragment extends PayFragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private EditText editTextPayAmount;
    private Button buttonPay;
    private Button buttonCancel;
    private TextView textViewErrorMessage;
    private TextView textViewBalance;

    private OnFragmentInteractionListener mListener;

    public CardPayFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment CardPayFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static CardPayFragment newInstance(String param1, String param2) {
        CardPayFragment fragment = new CardPayFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }

    }

    private void init(View view){
        editTextPayAmount=(EditText) view.findViewById(R.id.editTextPayAmount);
        textViewErrorMessage=(TextView)view.findViewById(R.id.textViewErrorMessage);
        textViewBalance=(TextView)view.findViewById(R.id.textviewBalance) ;

        buttonPay=(Button)view.findViewById(R.id.buttonPay);
        buttonPay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ShowErrorMessage(0,"");
                CityTourCard card= getPayActivity().card;
                if(card==null){
                    ShowErrorMessage(1,"请刷卡......");
                    return;
                }
                //卡类型判断
                if(card.getTypeIn05()<0x48||card.getTypeIn05()>0x72){
                    ShowErrorMessage(1,String.format("不支持卡类型在（0x48~0x72）之外的卡消费，当前卡类型为：%02X",card.getTypeIn05()));
                    return;
                }
                //卡有效期判断
                String valideDateStr=HexUtil.bytesToHexString(card.getFCIValidData().getValidDate());
                SimpleDateFormat sdf=new SimpleDateFormat("yyyyMMdd");
                try {
                    Date valideDate=sdf.parse(valideDateStr);
                    Date now=new Date();
                    if(valideDate.compareTo(now)<0){
                        ShowErrorMessage(1,String.format("过有效期，请去续期"));
                        return;
                    }
                } catch (ParseException e) {
                    e.printStackTrace();
                    ShowErrorMessage(1,String.format("有效期转换失败：%s",e.getLocalizedMessage()));
                    return;
                }

                String payAmountStr=editTextPayAmount.getText().toString();
                if(payAmountStr.isEmpty()){
                    ShowErrorMessage(1,"请输入消费金额");
                    return;
                }
                float payAmount=Float.parseFloat(payAmountStr);
                if(payAmount<=0){
                    ShowErrorMessage(1,"消费金额必须大于0");
                    return;
                }
                if(payAmount*100>getPayActivity().card.getBalance()){
                    ShowErrorMessage(1,"卡余额不足");
                    return;
                }

//                getPayActivity().printPayNote();
                getPayActivity().showPayPrompt();
/*
                if(getPayActivity().posDevice.complexDebit(getPayActivity().card,(int)(payAmount*100))==null)
                {
                    ShowErrorMessage(1,String.format("卡消费失败：%s",
                            getPayActivity().posDevice.getErrorMessage()));
                }
                else{
                    ShowErrorMessage(0,String.format("成功消费%f元",payAmount));
                }
*/
            }
        });

        buttonCancel=(Button)view.findViewById(R.id.buttonCancel);
        buttonCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getActivity().finish();
            }
        });
    }

    public void showBalance(int amount){
        if(textViewBalance!=null) {
            textViewBalance.setText(String.format("%.2f", (float) amount / 100.0));
            textViewBalance.setText(String.format("%d",amount));
        }
    }

    public void setPayAmount(int amount){
        editTextPayAmount.setText(String.format("%d",amount));
    }

    public float getPayAmount(){
        if(editTextPayAmount==null){
            return 0;
        }

        String payAmountStr=editTextPayAmount.getText().toString();
        if(payAmountStr.isEmpty()) {
            return 0;
        }

        return Float.parseFloat(payAmountStr);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view=inflater.inflate(R.layout.fragment_card_pay, container, false);
        init(view);
        return view;
    }

    // TODO: Rename method, update argument and hook method into UI event
    public void onButtonPressed(Uri uri) {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        if(getPayActivity().card!=null) {
            showBalance(getPayActivity().card.getBalance());
            if(getPayActivity().card!=null) {
                ShowErrorMessage(0,String.format("卡类型：%02X 有效期：%s",
                        getPayActivity().card.getFCIValidData().getCardType(),
                        HexUtil.bytesToHexString(getPayActivity().card.getFCIValidData().getValidDate())));
            }
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    @Override
    public void ShowErrorMessage(int type, String errorMessage) {
        int color= Color.rgb(0xFF,0x00,0x00);
        switch (type){
            case 0:
            {
                color=Color.rgb(0x00,0xFF,0x00);
                break;
            }
            case 1:{
                color= Color.rgb(0xFF,0x00,0x00);
                break;
            }
            default:{
                color=Color.rgb(0x00,0x00,0xFF);
                break;
            }
        }

        if(textViewErrorMessage!=null) {
            textViewErrorMessage.setTextColor(color);
            textViewErrorMessage.setText(errorMessage);
        }
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }
}
