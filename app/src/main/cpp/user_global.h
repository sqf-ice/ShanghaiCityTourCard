//user_global.h

#ifndef _USER_GLOBAL_H_
#define _USER_GLOBAL_H_
//start of file

#include "mydefine.h.h"

#define RLED_INDEX  0
#define GLED_INDEX  1
#define YLED_INDEX  2
//#define LED_ON    1
//#define LED_OFF   0 

typedef struct {
  UBYTE device_state;//设备状态
//  UWORD station_code;//车站编码
  UBYTE equipment_code;  //设备编码 1字节
  UDWORD device_code;//设备编码？
  UWORD SW1SW2;		//卡片返回
  UBYTE Posid[4];
  UBYTE CpuSamid[6];//sam卡号
//  UBYTE M1Samid1[6];//建设部
  UBYTE ModuleNo[6];//软密钥
  UBYTE SjtSamid[6];
  UBYTE Postype;
  //
  SHMETRO_PROT metro_prot;
  //
  UBYTE cmd;
  UBYTE cmd_retry;		//指令重发次数
  UBYTE resp_code;		//应答代码
  UBYTE sendlen;
  //
  //
  UBYTE Sam_keyversion;		//从sam卡读出 密钥版本号
  UBYTE Sam_errflag;		//
  UBYTE SamCnt[4];
  //
  //
  UBYTE TagType;//建设部m1卡为0，软密钥为1，cpu卡为2、单程票为3
  UBYTE Err17Flag;//M1清备份失败标志,0x01为M1卡清备份,CPU卡debit失败时置为x11，单程票消费失败0x21.正常时清0.
  UBYTE sameflag;//比较前后2张卡的卡号是否相同，相同则为1
  UBYTE CardSnr[8];//卡号
  UBYTE ErrCardSnr[4];
 
  ////m1卡数据存储
  UDWORD Blk8val,Blk5val,Blk6val,Balance;
  UWORD CardCount;
	UBYTE Huancheng[16];//另一条记录设备号3byte+行业信息+换乘时间MMDDHHmm+累计乘车金额3b+累计乘车次数2b
  UBYTE BalanceL,BalanceM,BalanceH,BalanceHH;
  UBYTE KeyA1[6],KeyA2[6],KeyA3[6],KeyA4[6],KeyA6[6],KeyA7[6],KeyA8[6],Keyauth[8];//扇区keyA
  UBYTE B1[16],B2[16];//card dir
  UBYTE CityNo[2];//城市代码 2字节
  UBYTE CardDeposit[2];//押金 2字节
  UBYTE CardIssueNo[4];//发行流水号
  UBYTE CardMac[4];//卡认证码
  UBYTE CardExpiredDate[4];//有效期
  UBYTE CardType,Cardversion;//卡类型0x04;//浦发卡,0x05地铁员工卡，02CPU卡，03单程票
  UBYTE ShSign;
//  UBYTE CardVer;//卡版本
  UBYTE CardEnableFlag;//卡启用标志

  UBYTE Amountcount,AmountbalanceL,AmountbalanceM,AmountbalanceH;
  UDWORD Amountbalance;

  UBYTE  B4[16],B5[16],B6[16],B8[16],B9[16],B10[16];//钱包块
  UBYTE  B12[16],B13[16];//3扇区 
  UBYTE  B28[16],B29[16],B30[16];//7扇区
  UBYTE  B32[16],B33[16],B34[16];//8扇区
  UBYTE  Keybuf[16];
  
  UBYTE CardOverDraft[4];//透支额度 b8前4字节
//  UBYTE M1AddVal[4];//累计加款值
//  UBYTE M1AddValCnt[2];//钱包充值次数
  UBYTE M1Balance[4];//钱包余额,低字节在前
  
//  UBYTE B24[16],B25[16],B26[16],BRT_BLK[16];//公共信息块
  UBYTE RecordBuf[16];
  UBYTE CntCardDateLimit[4];//年检日期
  UBYTE M1CardCnt[2];//卡计数器
//  UBYTE CardStatus;
  UBYTE CardPoint,BlkNo;//交易记录指针
  UBYTE CardProcessFlag;//交易进程标志
  UBYTE M1FileFlag;//文件标识
  UBYTE LockSign;//黑名单标识
  UBYTE TransDiscount;//换乘优惠
  UBYTE Metrosta,EntryFlag;//进闸标识
//  UBYTE Cardpoint;
  UBYTE EntryStation[2];//进站编码
  
  
//  UBYTE CntBalance[4];//计次钱包余额
//  UBYTE M1CntCardCnt[2];//月票累计交易次数
  UBYTE ExpireValue[4];
  UDWORD ExpValue;
  UBYTE SecNo,ExpireFlag,Amountyear,Amountmonth;
  UBYTE Result;//充值结果
  UBYTE CardBak[64];
   //CardBak内容：
  //CardSnr 4字节(0~3)，Balance 交易前金额4字节(4~7)
  //随机数4字节(8~11)，卡计数器2字节(12\13)，交易金额4字节(14~17)
  //(cpu卡交易类型,18)，交易日期时间7字节(19~25)
   //cpu卡消费初始化的密钥版本1字节(26)、算法标示1字节(27)，
//CardPoint(28),Metrosta(29),gUserGlobal.BalanceL~H(30~32);
  UBYTE Updatebuf[32];
  
  
  
//****************************
//CPU卡数据
  UBYTE ADF1_file0015[40];
  UBYTE ADF1_file0018[230];//历史消费记录文件，10*23
  UBYTE ADF1_file0006[92];//充值明细文件
  UBYTE ADF1_file0017[48];//复合消费记录文件，brt记录号为04
  UBYTE ADF2_file0015[40];//
//  UBYTE ADF2_file0018[230];//历史消费记录文件，10*23
  UBYTE ADF2_file0017[48];//复合消费记录文件，brt记录号为04
//  UBYTE ADF2_file0006[23];//充值明细文件
  UBYTE Sec7buf[16];
  UBYTE Cpu_80CAbuf[9];
  UBYTE Creditbuf[16];
  UBYTE Random[16];//随机数
  UBYTE Mac[8];
  UBYTE Tac[4];
  UBYTE ADF1_Balance[4],ADF2_Balance[4], ADF3_Balance[4];
  UBYTE ADF1_CardOverDraft[4],ADF2_CardOverDraft[4],ADF3_CardOverDraft[4];
  UBYTE ADF1_Cnt[2],ADF1_Cnt1[2];
  
  UBYTE ADF1_Algorithm;		//消费初始化，从卡片返回
  UBYTE ADF2_Algorithm;		//消费初始化，从卡片返回
  UBYTE Cpuflag;
//***************************
//sjt 数据
	UBYTE  Sjtbl0[4],Sjtbl1[4],Sjtbl2[4],Sjtbl3[4],Sjtbl4[4],Sjtbl5[4],Sjtbl6[4],Sjtbl7[4];
	UBYTE  Sjtbl9[4],Sjtbl10[4],Sjtbl11[4],Sjtbl12[4],Sjtbl13[4],Sjtbl14[4],Sjtbl15[4];
	UBYTE  Last[8],Laststation[2],Operater[2],SjtCount[2];
	UBYTE  Sjtbkflag;
  	UBYTE Psw;

  UBYTE Sjtremainl,Sjtremainh;

//metro
	UBYTE metrokeyver;
	UBYTE metro0015[32],metro0006[32];
	UBYTE metro0018[230],metro0019[64];//,metro_sec8[64],

//pboc
	UBYTE PbocExpDate[4],PbocStartDate[4],RMac[4],AID[32],GPOout[64];//AID[0]为长度，AID长度为5-16个字节，由5个字节的注册的应用提供者标识符(RID)和0-11个字节的专有应用标识符扩展(PIX)组成。
	UBYTE CappFlag,GPOlen,PAN,r_mac,flag_lian,indexbank,blackflag,fdda_result;
	UBYTE Pboc_0015[64], Pboc_sec8[64], Pboc_sec7[64],Pboc_001e[32],MT_UID[14], AIP[2], YYBBH[2], ATC[2];
	UBYTE AFL[32],fkhyysj[64],yymw[8],chanping[16],sqm[6],version[3];
	UBYTE tag5f25[3],tag5f24[3];
//PBOC KEY
	UBYTE szN[256],szE[3];
	UBYTE nNLen,nELen;
	UWORD ParaPoint;	//程序下载指针
	UWORD ParaAdr;
	UBYTE ParaVer[4];
	UBYTE pboc_parabuf[8192];
	
	
//pboc _fDDA_Data
	UBYTE szIssCA[250];	// 发卡行公钥证书	90	ok
	UBYTE szIssE[4];	// 发卡行公钥指数	9F32 ok
	UBYTE szIssRem[96];	// 发卡行公钥余数	92	ok	?需判断长度
	UBYTE szICCCA[250];	// IC卡公钥证书		9F46 ok
	UBYTE szICCE[4];	// IC卡公钥指数		9F47 OK
	UBYTE szICCRem[96];	// IC卡公钥余数		9F 48 OK ?需判断长度
	UBYTE szSD[248];	// 需认证的静态应用数据(被AFL标识的记录 + AIP, 此IC卡具体为	0301记录内容 + 7C00)
		//93 签名的静态应用数
	UBYTE szSign[250];	// 签名的动态应用数据	9F 4B	OK
	UBYTE szTDD[64];	// 终端动态数据(00版fDDA, 终端不可预知数)	ok
	UBYTE IssCA_ExpDate[2];

//
//通讯接收的参数
  UBYTE Logbuf[512];
  UBYTE LogNo;
  UBYTE ReadMode;

  UBYTE Date[8];//消费交易日期时间，格式yyyymmddhhmmss,最后一字节备用
  UBYTE TransValue[4];//交易金额
  UBYTE PosCnt[4];//pos交易流水号
  UBYTE Station[4];//
//  UBYTE CurrentDate[4];//yyyymmdd
//  UBYTE TransType;//交易类型，出站指令参数
  UBYTE BalanceType;//钱包类型，出站指令参数
  UBYTE Ride[2],Para1,Para2;
  UDWORD Decval;//
	
//下载功能
  UWORD ProgPoint;	//程序下载指针
  UBYTE ProgLen;	//一帧收到的长度
  UBYTE Proginf[16];
  UBYTE Progdl;
//  UBYTE ProgBuf[512];
//  UBYTE rled_flag;
  UBYTE rled_flag;
  UBYTE gled_flag;
  UBYTE yled_flag;
	}  USER_GLOBAL;

typedef struct _PUBKEY_
{
	unsigned char szN[512];
	unsigned char szE[3];
	unsigned int nNLen;
	unsigned int nELen;
} PubKey;

extern USER_GLOBAL gUserGlobal;

//函数
void shmetro_enter_state(UBYTE state);
#define shmetro_enter_idle()  	shmetro_enter_state(SHMETRO_IDLE_STATE)
#define shmetro_enter_read()  	shmetro_enter_state(SHMETRO_READ_STATE)
#define shmetro_enter_trans()  	shmetro_enter_state(SHMETRO_TRANS_STATE)
#define shmetro_enter_write()  	shmetro_enter_state(SHMETRO_WRITE_STATE)
#define shmetro_enter_stop()  	shmetro_enter_state(SHMETRO_STOP_STATE)



//end of file
#endif

