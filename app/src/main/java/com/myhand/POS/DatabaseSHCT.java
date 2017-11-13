package com.myhand.POS;

import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import com.centerm.smartpos.util.HexUtil;
import com.myhand.common.AppDatabase;
import com.myhand.cpucard.DebitRecord;
import com.myhand.manage.SettleSum;
import com.myhand.shtcdatafile.FHFileUploadInfo;

import java.sql.SQLData;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by wenha_000 on 2017-09-06.
 */

//设备端数据库
public class DatabaseSHCT extends AppDatabase{
    private static String tag=DatabaseSHCT.class.getSimpleName();

    private String tbBlkCard="TBBlackCard";
    private String TBDebit="TBDebit";

    /**
     * 结算表
     */
    private String tbSettle="TBSettle";
    private String sqlCreateTBSettle=String.format("Create Table %s(\n"
            +"id integer primary key autoincrement,\n"
            +"PatchNo integer not null default 0,\n"
            +"SettleTime date not null default now,\n"
            +"Username varchar2(20) not null,\n"
            +"CorpID varchar2(20) not null,\n"
            +"PosID varchar2(20) not null,\n"
            +"count integer not null default 0,\n"
            +"sum integer not null default 0,\n"
            +"starttime date not null,\n"
            +"endtime date not null)",tbSettle);

    /**
     * @param settleSum
     * @return
    result+=String.format("strftime(%s))",sdf.format(debitRecord.getLastUploadTime()));

     */
    public boolean insertSettle(SettleSum settleSum){
        SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

        String sqlInsert=String.format("Insert into %s values(\n"
                +"null,%d,strftime('%s'),'%s','%s','%s',"
                +"%d,%d,"
                +"strftime('%s'),strftime('%s'))"
                ,tbSettle
                ,settleSum.getPatchNo(),sdf.format(settleSum.getSettleTime()),settleSum.getCorpID(),settleSum.getPosID()
                ,settleSum.getCount(),settleSum.getSum()
                ,sdf.format(settleSum.getStartTime()),sdf.format(settleSum.getEndTime()));

        return ExecCommand(sqlInsert);
    }

    /**
     * 用户表
     */
    private String tbUser="TBUser";
    private String sqlCreateTBUser=String.format("Create Table %s(\n"
            +"username varchar2(20) not null primary key,\n"
            +"password varchar2(20) not null,\n"
            +"roleID integer not null default 0,\n"
            +"status integer not null default 0)",tbUser);

    public boolean insertUser(User user){
        String sqlInsert=String.format("Insert into %s values('%s','%s',%d,%d)",
                tbUser,user.getUsername(),user.getPassword(),user.getReloID(),user.getStatus());
        return ExecCommand(sqlInsert);
    }

    public User userLogin(String username,String password){
        String sqlSelectUser=String.format("Select * from %s\n"
                +"where username='%s' and password='%s' and status>0",tbUser,username,password);

        Cursor cursor=ExecQuery(sqlSelectUser);
        if(cursor==null||cursor.getCount()==0){
            return null;
        }

        cursor.moveToFirst();
        User result=new User();
        result.setUsername(cursor.getString(0));
        result.setPassword(cursor.getString(1));
        result.setReloID(cursor.getInt(2));
        result.setStatus(cursor.getInt(3));

        return result;
    }
    /**
     * 消费文件注册信息表
     */
    private String TBFHFileRegist="TBFHFileRegist";
    private String SQLCreateTBFHFileRegist=String.format("Create Table %s(\n"
            +"FileName varchar(30) not null primary key,"
            +"CreateTime date not null default now,"
            +"UploadTimes decimal(4) not null default 0,"
            +"LastUploadTime date)",TBFHFileRegist);

    public DatabaseSHCT(Context context, String databaseName, int version) {
        super(context, databaseName, version);
        // TODO Auto-generated constructor stub
    }


