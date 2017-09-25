package com.myhand.common;

import java.io.DataInputStream;
import java.util.ArrayList;
import java.util.List;

import android.R.integer;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

/**
 * 应用数据库类封装，利用SQLite管理应用数据
 * <p>在应用中调用该类时，必须对类的CONTEXT成员进行初始化，如：AppDatabase.CONTEXT=getApplicationContext();
 **/
public class AppDatabase {
    public int ErrorCode;
    public String errorMessageString;

    public String mDatabaseName = "AppDatabase";
    public int mDatabaseVersion = 1;
    public static final String TAG = "AppDatabase";
    public Context mContext;

    //table name define
    public static final String strServerSetTB="ServerSetTB";
    public static final String strErrorMessageTB="ErrorMessageTB";
    public static final String strConfigTable="ConfigTB";

    public static final String CREATE_SERVERSET_TABLE = String.format(
            "Create Table %s("
                    +"_id integer primary key autoincrement, "
                    +"ServerName varchar2(20) not null,"
                    +"SpaceName varchar2(50) not null,"
                    +"ServerURL varchar2(100) not null)"
            ,strServerSetTB);
    public static final String CREATE_ERRORMESSAGE_TABLE=String.format(
            "Create Table %s("
                    +"_id integer primary key autoincrement, "
                    +"ErrorCode int not null,"
                    +"ErrorMessage varchar2(50) not null,"
                    +"ErrorDescription varchar2(100) not null)"
            ,strErrorMessageTB);
    public static final String CREATE_CONFIG_TABLE=String.format(
            "Create Table %s("
                    +"_id integer primary key autoincrement,"
                    +"SectionName varchar2(30) not null,"
                    +"ValName varchar2(30) not null,"
                    +"value varchar2(100) not null)",strConfigTable);

