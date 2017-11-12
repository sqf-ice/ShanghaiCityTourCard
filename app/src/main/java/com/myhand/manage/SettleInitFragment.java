package com.myhand.manage;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.myhand.shanghaicitytourcard.R;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link SettleInitFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link SettleInitFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class SettleInitFragment extends SettleFragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    private TextView textViewSettleSum;
    private Button buttonSettleOK;
    private Button buttonSettleCancel;
    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    public SettleInitFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment SettleInitFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static SettleInitFragment newInstance(String param1, String param2) {
        SettleInitFragment fragment = new SettleInitFragment();
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

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view= inflater.inflate(R.layout.fragment_settle_init, container, false);
        init(view);
        return view;
    }

    public void init(View view){
        textViewSettleSum=(TextView)view.findViewById(R.id.textViewSettleSum);
        //确定按钮事件
        buttonSettleOK=(Button)view.findViewById(R.id.buttonSettleOK);
        buttonSettleOK.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getSettleActivity().sendData();
            }
        });
        //取消按钮事件
        buttonSettleCancel=(Button)view.findViewById(R.id.buttonSettleCancel);
        buttonSettleCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getSettleActivity().setResult(0);
                getSettleActivity().finish();
            }
        });
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
        if(textViewSettleSum!=null){
            SettleSum sum=getSettleActivity().currSettleSum;
            textViewSettleSum.setText(String.format("消费总笔数：%d 总金额：%.2f元",sum.getCount(),(float)sum.getSum()/100f));
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
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