    public String sqlInsertDebit(DebitRecord debitRecord)
    {
        String result="Insert into TBDebit values(null,"
                +String.format("'%s',",debitRecord.getCorpID())
                +String.format("%d,",debitRecord.getTxnAttr())
                +String.format("'%s',",debitRecord.getStationID())
                +String.format("'%s',",debitRecord.getOprID())
                +String.format("'%s',",debitRecord.getBusID())
                +String.format("%d,",debitRecord.getTxnType())
                +String.format("'%s',",debitRecord.getPosSeq())
                +String.format("'%s',",debitRecord.getCityCode())
                +String.format("'%s',",debitRecord.getCardFaceNum())
                +String.format("%02d,",debitRecord.getCardKind())
                +String.format("%d,",debitRecord.getBalanceBef())
                +String.format("%d,",debitRecord.getAmount())
                +String.format("'%s',",debitRecord.getTxnTime())
                +String.format("%d,",debitRecord.getTxnCounter())
                +String.format("'%s',",debitRecord.getPosID())
                +String.format("'%s',",debitRecord.getTac())
                +String.format("%d,0,",debitRecord.getCardVerNo());
                if(debitRecord.getLastUploadTime()==null){
                    result+=String.format("null)");
                }else{
                    SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    result+=String.format("strftime(%s))",sdf.format(debitRecord.getLastUploadTime()));
                }
        return result;
    }

    public void init()
    {
        Log.d(tag,"Create table");
        Log.d(tag,SQLCreateTableDEBIT);
        if(!ExecCommand(SQLCreateTableDEBIT))
        {
            Log.d(tag,"Create Table Debit failure.");
        }
        Log.d(tag,SQLCreatTableBLKCARD);
        if(!ExecCommand(SQLCreatTableBLKCARD))
        {
            Log.d(tag,"Create Table Blk Card failure");
        }
        if(!ExecCommand(sqlCreateAppInfor))
        {
            Log.d(tag,"创建POS信息表失败");
        }

        if(!ExecCommand(sqlCreateTBUser)){
            Log.d(tag,"Create User Table failure");
        }
        User user=new User("admin","123456",1,1);
        insertUser(user);
        user=new User("1001","123456",0,1);
        insertUser(user);

        ExecCommand(sqlCreateTBSettle);
    }

    public static final String TB_PosInfo="TBPosInfo";
    public static final String sqlCreateAppInfor=String.format("Create Table %s(PosID varchar(20) not null primary key,"
            +"sequence decimal(8) not null default 0,"
            +"sendSequence decimal(8) not null default 0)",TB_PosInfo);

    /**
     *
     * @param posID
     * @return
     */
    public long getPosSendSequence(String posID)
    {
        String sqlSelect=String.format("Select sendSequence from %s where PosID='%s'",TB_PosInfo,posID);
        Cursor cursor=ExecQuery(sqlSelect);
        long result=0;
        if(cursor==null||cursor.getCount()==0)
        {
            //没有找到记录
            String sqlInsert=String.format("Insert into %s values('%s',0,1)",TB_PosInfo,posID);
            if(!ExecCommand(sqlInsert))
            {
                Log.d(tag,"插入POS记录失败");
            }
            result=0;
        }else{
            cursor.moveToNext();
            result=cursor.getLong(0);
            String sqlUpdate=String.format("Update %s set SendSequence=%d where PosID='%s'",TB_PosInfo,result+1,posID);
            if(!ExecCommand(sqlUpdate))
            {
                Log.d(tag,"更新POS发送流水号失败");
            }
        }

        return result;
    }