    public AppDatabase(Context context,String databaseName,int version){
        mContext=context;
        mDatabaseName=databaseName;
        mDatabaseVersion=version;

        mDbHelper = new DatabaseHelper(context,databaseName,version);
        mDb = mDbHelper.getWritableDatabase();
    }
    /**
     * Context
     */
    private DatabaseHelper mDbHelper;
    public SQLiteDatabase mDb;
    /**
     * Inner private class. Database Helper class for creating and updating database.
     */
    private static class DatabaseHelper extends SQLiteOpenHelper {
        DatabaseHelper(Context context,String databaseName,int databaseVersion) {
            super(context, databaseName, null, databaseVersion);
        }
        /**
         * onCreate method is called for the 1st time when database doesn't exists.
         */
        @Override
        public void onCreate(SQLiteDatabase db) {
            //创建服务器设置表
            Log.d(TAG, "Creating DataBase: " + CREATE_SERVERSET_TABLE);
            db.execSQL(CREATE_SERVERSET_TABLE);

            //创建错误信息表
            db.execSQL(CREATE_ERRORMESSAGE_TABLE);

            //添加开发服务器设置
            String sqlInsert=String.format("Insert into %s values(0,'开发服务器',"
                    +"'http://tempuri.org/','http://192.168.1.102/AssetsSimpleSvr')",strServerSetTB);
            db.execSQL(sqlInsert);

            //创建应用配置信息表
            db.execSQL(CREATE_CONFIG_TABLE);
        }
        /**
         * onUpgrade method is called when database version changes.
         */
        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            Log.v(TAG, "Upgrading database from version " + oldVersion + " to "
                    + newVersion);
        }
    }

    /**
     * This method is used for creating/opening connection
     * @return instance of DatabaseUtil
     * @throws SQLException
     */
    public AppDatabase open(){
        mDbHelper = new DatabaseHelper(mContext,mDatabaseName,mDatabaseVersion);
        mDb = mDbHelper.getWritableDatabase();
        return this;
    }
    /**
     * This method is used for closing the connection.
     */
    public void close() {
        mDbHelper.close();
    }

    public int InsertServerSet(String serverName,String serverNamespace,String serviceURL)
    {
        try {
            String sqlInsert=String.format("Insert into %s values(null,'%s','%s','%s')"
                    ,strServerSetTB,serverName,serverNamespace,serviceURL);
            mDb.execSQL(sqlInsert);
            return 0;

        } catch (SQLiteException e) {
            // TODO: handle exception
            ErrorCode=1;
            errorMessageString=e.getMessage();
            return ErrorCode;
        }
    }

    public boolean ExecCommand(String sqlCommand) {
        try {
            mDb.execSQL(sqlCommand);
        } catch (Exception e) {
            // TODO: handle exception
            errorMessageString=e.getMessage();
            Log.e(TAG, errorMessageString);
            return false;
        }

        return true;
    }

    public Cursor ExecQuery(String sqlSelect) {
        try {
            return mDb.rawQuery(sqlSelect, null);
        } catch (Exception e) {
            // TODO: handle exception
            errorMessageString=e.getMessage();
            return null;
        }
    }

    public boolean UpdateServerSet(int id,String serverName,String serverNamespace,String serviceURL) {
        String sqlUpdateString=String.format("Update %s set servername='%s'\n"
                        +"namespace='%s' serverURL='%s'\n"
                        +"where _id=%d"
                ,strServerSetTB, serverName
                ,serverNamespace,serviceURL
                ,id);
        try {
            mDb.execSQL(sqlUpdateString);
        } catch (SQLiteException e) {
            // TODO: handle exception
            ErrorCode=1;
            errorMessageString=e.toString();
            return false;
        }

        return true;
    }


    //Get Error Message defined by errorCod
    public String GetErrorMessage(int errorCode) {
        String sqlSelect=String.format("Select * from %s where ErrorCode='%d'"
                , strErrorMessageTB,errorCode);
        Log.v("ErrorCode Query",sqlSelect);
        try {
            Cursor cursor=mDb.rawQuery(sqlSelect, null);
            if(cursor==null
                    ||cursor.getCount()<=0)
            {
                Log.v("ErrorCode Query","No defined.");
                return String.format("Error Code:%04d Error Message:Unknown error."
                        ,errorCode);
            }

            cursor.moveToFirst();
            return String.format("ErrorCode:%04d Message:%s"
                    ,errorCode,cursor.getString(2));
        } catch (SQLiteException e) {
            // TODO: handle exception
            Log.v("ErrorCode Query","SQLiteException");
            return String.format("ErrorCode:%4d Message:%s"
                    ,errorCode, e.getMessage());
        }
    }

    //获取配置变量字符串值
    public String GetIniString(String sectionName,String valName,String defaultValue) {
        String sqlSelect=String.format("Select value from %s where sectionName='%s' COLLATE NOCASE"
                        +" and ValName='%s' COLLATE NOCASE"
                , strConfigTable,sectionName,valName);

        Cursor cursor=mDb.rawQuery(sqlSelect,null);
        if(cursor==null
                ||cursor.getCount()<=0)
        {
            return defaultValue;
        }

        cursor.moveToFirst();
        Log.v("Database",cursor.getString(0));
        return cursor.getString(0);
    }

    public int GetIniInt(String sectionName,String valName,int defaultValue) {
        try {
            return Integer.parseInt(GetIniString(sectionName, valName,String.format("%d", defaultValue)));
        } catch (Exception e) {
            // TODO: handle exception
            return 0;
        }
    }

    public void WriteIniString(String sectionName,String valName,String value) {
        String sqlSelect=String.format("Select * from %s"
                        +" where sectionName='%s' COLLATE NOCASE"
                        +" and ValName='%s' COLLATE NOCASE"
                , strConfigTable,sectionName,valName);
        Cursor cursor=mDb.rawQuery(sqlSelect,null);
        if(cursor==null
                ||cursor.getCount()<=0)//No record find
        {
            String sqlInsert=String.format("Insert into %s values(null,'%s','%s','%s')"
                    , strConfigTable,sectionName,valName,value);
            mDb.execSQL(sqlInsert);
        }
        else {
            String sqlUpdate=String.format("Update %s set value='%s'"
                            +" where sectionName='%s' COLLATE NOCASE and valName='%s' COLLATE NOCASE"
                    , strConfigTable,value,sectionName,valName);
            mDb.execSQL(sqlUpdate);
        }
    }

    public void WriteIniInt(String sectionName,String valName,int value) {
        WriteIniString(sectionName, valName, String.format("%d",value));
    }

    /**
     * <p>获取所有表名</p>
     */
    public Cursor GetTablesName() {
        String selectTable=String.format("select 0 as _id,name from sqlite_master "
                +"where type ='table'");
        Cursor cursor=mDb.rawQuery(selectTable, null);
        return cursor;
    }

    /**
     * <p>获取表名列表
     * @return
     */
    public ArrayList<String> GetTablesNameList() {
        Cursor cursor=GetTablesName();
        ArrayList<String> tableNames=new ArrayList<String>();
        cursor.moveToFirst();
        for(int i=0;i<cursor.getCount();i++)
        {
            tableNames.add(cursor.getString(1));
            cursor.moveToNext();
        }

        return tableNames;
    }

    /**
     * <p>获取表记录数
     */
    public int GetTableRecordCnt(String tableName) {
        String sqlSelect=String.format("Select count(*) from %s", tableName);
        Cursor cursor=mDb.rawQuery(sqlSelect,null);

        return cursor.getCount();
    }
    //检查表是否存在
    public boolean IfTableExsit(String tablename) {

        String selectTable=String.format("select count(*) as c from sqlite_master "
                +"where type ='table' and name ='%s'",tablename);
        Cursor cursor=mDb.rawQuery(selectTable, null);

        return !(cursor==null || cursor.getCount()<=0);
    }

    //删除表
    public void DropTable(String tablename)
    {
        String dropTable=String.format("Drop table %s", tablename);

        mDb.execSQL(dropTable);
    }

    //查询表结构
    public Cursor GetTableFields(String tablename) {
        String sqlSelect=String.format("select * from sqlite_master"
                        +" where type='table' and name='%s'",
                tablename);

        return ExecQuery(sqlSelect);
    }
    public void deleteSection(String sectionName) {
        String sqlDelete=String.format("Delete from %s where sectionName='%s' COLLATE NOCASE",
                strConfigTable,sectionName);
        ExecCommand(sqlDelete);
    }

    public void deleteSectionItem(String sectionName,String itemName) {
        String sqlDelete=String.format("Delete from %s where sectionName='%s' COLLATE NOCASE"
                        +" and valName='%s' COLLATE NOCASE"
                , strConfigTable,sectionName,itemName);
        ExecCommand(sqlDelete);
    }
    public List<String> getSectionItems(String sectionName) {
        String sqlSelect=String.format("Select * from %s where sectionname='%s'",
                strConfigTable,sectionName);
        Cursor cursor=mDb.rawQuery(sqlSelect, null);
        if(cursor==null||cursor.getCount()<=0)
        {
            return null;
        }
        List<String> list=new ArrayList<String>();
        cursor.moveToFirst();
        for(int i=0;i<cursor.getCount();i++)
        {
            list.add(cursor.getString(2));
            cursor.moveToNext();
        }

        return list;
    }
}
