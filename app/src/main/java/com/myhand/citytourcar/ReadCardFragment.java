package com.myhand.citytourcar;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.myhand.shanghaicitytourcard.PayFragment;
import com.myhand.shanghaicitytourcard.R;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link Pay_Rdcard_Prompt_Fragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link Pay_Rdcard_Prompt_Fragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class ReadCardFragment extends PayFragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    private TextView textViewPrompt;
    private ProgressBar progressBarReadCard;

    private LinearLayout linearLayoutButton;
    private Button buttonCancel;

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    public ReadCardFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment Pay_Rdcard_Prompt_Fragment.
     */
    // TODO: Rename and change types and number of parameters
    public static ReadCardFragment newInstance(String param1, String param2) {
        ReadCardFragment fragment = new ReadCardFragment();
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
        View view=inflater.inflate(R.layout.fragment_pay__rdcard__prompt_, container, false);
        init(view);
        return view;
    }

    public void init(View view){
        textViewPrompt=(TextView)view.findViewById(R.id.textViewPrompt);
        buttonCancel=(Button)view.findViewById(R.id.buttonCancel);
        buttonCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

            }
        });
        linearLayoutButton=(LinearLayout)view.findViewById(R.id.linearLayoutButton);
    }

    public void showButtonLinearLayout(boolean visible){
        linearLayoutButton.setVisibility(visible?View.VISIBLE:View.INVISIBLE);
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

    String errorMessage="请将卡片置于示读区......";
    @Override
    public void ShowErrorMessage(int type, String errorMessage) {
        if(this.errorMessage.compareTo(errorMessage)==0){
            return;
        }
        this.errorMessage=errorMessage;
        final String messageText=errorMessage;
/*
        getPayActivity().handler.post(new Runnable() {
            @Override
            public void run() {
                if(textViewPrompt!=null){
                    textViewPrompt.setText(messageText);
                }
            }
        });
*/
    }

    @Override
    public void onResume() {
        super.onResume();
        if(errorMessage!=null&&textViewPrompt!=null){
            textViewPrompt.setText(errorMessage);
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