    //建表脚本、
    /**
     *  CorpId	N11	营运单位代码
     LocalTxnSeq	N8	本地流水号
     TxnAttr	N2	交易性质	00-99
     StationId	N6	采集点编号
     PosOprId	N16	POS操作员编号	缺省值为全0
     PosCarrId	N6	POS机载体编号	公交车线路号
     TxnType	N2	交易类型	88/99/98/87/84/83/48/49/6B/6C/6E/81
     PosSeq	N12	POS机流水号
     CityCode	N4	城市代码
     CardFaceNum	N12	卡面号
     CardKind	N2	卡类型	00-99
     BalBef	N8	消费前卡余额
     TxnAmt	N8	交易金额
     TxnDate 	YYYYMMDD	交易发生日期
     TxnTime	hhmmss	交易发生时间
     TxnCounter	N6	交易计数器
     PosId	N8	POS机号
     TAC 	H8	交易认证码
     CardVerNo	N2	卡内版本号	当作地域区分标志

     */
    private static final String SQLCreatTableBLKCARD="Create Table TBBlackCard(CardNO varchar(20) primary key,"
            +"level int not null,"
            +"Status int not null default 1,"
            +"UpdateTime datetime not null default now)";

    /**
     *  CorpId	N11	营运单位代码
     LocalTxnSeq	N8	本地流水号
     TxnAttr	N2	交易性质	00-99
     StationId	N6	采集点编号
     PosOprId	N16	POS操作员编号	缺省值为全0
     PosCarrId	N6	POS机载体编号	公交车线路号
     TxnType	N2	交易类型	88/99/98/87/84/83/48/49/6B/6C/6E/81
     PosSeq	N12	POS机流水号
     CityCode	N4	城市代码
     CardFaceNum	N12	卡面号
     CardKind	N2	卡类型	00-99
     BalBef	N8	消费前卡余额
     TxnAmt	N8	交易金额
     TxnDate 	YYYYMMDD	交易发生日期
     TxnTime	hhmmss	交易发生时间
     TxnCounter	N6	交易计数器
     PosId	N8	POS机号
     TAC 	H8	交易认证码
     CardVerNo	N2	卡内版本号	当作地域区分标志
     */
    private static final  String SQLCreateTableDEBIT="Create Table TBDebit(id integer primary key autoincrement,"
            +"corpID varchar(11) not null,"
            +"txnAttr decimal(3) not null,"
            +"stationID varchar(6) not null,"
            +"posOprID varchar(16) not null,"
            +"posCarrID varchar(6) not null,"
            +"txnType decimal(3) not null,"
            +"posSeq decimal(12) not null,"
            +"cityCode varchar(4) not null,"
            +"CardFaceNum varchar(12) not null,"
            +"CardKind decimal(3) not null,"
            +"BalanceBef decimal(8) not null,"
            +"TxnAmt decimal(8) not null,"
            +"TxnTime varchar(14) not null,"
            +"TxnCounter decimal(6) not null,"
            +"PosID varchar(8) not null,"
            +"TAC varchar(8) not null,"
            +"CardVerNo decimal(2) not null,"
            +"Status decimal(2) not null default 0,"
            +"LastUploadTime date)";

    /**
     * 添加黑卡记录
     * @param cardNo
     * @return
     */
    public boolean insertBlkCard(String cardNo,int level)
    {
        String sqlInsert=String.format("Insert into TBBlackCard(CardNo,Level) values('%s',%d)",cardNo,level);
        if(!ExecCommand(sqlInsert))
        {
            Log.d(tag,"Insert black card "+cardNo+" failure.");
            return false;
        }

        return true;
    }

    /**
     * 判断是否为黑卡
     * @param cardNo
     * @return
     */
    public boolean isBlackCard(String cardNo)
    {
        String sqlSelect=String.format("Select * from %s where CardNo='%s'",tbBlkCard,cardNo);
        Cursor cursor= ExecQuery(sqlSelect);
        if(cursor.getCount()==0)
        {
            return false;
        }

        return true;
    }

    /**
     * 保存消费记录
     * @param debitRecord
     * @return
     */
    public boolean saveDebit(DebitRecord debitRecord)
    {
        String sqlInsert=sqlInsertDebit(debitRecord);
        Log.d(tag,sqlInsert);
        return ExecCommand(sqlInsert);
    }

