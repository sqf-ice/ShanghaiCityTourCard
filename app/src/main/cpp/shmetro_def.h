//SHMETRO_DEF.h

#ifndef _SHMETRO_DEF_H_
#define _SHMETRO_DEF_H_
//start of file

//命令码
#define SHMETRO_OT_INITREADER	  0x20
#define SHMETRO_OT_RESETREADER	0x21
#define SHMETRO_OT_STOPREADER	  0x22
#define SHMETRO_OT_READERSTATUS	0x23
#define SHMETRO_OT_DISPSIGNAL	  0x2F
#define SHMETRO_OT_READCARD	    0x30
#define SHMETRO_OT_ENTRY	      0x31
#define SHMETRO_OT_EXIT	        0x32
#define SHMETRO_OT_LOCKCARD	    0x33
#define SHMETRO_OT_ADDVALUE	    0x34
#define SHMETRO_OT_ENABLECARD	  0x35
#define SHMETRO_OT_UPDATE	      0x36
#define SHMETRO_OT_EXPIREDATE	  0x37
#define SHMETRO_OT_READRECORD	  0x39
#define SHMETRO_OT_PAY	      0x3B
#define SHMETRO_OT_ABORT	      0x3F
#define SHMETRO_OT_TMOUNT	      0x40
#define SHMETRO_OT_AUTH	        0x41
#define SHMETRO_OT_VERIFYPIN	0x70
#define SHMETRO_OT_MODIFYPIN	0x71

#define SHMETRO_OT_GETKEY	    0x53
#define SHMETRO_OT_CREDITINI	0x54
#define SHMETRO_OT_CREDIT	    0x55


#define SHMETRO_OT_UNBLOCK		0x57
#define SHMETRO_OT_GETRAN		0x58
#define SHMETRO_OT_EXPIREDATE1		0x59
#define SHMETRO_OT_GETPROOF	    0x5A
#define SHMETRO_OT_FORBID		0x5B
#define SHMETRO_OT_LOCKCARD1    0x5c
#define SHMETRO_OT_CalcMAC		0x5D
#define SHMETRO_OT_CheckMAC		0x5E
#define SHMETRO_OT_ADDVALUE1	0x5F
#define SHMETRO_OT_ENABLECARD1	  0x60
#define SHMETRO_OT_READRECORD1	  0x61
#define SHMETRO_OT_CREDITINI1    0x62
#define SHMETRO_OT_ENABLECARD2	  0x63





#define SHMETRO_ST_ENTRY	    0xC1
#define SHMETRO_ST_EXIT	      0xC2
#define SHMETRO_ST_ADDVALUE	  0xC4
#define SHMETRO_ST_SALECARD	  0xC5
#define SHMETRO_ST_UPDATE	    0xC6
#define SHMETRO_ST_EXPIREDATE	0xC7
#define SHMETRO_ST_LOCK	      0xC8
#define SHMETRO_ST_SALEenmergencyCARD	0xC9
#define SHMETRO_ST_ENTRY2	            0xCA
#define SHMETRO_ST_READRECORD	        0xD0
#define SHMETRO_ST_RETURN	            0xD2
#define SHMETRO_ST_GETPROOF	          0xD3
#define SHMETRO_ST_FORMATCARD	        0xF0
#define SHMETRO_ST_AUTHCODE	          0xAC


#define SHMETRO_ET_ENTRY	      0xE1
#define SHMETRO_ET_EXIT	      0xE2
#define SHMETRO_ET_UPDATE	    0xE3
#define SHMETRO_ET_GETPROOF	    0xE4
#define SHMETRO_ET_LOCKCARD    0xE5
#define SHMETRO_ET_RDLOG	  0xE6



#define SHMETRO_OT_GETHWVER		0x2E

#define SHMETRO_PBOC_RDLOG		0XB9
#define SHMETRO_PBOC_ENTRY		0XBA
#define SHMETRO_PBOC_EXIT		0XBB
#define SHMETRO_PBOC_UPDATE		0XBC
#define SHMETRO_PBOC_GETPROOF	0xBD




#define SHMETRO_PBOC_GETPARAM	0xBE
#define SHMETRO_PBOC_SETPARAM	0xBF


#define SJT_RD		0X73
#define SJT_WR		0X74
#define cmdtest		0xef


