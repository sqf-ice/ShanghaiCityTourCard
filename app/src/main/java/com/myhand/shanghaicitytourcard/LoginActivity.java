package com.myhand.shanghaicitytourcard;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.myhand.POS.User;
import com.myhand.devices.V8Sounder;
import com.myhand.manage.AdminActivity;

public class LoginActivity extends AppCompatActivity {
    private static final String tag=LoginActivity.class.getSimpleName();

    private static final int REQ_ADMIN=1;
    private EditText editTextUsername;
    private EditText editTextPassword;
    private Button buttonLogin;
    private Button buttonCancel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
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

        editTextUsername=(EditText)findViewById(R.id.editTextUsername);
        editTextPassword=(EditText)findViewById(R.id.editTextPassword);
        buttonLogin=(Button)findViewById(R.id.buttonLogin);
        buttonLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String username=editTextUsername.getText().toString();
                if(username.isEmpty()){
                    editTextUsername.setHint("请输入用户名");
                    V8Sounder sounder=(V8Sounder) POSApplication.instance.getPosDevice().getSounder();
                    if(sounder!=null){
                        sounder.inputPwd();
                    }
                    return;
                }
                String password=editTextPassword.getText().toString();
                if(password.isEmpty()){
                    editTextPassword.setHint("请输入密码");
                    V8Sounder sounder=(V8Sounder) POSApplication.instance.getPosDevice().getSounder();
                    if(sounder!=null){
                        sounder.inputPwd();
                    }
                    return;
                }
                User user=POSApplication.instance.getAppDatabase().userLogin(username,password);
                if(user==null){
                    //无效的用户名
                    editTextPassword.setText("");
                    editTextUsername.setText("");
                    editTextUsername.setHint("请输入正确的用户名和密码");
                    V8Sounder sounder=(V8Sounder) POSApplication.instance.getPosDevice().getSounder();
                    if(sounder!=null){
                        sounder.inputPwd();
                    }
                    return;
                }

                if(user.getReloID()>0){
                    Intent intent=new Intent();
                    intent.setClass(LoginActivity.this, AdminActivity.class);
                    startActivityForResult(intent,REQ_ADMIN);
                    return;
                }

                POSApplication.instance.setUser(user);
                finish();
            }
        });

        buttonCancel=(Button)findViewById(R.id.buttonCancel);
        buttonCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }
}
