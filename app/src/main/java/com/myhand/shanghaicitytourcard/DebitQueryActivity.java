package com.myhand.shanghaicitytourcard;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.ListView;
import android.widget.TextView;

import com.myhand.control.DebitQueryAdapter;
import com.myhand.cpucard.DebitRecord;

import java.util.List;

public class DebitQueryActivity extends AppCompatActivity {
    private TextView textViewRecordCount;

    private List<DebitRecord> debitList;
    private ListView listViewDebit;
    private DebitQueryAdapter adapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debit_query);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });

        listViewDebit=(ListView)findViewById(R.id.listViewDebitQuery);
        adapter=new DebitQueryAdapter(this);
        POSApplication application=(POSApplication)getApplication();
        debitList=application.getAppDatabase().debitQuery("",1);
        adapter.setListDebit(debitList);
        listViewDebit.setAdapter(adapter);

        textViewRecordCount=(TextView)findViewById(R.id.textViewRecordCount);
        textViewRecordCount.setText(String.format("共有%d条消费记录",debitList.size()));
    }

}
