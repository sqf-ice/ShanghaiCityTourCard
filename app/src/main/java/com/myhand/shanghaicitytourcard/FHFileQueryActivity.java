package com.myhand.shanghaicitytourcard;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;

import com.myhand.POS.DatabaseSHCT;
import com.myhand.POS.FileMaker;
import com.myhand.POS.RegistFile;
import com.myhand.control.FHFileUploadInfoAdapter;
import com.myhand.control.RegistFileAdapter;
import com.myhand.shtcdatafile.FHFileUploadInfo;

import java.util.List;

public class FHFileQueryActivity extends AppCompatActivity {
    private ListView listViewFHFileUploadInfo;
    private List<FHFileUploadInfo> fhFileUploadInfoList;
    private FHFileUploadInfoAdapter fhFileUploadInfoAdapter;

    private Button btnQueryRegistFile;
    private Button btnFHFileMake;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fhfile_query);
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

        btnQueryRegistFile=(Button)findViewById(R.id.buttonRegistFileQuery);
        btnQueryRegistFile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                DatabaseSHCT db=POSApplication.instance.getAppDatabase();
                fhFileUploadInfoList=db.getFHFileList("");

                fhFileUploadInfoAdapter=new FHFileUploadInfoAdapter(FHFileQueryActivity.this,fhFileUploadInfoList);
                listViewFHFileUploadInfo.setAdapter(fhFileUploadInfoAdapter);
            }
        });

        listViewFHFileUploadInfo=(ListView)findViewById(R.id.listViewRegistFile);

        btnFHFileMake=(Button)findViewById(R.id.buttonMakeFHFile);
        btnFHFileMake.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                FileMaker.FHFileMake();
            }
        });
    }


}
