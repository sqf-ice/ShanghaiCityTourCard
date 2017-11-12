package com.myhand.manage;

import android.content.Context;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.myhand.shanghaicitytourcard.R;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link SendDataFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link SendDataFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class SendDataFragment extends SettleFragment {
    private static final String tag=SendDataFragment.class.getSimpleName();
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private TextView textViewSendResult;
    private TextView textViewSendInfo;
    private TextView textViewConnectStatus;

    private OnFragmentInteractionListener mListener;

    public SendDataFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment SendDataFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static SendDataFragment newInstance(String param1, String param2) {
        SendDataFragment fragment = new SendDataFragment();
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
        View view= inflater.inflate(R.layout.fragment_send_data, container, false);
        init(view);
        return view;
    }

    public void init(View view){
        textViewSendResult=(TextView)view.findViewById(R.id.textViewSendResult);
        textViewSendInfo=(TextView)view.findViewById(R.id.textViewSendInfo);
        textViewConnectStatus=(TextView)view.findViewById(R.id.textViewConnectStatus);
    }

    public void showMessage(int index,int msgType,String messageText){
        int color=Color.rgb(0x00,0x00,0x00);
        if(getActivity()!=null&&getActivity().getResources()!=null) {
            switch (msgType) {
                case 0: {
                    color = getActivity().getResources().getColor(R.color.black);
                    break;
                }
                case 1: {
                    color = getActivity().getResources().getColor(R.color.red);
                    break;
                }
            }
        }
        switch (index){
            case 0:{
                //textViewConnectStatus.setTextColor(color);
                textViewConnectStatus.setText(messageText);
                break;
            }
            case 1:{
                //textViewSendInfo.setTextColor(color);
                textViewSendInfo.setText(messageText);
                break;
            }
            case 2:{
                //textViewSendResult.setTextColor(color);
                textViewSendResult.setText(messageText);
                break;
            }
            default:{
                //textViewConnectStatus.setTextColor(color);
                textViewConnectStatus.setText(messageText);
            }
        }

        Log.d(tag,String.format("Show message:index(%d) type(%d) message:%s",index,msgType,messageText));
    }

    @Override
    public void onResume() {
        super.onResume();
        textViewSendResult.setText("Test");
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