#define SHMETRO_PRG_DOWNLOAD	0X0F
//
//返回码
#define SHMETRO_CE_OK	                0x00	//命令成功完成
#define SHMETRO_CE_CHECKERROR	        0x01	//校验和错误
#define SHMETRO_CE_BADREQUEST	        0x02	//无效命令
#define SHMETRO_CE_BADPARAM	          0x03	//输入参数错
#define SHMETRO_CE_STATUSERROR	      0x04	//读写器状态错
#define SHMETRO_CE_AUTHENERROR	      0x10	//认证失败
#define SHMETRO_CE_READERROR	        0x11	//读卡故障
#define SHMETRO_CE_WRITEERROR	        0x12	//写卡故障
#define SHMETRO_CE_CARDREMOVED	      0x13	//操作过程中卡移动
#define SHMETRO_CE_INVALIDCARD	      0x14	//无效卡
#define SHMETRO_CE_LOCKED	     	 0x15	//止付卡
#define SHMETRO_CE_NORECORD	          0x16	//无交易记录
#define SHMETRO_CE_PUTBACKCARD1	      0x17	//交易时清备份失败，必须重新读卡
#define SHMETRO_CE_PUTBACKCARD2	      0x18	//更新累计充资金额失败，必须重新读卡
#define SHMETRO_CE_NOPSAM	            0x19	//无PSAM卡
#define SHMETRO_CE_NOISAM	            0x1A	//无ISAM卡
#define SHMETRO_CE_PSAMERR	          0x1B	//PSAM卡故障
#define SHMETRO_CE_ISAMERR	          0x1C	//ISAM卡故障
#define SHMETRO_CE_NOESAM	            0x1D	//无ESAM卡
#define SHMETRO_CE_ESAMERR	          0x1E	//ESAM卡故障
#define SHMETRO_CE_MAC2ERR	          0x20	//mac2错
#define SHMETRO_CE_NOFUNCTION	      0x21	//6a81
#define SHMETRO_CE_MAC1ERR	          0x25	//mac1错
#define SHMETRO_CE_NOKEY	            0x60	//无操作密钥
#define SHMETRO_CE_MACERR	            0x61	//充值授权MAC错误
#define SHMETRO_CE_AUTHERR	          0x62	//外部认证失败
#define SHMETRO_CE_AUTHFAIL	          0x40	//充值授权不足
#define SHMETRO_CE_NOAUTH	            0x41	//未授权
#define SHMETRO_CE_PINERROR	          0x63	//外部认证卡PIN 错
#define SHMETRO_CE_MODIFYPIN	        0x64	//修改 外部认证卡PIN 失败
#define SHMETRO_CE_READERERROR	      0x70	//读写器故障
#define SHMETRO_CE_UNKNOWN	          0x7F	//未知错误

//时间格式
#define SHMETRO_TIME_FORMAT_BCD   0x00   //
#define SHMETRO_TIME_FORMAT_UNIX  0x01

//工作模式, 参见读写器的工作状态
#define SHMETRO_WORK_MODE_TEST  0x00  //– 测试模式（保留）
#define SHMETRO_WORK_MODE_ENTRY 0x20  //– 进站模式
#define SHMETRO_WORK_MODE_EXIT  0x40  //– 出站模式
#define SHMETRO_WORK_MODE_BOM   0x60  //– SATVM/BOM模式
#define SHMETRO_WORK_MODE_ATVM  0x80  //– ATVM模式
#define SHMETRO_WORK_MODE_LCM   0xA0  //– 线路编码机模式 
#define SHMETRO_WORK_MODE_CCM   0xA1  //– 清分编码机模式

//
#define SHMETRO_IDLE_STATE    0x00 //空闲状态
#define SHMETRO_READ_STATE    0x01 //读状态
#define SHMETRO_TRANS_STATE   0x02 //交易状态
#define SHMETRO_WRITE_STATE   0x04 //写状态
//#define SHMETRO_STOP_STATE    0x1f //停止状态(仅限停止模式)

//POS软件版本
#define SHMETRO_SOFT_VER  0x43
#define HWVER 0x01		//0共用，1华铭，2华虹计通，3怡力，4邮通，5华腾，6华腾智能，7 DAT，8冠人



//sam 卡槽定义
#define CPU_SAM  0x3
//#define M1_SAM  0x2
#define SJT_SAM  0x5
//#define M1_SAM_S  0x3

//调试定义
#define debugflag  0x0//1,打开调试信息
#define debugflag1  0x0
#define shmode	0x0//1,为上海模式的测试卡，测试专用
#define samkey 0x01//1,用sam卡计算keyA。0用模块计算keyA
//end of file
#endif