    //查询消费记录
    public List<DebitRecord> debitQuery(String condition,int order)
    {
        List<DebitRecord> result=new ArrayList<DebitRecord>();

        String sqlSelect=String.format("Select * from %s ",TBDebit);
        if(condition!=null&&(!condition.isEmpty()))
        {
            sqlSelect+=condition;
        }
        if(order>0){
            sqlSelect+=String.format("\norder by id desc");
        }

        Cursor cursor=ExecQuery(sqlSelect);
        while (cursor.moveToNext())
        {
            DebitRecord record=new DebitRecord();

            record.setLocalTxnSeq(cursor.getLong(0));
            record.setCorpID(cursor.getString(1));
            record.setTxnAttr((byte)cursor.getInt(2));
            record.setStationID(cursor.getString(3));
            record.setOprID(cursor.getString(4));
            record.setBusID(cursor.getString(5));
            record.setTxnType((byte)cursor.getInt(6));
            record.setPosSeq(cursor.getInt(7));
            record.setCityCode(cursor.getString(8));
            record.setCardFaceNum(cursor.getString(9));
            record.setCardKind((byte)cursor.getInt(10));
            record.setBalanceBef(cursor.getLong(11));
            record.setAmount(cursor.getLong(12));
            record.setTxnTime(cursor.getString(13));
            record.setTxnCounter(cursor.getInt(14));
            record.setPosID(cursor.getString(15));
            record.setTac(cursor.getString(16));
            record.setCardVerNo((byte)cursor.getInt(17));
            record.setStatus((byte)cursor.getInt(18));

            if(cursor.isNull(19)){
                Log.d(tag,"未设置");
                record.setLastUploadTime(null);
            }else {
                Log.d(tag, "LastUploadTime:"+cursor.getString(19));
                SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                try {
                    record.setLastUploadTime(sdf.parse(cursor.getString(19)));
                } catch (ParseException e) {
                    e.printStackTrace();
                }
            }
            result.add(record);
        }

        return  result;
    }
    /**
     * 更新消费记录状态
     * @param localSeq
     * @param status
     * @return
     */
    public boolean updateRecordStatus(long localSeq,int status){
        String sqlUpdate=String.format("Update %s set status=%d,LastUploadTime=datetime('now', 'localtime') where id=%d",
                TBDebit,status,localSeq);

        return ExecCommand(sqlUpdate);
    }

    /**
     * 注册消费文件
     * @param filename
     * @return
     */
    public boolean registFHFile(String filename){
        String sqlInsert=String.format("Insert into %s(FileName) values('%s')",TBFHFileRegist,filename);

        return ExecCommand(sqlInsert);
    }

    /**
     * 更新消费文件上传记录
     * @param filename
     * @return
     */
    public boolean uploadFHFileRegist(String filename){
        String sqlUpdate=String.format("Update %s set uploadTimes=uploadTimes+1 where FileName='%s'",TBFHFileRegist,filename);

        return ExecCommand(sqlUpdate);
    }

    /**
     * 查询指定条件的消费文件上传信息
     * @param condition
     * @return
     */
    public List<FHFileUploadInfo> getFHFileList(String condition){
        String sqlSelect=String.format("Select * from %s\n",TBFHFileRegist);
        if(condition!=null&&!condition.isEmpty()){
            sqlSelect+=condition;
        }

        Cursor cursor=ExecQuery(sqlSelect);
        List<FHFileUploadInfo> result=new ArrayList<FHFileUploadInfo>();
        if(cursor==null){
            return result;
        }
        while(cursor.moveToNext()){
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            FHFileUploadInfo fhFileUploadInfo= null;
            try {
                String uploadTimeStr=cursor.getString(3);
                fhFileUploadInfo = new FHFileUploadInfo(cursor.getString(0),
                        sdf.parse(cursor.getString(1)),
                        cursor.getInt(2),
                        uploadTimeStr==null?null:sdf.parse(uploadTimeStr));
            } catch (ParseException e) {
                e.printStackTrace();
                Log.d(tag,e.getLocalizedMessage());
                return result;
            }

            result.add(fhFileUploadInfo);
        }

        return result;
    }
}
