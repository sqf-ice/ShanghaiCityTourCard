//cmd_shmetro_func1.c

#ifndef _CMD_SHMETRO_FUNC1_C_
#define _CMD_SHMETRO_FUNC1_C_


#include "shmetro_common.h"
#include "user_global.h"
#include "prot_shmetro.h"

//start of file

//UBYTE HL_Active(void);

/****************************************************************************
* 名称：Chk_xor_sum()
* 功能：
* 入口参数：*buff,lenth
* 出口参数：Result
****************************************************************************/
UBYTE Chk_xor_sum(UBYTE *buff,UBYTE lenth)
{	UBYTE i,Result;
	Result=0;
	for(i=0;i<lenth;i++)
		{Result=Result^buff[i];}
	return Result;
}

/****************************************************************************
* 名称：Chk_psw()
* 功能：检查专用块是否符合要求
****************************************************************************/
UBYTE  Chk_psw(void)
{	UBYTE i,temp;
//	USER_GLOBAL *p = &gUserGlobal;
	temp = gUserGlobal.Psw;
	i = temp%2+(temp/2)%2+(temp/4)%2+(temp/8)%2+(temp/16)%2+(temp/32)%2+(temp/64)%2+(temp/128)%2;
	i = i%2;
	if(i==0)
		{return 0;}
	else
		{return 1;}
}


/****************************************************************************
* 名称：Form_psw()
* 功能：检查专用块是否符合要求
****************************************************************************/
void  Form_psw(void)
{	UBYTE i,temp;
//	USER_GLOBAL *p = &gUserGlobal;
	temp = gUserGlobal.Psw&0xfe;
	i=temp%2+(temp/2)%2+(temp/4)%2+(temp/8)%2+(temp/16)%2+(temp/32)%2+(temp/64)%2+(temp/128)%2;
	i=i%2;
	gUserGlobal.Psw=temp+i;
}
/*=======================================================================================
函数：通讯
功能：0.1 发送指令执行失败的应答
=========================================================================================*/
void send_err_resp(void)
{
	USER_GLOBAL *p = &gUserGlobal;
//	memcpy(gUserGlobal.metro_prot.send_buf,buf,20);
	gUserGlobal.metro_prot.send_buf[0] = p->cmd;
	gUserGlobal.metro_prot.send_buf[1] = p->cmd_retry;
	gUserGlobal.metro_prot.send_buf[2] = p->resp_code;
	gUserGlobal.metro_prot.send_bytes = 3;
	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
	prot_shmetro_send_process(&(p->metro_prot));
	gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
	return;

}


/*=======================================================================================
函数：
功能：0.90
=========================================================================================*/
//void chk_same(void)
//{
//	USER_GLOBAL *p = &gUserGlobal;
////	UBYTE i,temp;
//	if(gUserGlobal.Err17Flag!=0)
//		{if((p->CardBak[0]==p->CardSnr[0])&&(p->CardBak[1]==p->CardSnr[1])&&(p->CardBak[2]==p->CardSnr[2])&&(p->CardBak[3]==p->CardSnr[3]))
//			{gUserGlobal.sameflag=1;}
//		else
//			{gUserGlobal.sameflag=0;}
//		}
//	else
//		{gUserGlobal.sameflag=0;}
//	return;
//}


/*=======================================================================================
函数：
功能：0.90 比较2个数据块是否一样（16字节）
=========================================================================================*/
UBYTE compareBlk(UBYTE *inbuf1,UBYTE *inbuf2)
{
	UBYTE i;
	for(i=0;i<16;i++)
	{if(inbuf1[i]!=inbuf2[i])
		{return 1;}
	}
	return 0;
}
/*=======================================================================================
函数：
功能：0.90 比较2个数据块是否一样（16字节）
=========================================================================================*/
UBYTE compareBlk4(UBYTE *inbuf1,UBYTE *inbuf2)
{
	UBYTE i;
	for(i=0;i<4;i++)
	{if(inbuf1[i]!=inbuf2[i])
		{return 1;}
	}
	return 0;
}
/****************************************************************************
* 名称：Chk_sjtblk()
* 功能：检查2个块是否相等
****************************************************************************/
UBYTE  Chk_sjtblk(UBYTE *inbuf1,UBYTE *inbuf2)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE i;
	for(i=0;i<4;i++)
	{if(inbuf1[i]!=inbuf2[i])
		{return 1;}
	}
	return 0;
}
/*=======================================================================================
函数：
功能：0.90检查钱包数据块是否正常,返回0
=========================================================================================*/
UBYTE Chk_balanceblk(UBYTE *ibuf)
{	UBYTE temp;
	temp=~ibuf[4];
	if((ibuf[0]!=ibuf[8])||(ibuf[0]!=temp))
		{return 1;}
	temp=~ibuf[5];
	if((ibuf[1]!=ibuf[9])||(ibuf[1]!=temp))
		{return 1;}
	temp=~ibuf[6];
	if((ibuf[2]!=ibuf[10])||(ibuf[2]!=temp))
		{return 1;}
	temp=~ibuf[7];
	if((ibuf[3]!=ibuf[11])||(ibuf[3]!=temp))
		{return 1;}
	temp=~ibuf[12];
	if ((ibuf[12]!=ibuf[14])||(ibuf[13]!=ibuf[15])||(ibuf[13]!=temp)) 
		{return 1;}
	else
		{return 0;}
}

void Add_balance(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	gUserGlobal.Blk8val=gUserGlobal.B8[0]+(gUserGlobal.B8[1]<<8)+(gUserGlobal.B8[2]<<16)+(gUserGlobal.B8[3]<<24);
//Blk5val=0x0c1d;
	if(gUserGlobal.Blk8val>gUserGlobal.Blk5val)
		{gUserGlobal.Balance=gUserGlobal.Blk8val-gUserGlobal.Blk5val-1;
		gUserGlobal.BalanceL=gUserGlobal.Balance%0x100;
		gUserGlobal.BalanceM=(gUserGlobal.Balance/0x100)%0x100;
		gUserGlobal.BalanceH=(gUserGlobal.Balance/0x10000)%0x100;
		}
	else
		{gUserGlobal.Balance=gUserGlobal.Blk8val-gUserGlobal.Blk5val-1;
		gUserGlobal.Balance=(gUserGlobal.Balance|0x80000000);
		gUserGlobal.BalanceL=gUserGlobal.Balance%0x100;
		gUserGlobal.BalanceM=(gUserGlobal.Balance/0x100)%0x100;
		gUserGlobal.BalanceH=(gUserGlobal.Balance/0x10000)%0x100;
		gUserGlobal.BalanceH=(gUserGlobal.BalanceH|0x80);
		}
}

UBYTE Chk_balance(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.Balance>=0xfffff448)		//-30.00
		{return 0;}
	if(gUserGlobal.Balance>=0x80000000&&gUserGlobal.Balance<0xfffff448)
		{return 1;}
	if(gUserGlobal.Balance<=100000)
		{return 0;}
	else
		{return 0;}
}

UBYTE Chk_des(UBYTE *inbuf1,UBYTE *inbuf2)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE i;
	for(i=0;i<8;i++)
	{if(inbuf1[i]!=inbuf2[i])
		{return 1;}
	}
	return 0;
}
/****************************************************************************
* 名称：Chk_c()
//CardBak[64];
  //CardBak内容：
  //CardSnr 4字节(0~3)，Balance 交易前金额4字节(4~7)(high--low(M1卡为Block6的前4字节))
  //随机数4字节(8~11)，卡计数器2字节(12\13)，交易金额4字节(14~17)(high--low)
  //(cpu卡交易类型,18)，交易日期时间7字节(19~25)
  //cpu卡消费初始化的密钥版本1字节(26)、算法标示1字节(27)，
//CardPoint(28),Metrosta(29),gUserGlobal.BalanceL~H(30~32);
SAME Return 1
****************************************************************************/
void Chk_c(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE point,count1,count2;
gUserGlobal.Blk6val=(gUserGlobal.B6[7]<<24)+(gUserGlobal.B6[6]<<16)+(gUserGlobal.B6[5]<<8)+gUserGlobal.B6[4];

if(gUserGlobal.Err17Flag==0x01)
	{if(gUserGlobal.CardPoint==0)
		{point=0x09;}
	else
		{point=gUserGlobal.CardPoint-1;}
	count1=(gUserGlobal.CardCount-1)/0x100;
	count2=(gUserGlobal.CardCount-1)%0x100;
	if((p->CardBak[0]==p->CardSnr[0])&&(p->CardBak[1]==p->CardSnr[1])&&(p->CardBak[2]==p->CardSnr[2])&&(p->CardBak[3]==p->CardSnr[3]))
		{if((p->CardBak[4]==p->B6[0])&&(p->CardBak[5]==p->B6[1])&&(p->CardBak[6]==p->B6[2])&&(p->CardBak[7]==p->B6[3])&&(p->CardBak[12]==count1)&&(p->CardBak[13]==count2)&&(p->CardBak[28]==point))
			{gUserGlobal.sameflag = 1;
			gUserGlobal.CardPoint=point;
			gUserGlobal.M1CardCnt[0]=count1;
			gUserGlobal.M1CardCnt[1]=count2;
			gUserGlobal.CardCount=(count1<<8)+count2;
			gUserGlobal.Metrosta=gUserGlobal.CardBak[29];
			}
		}
	else
		{gUserGlobal.sameflag=0;}
	}
else
	{gUserGlobal.sameflag=0;}
}

/****************************************************************************
* 名称：Chk_sjtc()
//CardBak[64];
  //CardBak内容：
  //CardSnr 4字节(0~3)，Balance 交易前金额4字节(4~7)(high--low(M1卡为Block6的前4字节))
  //随机数4字节(8~11)，卡计数器2字节(12\13)，交易金额4字节(14~17)(high--low)
  //(cpu卡交易类型,18)，交易日期时间7字节(19~25)
  //cpu卡消费初始化的密钥版本1字节(26)、算法标示1字节(27)，
//CardPoint(28),Metrosta(29),gUserGlobal.BalanceL~H(30~32);
****************************************************************************/
UBYTE Chk_sjtc(void)
{	USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.Err17Flag!=0x21)
		{return 0;}
	if((p->CardBak[0]!=p->CardSnr[0])||(p->CardBak[1]!=p->CardSnr[1])||(p->CardBak[2]!=p->CardSnr[2])||(p->CardBak[3]!=p->CardSnr[3]))
		{return 0;}
	else if((p->CardBak[29]==p->Metrosta))
		{return 0;}
	else
		{
		p->SjtCount[0] = p->CardBak[12];
		p->SjtCount[1] = p->CardBak[13];
		p->CardCount=(p->SjtCount[0]*0x100)+p->SjtCount[1];
		return 1;
		}
}
/*=======================================================================================
函数：
功能：0.90 形成卡上公共信息记录
=========================================================================================*/
//void form_B24(void)
//{
////	USER_GLOBAL *p = &gUserGlobal;
////	UBYTE temp;
////	UWORD temp1;
//
//	return;
//}

/*=======================================================================================
函数：
功能：0.91 形成卡上9条历史记录16字节
=========================================================================================*/
void form_record(void)
{
	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp1,temp2,temp3;
	UDWORD temp32;
	gUserGlobal.RecordBuf[0] = gUserGlobal.Ride[0];//0x11;
	gUserGlobal.RecordBuf[1] = gUserGlobal.Posid[0];
	gUserGlobal.RecordBuf[2] =  gUserGlobal.Posid[1];
	gUserGlobal.RecordBuf[3] =  gUserGlobal.Posid[3];
	gUserGlobal.RecordBuf[4] = gUserGlobal.TransValue[3];//低字节
	gUserGlobal.RecordBuf[5] = gUserGlobal.TransValue[2];
	temp32 = gUserGlobal.Balance-gUserGlobal.Decval;

	gUserGlobal.RecordBuf[6] = temp32%0x100;
	gUserGlobal.RecordBuf[7] = (temp32>>8)%0x100;//
	gUserGlobal.RecordBuf[8] =  (temp32>>16)%0x100;;//高字节
	gUserGlobal.RecordBuf[9] = gUserGlobal.TransValue[1];//扣款高字节

	gUserGlobal.RecordBuf[10] = gUserGlobal.TransValue[3];//低字节

	temp32=(gUserGlobal.Date[0]<<8)+gUserGlobal.Date[1];
	temp32=(temp32/0x1000)*1000+((temp32/0x100)&0x0f)*100+((temp32>>4)%0x10)*10+temp32%0x10;
	temp1=gUserGlobal.Date[2];
	temp32 = (temp32<<4)+((temp1>>4)%0x10)*10+(temp1%0x10);

	gUserGlobal.RecordBuf[10]=(temp32>>8)%0X100;
	gUserGlobal.RecordBuf[11]=temp32%0x100;
	temp1=(gUserGlobal.Date[3]/0x10)*10+(gUserGlobal.Date[3]%0x10);//日
	temp2=(gUserGlobal.Date[4]/0x10)*10+(gUserGlobal.Date[4]%0x10);//时
	temp3=(gUserGlobal.Date[5]/0x10)*10+(gUserGlobal.Date[5]%0x10);//分
	temp32= (temp1<<11)+(temp2<<6)+temp3;
	gUserGlobal.RecordBuf[12] = (temp32>>8)%0X100;
	gUserGlobal.RecordBuf[13] = temp32%0x100;
	temp1 = (gUserGlobal.Date[6]/0x10)*10+(gUserGlobal.Date[6]%0x10);
	gUserGlobal.RecordBuf[14] = temp1<<2;
	lrc_gen2(gUserGlobal.RecordBuf,15);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.RecordBuf,16);
		gUserGlobal.metro_prot.send_bytes = 16;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	return;
}



/*=======================================================================================
函数：
功能：0.92 指针to数据块号
=========================================================================================*/
void CardPoint_to_blkno(UBYTE para)
{	//USER_GLOBAL *p = &gUserGlobal;
	switch(para)
		{case 0:gUserGlobal.BlkNo=14;
			break;
		case 1:gUserGlobal.BlkNo=16;
			break;
		case 2:gUserGlobal.BlkNo=17;
			break;
		case 3:gUserGlobal.BlkNo=18;
			break;
		case 4:gUserGlobal.BlkNo=20;
			break;
		case 5:gUserGlobal.BlkNo=21;
			break;
		case 6:gUserGlobal.BlkNo=22;
			break;
		case 7:gUserGlobal.BlkNo=24;
			break;
		case 8:gUserGlobal.BlkNo=25;
			break;
		default:gUserGlobal.BlkNo=26;
			break;
		}
	return;
}
/*=======================================================================================
函数：
功能：0.93 备份卡片数据在内存中
//CardBak[64];
  //CardBak内容：
  //CardSnr 4字节(0~3)，Balance 交易前金额4字节(4~7)(high--low(M1卡为Block6的前4字节))
  //随机数4字节(8~11)，卡计数器2字节(12\13)，交易金额4字节(14~17)(high--low)
  //(cpu卡交易类型,18)，交易日期时间7字节(19~25)
  //cpu卡消费初始化的密钥版本1字节(26)、算法标示1字节(27)，
//CardPoint(28),Metrosta(29),gUserGlobal.BalanceL~H(30~32);
//员工卡城市代码应用序列号(33~42),metrokeyver(43)
//重取tac的卡号(44~47)
=========================================================================================*/
void Backup(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	memcpy(gUserGlobal.CardBak,gUserGlobal.CardSnr,4);
	memcpy(gUserGlobal.CardBak+44,gUserGlobal.CardSnr,4);
	memcpy(gUserGlobal.Tac,"\x00\x00\x00\x00",4);
	if(gUserGlobal.TagType == 2)//cpu卡
		{
		memcpy(gUserGlobal.CardBak+44,gUserGlobal.ADF1_file0015+16,4);//ADF1_file0015[16]
		memcpy(gUserGlobal.CardBak+4,gUserGlobal.ADF1_Balance,4);
		memcpy(gUserGlobal.CardBak+8,gUserGlobal.Random,4);
		gUserGlobal.CardBak[18] = 0x09;//cpu卡交易类型06/09
		gUserGlobal.CardBak[26] = gUserGlobal.Sam_keyversion;
		gUserGlobal.CardBak[27] = gUserGlobal.ADF1_Algorithm;
		memcpy(gUserGlobal.CardBak+4,gUserGlobal.ADF1_Balance,4);
		gUserGlobal.CardBak[12] =gUserGlobal.ADF1_Cnt[0];
		gUserGlobal.CardBak[13] =gUserGlobal.ADF1_Cnt[1];
		gUserGlobal.CardBak[29] = gUserGlobal.Metrosta;
		}
	else if(gUserGlobal.TagType == 5)//员工卡
		{
		memcpy(gUserGlobal.CardBak+4,gUserGlobal.ADF1_Balance,4);
		memcpy(gUserGlobal.CardBak+8,gUserGlobal.Random,4);
		gUserGlobal.CardBak[18] = 0x09;//cpu卡交易类型06/09
		gUserGlobal.CardBak[43] = gUserGlobal.metrokeyver;
		gUserGlobal.CardBak[27] = gUserGlobal.ADF1_Algorithm;
		memcpy(gUserGlobal.CardBak+4,gUserGlobal.ADF1_Balance,4);
		gUserGlobal.CardBak[12] =gUserGlobal.ADF1_Cnt[0];
		gUserGlobal.CardBak[13] =gUserGlobal.ADF1_Cnt[1];
		gUserGlobal.CardBak[29] = gUserGlobal.Metrosta;
		memcpy(gUserGlobal.CardBak+33,gUserGlobal.metro0015+10,10);//城市代码+应用序列号
		}
	else//m1卡
		{gUserGlobal.CardBak[4] = gUserGlobal.B6[0];
		gUserGlobal.CardBak[5] = gUserGlobal.B6[1];
		gUserGlobal.CardBak[6] = gUserGlobal.B6[2];
		gUserGlobal.CardBak[7] = gUserGlobal.B6[3];
		gUserGlobal.CardBak[12] = gUserGlobal.M1CardCnt[0];//卡计数器
		gUserGlobal.CardBak[13] = gUserGlobal.M1CardCnt[1];
		gUserGlobal.CardBak[28] = gUserGlobal.CardPoint;
		gUserGlobal.CardBak[29] = gUserGlobal.Metrosta;
		gUserGlobal.CardBak[30] = gUserGlobal.BalanceL;
		gUserGlobal.CardBak[31] = gUserGlobal.BalanceM;
		gUserGlobal.CardBak[32] = gUserGlobal.BalanceH;
		}
	memcpy(gUserGlobal.CardBak+14,gUserGlobal.TransValue,4);//[0]高字节，[3]低字节
	memcpy(gUserGlobal.CardBak+19,gUserGlobal.Date,7);
	return;
}

/*=======================================================================================
函数：
功能：0.93 备份卡片数据在内存中
//CardBak[64];
  //CardBak内容：
  //CardSnr 4字节(0~3)，Balance 交易前金额4字节(4~7)(high--low(M1卡为Block6的前4字节))
  //随机数4字节(8~11)，卡计数器2字节(12\13)，交易金额4字节(14~17)(high--low)
  //(cpu卡交易类型,18)，交易日期时间7字节(19~25)
  //cpu卡消费初始化的密钥版本1字节(26)、算法标示1字节(27)，
//CardPoint(28),Metrosta(29),gUserGlobal.BalanceL~H(30~32);
=========================================================================================*/
void Backupsjt(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	memcpy(gUserGlobal.CardBak,gUserGlobal.CardSnr,4);
	memcpy(gUserGlobal.Tac,"\x00\x00\x00\x00",4);
	gUserGlobal.CardBak[12] = gUserGlobal.SjtCount[0];//卡计数器
	gUserGlobal.CardBak[13] = gUserGlobal.SjtCount[1];
	gUserGlobal.CardBak[29] = gUserGlobal.Metrosta;
	gUserGlobal.Err17Flag=0x21;
	return;
}

/*=======================================================================================
函数：
功能：0.94 WR SJT 1block
=========================================================================================*/
UBYTE Sjt_Write(UBYTE *inbuf,UBYTE block)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,buf[32];
	temp=tocken_write(block,inbuf);
	if(temp!=0)
		{return 1;}
	temp=tocken_read(block,buf);
	if(temp!=0)
		{return 2;}
	temp=compareBlk4(inbuf,buf);
	if(temp!=0)
		{return 3;}
	return 0;
}

UBYTE Sjt_backup(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,rdbuf[16];
	if(gUserGlobal.Sjtbkflag==1)
		{return 0;}
	rdbuf[0]=gUserGlobal.Metrosta;
	rdbuf[1]=gUserGlobal.BalanceM;
	rdbuf[2]=gUserGlobal.BalanceL;
	rdbuf[3]=gUserGlobal.Metrosta^gUserGlobal.BalanceM^gUserGlobal.BalanceL;
	temp=Sjt_Write(rdbuf,0x0f);
	if(temp!=0)
		{return 1;}

	rdbuf[0]=gUserGlobal.Sjtbl9[0];
	rdbuf[1]=gUserGlobal.Sjtbl9[1];
	rdbuf[2]=gUserGlobal.Sjtbl9[2];
	rdbuf[3]=~gUserGlobal.Sjtbl9[3];
	temp=Sjt_Write(rdbuf,0x09);
	if(temp!=0)
		{return 1;}

	return 0;
}

UBYTE Sjt_clrblk(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,Rdbuf[16];
	Rdbuf[0]=0;
	Rdbuf[1]=0;
	Rdbuf[2]=0;
	Rdbuf[3]=0;
	temp=Sjt_Write(Rdbuf,10);
	if(temp!=0)
		{return 1;}
	temp=Sjt_Write(Rdbuf,11);
	if(temp!=0)
		{return 1;}
	temp=Sjt_Write(Rdbuf,12);
	if(temp!=0)
		{return 1;}
	temp=Sjt_Write(Rdbuf,13);
	if(temp!=0)
		{return 1;}
	temp=Sjt_Write(Rdbuf,14);
	if(temp!=0)
		{return 1;}
	else
		{return 0;}
}
/*=======================================================================================
函数：
功能：0.94 写卡上记录
=========================================================================================*/
UBYTE WrRecord(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,secno;
	form_record();
	CardPoint_to_blkno(gUserGlobal.CardPoint);
	secno=gUserGlobal.BlkNo/4;
	if(secno == 0x03)
		{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
	else
		{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
	if(temp != 0x00)
		{return 1;}
	temp = fm_mcmh_write(gUserGlobal.BlkNo,gUserGlobal.RecordBuf,1,0);
	if(temp != 0x00)
		{return SHMETRO_CE_CARDREMOVED;}
	return 0;
}


/*=======================================================================================
函数：
功能：0.94 写卡上备份
=========================================================================================*/
UBYTE Wrbackup(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[32],temp;

	buf[0]=0x55;
	buf[1]=0xaa;
	buf[2]=0x55;
	buf[3]=gUserGlobal.B5[0];
	buf[4]=gUserGlobal.B5[1];
	buf[5]=gUserGlobal.B5[2];
	buf[6]=gUserGlobal.B5[3];
	buf[7]=gUserGlobal.CardPoint;
	buf[8]=gUserGlobal.M1CardCnt[0];
	buf[9]=gUserGlobal.M1CardCnt[1];
	buf[10]=0x02;
	buf[11]=0x00;
	buf[12]=0x00;
	buf[13]=0x00;
	buf[14]=gUserGlobal.Metrosta;
	lrc_gen2(buf,15);
	temp = mcml_load_key(0x00,0X00,0x03,gUserGlobal.KeyA3);
	if(temp != 0)
		{return 1;}
	temp = fm_mcmh_write(12,buf,1,0);

//	temp = mcml_write(12,buf);
	if(temp != 0x00)
		{return 1;}
	temp = mcml_read(12,gUserGlobal.B12);
	if(temp != 0x00)
		{return 1;}
	temp = compareBlk(buf,gUserGlobal.B12);
	if(temp!=0)
		{return 1;}
	return 0;
}
/*=======================================================================================
函数：
功能：0.95 读卡上记录
=========================================================================================*/
UBYTE ReadRecord(UBYTE point,UBYTE *outbuf)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,secno;
	switch(point)
		{case 0:gUserGlobal.BlkNo=12;
			break;
		case 1:gUserGlobal.BlkNo=13;
			break;
		case 2:gUserGlobal.BlkNo=14;
			break;
		case 3:gUserGlobal.BlkNo=16;
			break;
		case 4:gUserGlobal.BlkNo=17;
			break;
		case 5:gUserGlobal.BlkNo=18;
			break;
		case 6:gUserGlobal.BlkNo=20;
			break;
		case 7:gUserGlobal.BlkNo=21;
			break;
		case 8:gUserGlobal.BlkNo=22;
			break;
		default:gUserGlobal.BlkNo=23;
			break;
		}
	secno=gUserGlobal.BlkNo/4;
	if(secno == 0x03)
		{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
	else
		{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
	if(temp != 0x00)
		{return 1;}

	temp = mcmh_read_s50(gUserGlobal.BlkNo,outbuf,1,0);
	if(temp != 0x00)
		{return SHMETRO_CE_CARDREMOVED;}
	return 0;
}
/*=======================================================================================
函数：
功能：0.95 重取交易tacCpugetprove
=========================================================================================*/
UBYTE Cpu_getprove(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],len,temp;
	UWORD temp1;
	memcpy(buf,"\x80\x5a\x00\x09\x02",5);
	buf[3] = gUserGlobal.CardBak[18];
	temp1 = (gUserGlobal.CardBak[12]<<8)+gUserGlobal.CardBak[13]+1;
	buf[5] = temp1/0x100;
	buf[6] = temp1%0x100;
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,7);
		gUserGlobal.metro_prot.send_bytes = 7;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(buf,7,buf,&len);//
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(len == 2)
		{return 2;}
	else if(len == 10)
		{
		if(gUserGlobal.SW1SW2!=0x9000)
			{return 1;}
		memcpy(gUserGlobal.Mac,buf,4);
		memcpy(gUserGlobal.Tac,buf+4,4);
		return 0;
		}
	else
		{return 1;}
}

/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡计算keyA  //TagType建设部m1卡为0，软密钥为1，cpu卡为2、单程票为3
=========================================================================================*/
UBYTE Sam_KeyA(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x80\xFC\x00\x01\x13",5);
	buf[5] = gUserGlobal.CityNo[0];
	buf[6] = gUserGlobal.CityNo[1];
	memcpy(buf+7,gUserGlobal.CardSnr,4);//卡号
	memcpy(buf+11,gUserGlobal.B1+8,6);//发行流水号+认证码
	buf[17] = 0x10;	//1
	buf[18] = 0x01;	//2
	buf[19] = 0x04;//3
	buf[20] = 0x03;//4
	buf[21] = 0x20;//7
	buf[22] = 0x30;//12  0x21;//8
	buf[23] = 0xff;
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,23);
		gUserGlobal.metro_prot.send_bytes = 23;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,24,buf,&len);
	if(ret!=0)
		{return SHMETRO_CE_NOPSAM;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{return 0;}
	else if(buf[len-2] == 0x61)
		{
		len= buf[len-1];
		memcpy(buf,"\x00\xc0\x00\x00",4);
		buf[4]=len;
		ret = sam_apdu(buf,5,buf,&len);
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.KeyA1,buf,6);
			memcpy(gUserGlobal.KeyA2,buf+6,6);
			memcpy(gUserGlobal.KeyA3,buf+12,6);
			memcpy(gUserGlobal.KeyA4,buf+18,6);
			memcpy(gUserGlobal.KeyA7,buf+24,6);
			memcpy(gUserGlobal.KeyA8,buf+30,6);
			memcpy(gUserGlobal.Keyauth,buf+36,6);
			gUserGlobal.Keyauth[6] = 0x00;
			gUserGlobal.Keyauth[7] = 0x00;
			return 0;
			}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
	return SHMETRO_CE_INVALIDCARD;
}
/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡计算keyA
=========================================================================================*/
UBYTE Sam_KeyA1(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
	UWORD sw1sw2;

	memcpy(buf,"\x00\x88\x02\x01\x10",5);

	buf[5] = gUserGlobal.Sjtbl3[3];		//版本号
	buf[6] = gUserGlobal.CardSnr[0];
	buf[7] = gUserGlobal.CardSnr[1];
	buf[8] = gUserGlobal.CardSnr[2];
	buf[9] = gUserGlobal.CardSnr[3];
	buf[10] = gUserGlobal.Sjtbl2[0];	//发行企业
	buf[11] = gUserGlobal.Sjtbl2[1];
	buf[12] = gUserGlobal.Sjtbl3[0];	//发行流水号
	buf[13] = gUserGlobal.Sjtbl3[1];
	buf[14] = gUserGlobal.Sjtbl3[2];		//发行流水号
	buf[15] = gUserGlobal.Sjtbl0[0];
	buf[16] = gUserGlobal.Sjtbl0[1];
	memcpy(buf+17,gUserGlobal.Sjtbl4,4);//认证码
	if(debugflag1)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,21,buf,&len);
	if(ret!=0)
		{return SHMETRO_CE_NOPSAM;}
	if(debugflag1)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{return 0;}
	else if(buf[len-2] == 0x61)
		{
		len= buf[len-1];
		memcpy(buf,"\x00\xc0\x00\x00",4);
		buf[4]=len;
		if(debugflag1)
			{
			memcpy(gUserGlobal.metro_prot.send_buf,buf,5);
			gUserGlobal.metro_prot.send_bytes = 5;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		ret = sam_apdu(buf,5,buf,&len);
		if(debugflag1)
			{
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.KeyA1,buf,4);
			gUserGlobal.KeyA1[4] =0x00;
			gUserGlobal.KeyA1[5] =0x00;
			return 0;
			}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
	return SHMETRO_CE_PSAMERR;
}

/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡计算keyA
=========================================================================================*/
UBYTE Sam_SjtMac(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
	UWORD sw1sw2;

	memcpy(buf,"\x00\x88\x02\x02\x10",5);
	memcpy(buf+5,gUserGlobal.Sjtbl3,3);//发行流水号
	memcpy(buf+8,gUserGlobal.CardSnr,4);
	memcpy(buf+12,gUserGlobal.Sjtbl2,2);//发行企业
	buf[14] = gUserGlobal.Sjtbl3[3];		//版本号
	memcpy(buf+15,gUserGlobal.Sjtbl2+2,2);//发行日期
	memcpy(buf+17,gUserGlobal.Sjtbl0+2,2);//发行日期
	buf[19] = 0x00;
	buf[20] = 0x00;
	if(debugflag1)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,21,buf,&len);
	if(ret!=0)
		{return SHMETRO_CE_NOPSAM;}
	if(debugflag1)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{return 0;}
	else if(buf[len-2] == 0x61)
		{
		len= buf[len-1];
		memcpy(buf,"\x00\xc0\x00\x00",4);
		buf[4]=len;
		if(debugflag1)
			{
			memcpy(gUserGlobal.metro_prot.send_buf,buf,5);
			gUserGlobal.metro_prot.send_bytes = 5;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		ret = sam_apdu(buf,5,buf,&len);
		if(debugflag1)
			{
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.Mac,buf,4);
			return 0;
			}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
	return SHMETRO_CE_PSAMERR;
}
/*=======================================================================================
函数：sam卡函数
功能：1.1
=========================================================================================*/
UBYTE Sam_select0003(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
//	UWORD sw1sw2;
	memcpy(buf,"\x00\xA4\x00\x00\x02\x0\x03",7);
	if(debugflag1)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,7);
		gUserGlobal.metro_prot.send_bytes = 7;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,7,buf,&len);
	if(debugflag1)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000 || buf[len-2] == 0x61)
		{return 0;}
	return 2;
}
/*=======================================================================================
函数：sam卡函数
功能：1.1
=========================================================================================*/
UBYTE Sam_select0002(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
//	UWORD sw1sw2;
	memcpy(buf,"\x00\xA4\x00\x00\x02\x0\x02",7);
	if(debugflag1)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,7);
		gUserGlobal.metro_prot.send_bytes = 7;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,7,buf,&len);
	if(debugflag1)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000 || buf[len-2] == 0x61)
		{return 0;}
	return 2;
}

/*=======================================================================================
函数：sam卡函数
功能：1.1
=========================================================================================*/
UBYTE Sam_select1001(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
//	UWORD sw1sw2;
	memcpy(buf,"\x00\xA4\x00\x00\x02\x10\x01",7);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,7);
		gUserGlobal.metro_prot.send_bytes = 7;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}
	ret = sam_apdu(buf,7,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}
//	memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));
	if(ret!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000 || buf[len-2] == 0x61)
		{return 0;}
	return 1;
}
/*=======================================================================================
函数：sam卡函数
功能：1.1
=========================================================================================*/
UBYTE Sam_select1002(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
//	UWORD sw1sw2;
	memcpy(buf,"\x00\xA4\x00\x00\x02\x10\x02",7);
	ret = sam_apdu(buf,7,buf,&len);
//	memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));
	if(ret!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000 || buf[len-2] == 0x61)
		{return 0;}
	return 1;
}

/*=======================================================================================
函数：sam卡函数
功能：1.1
=========================================================================================*/
UBYTE Sam_select1003(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
//	UWORD sw1sw2;
	memcpy(buf,"\x00\xA4\x00\x00\x02\x10\x03",7);
	ret = sam_apdu(buf,7,buf,&len);
//	memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));
	if(ret!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000 || buf[len-2] == 0x61)
		{return 0;}
	return 1;
}
/*=======================================================================================
函数：sam卡函数
功能：1.1
=========================================================================================*/
UBYTE Sam_select3f00(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
//	UWORD sw1sw2;
	memcpy(buf,"\x00\xA4\x00\x00\x02\x3f\x00",7);
	ret = sam_apdu(buf,7,buf,&len);
//	memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));
	if(ret!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000 || buf[len-2] == 0x61)
		{return 0;}
	return 1;
}
/*=======================================================================================
函数：sam卡函数
功能：1.1
=========================================================================================*/
UBYTE Sam_changesamid(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
//	UWORD sw1sw2;
	memcpy(buf,"\x00\xd6\x96\x00\x06\x20\x00",7);
	memcpy(buf+7,gUserGlobal.CpuSamid+2,4);
	ret = sam_apdu(buf,11,buf,&len);
//	memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));
	if(ret!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000 || buf[len-2] == 0x61)
		{return 0;}
	return 1;
}
/*=======================================================================================
函数：sam卡函数
功能：1.2 取sam卡号 0015文件
=========================================================================================*/
UBYTE Read_Sam0015(UBYTE *obuf)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x00\xb0\x95\x00\x04",5);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}
	ret = sam_apdu(buf,5,buf,&len);
//	memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));
	if(ret!=0)
		{return 1;}
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{memcpy(obuf,buf,4);
		return 0;
		}
	else if(buf[len-2]==0x61)
		{
		len= buf[len-1];
		memcpy(buf,"\x00\xc0\x00\x00\x04",5);
		buf[4]=len;
		ret = sam_apdu(buf,5,buf,&len);
//		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//		gUserGlobal.metro_prot.send_bytes = len;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{memcpy(obuf,buf,4);
			return 0;
			}
		else
			{return SHMETRO_CE_PSAMERR;}
		}

	return 1;
}
/*=======================================================================================
函数：sam卡函数
功能：1.2 取sam卡号 0005文件
=========================================================================================*/
//UBYTE Read_Sam0005(void)
//{	//USER_GLOBAL *p = &gUserGlobal;
//	UBYTE buf[256],ret,len;
//	UWORD sw1sw2;
//	memcpy(buf,"\x00\xb0\x85\x00\x00",5);
//	ret = sam_apdu(buf,5,buf,&len);
//	if(ret!=0)
//		{return 1;}
////	memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
////	gUserGlobal.metro_prot.send_bytes = len;
////	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
////	prot_shmetro_send_process(&(p->metro_prot));
//	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
//	if(sw1sw2 == 0x9000)
//		{return 0;}
//	else if(buf[len-2]==0x61)
//		{
//		len= buf[len-1];
//		memcpy(buf,"\x00\xc0\x00\x00\x0c",5);
//		buf[4]=len;
//		ret = sam_apdu(buf,5,buf,&len);
////		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
////		gUserGlobal.metro_prot.send_bytes = len;
////		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
////		prot_shmetro_send_process(&(p->metro_prot));
//		if(ret!=0)
//			{return SHMETRO_CE_PSAMERR;}
//		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
//		if(sw1sw2 == 0x9000)
//			{
//			memcpy(gUserGlobal.M1Samid2,buf+4,6);
//			return 0;
//			}
//		else
//			{return SHMETRO_CE_PSAMERR;}
//		}
//
//	return 1;
//}
/*=======================================================================================
函数：sam卡函数
功能：1.2 取sam卡号 0016文件
=========================================================================================*/
UBYTE Read_Sam0016(UBYTE *obuf)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x00\xb0\x96\x00\x06",5);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}
	ret = sam_apdu(buf,5,buf,&len);
	if(ret!=0)
		{return 1;}
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{
		memcpy(obuf,buf,6);
		return 0;
		}
	else if(buf[len-2]==0x61)
		{
		len= buf[len-1];
		memcpy(buf,"\x00\xc0\x00\x00\x0c",5);
		buf[4]=len;
		ret = sam_apdu(buf,5,buf,&len);
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{memcpy(obuf,buf,6);
			return 0;
			}
		else
			{return SHMETRO_CE_PSAMERR;}
		}

	return 1;
}
/*=======================================================================================
函数：sam卡函数
功能：1.3 读密钥版本，0017文件
=========================================================================================*/
UBYTE Read_Keyversion(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[32],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x00\xb0\x97\x00\x01",5);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}
	ret = sam_apdu(buf,5,buf,&len);
	if(ret!=0)
		{return 1;}
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 != 0x9000)
		{return 1;}
	gUserGlobal.Sam_keyversion=buf[0];
	return 0;
}
/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡80ca认证
=========================================================================================*/
UBYTE Sam_80ca(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[64],ret,len;
	UWORD sw1sw2;
	memset(buf,0,20);
	memcpy(buf,"\x80\xca\x00\x00\x09",5);
	memcpy(buf+5,gUserGlobal.Cpu_80CAbuf,9);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,20);
		gUserGlobal.metro_prot.send_bytes = 20;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,14,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{return SHMETRO_CE_PSAMERR;}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 != 0x9000)
		{return SHMETRO_CE_INVALIDCARD;}
	return 0;
}

/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡认证
=========================================================================================*/
UBYTE Sam_Ext_auth(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
	memset(buf,0,20);
	memcpy(buf,"\x80\x1a\x45\x01\x10",5);
	buf[5] = gUserGlobal.ADF1_file0015[19];//gUserGlobal.CardSnr[3];
	buf[6] = gUserGlobal.ADF1_file0015[18];//gUserGlobal.CardSnr[2];
	buf[7] = gUserGlobal.ADF1_file0015[17];//gUserGlobal.CardSnr[1];
	buf[8] = gUserGlobal.ADF1_file0015[16];//gUserGlobal.CardSnr[0];
	buf[9] = gUserGlobal.ADF1_file0015[15];
	buf[10] = gUserGlobal.ADF1_file0015[14];
	buf[11] = gUserGlobal.ADF1_file0015[13];
	buf[12] = gUserGlobal.ADF1_file0015[12];
//	memcpy(buf+5,gUserGlobal.ADF1_file0015+12,8);

	memcpy(buf+13,"\x20\x00\xff\x0\x0\x0\x0\x0",8);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,21,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{return SHMETRO_CE_PSAMERR;}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 != 0x9000)
		{return SHMETRO_CE_PSAMERR;}
	memcpy(buf,"\x80\xfa\x00\x00\x08",5);
	memcpy(buf+5,gUserGlobal.Random,8);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,13);
		watchdog();
		gUserGlobal.metro_prot.send_bytes = 13;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,13,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		watchdog();
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{return SHMETRO_CE_PSAMERR;}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{memcpy(gUserGlobal.Mac,buf,8);
		return 0;
		}
	else if(sw1sw2 == 0x6108)
		{memcpy(buf,"\x00\xc0\x00\x00\x08",5);
		ret = sam_apdu(buf,5,buf,&len);
		if(debugflag1)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			watchdog();
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.Mac,buf,8);
			return 0;
			}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
	return SHMETRO_CE_PSAMERR;
}
/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡MAC1认证
=========================================================================================*/
UBYTE Sam_mac1_sh(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x80\x70\x00\x01\x28",5);
	memcpy(buf+5,gUserGlobal.Random,4);
	buf[9] = gUserGlobal.ADF1_Cnt[0];
	buf[10] = gUserGlobal.ADF1_Cnt[1];
	memcpy(buf+11,gUserGlobal.TransValue,4);
	buf[15] = 0x09;		//0x09;复合消费或者简单消费
	memcpy(buf+16,gUserGlobal.Date,7);
	buf[23] = gUserGlobal.Sam_keyversion;
	buf[24] = gUserGlobal.ADF1_Algorithm;
	memcpy(buf+25,gUserGlobal.PosCnt,4);//流水号
	memcpy(buf+29,gUserGlobal.ADF1_file0015+12,8);//卡号
	buf[37] = 0x20;//gUserGlobal.CityNo[0];
	buf[38] = 0x00;//gUserGlobal.CityNo[1];
	memcpy(buf+39,"\xff\x00\x00\x00\x00\x00",6);

//	memcpy(buf+25,gUserGlobal.ADF1_file0015+12,8);//卡号
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,45);
		gUserGlobal.metro_prot.send_bytes = 45;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,45,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{return SHMETRO_CE_PSAMERR;}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{return 0;}
	if(sw1sw2 == 0x6108)
		{memcpy(buf,"\x00\xc0\x00\x00\x08",5);
		ret = sam_apdu(buf,5,buf,&len);
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		if(debugflag1)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.SamCnt,buf,4);
			memcpy(gUserGlobal.Mac,buf+4,4);
			return 0;
			}
		else if(sw1sw2 == 0x9302)
			{return SHMETRO_CE_MAC1ERR;}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
	return SHMETRO_CE_PSAMERR;
}
/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡MAC2认证
=========================================================================================*/
UBYTE Sam_mac2(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x80\x72\x00\x00\x04",5);
	memcpy(buf+5,gUserGlobal.Mac,4);
//	memcpy(gUserGlobal.metro_prot.send_buf,buf,9);
//	gUserGlobal.metro_prot.send_bytes = 9;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));
	ret = sam_apdu(buf,9,buf,&len);
	if(ret!=0)
		{return 1;}
//	memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));

	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 != 0x9000)
		{
		memcpy(gUserGlobal.Tac,"\x00\x00\x00\x00",4);
		return SHMETRO_CE_MAC2ERR;
		}
	return 0;
}


/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡MAC1认证
=========================================================================================*/
UBYTE pboc_cappmac4(UBYTE para,UBYTE *inbuf)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x80\x1a\x28\x01\x08",5);
	buf[3] = para;
	memcpy(buf+5,gUserGlobal.MT_UID+3,8);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,13);
		gUserGlobal.metro_prot.send_bytes = 13;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,13,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{return SHMETRO_CE_PSAMERR;}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 != 0x9000)
		{return SHMETRO_CE_PSAMERR;}
//80 fa 05 00 18
	memcpy(buf,"\x80\xfa\x05\x00\x50",5);
	memcpy(buf+5,gUserGlobal.Random,8);
	memcpy(buf+13,inbuf,64);
	memcpy(buf+77,"\x80\x00\x00\x00\x00\x00\x00\x00",8);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,85);
		gUserGlobal.metro_prot.send_bytes = 85;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,85,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{return SHMETRO_CE_PSAMERR;}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{return SHMETRO_CE_PSAMERR;}
	else if(sw1sw2 == 0x6104)
		{memcpy(buf,"\x00\xc0\x00\x00\x04",5);
		ret = sam_apdu(buf,5,buf,&len);
		if(debugflag1)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.Mac,buf,4);
			return 0;
			}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
	return SHMETRO_CE_PSAMERR;
}

/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡取随机数
=========================================================================================*/
UBYTE Sam_getrandom(UBYTE len)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret;
	UWORD sw1sw2;
	memcpy(buf,"\x0\x84\x00\x00\x08",5);
	buf[4] = len;
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		}
	ret = sam_apdu(buf,5,buf,&len);
	if(ret!=0)
		{return 1;}
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{
		memcpy(gUserGlobal.Random,buf,len);
		return 0;
		}
	return 2;
}

/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡TAC计算
//CardBak[64];
  //CardBak内容：
  //CardSnr 4字节(0~3)，Balance 交易前金额4字节(4~7)(high--low(M1卡为Block6的前4字节))
  //随机数4字节(8~11)，卡计数器2字节(12\13)，交易金额4字节(14~17)(high--low)
  //(cpu卡交易类型,18)，交易日期时间7字节(19~25)
  //cpu卡消费初始化的密钥版本1字节(26)、算法标示1字节(27)，
//CardPoint(28),Metrosta(29),gUserGlobal.BalanceL~H(30~32);

=========================================================================================*/
UBYTE Sam_Tac(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
//80 1a 44 01 10
	memcpy(buf,"\x80\x1a\x44\x01\x10",5);
	memcpy(buf+5,gUserGlobal.CardSnr,4);
	memcpy(buf+9,"\xff\x0\x0\x0\x20\x0\xff\x0\x0\x0\x0\x0",12);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,21,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
		return 0;//return SHMETRO_CE_PSAMERR;
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 != 0x9000)
		{memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
		return 0;//return SHMETRO_CE_PSAMERR;
		}
//80 fa 05 00 18
	memcpy(buf,"\x80\xfa\x01\x00\x20",5);
	buf[5] = 0x88;
	buf[6] = gUserGlobal.PosCnt[1];
	buf[7] = gUserGlobal.PosCnt[2];
	buf[8] = gUserGlobal.PosCnt[3];
	buf[9] = gUserGlobal.CityNo[0];
	buf[10] = gUserGlobal.CityNo[1];
	memcpy(buf+11,gUserGlobal.CardSnr,4);
	buf[15] = gUserGlobal.CardType;
	buf[16] = gUserGlobal.CardBak[30];		//低字节BalanceL
	buf[17] = gUserGlobal.CardBak[31];
	buf[18] = gUserGlobal.CardBak[32];
	buf[19] = gUserGlobal.CardBak[17];	//交易金额低字节,交易金额4字节(14~17)(high--low)
	buf[20] = gUserGlobal.CardBak[16];	//
	buf[21] = gUserGlobal.CardBak[15];
	memcpy(buf+22,gUserGlobal.CardBak+19,7);	//交易日期时间7字节(19~25)
	buf[29] = gUserGlobal.CardBak[12];	//卡计数器2字节(12\13)
	buf[30] = gUserGlobal.CardBak[13];
	memcpy(buf+31,gUserGlobal.CpuSamid+2,4);		//CPU卡sam卡卡号后4字节
	buf[35] = 0x80;
	buf[36] = 0x00;
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,37);
		gUserGlobal.metro_prot.send_bytes = 37;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,37,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
		return 0;//return SHMETRO_CE_PSAMERR;
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{memcpy(gUserGlobal.Tac,buf,4);
		return 0;
		}
	else if(sw1sw2 == 0x6104)
		{memcpy(buf,"\x00\xc0\x00\x00\x04",5);
		ret = sam_apdu(buf,5,buf,&len);
		if(debugflag1)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		if(ret!=0)
			{memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
			return 0;//return SHMETRO_CE_PSAMERR;
			}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.Tac,buf,4);
			return 0;
			}
		else
			{memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
			return 0;//return SHMETRO_CE_PSAMERR;
			}
		}
	memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
	return 0;//return SHMETRO_CE_PSAMERR;
}

/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡计算老人卡验证码
=========================================================================================*/
UBYTE Calc_Tac(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
//80 1a 44 01 10
	memcpy(buf,"\x80\x1a\x44\x01\x10",5);
	memcpy(buf+5,gUserGlobal.CardSnr,4);
	memcpy(buf+9,"\xff\x0\x0\x0\x20\x0\xff\x0\x0\x0\x0\x0",12);
//	if(debugflag)
//		{memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
//		gUserGlobal.metro_prot.send_bytes = 21;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	ret = sam_apdu(buf,21,buf,&len);
//	if(debugflag)
//		{memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//		gUserGlobal.metro_prot.send_bytes = len;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	if(ret!=0)
		{return SHMETRO_CE_PSAMERR;}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 != 0x9000)
		{return SHMETRO_CE_PSAMERR;}
//80 fa 05 00 18
	memcpy(buf,"\x80\xfa\x01\x00\x20",5);
	memcpy(buf+5,gUserGlobal.B28,13);
	buf[18] = 0x20;
	buf[19] = 0x00;
	memcpy(buf+20,gUserGlobal.CardSnr,4);
	memcpy(buf+24,"\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0",13);
//	if(debugflag)
//		{memcpy(gUserGlobal.metro_prot.send_buf,buf,37);
//		gUserGlobal.metro_prot.send_bytes = 37;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	ret = sam_apdu(buf,37,buf,&len);
//	if(debugflag)
//		{memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//		gUserGlobal.metro_prot.send_bytes = len;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	if(ret!=0)
		{return SHMETRO_CE_PSAMERR;}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{memcpy(gUserGlobal.Tac,buf,4);
		return 0;
		}
	else if(sw1sw2 == 0x6104)
		{memcpy(buf,"\x00\xc0\x00\x00\x04",5);
		ret = sam_apdu(buf,5,buf,&len);
//		if(debugflag)
//			{memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
//			gUserGlobal.metro_prot.send_bytes = len;
//			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//			prot_shmetro_send_process(&(p->metro_prot));
//			delay_ms(10);
//			}
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.Tac,buf,4);
			return 0;
			}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
	return SHMETRO_CE_PSAMERR;
}

/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡TAC计算
=========================================================================================*/
UBYTE Sjt_Tac(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
//80 fa 05 00 18
//	memset(buf,0,37);
	memcpy(buf,"\x00\x88\x02\x03\x20",5);
	memcpy(buf+5,gUserGlobal.CardSnr,4);
	memcpy(buf+9,gUserGlobal.Sjtbl3,4);//发卡流水号 + 版本号
	memcpy(buf+13,gUserGlobal.Date,4);
	memcpy(buf+17,"\x00\x00\x00\x00\x00\x00\x00\x00",8);
	buf[17] = gUserGlobal.Psw;
	buf[21] = gUserGlobal.CardType;
	buf[25] = gUserGlobal.TransValue[2];
	buf[26] = gUserGlobal.TransValue[3];
	buf[27] = 0x00;
	buf[28] = 0x00;
	
	buf[29] = gUserGlobal.Sjtremainh;
	buf[30] = gUserGlobal.Sjtremainl;
	buf[31] = 0x00;
	buf[32] = 0x00;
	memcpy(buf+33,gUserGlobal.Posid,4);
//	memcpy(buf+23,gUserGlobal.PosCnt,4);
//	memcpy(buf+27,gUserGlobal.SjtCount,2);
//	memcpy(buf+29,gUserGlobal.SjtSamid+2,4);
//	memcpy(buf+33,"\x00\x0\x0\x00",4);

	if(debugflag1)
		{memcpy(gUserGlobal.metro_prot.send_buf,buf,37);
		gUserGlobal.metro_prot.send_bytes = 37;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,37,buf,&len);
	if(debugflag1)
		{memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
		return 0;//return SHMETRO_CE_PSAMERR;
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{memcpy(gUserGlobal.Tac,buf,4);
		return 0;
		}
	else if(sw1sw2 == 0x6104)
		{memcpy(buf,"\x00\xc0\x00\x00\x04",5);
		if(debugflag1)
			{memcpy(gUserGlobal.metro_prot.send_buf,buf,5);
			gUserGlobal.metro_prot.send_bytes = 5;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		ret = sam_apdu(buf,5,buf,&len);
		if(debugflag1)
			{memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		if(ret!=0)
			{memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
			return 0;//return SHMETRO_CE_PSAMERR;
			}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.Tac,buf,4);
			return 0;
			}
		else
			{memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
			return 0;//return SHMETRO_CE_PSAMERR;
			}
		}
	memcpy(gUserGlobal.Tac,"\xff\xff\xff\xff",4);
	return 0;//return SHMETRO_CE_PSAMERR;
}
/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡计算锁卡MAC
=========================================================================================*/
UBYTE Sam_Lockmac(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x80\x1a\x45\x01\x10",5);
	memcpy(buf+5,gUserGlobal.ADF1_file0015+12,8);//卡号
	memcpy(buf+13,"\x20\x00\xff\x00\x00\x00\x00\x00",8);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,21,buf,&len);
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(ret!=0)
		{return SHMETRO_CE_PSAMERR;}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 != 0x9000)
		{return SHMETRO_CE_PSAMERR;}
	if(sw1sw2 == 0x9000)
		{
		memcpy(buf,"\x80\xfa\x05\x00\x10",5);
		memcpy(buf+5,gUserGlobal.Random,4);
		memcpy(buf+9,"\x00\x00\x00\x00\x84\x1e\x00\x00\x04\x80\x0\x0",12);
		buf[16] = gUserGlobal.Ride[1];
		ret = sam_apdu(buf,21,buf,&len);
		if(ret!=0)
			{return SHMETRO_CE_PSAMERR;}
		if(debugflag1)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.Mac,buf,4);
			return 0;
			}
		else if(sw1sw2 == 0x6104)
			{memcpy(buf,"\x00\xc0\x00\x00\x04",5);
			ret = sam_apdu(buf,5,buf,&len);
			if(debugflag1)
				{
				watchdog();
				memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
				gUserGlobal.metro_prot.send_bytes = len;
				gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
				prot_shmetro_send_process(&(p->metro_prot));
				delay_ms(10);
				}
			if(ret!=0)
				{return SHMETRO_CE_PSAMERR;}
			sw1sw2 = (buf[len-2]<<8)+buf[len-1];
			if(sw1sw2 == 0x9000)
				{
				memcpy(gUserGlobal.Mac,buf,4);
				return 0;
				}
			else
				{return SHMETRO_CE_PSAMERR;}
			}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
	return SHMETRO_CE_PSAMERR;
}
/*=======================================================================================
函数：寻卡
功能：0.3 寻卡，返回TagType，单程票为3
=========================================================================================*/
UBYTE HL_Active(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,rdbuf[256];
	UWORD temp1;
	gUserGlobal.TagType=0x00;
	watchdog();
	temp = mcml_request(rdbuf);
	if(debugflag)
		{
		watchdog();
		gUserGlobal.metro_prot.send_buf[0] = temp;
		gUserGlobal.metro_prot.send_bytes = 1;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{watchdog();
		if(temp == 0xfe)
			{fm_reset();
			delay_ms(10);
			}
		temp = mcml_request(rdbuf);
		if(temp == 0x00)
			{goto active1_ok;}
		if(temp == 0xfe)
			{return 1;}
		watchdog();
		mcml_pwr_off();
		delay_ms(5);
		mcml_pwr_on();
		delay_ms(5);
//		mcml_cardtype(0);
		temp = mcml_request(rdbuf);
		if(debugflag)
			{
			watchdog();
			gUserGlobal.metro_prot.send_buf[0] = temp;
			gUserGlobal.metro_prot.send_bytes = 2;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		if(temp != 0x00)
			{return 1;}
		}
active1_ok:
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,3);
		gUserGlobal.metro_prot.send_bytes = 3;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(rdbuf[0] == 0x03||rdbuf[0] == 0x05)
		{
		mcml_cardtype(0);
		memset(gUserGlobal.CardSnr,0,4);
		temp = mcml_select_token(gUserGlobal.CardSnr,rdbuf);
		temp = mcml_read(0x01,gUserGlobal.Sjtbl1);//tocken_readnoauth(0x01,gUserGlobal.Sjtbl1);
//		if(temp!=0)
//			{return 1;}
//		temp=tocken_read(0x01,gUserGlobal.Sjtbl1);
		if(temp != 0)
			{return 1;}		
		gUserGlobal.CardSnr[0]=gUserGlobal.Sjtbl1[0];
		gUserGlobal.CardSnr[1]=gUserGlobal.Sjtbl1[1];
		gUserGlobal.CardSnr[2]=gUserGlobal.Sjtbl1[2];
		gUserGlobal.CardSnr[3]=gUserGlobal.Sjtbl1[3];
		gUserGlobal.CardSnr[4]= gUserGlobal.CardSnr[0]^gUserGlobal.CardSnr[1]^gUserGlobal.CardSnr[2]^gUserGlobal.CardSnr[3];
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.CardSnr,4);
			gUserGlobal.metro_prot.send_bytes = 4;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		mcml_pwr_off();
		delay_ms(5);
		mcml_pwr_on();
		delay_ms(5);		
		temp = mcml_request(rdbuf);
		if(temp!=0)
			{//delay_ms(5);
			temp = mcml_request(rdbuf);
			if(temp == 0x00)
				{goto xx1;}
			else
				{return 1;}
			}
xx1:
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
			gUserGlobal.metro_prot.send_bytes = 5;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		temp = mcml_select_token(gUserGlobal.CardSnr,rdbuf);
		gUserGlobal.TagType=0x03;//单程票
		return 0;
		}
	mcml_cardtype(2);
	temp = mcml_anticoll(rdbuf);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,4);
		gUserGlobal.metro_prot.send_bytes = 4;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if((rdbuf[0]!=p->CardSnr[0])||(rdbuf[1]!=p->CardSnr[1])||(rdbuf[2]!=p->CardSnr[2])||(rdbuf[3]!=p->CardSnr[3]))
		{gUserGlobal.Cpuflag = 0x00;}
//	chk_same();
	memcpy(gUserGlobal.CardSnr,rdbuf,5);
	temp = mcml_select2(gUserGlobal.CardSnr,rdbuf);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.ShSign=rdbuf[0];
	gUserGlobal.sameflag = 0x00;
	if(gUserGlobal.Err17Flag != 0)
		{
		if((gUserGlobal.CardBak[0]==gUserGlobal.CardSnr[0])&&(gUserGlobal.CardBak[1]==gUserGlobal.CardSnr[1])&&(gUserGlobal.CardBak[2]==gUserGlobal.CardSnr[2])&&(gUserGlobal.CardBak[3]==gUserGlobal.CardSnr[3]))
			{gUserGlobal.sameflag = 0x01;
			if(gUserGlobal.Err17Flag == 0x01)//m1
				{memcpy(rdbuf,"\xfc\x61\xcb\xb5\x37\x51",6);
				temp = fm_mcmh_authentication(0,0,rdbuf);//0扇区
				if(temp != 0x00)
					{return 1;}
				temp = mcml_read(1,gUserGlobal.B1);
				if(temp != 0x00)
					{return 1;}
				temp = Chk_xor_sum(gUserGlobal.B1,16);
				if(temp!=0)
					{return 1;}
				temp = mcml_read(2,gUserGlobal.B2);
				if(temp != 0x00)
					{return 1;}
				if(gUserGlobal.B1[0]<0x10)
					{return SHMETRO_CE_INVALIDCARD;}
				gUserGlobal.CityNo[0] = gUserGlobal.B1[6];
				gUserGlobal.CityNo[1] = gUserGlobal.B1[7];
				temp1 = (gUserGlobal.CityNo[0]<<8)+gUserGlobal.CityNo[1];
				if(temp1==0x2000&&gUserGlobal.ShSign!=0x53)
					{return SHMETRO_CE_INVALIDCARD;}
				return 0;
				}
			else if(gUserGlobal.Err17Flag == 0x11)
				{delay_ms(5);
				temp = fm_rats(0x01,rdbuf);
				if(temp != 0)
					{return 1;}
				gUserGlobal.TagType=0x02;//CPU卡
				if((gUserGlobal.ErrCardSnr[0]==gUserGlobal.CardSnr[0])&&(gUserGlobal.ErrCardSnr[1]==gUserGlobal.CardSnr[1])&&(gUserGlobal.ErrCardSnr[2]==gUserGlobal.CardSnr[2])&&(gUserGlobal.ErrCardSnr[3]==gUserGlobal.CardSnr[3]))
					{return SHMETRO_CE_INVALIDCARD;}
				delay_ms(5);
				return 0;
				}
			}
		}
	temp = gUserGlobal.ReadMode&0xfe;
	if(temp==0x90)
		{
		delay_ms(5);
//		temp = mifpro_ats(0x00,rdbuf,&rdlen);
		temp = fm_rats(0x01,rdbuf);
		if(temp != 0)
			{goto M1_label1;}
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,16);
			gUserGlobal.metro_prot.send_bytes = 16;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
//		if((rdbuf[1]&0x10) == 0x10)
//			{if((rdbuf[2]& 0x22) == 0x22)
//				{temp = fm_pps(0x11,0X0A,rdbuf);
//				}
//			else if((rdbuf[2]& 0x11) == 0x11)
//				{temp = fm_pps(0x11,0X05,rdbuf);}
//			}
		gUserGlobal.TagType=0x02;//CPU卡
		if((gUserGlobal.ErrCardSnr[0]==gUserGlobal.CardSnr[0])&&(gUserGlobal.ErrCardSnr[1]==gUserGlobal.CardSnr[1])&&(gUserGlobal.ErrCardSnr[2]==gUserGlobal.CardSnr[2])&&(gUserGlobal.ErrCardSnr[3]==gUserGlobal.CardSnr[3]))
			{return SHMETRO_CE_INVALIDCARD;}
		delay_ms(5);
		return 0;
		}
	else
		{goto M1_label2;}

M1_label1:
	mcml_pwr_off();
	delay_ms(5);
	mcml_pwr_on();
	delay_ms(5);
	temp = mcml_request(rdbuf);
	if(temp!=0)
		{//delay_ms(10);
		temp = mcml_request(rdbuf);
		if(temp==0)
			{goto active2_ok;}
		mcml_pwr_off();
		delay_ms(5);
		mcml_pwr_on();
		delay_ms(5);
		temp = mcml_request(rdbuf);
		if(temp!=0)
			{return 1;}
		}
active2_ok:
	temp = mcml_anticoll(rdbuf);
	if(temp!=0)
		{return 1;}
	temp = mcml_select2(gUserGlobal.CardSnr,rdbuf);
	if(temp!=0)
		{return 1;}
M1_label2:
	memcpy(rdbuf,"\xfc\x61\xcb\xb5\x37\x51",6);
	temp = fm_mcmh_authentication(0,0,rdbuf);//0扇区
	if(temp != 0x00)
		{goto M1_label3;}
	temp = mcml_read(1,gUserGlobal.B1);
	if(temp != 0x00)
		{goto M1_label3;}
	temp = Chk_xor_sum(gUserGlobal.B1,16);
//	temp = 0x01;
	if(temp!=0)
		{return SHMETRO_CE_INVALIDCARD;}
	temp = mcml_read(2,gUserGlobal.B2);
	if(temp != 0x00)
		{return 1;}
	if(gUserGlobal.B1[0]<0x10)
		{return SHMETRO_CE_INVALIDCARD;}
	gUserGlobal.CityNo[0] = gUserGlobal.B1[6];
	gUserGlobal.CityNo[1] = gUserGlobal.B1[7];
	temp1 = (gUserGlobal.CityNo[0]<<8)+gUserGlobal.CityNo[1];
	if(temp1==0x2000&&gUserGlobal.ShSign!=0x53)
		{return SHMETRO_CE_INVALIDCARD;}
	return 0;
M1_label3:
	mcml_pwr_off();
	delay_ms(5);
	mcml_pwr_on();
	delay_ms(5);
	temp = mcml_request(rdbuf);
	if(temp!=0)
		{return 1;}
	temp = mcml_anticoll(rdbuf);
	if(temp!=0)
		{return 1;}
	temp = mcml_select2(gUserGlobal.CardSnr,rdbuf);
	if(temp!=0)
		{return 1;}
	delay_ms(5);
	temp = fm_rats(0x01,rdbuf);
	if(temp != 0)
		{return 1;}
	gUserGlobal.TagType=0x02;//CPU卡
	if((gUserGlobal.ErrCardSnr[0]==gUserGlobal.CardSnr[0])&&(gUserGlobal.ErrCardSnr[1]==gUserGlobal.CardSnr[1])&&(gUserGlobal.ErrCardSnr[2]==gUserGlobal.CardSnr[2])&&(gUserGlobal.ErrCardSnr[3]==gUserGlobal.CardSnr[3]))
		{return SHMETRO_CE_INVALIDCARD;}
	delay_ms(5);
	return 0;
}
/*=======================================================================================
函数：寻卡
功能：0.3 寻卡
=========================================================================================*/
UBYTE HL_Active2(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,rdbuf[256];
	UWORD temp1;

	mcml_pwr_off();
	delay_ms(15);
	mcml_pwr_on();
	delay_ms(15);
	temp = mcml_request(rdbuf);
	if(temp != 0x00)
		{delay_ms(15);
		temp = mcml_request(rdbuf);
		if(temp != 0x00)
			{return 1;}
		}
	temp = mcml_anticoll(rdbuf);
	if(temp!=0)
		{return 1;}
	memcpy(gUserGlobal.CardSnr,rdbuf,5);
	temp = mcml_select2(gUserGlobal.CardSnr,rdbuf);
	if(temp!=0)
		{return 1;}
	gUserGlobal.ShSign=rdbuf[0];
	memcpy(rdbuf,"\xfc\x61\xcb\xb5\x37\x51",6);
	temp = fm_mcmh_authentication(0,0,rdbuf);//0扇区
	if(temp != 0x00)
		{return 2;}
	temp = mcml_read(1,gUserGlobal.B1);
	if(temp != 0x00)
		{return 2;}
	temp = Chk_xor_sum(gUserGlobal.B1,16);
	if(temp!=0)
		{return SHMETRO_CE_INVALIDCARD;}
	temp = mcml_read(2,gUserGlobal.B2);
	if(temp != 0x00)
		{return 1;}
	if(gUserGlobal.B1[0]<0x10)
		{return SHMETRO_CE_INVALIDCARD;}
	gUserGlobal.CityNo[0] = gUserGlobal.B1[6];
	gUserGlobal.CityNo[1] = gUserGlobal.B1[7];
	temp1 = (gUserGlobal.CityNo[0]<<8)+gUserGlobal.CityNo[1];
	if(temp1==0x2000&&gUserGlobal.ShSign!=0x53)
		{return SHMETRO_CE_INVALIDCARD;}
	return 0;
}

/*=======================================================================================
函数：寻卡
功能：0.3 寻卡
=========================================================================================*/
UBYTE HL_Activecpu(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,rdbuf[256];
//	UWORD temp1;

	mcml_pwr_off();
	delay_ms(15);
	mcml_pwr_on();
	delay_ms(15);
	temp = mcml_request(rdbuf);
	if(temp != 0x00)
		{delay_ms(15);
		temp = mcml_request(rdbuf);
		if(temp != 0x00)
			{return 1;}
		}
	temp = mcml_anticoll(rdbuf);
	if(temp!=0)
		{return 1;}
	memcpy(gUserGlobal.CardSnr,rdbuf,5);
	temp = mcml_select2(gUserGlobal.CardSnr,rdbuf);
	if(temp!=0)
		{return 1;}
	gUserGlobal.ShSign=rdbuf[0];
	delay_ms(5);
	temp = fm_rats(0x01,rdbuf);
	if(temp != 0)
		{return 1;}
	gUserGlobal.TagType=0x02;//CPU卡
	if((gUserGlobal.ErrCardSnr[0]==gUserGlobal.CardSnr[0])&&(gUserGlobal.ErrCardSnr[1]==gUserGlobal.CardSnr[1])&&(gUserGlobal.ErrCardSnr[2]==gUserGlobal.CardSnr[2])&&(gUserGlobal.ErrCardSnr[3]==gUserGlobal.CardSnr[3]))
		{return SHMETRO_CE_INVALIDCARD;}
	delay_ms(5);
	return 0;
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.4 选择建设部标准的adf1目录
=========================================================================================*/
UBYTE Cpu_getrandom(UBYTE len)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],temp;
	if(len>8)
		{return 3;}
	memcpy(buf,"\x00\x84\x00\x00",4);
	buf[4] = len;
	temp = mifpro_icmd(buf,5,buf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	memcpy(gUserGlobal.Random,buf,len);
	return 0;
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.4 选择建设部标准的adf1目录
=========================================================================================*/
UBYTE Cpu_Lockcard(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],len,temp;

	memcpy(buf,"\x84\x1e\x00\x00\x04",5);
	buf[3] = gUserGlobal.Ride[1];
	memcpy(buf+5,gUserGlobal.Mac,4);
	temp = mifpro_icmd(buf,9,buf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	return 0;
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.4 选择建设部标准的adf1目录
=========================================================================================*/
UBYTE Cpu_sel1001(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],buf[256],len,i,j,temp;

	memcpy(rdbuf,"\x00\xa4\x04\x00\x09\xa0\x00\x00\x00\x03\x86\x98\x07\x01",14);
	temp = mifpro_icmd(rdbuf,14,buf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}

	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2==0x9000||gUserGlobal.SW1SW2==0x6A81)
		{for(i=0;i<len;i++)
			{
			if((buf[i+5]==0x9f)&&(buf[i+6]==0x0c))
				{break;}
			}
		for(j=0;j<30;j++)
			{gUserGlobal.ADF1_file0015[j]=buf[i+j+8];}
		gUserGlobal.CityNo[0] = gUserGlobal.ADF1_file0015[2];
		gUserGlobal.CityNo[1] = gUserGlobal.ADF1_file0015[3];
		gUserGlobal.Cardversion = gUserGlobal.ADF1_file0015[9];//buffer[i+17];
		gUserGlobal.CardType = gUserGlobal.ADF1_file0015[28];//buffer[i+36];
		return 0;
		}
	else
		{return 2;}
}
/*=======================================================================================
函数：非接卡函数
功能：0.5 ADF1消费初始化8050 用来
=========================================================================================*/
UBYTE Ini_purchase_ADF1(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	UDWORD temp1;
	memcpy(rdbuf,"\x80\x50\x01\x02\x0b",5);
	rdbuf[5] = gUserGlobal.Sam_keyversion;
	rdbuf[6] = gUserGlobal.TransValue[0];
	rdbuf[7] = gUserGlobal.TransValue[1];
	rdbuf[8] = gUserGlobal.TransValue[2];
	rdbuf[9] = gUserGlobal.TransValue[3];
	memcpy(rdbuf+10,gUserGlobal.CpuSamid,6);
//	rdbuf[10] = gUserGlobal.CpuSamid[0];
	rdbuf[16] = 0x0f;

	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,17);
		gUserGlobal.metro_prot.send_bytes = 17;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,17,rdbuf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	memcpy(gUserGlobal.ADF1_Balance,rdbuf,4);//余额
	gUserGlobal.ADF1_Cnt[0]=rdbuf[4];	//卡计数器
	gUserGlobal.ADF1_Cnt[1]=rdbuf[5];
	gUserGlobal.ADF1_CardOverDraft[1]=rdbuf[6];//透支限额
	gUserGlobal.ADF1_CardOverDraft[2]=rdbuf[7];//透支限额
	gUserGlobal.ADF1_CardOverDraft[3]=rdbuf[8];//透支限额

	gUserGlobal.ADF1_Algorithm=rdbuf[10];//算法标识
	gUserGlobal.Random[0]=rdbuf[11];
	gUserGlobal.Random[1]=rdbuf[12];
	gUserGlobal.Random[2]=rdbuf[13];
	gUserGlobal.Random[3]=rdbuf[14];

	gUserGlobal.Balance=gUserGlobal.ADF1_Balance[3]+(gUserGlobal.ADF1_Balance[2]<<8)+(gUserGlobal.ADF1_Balance[1]<<16)+(gUserGlobal.ADF1_Balance[0]<<24);
	if((gUserGlobal.device_state&0xf0)==0x60)
		{return 0;}
	temp1=rdbuf[8]+(rdbuf[7]<<8)+(rdbuf[6]<<16);
	if(gUserGlobal.CityNo[0]==0x21&&gUserGlobal.CityNo[1]==0x53)
		{gUserGlobal.Balance=gUserGlobal.Balance-temp1;}
	if(gUserGlobal.CityNo[0]==0x20&&gUserGlobal.CityNo[1]==0x00)
		{gUserGlobal.Balance=gUserGlobal.Balance-temp1;}
	gUserGlobal.ADF1_Balance[3]=gUserGlobal.Balance%0x100;
	gUserGlobal.ADF1_Balance[2]=(gUserGlobal.Balance>>8)%0x100;
	gUserGlobal.ADF1_Balance[1]=(gUserGlobal.Balance>>16)%0x100;
	gUserGlobal.ADF1_Balance[0]=(gUserGlobal.Balance>>24)%0x100;

	return 0;
}
/*=======================================================================================
函数：非接卡函数
功能：0.6 ADF1复合消费初始化8050
=========================================================================================*/
UBYTE Ini_capp_ADF1(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x80\x50\x03\x02\x0b",5);
	rdbuf[5] = gUserGlobal.Sam_keyversion;
	rdbuf[6] = gUserGlobal.TransValue[0];	//交易金额
	rdbuf[7] = gUserGlobal.TransValue[1];
	rdbuf[8] = gUserGlobal.TransValue[2];
	rdbuf[9] = gUserGlobal.TransValue[3];
	memcpy(rdbuf+10,gUserGlobal.CpuSamid,6);
//	rdbuf[10] = gUserGlobal.CpuSamid[0];
	rdbuf[16] = 0x0f;
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,17);
		gUserGlobal.metro_prot.send_bytes = 17;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,17,rdbuf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
//	memcpy(gUserGlobal.ADF1_Balance,rdbuf,4);//余额
	gUserGlobal.ADF1_Cnt[0]=rdbuf[4];	//卡计数器
	gUserGlobal.ADF1_Cnt[1]=rdbuf[5];
	gUserGlobal.ADF1_CardOverDraft[1]=rdbuf[6];//透支限额
	gUserGlobal.ADF1_CardOverDraft[2]=rdbuf[7];//透支限额
	gUserGlobal.ADF1_CardOverDraft[3]=rdbuf[8];//透支限额
	gUserGlobal.ADF1_Algorithm=rdbuf[10];//算法标识
	gUserGlobal.Random[0]=rdbuf[11];
	gUserGlobal.Random[1]=rdbuf[12];
	gUserGlobal.Random[2]=rdbuf[13];
	gUserGlobal.Random[3]=rdbuf[14];

	return 0;
}
/*=======================================================================================
函数：非接卡函数
功能：0.6 ADF1复合充值初始化8050
=========================================================================================*/
UBYTE Ini_forload_ADF1(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x80\x50\x00\x02\x0b",5);
	rdbuf[5] = gUserGlobal.Sam_keyversion;
	rdbuf[6] = gUserGlobal.TransValue[0];	//交易金额
	rdbuf[7] = gUserGlobal.TransValue[1];
	rdbuf[8] = gUserGlobal.TransValue[2];
	rdbuf[9] = gUserGlobal.TransValue[3];
	memcpy(rdbuf+10,gUserGlobal.ModuleNo,6);
//	rdbuf[10] = gUserGlobal.CpuSamid[0];
	rdbuf[16] = 0x0f;
//		debug_printf("\x0d\x0a");
//		for(i=0;i<17;i++)
//			{debug_printf("%02x",rdbuf[i]);}
//		delay_ms(10);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,17);
		gUserGlobal.metro_prot.send_bytes = 17;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,17,rdbuf,&len);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
//		debug_printf("\x0d\x0a");
//		for(i=0;i<len;i++)
//			{debug_printf("%02x",rdbuf[i]);}
//		delay_ms(10);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2==0x9000)
		{
		memcpy(gUserGlobal.Creditbuf,rdbuf,16);//余额
//		debug_printf("\x0d\x0a");
//		for(i=0;i<16;i++)
//			{debug_printf("%02x",gUserGlobal.Creditbuf);}
//		delay_ms(10);
//		gUserGlobal.ADF1_Cnt1[0]=rdbuf[4];	//卡计数器
//		gUserGlobal.ADF1_Cnt1[1]=rdbuf[5];
		return 0;	
		}

	return 2;
}

/*=======================================================================================
函数：非接卡函数
功能：0.6 复合消费更新缓存
=========================================================================================*/
UBYTE Update_capp0017(UBYTE *inbuf)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;

	memcpy(rdbuf,"\x80\xdc\x09\xb8\x30",5);

	memcpy(rdbuf+5,inbuf,48);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,53);
		gUserGlobal.metro_prot.send_bytes = 53;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,53,rdbuf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}

	if(temp!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	return 0;
}
UBYTE Update_capp0007(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;

	memcpy(rdbuf,"\x80\xdc\x00\x3b\x10",5);
	memcpy(rdbuf+5,gUserGlobal.RecordBuf,16);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,21,rdbuf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	return 0;
}

/************************************************************/
//更新复合消费缓存，0014文件/即第12扇区,0014 sector 12
/************************************************************/
UBYTE Update_cappS8(UBYTE point)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
//	if(point>3||point == 0)
//		{return 3;}
	memcpy(rdbuf,"\x80\xdc\x01\xa4\x10",5);
	if(point == 4)
		{memcpy(rdbuf+5,gUserGlobal.B32,16);
		rdbuf[2] = 4;
		}
	else if(point == 5)
		{memcpy(rdbuf+5,gUserGlobal.B33,16);
		rdbuf[2] = 5;
		}
	else
		{memcpy(rdbuf+5,gUserGlobal.B34,16);
		rdbuf[2] = 6;
		}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,21,rdbuf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	return 0;
}
UBYTE Update_cappS7(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x80\xdc\x02\x84\x10",5);
	temp=gUserGlobal.Posid[0]&0x3f;
	temp=temp+0x80;
	gUserGlobal.Sec7buf[2]=temp;
	gUserGlobal.Sec7buf[0]=gUserGlobal.Posid[1];
	gUserGlobal.Sec7buf[1]=gUserGlobal.Posid[3];
	gUserGlobal.Sec7buf[3]=gUserGlobal.Date[2];
	gUserGlobal.Sec7buf[4]=gUserGlobal.Date[3];
	gUserGlobal.Sec7buf[5]=gUserGlobal.Date[4];
	gUserGlobal.Sec7buf[6]=gUserGlobal.Date[5];
	lrc_gen2(gUserGlobal.Sec7buf,15);
	memcpy(rdbuf+5,gUserGlobal.Sec7buf,16);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,21,rdbuf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}

	memcpy(rdbuf,"\x80\xdc\x03\x84\x10",5);
	memcpy(rdbuf+5,gUserGlobal.Sec7buf,16);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,21,rdbuf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}

	return 0;
}
/*=======================================================================================
函数：非接卡函数
功能：0.6 复合消费 8054
=========================================================================================*/
UBYTE Debitcapp(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x80\x54\x01\x00\x0f",5);
	memcpy(rdbuf+5,gUserGlobal.SamCnt,4);		//pos流水号
	memcpy(rdbuf+9,gUserGlobal.Date,7);		//交易日期
	memcpy(rdbuf+16,gUserGlobal.Mac,4);		//mac
	rdbuf[20] = 0x08;

	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}

	temp = mifpro_icmd(rdbuf,21,rdbuf,&len);

	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}

//	memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));

	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(len == 2)
		{return 2;}
	if(len == 10)
		{
		if(gUserGlobal.SW1SW2!=0x9000)
			{return 1;}
		memcpy(gUserGlobal.Tac,rdbuf,4);//TAC
		memcpy(gUserGlobal.Mac,rdbuf+4,4);//TAC
		return 0;
		}
	else
		{return 1;}
}


/*=======================================================================================
函数：非接函数
功能：0.7 cpu卡取建设部认证码，80ca
=========================================================================================*/
UBYTE Cpu_80ca(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x80\xca\x00\x00\x09",5);
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);//cpu卡取80ca
	if(temp!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	memcpy(gUserGlobal.Cpu_80CAbuf,rdbuf,9);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.Cpu_80CAbuf,9);
		gUserGlobal.metro_prot.send_bytes = 9;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}

	return 0;
}
///*=======================================================================================
//函数：非接函数
//功能：0.8 cpu卡取4字节随机数
//=========================================================================================*/
//UBYTE Cpu_getrandom(void)
//{	USER_GLOBAL *p = &gUserGlobal;
//	UBYTE rdbuf[32],len,temp;
//	memcpy(rdbuf,"\x00\x84\x00\x00\x04",5);
//	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);//cpu卡取随机数
//	if(temp!=0)
//		{return 1;}
//	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
//	if(gUserGlobal.SW1SW2!=0x9000)
//		{return 2;}
//	memcpy(gUserGlobal.Random,rdbuf,4);
//	return 0;
//}
/*=======================================================================================
函数：非接函数
功能：0.9 cpu卡读0015二进制文件
=========================================================================================*/
UBYTE Cpu_rd0015(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x00\xb0\x95\x00\x00",5);
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);//
//	memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));
	if(temp!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	memcpy(gUserGlobal.ADF1_file0015,rdbuf,40);
	return 0;
}
/*=======================================================================================
函数：非接函数
功能：0.10 cpu卡读0018消费循环记录文件，交易明细记录
=========================================================================================*/
UBYTE Cpu_rd0018(UBYTE record,UBYTE *outbuf)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x00\xb2\x01\xc4\x17",5);
	rdbuf[2]= record;
//	if(debugflag)
//		{
//		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
//		gUserGlobal.metro_prot.send_bytes = 5;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);//
	if(temp!=0)
		{return 1;}
//	if(debugflag)
//		{
//		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
//		gUserGlobal.metro_prot.send_bytes = len;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2==0x9000)
		{memcpy(outbuf,rdbuf,23);
		return 0;
		}
	else if(gUserGlobal.SW1SW2==0x6a83)
		{memset(outbuf,0,23);
		return 0;
		}
	return 2;
}

/*=======================================================================================
函数：非接函数
功能：0.10 cpu卡读7扇区数据
=========================================================================================*/
UBYTE Cpu_rdsec7(UBYTE *outbuf,UBYTE record)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x00\xb2\x01\x84\x10",5);
	rdbuf[2]= record;
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);//
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2==0x9000)
		{memcpy(outbuf,rdbuf,16);
		return 0;
		}
	else if(gUserGlobal.SW1SW2==0x6a83)
		{memset(outbuf,0,16);
		return 0;
		}
	return 2;
}

/*=======================================================================================
函数：非接函数
功能：0.10 cpu卡读12扇区数据
=========================================================================================*/
UBYTE Cpu_rdsec8(UBYTE *outbuf,UBYTE record)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x00\xb2\x01\xa4\x10",5);
	rdbuf[2]= record;
//	if(debugflag)
//		{
//		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
//		gUserGlobal.metro_prot.send_bytes = 5;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);//
	if(temp!=0)
		{return 1;}
//	if(debugflag)
//		{
//		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
//		gUserGlobal.metro_prot.send_bytes = len;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2==0x9000)
		{memcpy(outbuf,rdbuf,16);
		return 0;
		}
	else if(gUserGlobal.SW1SW2==0x6a83)
		{memset(outbuf,0,16);
		return 0;
		}
	return 2;
}
/*=======================================================================================
函数：非接函数
功能：0.11 cpu卡读0006充值循环记录文件，交易明细记录
=========================================================================================*/
UBYTE Cpu_rd0006(UBYTE point,UBYTE *outbuf)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x00\xb2\x01\x34\x17",5);
	rdbuf[2] = point;
//	if(debugflag)
//		{
//		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
//		gUserGlobal.metro_prot.send_bytes = 5;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);//
	if(temp!=0)
		{return 1;}
//	if(debugflag)
//		{
//		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
//		gUserGlobal.metro_prot.send_bytes = len;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000)
		{memcpy(outbuf,rdbuf,23);
		return 0;
		}
	else if(gUserGlobal.SW1SW2==0x6a83)
		{memset(outbuf,0,23);
		return 0;
		}
	return 2;
}


/*=======================================================================================
函数：非接函数
功能：0.12 cpu卡读0017 BRT记录文件00B204bc00
=========================================================================================*/
UBYTE Cpu_rd0017(UBYTE *outbuf)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x00\xB2\x09\xB8\48",5);

	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);//
	if(temp!=0)
		{return 1;}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000)
		{memcpy(outbuf,rdbuf,48);
//	应用锁定标志位rdbuf[2],终端机编号（城市代码）5-10,Decmoney 11-14;year 15-21,mac 22-25,
//进出状态26，进出标志27，进时间28-32YYMMDDHHSS，进设备33-35，交易金36-37，累积次数38，累计金39-41，校验42，RFU43-47
		gUserGlobal.Metrosta=rdbuf[26];
		if(gUserGlobal.Metrosta==0xcc)
			{
			memcpy(gUserGlobal.B32,rdbuf+27,16);
			memset(gUserGlobal.B33,0,16);
			gUserGlobal.Amountcount=gUserGlobal.B32[11];
			gUserGlobal.AmountbalanceL=gUserGlobal.B32[12];
			gUserGlobal.AmountbalanceM=gUserGlobal.B32[13];
			gUserGlobal.AmountbalanceH=gUserGlobal.B32[14];
			}
		else if(gUserGlobal.Metrosta==0xdd)
			{memcpy(gUserGlobal.B33,rdbuf+27,16);
			memset(gUserGlobal.B32,0,16);
			gUserGlobal.Amountcount=gUserGlobal.B33[11];
			gUserGlobal.AmountbalanceL=gUserGlobal.B33[12];
			gUserGlobal.AmountbalanceM=gUserGlobal.B33[13];
			gUserGlobal.AmountbalanceH=gUserGlobal.B33[14];
			}
		else
			{memset(gUserGlobal.B32,0,16);
			memset(gUserGlobal.B33,0,16);
			gUserGlobal.B33[0]=0x81;
			gUserGlobal.B33[15]=0x81;
			gUserGlobal.B32[0]=0x41;
			gUserGlobal.B32[15]=0x41;
			gUserGlobal.Metrosta=0xdd;
			gUserGlobal.Amountcount=0;
			gUserGlobal.AmountbalanceL=0;
			gUserGlobal.AmountbalanceM=0;
			gUserGlobal.AmountbalanceH=0;
			}

		return 0;
		}
	else if(gUserGlobal.SW1SW2==0x6a83)
		{memset(gUserGlobal.B32,0,16);
		memset(gUserGlobal.B33,0,16);
		gUserGlobal.B33[0]=0x81;
		gUserGlobal.B33[15]=0x81;
		gUserGlobal.B32[0]=0x41;
		gUserGlobal.B32[15]=0x41;
		gUserGlobal.Metrosta=0xdd;
		gUserGlobal.Amountcount=0;
		gUserGlobal.AmountbalanceL=0;
		gUserGlobal.AmountbalanceM=0;
		gUserGlobal.AmountbalanceH=0;
		return 0;
		}
	return 2;
}
/*=======================================================================================
函数：非接函数
功能：0.12 cpu卡读0017 BRT记录文件00B204bc00
=========================================================================================*/
UBYTE Cpu_Ext_auth(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	gUserGlobal.SW1SW2 =0x00;
	memcpy(rdbuf,"\x00\x82\x00\x02\x08",5);
	memcpy(rdbuf+5,gUserGlobal.Mac,8);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,13);
		gUserGlobal.metro_prot.send_bytes = 13;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}

	temp = mifpro_icmd(rdbuf,13,rdbuf,&len);//
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000)
		{return 0;}
	else
		{return 2;}
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 选择adf2目录
=========================================================================================*/
UBYTE Cpu_sel1002(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,i,j,temp;
	memcpy(rdbuf,"\x00\xa4\x04\x00\x09\xa0\x00\x00\x00\x03\x86\x98\x07\x02",14);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,14);
		gUserGlobal.metro_prot.send_bytes = 14;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,14,rdbuf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	for(i=0;i<len;i++)
		{
		if((rdbuf[i+5]==0x9f)&&(rdbuf[i+6]==0x0c))
			{break;}
		}
	for(j=0;j<40;j++)
		{gUserGlobal.ADF2_file0015[j]=rdbuf[i+j+8];}
	return 0;
}

/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 选择pboc目录
=========================================================================================*/
UBYTE pf_selectpay(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,i,j,temp;
	memcpy(rdbuf,"\x00\xA4\x04\x00\x0E\x32\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x31",19);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,19);
		gUserGlobal.metro_prot.send_bytes = 19;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,19,rdbuf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	for(i=0;i<len;i++)
		{
		if(rdbuf[i]==0x4f)
			{break;}
		}
	len = rdbuf[i+1]+1;
	if (len >17)
		{return 2;}
	for(j=0;j<len;j++)
		{p->AID[j]=rdbuf[i+1+j];}
	temp=16-p->AID[0];
	for(j=0;j<temp;j++)
		{p->AID[16-j]=0x00;}	
	return 0;
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 选择pboc目录
=========================================================================================*/
UBYTE pf_selectfile(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,len1,i,j,k,temp;
	memcpy(rdbuf,"\x00\xA4\x04\x00",4);
	len = gUserGlobal.AID[0]+1;
	memcpy(rdbuf+4,gUserGlobal.AID,len);
	len = len +4;
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,len,rdbuf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	memset(gUserGlobal.AID,0,32);
	if(rdbuf[2]==0x84)
		{temp=rdbuf[3]+1;
		if(temp>17)
			{temp=17;
			rdbuf[3]=16;
			}
		for(i=0;i<temp;i++)
			{gUserGlobal.AID[i]=rdbuf[3+i];}
		}
	gUserGlobal.CappFlag = 0X01;
	len = rdbuf[1];
	for(i=0;i<len;i++)
		{if((rdbuf[i+1]==0xdf)&&(rdbuf[i+2]==0x61))
			{
			gUserGlobal.CappFlag = rdbuf[i+4];
			break;
			}
		}
	len = rdbuf[1];
	len1 = 0;
	for(i=0;i<len;i++)
		{if((rdbuf[i+1]==0x9f)&&(rdbuf[i+2]==0x38))
			{
			len1 = rdbuf[i+3];
			break;
			}
		}
	j = 0;
	k = 1;
	gUserGlobal.GPOlen = 0x00;
	while(j<len1)
	{
	if((rdbuf[j+i+4]==0x9f)&&(rdbuf[j+i+5]==0x66))
		{gUserGlobal.GPOout[k]=0x01;
		k=k+1;
		gUserGlobal.GPOlen=gUserGlobal.GPOlen+4;
		j=j+3;
		}
	else if((rdbuf[j+i+4]==0x9f)&&(rdbuf[j+i+5]==0x02))
		{gUserGlobal.GPOout[k]=0x02;
		k=k+1;
		gUserGlobal.GPOlen=gUserGlobal.GPOlen+6;
		j=j+3;
		}
	else if((rdbuf[j+i+4]==0x9f)&&(rdbuf[j+i+5]==0x03))
		{gUserGlobal.GPOout[k]=0x03;
		k=k+1;
		gUserGlobal.GPOlen = gUserGlobal.GPOlen+6;
		j=j+3;
		}
	else if((rdbuf[j+i+4]==0x9f)&&(rdbuf[j+i+5]==0x1a))
		{gUserGlobal.GPOout[k]=0x04;
		k=k+1;
		gUserGlobal.GPOlen = gUserGlobal.GPOlen+2;
		j=j+3;
		}
	else if((rdbuf[j+i+4]==0x5f)&&(rdbuf[j+i+5]==0x2a))
		{gUserGlobal.GPOout[k]=0x05;
		k=k+1;
		gUserGlobal.GPOlen= gUserGlobal.GPOlen+2;
		j=j+3;
		}
	else if(rdbuf[j+i+4]==0x95)
		{gUserGlobal.GPOout[k]=0x06;
		k=k+1;
		gUserGlobal.GPOlen= gUserGlobal.GPOlen+5;
		j=j+2;
		}
	else if(rdbuf[j+i+4]==0x9a)
		{gUserGlobal.GPOout[k]=0x07;
		k=k+1;
		gUserGlobal.GPOlen= gUserGlobal.GPOlen+3;
		j=j+2;
		}
	else if(rdbuf[j+i+4]==0x9c)
		{gUserGlobal.GPOout[k]=0x08;
		k=k+1;
		gUserGlobal.GPOlen= gUserGlobal.GPOlen+1;
		j=j+2;
		}
	else if((rdbuf[j+i+4]==0x9f)&&(rdbuf[j+i+5]==0x37))
		{gUserGlobal.GPOout[k]=0x09;
		k=k+1;
		gUserGlobal.GPOlen= gUserGlobal.GPOlen+4;
		j=j+3;
		}
	else if((rdbuf[j+i+4]==0xdf)&&(rdbuf[j+i+5]==0x60))
		{gUserGlobal.GPOout[k]=0x0a;
		k=k+1;
		gUserGlobal.GPOlen= gUserGlobal.GPOlen+1;
		j=j+3;
		}
	else
		{if((rdbuf[j+i+4]==0x5f)||(rdbuf[j+i+4]==0x9f))
			{gUserGlobal.GPOout[k]=rdbuf[j+i+6]|0x80;
			k=k+1;
			gUserGlobal.GPOlen=gUserGlobal.GPOlen+rdbuf[j+i+6];
			j=j+3;
			}
		else
			{gUserGlobal.GPOout[k]=rdbuf[j+i+5]|0x80;
			k=k+1;
			gUserGlobal.GPOlen=gUserGlobal.GPOlen+gUserGlobal.GPOlen+rdbuf[j+i+5];
			j=j+2;
			}
		}
	}
	gUserGlobal.GPOout[0]=k;
	return 0;
}

/*=======================================================================================
函数：非接函数
功能：0.9 cpu卡读0015二进制文件
=========================================================================================*/
UBYTE pf_rd0015(UBYTE id,UBYTE *outbuf)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],buffer[256],temp;
	UWORD len;
	memcpy(rdbuf,"\x80\xb4\x00\xa8",4);
	temp=gUserGlobal.CappFlag&0x80;
//	temp = 0;
	if(temp==0x00)
		{
		rdbuf[4]=0x02;
		rdbuf[5]=id;	//ID标识第一字节
		rdbuf[6]=0x21;
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,7);
			gUserGlobal.metro_prot.send_bytes = 7;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(100);
			}
//		temp = mifpro_icmd(rdbuf,7,buffer,&len);
		temp = fm_apdu_access(7,rdbuf,&len,buffer);
		}
	else
		{
		rdbuf[4]=0x0a;
		rdbuf[5]=id;	//ID标识第一字节
		rdbuf[6]=0x21;
		memcpy(rdbuf+7,gUserGlobal.Random,8);
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,15);
			gUserGlobal.metro_prot.send_bytes = 15;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
//		temp = mifpro_icmd(rdbuf,15,buffer,&len);
		temp = fm_apdu_access(15,rdbuf,&len,buffer);
		}
	if(debugflag)
		{
		watchdog();
		gUserGlobal.metro_prot.send_buf[0] = temp;
		gUserGlobal.metro_prot.send_bytes = 1;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}		
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,outbuf,len);
		gUserGlobal.metro_prot.send_bytes = len%0x100;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (buffer[len-2]<<8)+buffer[len-1];
	if(gUserGlobal.SW1SW2 ==0x9000)
		{
		memcpy(outbuf,buffer,64);
		memcpy(gUserGlobal.RMac,buffer+len-6,4);
//		{for(temp=0;temp<4;temp++)
//			{Keya2[temp]=program_buffer[tagtype-2+temp];}  //mac?
		return 0;
		}
	else
		{return 1;}
}

/*=======================================================================================
函数：非接函数
功能：0.9 浦发卡读余额
=========================================================================================*/
UBYTE pf_getbalance(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
//	UDWORD temp1;
	memcpy(rdbuf,"\x80\xca\x9f\x79\x00",5);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	gUserGlobal.Balance = rdbuf[8]%0x10+(rdbuf[8]/0x10)*0x0a;
	gUserGlobal.Balance = gUserGlobal.Balance+(rdbuf[7]%0x10)*0x64+(rdbuf[7]/0x10)*0x3e8;
	gUserGlobal.Balance = gUserGlobal.Balance+(rdbuf[6]%0x10)*0x2710+(rdbuf[6]/0x10)*0x186a0;
	gUserGlobal.BalanceL = gUserGlobal.Balance%0x100;
	gUserGlobal.BalanceM = (gUserGlobal.Balance>>8)%0x100;
	gUserGlobal.BalanceH = (gUserGlobal.Balance>>16)%0x100;
	//	BalanceHH=Balance/0x1000000;
	gUserGlobal.BalanceHH=0x00;
	gUserGlobal.ADF1_CardOverDraft[3] = 0xd0;
	gUserGlobal.ADF1_CardOverDraft[2] = 0x07;
	gUserGlobal.ADF1_CardOverDraft[1] = 0x00;
	gUserGlobal.ADF1_CardOverDraft[0] = 0x00;
	gUserGlobal.Blk5val=0x07d0;
	return 0;
}
/*=======================================================================================
函数：非接函数
功能：0.9 浦发卡读余额
=========================================================================================*/
UBYTE pf_gettouzi(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
//	UDWORD temp1;
	memcpy(rdbuf,"\x80\xca\xdf\x62\x00",5);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	gUserGlobal.Blk5val = rdbuf[8]%0x10+(rdbuf[8]/0x10)*0x0a;
	gUserGlobal.Blk5val = gUserGlobal.Blk5val+(rdbuf[7]%0x10)*0x64+(rdbuf[7]/0x10)*0x3e8;
	gUserGlobal.Blk5val = gUserGlobal.Blk5val+(rdbuf[6]%0x10)*0x2710+(rdbuf[6]/0x10)*0x186a0;

	gUserGlobal.ADF1_CardOverDraft[3] = gUserGlobal.Blk5val%0x100;
	gUserGlobal.ADF1_CardOverDraft[2] = (gUserGlobal.Blk5val>>8)%0x100;
	gUserGlobal.ADF1_CardOverDraft[1] = (gUserGlobal.Blk5val>>16)%0x100;
	gUserGlobal.ADF1_CardOverDraft[0] = 0x00;
	return 0;
}

/*=======================================================================================
函数：非接函数
功能：0.9 浦发卡读余额
=========================================================================================*/
UBYTE pf_gettouzi1(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
//	UDWORD temp1;
	memcpy(rdbuf,"\x80\xca\xdf\x63\x00",5);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	gUserGlobal.Blk6val = rdbuf[8]%0x10+(rdbuf[8]/0x10)*0x0a;
	gUserGlobal.Blk6val = gUserGlobal.Blk6val+(rdbuf[7]%0x10)*0x64+(rdbuf[7]/0x10)*0x3e8;
	gUserGlobal.Blk6val = gUserGlobal.Blk6val+(rdbuf[6]%0x10)*0x2710+(rdbuf[6]/0x10)*0x186a0;
	gUserGlobal.Balance = gUserGlobal.Balance -gUserGlobal.Blk6val;

	gUserGlobal.BalanceL = gUserGlobal.Balance%0x100;
	gUserGlobal.BalanceM = (gUserGlobal.Balance>>8)%0x100;
	gUserGlobal.BalanceH = (gUserGlobal.Balance>>16)%0x100;
	gUserGlobal.BalanceHH = (gUserGlobal.Balance>>24)%0x100;;

	return 0;
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 选择员工卡目录00A4040008 41 50 50 4D 45 43 30 32	
=========================================================================================*/
UBYTE metro_select(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x00\xa4\x04\x00\x08\x41\x50\x50\x4D\x45\x43\x30\x32",13);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,7);
		gUserGlobal.metro_prot.send_bytes = 7;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,13,rdbuf,&len);
	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	return 0;
}

/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 读员工卡0015	
=========================================================================================*/
UBYTE metro_rd0015(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],buf[256],len,temp;
	memcpy(rdbuf,"\x00\xb0\x95\x00\x00",5);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,5,buf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}
	memcpy(gUserGlobal.metro0015,buf,30);
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	gUserGlobal.CityNo[0] = gUserGlobal.metro0015[10];
	gUserGlobal.CityNo[1] = gUserGlobal.metro0015[11];

	return 0;
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 读员工卡0015	
=========================================================================================*/
UBYTE metro_rd0006(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],buf[256],len,temp;

	memcpy(rdbuf,"\x00\xb0\x86\x00\x20",5);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,5);
		gUserGlobal.metro_prot.send_bytes = 5;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(rdbuf,5,buf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}

	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	memcpy(gUserGlobal.metro0006,buf,32);
	return 0;
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 读员工卡0015	
=========================================================================================*/
UBYTE metro_rd0018(UBYTE block,UBYTE *outbuf)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],buf[256],len,temp;

	memcpy(rdbuf,"\x00\xb2\x01\xc4\x00",5);
	rdbuf[2] = block;
	temp = mifpro_icmd(rdbuf,5,buf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}

	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000)
		{memcpy(outbuf,buf,23);
		return 0;
		}
	else if(gUserGlobal.SW1SW2==0x6a83)
		{memset(outbuf,0,23);
		return 0;
		}
	return 2;
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 读员工卡0015	
=========================================================================================*/
UBYTE metro_rd0019(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],buf[256],len,temp;

	memcpy(rdbuf,"\x00\xb2\x09\xc8\x40",5);
	temp = mifpro_icmd(rdbuf,5,buf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}

	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	memcpy(gUserGlobal.metro0019,buf,64);
	return 0;
}

/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 读员工卡0015	
=========================================================================================*/
UBYTE metro_getbalance(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[128],len,temp;

	memcpy(rdbuf,"\x80\x5c\x00\x02\x04",5);
	temp = mifpro_icmd(rdbuf,5,rdbuf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}

	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	memcpy(gUserGlobal.ADF1_Balance,rdbuf,4);//余额
	gUserGlobal.Balance = gUserGlobal.ADF1_Balance[3]+(gUserGlobal.ADF1_Balance[2]<<8)+(gUserGlobal.ADF1_Balance[1]<<16)+(gUserGlobal.ADF1_Balance[0]<<24);

	return 0;
}

/*=======================================================================================
函数：非接cpu卡函数
功能：0.13 选择3f00目录
=========================================================================================*/
UBYTE Cpu_sel3f00(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x00\xa4\x00\x00\x02\x3f\x00",7);
//	if(debugflag)
//		{
//		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,7);
//		gUserGlobal.metro_prot.send_bytes = 7;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	temp = mifpro_icmd(rdbuf,7,rdbuf,&len);
	if(temp!=0)
		{return 1;}
//	if(debugflag)
//		{
//		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
//		gUserGlobal.metro_prot.send_bytes = len;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(gUserGlobal.SW1SW2!=0x9000)
		{return 2;}
	return 0;
}

/*=======================================================================================
函数：
功能：0.95 重取交易tac
//CardBak[64];
  //CardBak内容：
  //CardSnr 4字节(0~3)，Balance 交易前金额4字节(4~7)(high--low(M1卡为Block6的前4字节))
  //随机数4字节(8~11)，卡计数器2字节(12\13)，交易金额4字节(14~17)(high--low)
  //(cpu卡交易类型,18)，交易日期时间7字节(19~25)
  //cpu卡消费初始化的密钥版本1字节(26)、算法标示1字节(27)，
//CardPoint(28),Metrosta(29),gUserGlobal.BalanceL~H(30~32);
=========================================================================================*/
UBYTE Geproof(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,len,buf[256];
	UWORD sw1sw2;
	if(gUserGlobal.TagType == 0x05)
		{goto etproof;}
	sam_select(CPU_SAM);
	temp = Sam_select1001();
	if(temp!=0)
		{return SHMETRO_CE_PSAMERR;}
	temp = Cpu_80ca();
	if(temp!=0)
		{return 1;}
	temp = Sam_80ca();
	if(temp!=0)
		{return 1;}
	temp = Cpu_getprove();
	if(temp == 1)
		{return 1;}
	else if(temp == 2)
		{
		return 2;
		}
	memcpy(buf,"\x80\x70\x00\x01\x28",5);
	memcpy(buf+5,gUserGlobal.CardBak+8,20);//随机数4+卡计数器2+交易金额4+交易类型1+交易日期7+密钥版本1+算法标识1
	memcpy(buf+25,gUserGlobal.PosCnt,4);//流水号
	memcpy(buf+29,gUserGlobal.ADF1_file0015+12,8);//卡号
	buf[37] = gUserGlobal.CityNo[0];
	buf[38] = gUserGlobal.CityNo[1];
	memcpy(buf+39,"\xff\x00\x00\x00\x00\x00",6);
//	len1 = 45;
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,45);
		gUserGlobal.metro_prot.send_bytes = 45;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = sam_apdu(buf,45,buf,&len);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{
		gUserGlobal.Err17Flag = 0;
		return SHMETRO_CE_PSAMERR;
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{memcpy(gUserGlobal.SamCnt,buf,4);
//		memcpy(gUserGlobal.Mac,buf+4,4);
		goto getproof1;
		}
	if(sw1sw2 == 0x6108)
		{memcpy(buf,"\x00\xc0\x00\x00\x08",5);
		temp = sam_apdu(buf,5,buf,&len);
		if(temp!=0)
			{return SHMETRO_CE_PSAMERR;}
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.SamCnt,buf,4);
//			memcpy(gUserGlobal.Mac,buf+4,4);
			goto getproof1;
			}
		else if(sw1sw2 == 0x9302)
			{return SHMETRO_CE_MAC1ERR;}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
getproof1:
	temp = Sam_mac2();
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.CardBak,32);
		memcpy(gUserGlobal.metro_prot.send_buf+32,gUserGlobal.PosCnt,4);
		memcpy(gUserGlobal.metro_prot.send_buf+36,gUserGlobal.Tac,4);
		gUserGlobal.metro_prot.send_bytes = 40;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.Err17Flag = 0;
	return 0;
etproof:
	sam_select(SJT_SAM);
	if(shmode == 1)
		{
		Sam_select1001();
		temp = Cpu_80ca();
		if(temp==1)
			{return SHMETRO_CE_WRITEERROR;}
		temp = Sam_80ca();
		if(temp!=0)
			{return temp;}
		}
	else
		{	
		temp =  Sam_select1002();
		if(temp!=0)
			{return temp;}
		}
	temp = Cpu_getprove();
	if(temp == 1)
		{return 1;}
	else if(temp == 2)
		{return 2;}
	if(shmode == 1)
		{
		memcpy(buf,"\x80\x70\x00\x01\x28",5);
		memcpy(buf+5,gUserGlobal.CardBak+8,18);//随机数4+卡计数器2+交易金额4+交易类型1+交易日期7
		buf[23] = gUserGlobal.metrokeyver;
		buf[24] = gUserGlobal.ADF1_Algorithm;
		memcpy(buf+25,gUserGlobal.PosCnt,4);//流水号
		memcpy(buf+29,gUserGlobal.metro0015+12,8);//卡号
		buf[37] = gUserGlobal.CityNo[0];
		buf[38] = gUserGlobal.CityNo[1];
		memcpy(buf+39,"\xff\x00\x00\x00\x00\x00",6);
	//	len1 = 45;
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,45);
			gUserGlobal.metro_prot.send_bytes = 45;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		temp = sam_apdu(buf,45,buf,&len);
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		}
	else
		{
		memcpy(buf,"\x80\x70\x00\x00\x1c",5);
		memcpy(buf+5,gUserGlobal.CardBak+8,18);//随机数4+卡计数器2+交易金额4+交易类型1+交易日期7
//		buf[9] = gUserGlobal.CardBak[12];
//		buf[10] = gUserGlobal.CardBak[13];
//		memcpy(buf+11,gUserGlobal.TransValue,4);
//		buf[15] = 0x09;		//0x09;复合消费或者简单消费
//		memcpy(buf+16,gUserGlobal.CardBak+19,7);
		buf[23] = gUserGlobal.metrokeyver;
		buf[24] = gUserGlobal.ADF1_Algorithm;
		memcpy(buf+25,gUserGlobal.metro0015+12,8);//卡号
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,33);
			gUserGlobal.metro_prot.send_bytes = 33;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		temp = sam_apdu(buf,33,buf,&len);
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		}
	if(temp!=0)
		{
		gUserGlobal.Err17Flag = 0;
		return SHMETRO_CE_PSAMERR;
		}
	sw1sw2 = (buf[len-2]<<8)+buf[len-1];
	if(sw1sw2 == 0x9000)
		{memcpy(gUserGlobal.SamCnt,buf,4);
//		memcpy(gUserGlobal.Mac,buf+4,4);
		goto getproof1;
		}
	if(sw1sw2 == 0x6108)
		{memcpy(buf,"\x00\xc0\x00\x00\x08",5);
		temp = sam_apdu(buf,5,buf,&len);
		if(temp!=0)
			{return SHMETRO_CE_PSAMERR;}
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
			gUserGlobal.metro_prot.send_bytes = len;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		sw1sw2 = (buf[len-2]<<8)+buf[len-1];
		if(sw1sw2 == 0x9000)
			{
			memcpy(gUserGlobal.SamCnt,buf,4);
//			memcpy(gUserGlobal.Mac,buf+4,4);
			goto getproof1;
			}
		else if(sw1sw2 == 0x9302)
			{return SHMETRO_CE_MAC1ERR;}
		else
			{return SHMETRO_CE_PSAMERR;}
		}
		goto getproof1;
}
UBYTE Rd_sector7(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = mcml_load_key(0x00,0X00,0X07,gUserGlobal.KeyA7);
	if(temp != 0x00)
		{return 1;}
	temp = mcmh_read_s50(0x1d,gUserGlobal.B29,1,0);
	if(temp != 0x00)
		{return 1;}
	temp = mcml_read(0x1e,gUserGlobal.B30);
	if(temp != 0x00)
		{return 1;}
	temp = Chk_xor_sum(gUserGlobal.B29,0x10);
	if(temp == 0)
		{temp = compareBlk(gUserGlobal.B29,gUserGlobal.B30);
		if(temp != 0)
			{temp = mcml_write(0x1e,gUserGlobal.B29);
			if(temp != 0x00)
				{return 1;}
			gUserGlobal.Huancheng[3]=gUserGlobal.B29[2];
			gUserGlobal.Huancheng[4]=gUserGlobal.B29[3];
			gUserGlobal.Huancheng[5]=gUserGlobal.B29[4];
			gUserGlobal.Huancheng[6]=gUserGlobal.B29[5];
			gUserGlobal.Huancheng[7]=gUserGlobal.B29[6];
			//累计乘车金额3bytes c\d\e
			gUserGlobal.Huancheng[8]=gUserGlobal.B29[12];
			gUserGlobal.Huancheng[9]=gUserGlobal.B29[13];
			gUserGlobal.Huancheng[10]=gUserGlobal.B29[14];
			//累计乘车次数2bytes a\b
			gUserGlobal.Huancheng[11]=gUserGlobal.B29[10];
			gUserGlobal.Huancheng[12]=gUserGlobal.B29[11];
			return 0;
			}
		else
			{
			gUserGlobal.Huancheng[3]=gUserGlobal.B29[2];
			gUserGlobal.Huancheng[4]=gUserGlobal.B29[3];
			gUserGlobal.Huancheng[5]=gUserGlobal.B29[4];
			gUserGlobal.Huancheng[6]=gUserGlobal.B29[5];
			gUserGlobal.Huancheng[7]=gUserGlobal.B29[6];
			//累计乘车金额3bytes c\d\e
			gUserGlobal.Huancheng[8]=gUserGlobal.B29[12];
			gUserGlobal.Huancheng[9]=gUserGlobal.B29[13];
			gUserGlobal.Huancheng[10]=gUserGlobal.B29[14];
			//累计乘车次数2bytes a\b
			gUserGlobal.Huancheng[11]=gUserGlobal.B29[10];
			gUserGlobal.Huancheng[12]=gUserGlobal.B29[11];
			return 0;
			}

		}
	else//B29 ERR
		{temp = Chk_xor_sum(gUserGlobal.B30,0x10);
		if(temp == 0)
			{temp = mcml_write(0x1d,gUserGlobal.B30);
			if(temp != 0x00)
				{return 1;}
			gUserGlobal.Huancheng[3]=gUserGlobal.B30[2];
			gUserGlobal.Huancheng[4]=gUserGlobal.B30[3];
			gUserGlobal.Huancheng[5]=gUserGlobal.B30[4];
			gUserGlobal.Huancheng[6]=gUserGlobal.B30[5];
			gUserGlobal.Huancheng[7]=gUserGlobal.B30[6];
			//累计乘车金额3bytes c\d\e
			gUserGlobal.Huancheng[8]=gUserGlobal.B30[12];
			gUserGlobal.Huancheng[9]=gUserGlobal.B30[13];
			gUserGlobal.Huancheng[10]=gUserGlobal.B30[14];
			//累计乘车次数2bytes a\b
			gUserGlobal.Huancheng[11]=gUserGlobal.B30[10];
			gUserGlobal.Huancheng[12]=gUserGlobal.B30[11];
			return 0;
			}
		else//B29 B30 ERR
			{memset(gUserGlobal.B29,0,16);
			memset(gUserGlobal.B30,0,16);
			memset(gUserGlobal.Huancheng,0,16);
			temp = mcml_write(0x1d,gUserGlobal.B29);
			if(temp != 0x00)
				{return 1;}
			temp = mcml_write(0x1e,gUserGlobal.B29);
			if(temp != 0x00)
				{return 1;}
			return 0;
			}
		}
}


UBYTE CpuRd_sector7(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,Rdbuf[256],Rdbkbuf[256];
	UWORD temp1;

//	Para1=0;
	temp1=(gUserGlobal.CityNo[0]<<8)+gUserGlobal.CityNo[1];
	if(temp1!=0x2000)
		{gUserGlobal.Huancheng[3]=0;
		gUserGlobal.Huancheng[4]=0;
		gUserGlobal.Huancheng[5]=0;
		gUserGlobal.Huancheng[6]=0;
		gUserGlobal.Huancheng[7]=0;
		gUserGlobal.Huancheng[8]=0;
		gUserGlobal.Huancheng[9]=0;
		gUserGlobal.Huancheng[10]=0;
		gUserGlobal.Huancheng[11]=0;
		gUserGlobal.Huancheng[12]=0;
		return 0;
		}
	temp=Cpu_rdsec7(Rdbuf,0x02);
	if(temp!=0)
		{return 1;}
	if(gUserGlobal.SW1SW2!=0x9000)
		{return SHMETRO_CE_INVALIDCARD;}
	temp=Cpu_rdsec7(Rdbkbuf,0x03);
	if(temp!=0)
		{return 1;}
	temp = Chk_xor_sum(Rdbuf,0x10);
	if(temp ==0)
		{gUserGlobal.Huancheng[3]=Rdbuf[2];
		gUserGlobal.Huancheng[4]=Rdbuf[3];
		gUserGlobal.Huancheng[5]=Rdbuf[4];
		gUserGlobal.Huancheng[6]=Rdbuf[5];
		gUserGlobal.Huancheng[7]=Rdbuf[6];
		//累计乘车金额3bytes c\d\e
		gUserGlobal.Huancheng[8]=Rdbuf[12];
		gUserGlobal.Huancheng[9]=Rdbuf[13];
		gUserGlobal.Huancheng[10]=Rdbuf[14];
		//累计乘车次数2bytes a\b
		gUserGlobal.Huancheng[11]=Rdbuf[10];
		gUserGlobal.Huancheng[12]=Rdbuf[11];
		memcpy(gUserGlobal.Sec7buf,Rdbuf,16);
		return 0;
		}
	else
		{temp = Chk_xor_sum(Rdbkbuf,0x10);
		if(temp ==0)
			{gUserGlobal.Huancheng[3]=Rdbkbuf[2];
			gUserGlobal.Huancheng[4]=Rdbkbuf[3];
			gUserGlobal.Huancheng[5]=Rdbkbuf[4];
			gUserGlobal.Huancheng[6]=Rdbkbuf[5];
			gUserGlobal.Huancheng[7]=Rdbkbuf[6];
			gUserGlobal.Huancheng[8]=Rdbkbuf[12];
			gUserGlobal.Huancheng[9]=Rdbkbuf[13];
			gUserGlobal.Huancheng[10]=Rdbkbuf[14];
			//累计乘车次数2bytes a\b
			gUserGlobal.Huancheng[11]=Rdbkbuf[10];
			gUserGlobal.Huancheng[12]=Rdbkbuf[11];
			memcpy(gUserGlobal.Sec7buf,Rdbkbuf,16);
			return 0;
			}
		else
			{gUserGlobal.Huancheng[3]=0;
			gUserGlobal.Huancheng[4]=0;
			gUserGlobal.Huancheng[5]=0;
			gUserGlobal.Huancheng[6]=0;
			gUserGlobal.Huancheng[7]=0;
			gUserGlobal.Huancheng[8]=0;
			gUserGlobal.Huancheng[9]=0;
			gUserGlobal.Huancheng[10]=0;
			gUserGlobal.Huancheng[11]=0;
			gUserGlobal.Huancheng[12]=0;
			memset(gUserGlobal.Sec7buf,0,16);
			return 0;
			}
		}
}

//****************************************
UBYTE Sec7_b(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = mcml_load_key(0x00,0X00,0x07,gUserGlobal.KeyA7);
	if(temp != 0)
		{return 1;}
	temp = mcmh_read_s50(0x1e,gUserGlobal.B30,1,0);
	if(temp != 0x00)
		{return 1;}
	temp = mcml_read(0x1D,gUserGlobal.B29);
	if(temp != 0x00)
		{return 1;}
	temp = Chk_xor_sum(gUserGlobal.B30,16);
	if(temp == 0)
		{temp = compareBlk(gUserGlobal.B30,gUserGlobal.B29);
		if(temp!=0)
			{temp = mcml_write(0x1d,gUserGlobal.B30);
			if(temp!=0)
				{return 1;}
			memcpy(gUserGlobal.B29,gUserGlobal.B30,16);
			return 0;
			}
		else
			{return 0;}
		}
	else
		{temp = Chk_xor_sum(gUserGlobal.B29,16);
		if(temp == 0)
			{temp = mcml_write(0x1e,gUserGlobal.B29);
			if(temp!=0)
				{return 1;}
			memcpy(gUserGlobal.B30,gUserGlobal.B29,16);
			return 0;
			}
		else
			{memset(gUserGlobal.B29,0,16);
			memset(gUserGlobal.B30,0,16);
			temp = mcml_write(0x1d,gUserGlobal.B29);
			if(temp!=0)
				{return 1;}
			temp = mcml_write(0x1e,gUserGlobal.B29);
			if(temp!=0)
				{return 1;}
			return 0;
			}

		}
}
//****************************************
UBYTE Recover_c(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,buf[256];
	temp = mcml_read(0xd,gUserGlobal.B13);
	if(temp != 0x00)
		{return 1;}
	temp = Chk_xor_sum(gUserGlobal.B13,0x10);
	if(temp!=0)
		{return SHMETRO_CE_INVALIDCARD;}
	if(gUserGlobal.B13[11]!=0x02)
		{return SHMETRO_CE_INVALIDCARD;}
	//CardSta=Rdbuf[11];
	gUserGlobal.CardPoint = gUserGlobal.B13[12];
	gUserGlobal.M1CardCnt[0] = gUserGlobal.B13[13];
	gUserGlobal.M1CardCnt[1] = gUserGlobal.B13[14];
	gUserGlobal.CardCount = (gUserGlobal.B13[13]<<8)+gUserGlobal.B13[14];
	if(gUserGlobal.CardPoint>9)
		{return SHMETRO_CE_INVALIDCARD;}
	temp = mcml_load_key(0x00,0X00,0x01,gUserGlobal.KeyA1);
	if(temp != 0)
		{return 1;}
	temp = mcmh_read_s50(5,gUserGlobal.B5,1,0);
	if(temp != 0x00)
		{return 1;}
	temp = mcml_read(6,gUserGlobal.B6);
	if(temp != 0x00)
		{return 1;}
	temp=Chk_balanceblk(gUserGlobal.B6);
	if(temp!=0)
		{return SHMETRO_CE_INVALIDCARD;}
	temp=Chk_balanceblk(gUserGlobal.B5);
	if(temp!=0)
		{return SHMETRO_CE_INVALIDCARD;}
	temp = compareBlk(gUserGlobal.B5,gUserGlobal.B6);
	if(temp==0)//blk5==blk6
		{if(gUserGlobal.CardCount==0xffff)
			{return SHMETRO_CE_INVALIDCARD;}
		Chk_c();
		if(gUserGlobal.sameflag==1)
			{temp = Wrbackup();
			return 1;
			}
		else
			{temp=Sec7_b();
			if(temp!=0)
				{return 1;}
			temp = mcml_load_key(0x00,0X00,0x03,gUserGlobal.KeyA3);
			if(temp != 0)
				{return 1;}
			memset(gUserGlobal.B12,0,16);
			temp = fm_mcmh_write(12,gUserGlobal.B12,1,0);
//			temp = mcml_write(12,gUserGlobal.B12);
			if(temp!=0)
				{return 1;}
			temp = mcml_read(12,buf);
			if(temp != 0x00)
				{return 1;}
			temp = compareBlk(gUserGlobal.B12,buf);
			if(temp!=0)
				{return 1;}
			else
				{return 0;}

			}
		}
	else	//blk5!=blk6
		{
		Chk_c();
		if(gUserGlobal.sameflag==1)
			{temp = Wrbackup();
			return 1;
			}
		else
			{
			temp = mcml_load_key(0x00,0X00,12,gUserGlobal.KeyA8);
			if(temp != 0)
				{return 1;}
			temp = mcmh_read_s50(50,gUserGlobal.B34,1,0);
			if(temp != 0x00)
				{return 1;}
			temp = Chk_xor_sum(gUserGlobal.B34,16);
			if(temp!=0)
				{gUserGlobal.B34[0] = 0x00;
				gUserGlobal.B34[15] = 0x00;
				}
			if (gUserGlobal.B34[0]==0xcc) 
				{gUserGlobal.Metrosta=0xdd;}
			else if(gUserGlobal.B34[0]==0xdd) 
				{gUserGlobal.Metrosta=0xcc;}
			else
				{gUserGlobal.Metrosta=0x00;}	
//		if(debugflag)
//			{
//			gUserGlobal.metro_prot.send_buf[0]=gUserGlobal.B34[0];
//			gUserGlobal.metro_prot.send_buf[1]=gUserGlobal.Metrosta;
//			
//			gUserGlobal.metro_prot.send_bytes = 2;
//			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//			prot_shmetro_send_process(&(p->metro_prot));
//			delay_ms(10);
//			}

			if(gUserGlobal.CardPoint==0)
				{gUserGlobal.CardPoint=0x09;}
			else
				{gUserGlobal.CardPoint=gUserGlobal.CardPoint-1;}
			gUserGlobal.CardCount=gUserGlobal.CardCount-1;
			gUserGlobal.M1CardCnt[0]=gUserGlobal.CardCount/0x100;
			gUserGlobal.M1CardCnt[1]=gUserGlobal.CardCount%0x100;
//			gUserGlobal.Metrosta=0x00;
			temp = Wrbackup();
			return 1;
			}
		}
//	return 1;
}

UBYTE Recover_b(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,buf[256];
//	Blk5[0]=Rdbuf[3];
//	Blk5[1]=Rdbuf[4];
//	Blk5[2]=Rdbuf[5];
//	Blk5[3]=Rdbuf[6];
	gUserGlobal.CardPoint = gUserGlobal.B12[7];
	gUserGlobal.M1CardCnt[0] = gUserGlobal.B12[8];
	gUserGlobal.M1CardCnt[1] = gUserGlobal.B12[9];
	gUserGlobal.CardCount = (gUserGlobal.M1CardCnt[0]<<8)+gUserGlobal.M1CardCnt[1];
	gUserGlobal.LockSign = gUserGlobal.B12[10];
	gUserGlobal.Metrosta = gUserGlobal.B12[14];
	if(gUserGlobal.LockSign != 0x02)
		{return SHMETRO_CE_INVALIDCARD;}
	if(gUserGlobal.CardPoint>9||gUserGlobal.CardCount==0xffff)
		{return SHMETRO_CE_INVALIDCARD;}
	memset(gUserGlobal.B13,0,11);
	gUserGlobal.B13[11] = gUserGlobal.LockSign;
	gUserGlobal.B13[12] = gUserGlobal.CardPoint;
	gUserGlobal.B13[13]= gUserGlobal.M1CardCnt[0];
	gUserGlobal.B13[14]= gUserGlobal.M1CardCnt[1];
	lrc_gen2(gUserGlobal.B13,15);
//	Chk_xor_sum(Rdbuf,0x0f);
//	Rdbuf[15]=Result;
	temp = mcml_write(0x0d,gUserGlobal.B13);
	if(temp!=0)
		{return 1;}
	temp = mcml_read(0x0d,buf);
	if(temp != 0x00)
		{return 1;}
	temp=compareBlk(gUserGlobal.B13,buf);
	if(temp!=0)
		{return 1;}
	temp = mcml_load_key(0x00,0X00,0x01,gUserGlobal.KeyA1);
	if(temp != 0)
		{return 1;}
	temp = mcmh_read_s50(6,gUserGlobal.B6,1,0);
	if(temp != 0x00)
		{return 1;}
	temp=Chk_balanceblk(gUserGlobal.B6);
	if(temp!=0)
		{return SHMETRO_CE_INVALIDCARD;}
	temp = mcml_restore(6);
	if(temp != 0x00)
		{return 1;}
	temp = mcml_transfer(5);
	if(temp != 0x00)
		{return 1;}
	temp = mcml_read(5,buf);
	if(temp!=0)
		{return 1;}
	temp=compareBlk(gUserGlobal.B6,buf);
	if(temp!=0)
		{return 1;}
	if(gUserGlobal.Metrosta==0xcc||gUserGlobal.Metrosta==0xdd)
		{memset(gUserGlobal.B34,0,16);
		gUserGlobal.B34[0]=gUserGlobal.Metrosta;
		gUserGlobal.B34[15]=gUserGlobal.Metrosta;
	temp = mcml_load_key(0x00,0X00,12,gUserGlobal.KeyA8);
	if(temp != 0)
		{return 1;}
		temp = fm_mcmh_write(50,gUserGlobal.B34,1,0);
//		temp = mcml_write(34,gUserGlobal.B34);
		if(temp!=0)
			{return 1;}
		temp = mcml_read(50,buf);
		if(temp != 0x00)
			{return 1;}
		temp=compareBlk(gUserGlobal.B34,buf);
		if(temp!=0)
			{return 1;}
		}
	temp=Sec7_b();
	if(temp!=0)
		{return 1;}
	temp = mcml_load_key(0x00,0X00,0x03,gUserGlobal.KeyA3);
	if(temp != 0)
		{return 1;}
	memset(gUserGlobal.B12,0,16);
	temp = fm_mcmh_write(12,gUserGlobal.B12,1,0);
	if(temp!=0)
		{return 1;}
	temp = mcml_read(12,buf);
	if(temp != 0x00)
		{return 1;}
	temp=compareBlk(gUserGlobal.B12,buf);
	if(temp!=0)
		{return 1;}
	if(gUserGlobal.sameflag == 0x01&&gUserGlobal.Err17Flag==0x01)
		{gUserGlobal.Err17Flag=0;}
	return 0;
}
/****************************************************************************
* 名称：Send_io()
****************************************************************************/
UBYTE Send_io(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE para1,temp;
	UDWORD date1,date2;
	temp = Chk_xor_sum(gUserGlobal.B34,16);
//	temp =1;
	if(temp == 0)
		{if(gUserGlobal.B34[0]==0xcc||gUserGlobal.B34[0]==0xdd)
			{gUserGlobal.Metrosta = gUserGlobal.B34[0];}
		else
			{gUserGlobal.Metrosta = 0x00;}
		}
	else
		{gUserGlobal.Metrosta = 0x00;}
	para1 = 0x00;
	temp = Chk_xor_sum(gUserGlobal.B32,16);
	if (temp!=0) 
		{para1 = 0x01;}//b32 xx
	gUserGlobal.Psw=gUserGlobal.B32[0];
	temp = Chk_psw();
	if(temp != 0)
		{para1=0x01;}
	temp = Chk_xor_sum(gUserGlobal.B33,16);
	if (temp!= 0)
		{para1 = para1|0x02;}// =2 or 3 b33xx
	gUserGlobal.Psw=gUserGlobal.B33[0];
	temp = Chk_psw();
	if(temp != 0)
		{para1 = para1|0x02;}
//	if(debugflag)
//		{
////			memcpy(gUserGlobal.metro_prot.send_buf+4,gUserGlobal.CardSnr,4);
//		gUserGlobal.metro_prot.send_buf[0] = gUserGlobal.Metrosta;
//		gUserGlobal.metro_prot.send_buf[1] = para1;
//		gUserGlobal.metro_prot.send_bytes = 2;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(10);
//		}
	if(gUserGlobal.Metrosta == 0x00)
		{
		if (para1 == 0x01||para1 == 0x03)
			{gUserGlobal.Metrosta = 0xdd;}
		else if (para1 == 0x02)
			{gUserGlobal.Metrosta = 0xcc;}
		else
			{
			if(gUserGlobal.B32[1]>gUserGlobal.B33[1])
				{gUserGlobal.Metrosta = 0xcc;}
			else if (gUserGlobal.B32[1]<gUserGlobal.B33[1]) 
				{gUserGlobal.Metrosta = 0xdd;}
			else
				{date1 = (gUserGlobal.B32[2]<<24)+(gUserGlobal.B32[3]<<16)+(gUserGlobal.B32[4]<<8)+gUserGlobal.B32[5];
				date2 = (gUserGlobal.B33[2]<<24)+(gUserGlobal.B33[3]<<16)+(gUserGlobal.B33[4]<<8)+gUserGlobal.B33[5];
				if(debugflag)
					{
					watchdog();
					memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.B32,16);
					memcpy(gUserGlobal.metro_prot.send_buf+16,gUserGlobal.B33,16);
					gUserGlobal.metro_prot.send_bytes = 32;
					gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
					prot_shmetro_send_process(&(p->metro_prot));
					delay_ms(10);
					}
				if(date1 > date2)
					{gUserGlobal.Metrosta = 0xcc;}
				else
					{gUserGlobal.Metrosta = 0xdd;}
				}
			}
		}
//Huancheng[0]=Rdbkbuf[6];//另外地铁一条记录的设备编号
//Huancheng[1]=Rdbkbuf[7];
//Huancheng[2]=Rdbkbuf[8];
//	para1=3;
//	temp = Chk_xor_sum(gUserGlobal.B32,16);
//	if(temp == 0)
//		{gUserGlobal.Psw=gUserGlobal.B32[0];
//		temp = Chk_psw();
//		if(temp == 0)
//			{para1=para1&0xfe;}
//		}
//	temp = Chk_xor_sum(gUserGlobal.B33,16);
//	if(temp == 0)
//		{gUserGlobal.Psw = gUserGlobal.B33[0];
//		temp = Chk_psw();
//		if(temp == 0)
//			{para1=para1&0xfd;}
//		}
		if(debugflag)
			{
//			memcpy(gUserGlobal.metro_prot.send_buf+4,gUserGlobal.CardSnr,4);
			watchdog();
			gUserGlobal.metro_prot.send_buf[0] = gUserGlobal.Metrosta;
			gUserGlobal.metro_prot.send_buf[1] = para1;
			gUserGlobal.metro_prot.send_bytes = 2;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
switch(para1)
	{case 0://in out all ok
		if(gUserGlobal.Metrosta == 0xcc)
			{gUserGlobal.Amountcount=gUserGlobal.B32[11];
			gUserGlobal.AmountbalanceL=gUserGlobal.B32[12];
			gUserGlobal.AmountbalanceM=gUserGlobal.B32[13];
			gUserGlobal.AmountbalanceH=gUserGlobal.B32[14];
			}
		else
			{
			gUserGlobal.Amountcount=gUserGlobal.B33[11];
			gUserGlobal.AmountbalanceL=gUserGlobal.B33[12];
			gUserGlobal.AmountbalanceM=gUserGlobal.B33[13];
			gUserGlobal.AmountbalanceH=gUserGlobal.B33[14];
			}
		break;
	case 1://out ok
		gUserGlobal.Amountcount=gUserGlobal.B33[11];
		gUserGlobal.AmountbalanceL=gUserGlobal.B33[12];
		gUserGlobal.AmountbalanceM=gUserGlobal.B33[13];
		gUserGlobal.AmountbalanceH=gUserGlobal.B33[14];
		if(gUserGlobal.Metrosta == 0xcc)
			{memcpy(gUserGlobal.B32,gUserGlobal.B33,16);
			gUserGlobal.B32[0] = 0x41;
			gUserGlobal.B32[9] = 0;
			gUserGlobal.B32[10] = 0;
			lrc_gen2(gUserGlobal.B32,15);
			}
		break;
	case 2://in ok
		gUserGlobal.Amountcount=gUserGlobal.B32[11];
		gUserGlobal.AmountbalanceL=gUserGlobal.B32[12];
		gUserGlobal.AmountbalanceM=gUserGlobal.B32[13];
		gUserGlobal.AmountbalanceH=gUserGlobal.B32[14];
		if(gUserGlobal.Metrosta == 0xdd)
			{memcpy(gUserGlobal.B33,gUserGlobal.B32,16);
			gUserGlobal.B33[0] = 0x81;
			lrc_gen2(gUserGlobal.B33,15);
			}
		break;
	default:// all wrong
		gUserGlobal.Amountcount=0;
		gUserGlobal.AmountbalanceL=0;
		gUserGlobal.AmountbalanceM=0;
		gUserGlobal.AmountbalanceH=0;
		memset(gUserGlobal.B32,0,16);
		memset(gUserGlobal.B33,0,16);
		gUserGlobal.B32[0]=0x41;
		gUserGlobal.B32[15]=0x41;
		gUserGlobal.B33[0]=0x81;
		gUserGlobal.B33[15]=0x81;
		gUserGlobal.Metrosta = 0xdd;
		break;
	}
gUserGlobal.Amountbalance=gUserGlobal.AmountbalanceL+(gUserGlobal.AmountbalanceM<<8)+(gUserGlobal.AmountbalanceH<<16);
	return 0;
}


/*=======================================================================================
函数：
功能：0.99 读卡
=========================================================================================*/

UBYTE Readcard(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,Para1,buf[256],encry[256];
	UWORD temp1;
//	UBYTE rdlen;
	UDWORD balance;
	watchdog();
	mcml_pwr_off();
	delay_ms(5);
	mcml_pwr_on();
	delay_ms(5);
	temp=HL_Active();
	if(temp != 0)
		{return temp;}
//	if(p->yled_flag){
//		yled(LED_OFF);
//		p->yled_flag = 0;
//		}
//	else{
//		yled(LED_ON);
//		p->yled_flag = 1;
//		}
	if(gUserGlobal.TagType == 0x03)
		{goto label_SJT;}
	else if(gUserGlobal.TagType == 0x02)
		{goto label_CPU;}

m1_step:
	sam_select(CPU_SAM);
	watchdog();
	temp = Sam_select1003();
	if(temp!=0)
		{gUserGlobal.device_state = gUserGlobal.device_state&0xe0;
		return SHMETRO_CE_PSAMERR;
		}
	watchdog();
	temp = Sam_KeyA();
	if(temp!=0)
		{
		return temp;
		}
//	temp = fm_get_key();
//	if(temp == 0xfe)
//		{
//		gUserGlobal.device_state = gUserGlobal.device_state&0xe0;
//		return 0x23;
//		}
//	else if(temp == 0xff)
//		{return SHMETRO_CE_INVALIDCARD;}

recov:
	watchdog();
	temp = mcml_load_key(0x00,0X00,0X03,gUserGlobal.KeyA3);
	if(temp != 0x00)
		{return 1;}
	temp = mcmh_read_s50(12,gUserGlobal.B12,1,0);//mcmh_read_s50(UBYTE block, UBYTE *outbuf,UBYTE op_type,UBYTE key_type);
	if(temp != 0x00)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.B12,16);
		gUserGlobal.metro_prot.send_bytes = 16;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = Chk_xor_sum(gUserGlobal.B12,0x10);
	if(temp != 0)
		{
		watchdog();
		temp=Recover_c();
		if(temp == SHMETRO_CE_INVALIDCARD)
			{return SHMETRO_CE_INVALIDCARD;}
		goto recov;
		}
	if(gUserGlobal.B12[0]==0x55&&gUserGlobal.B12[1]==0xaa&&gUserGlobal.B12[2]==0x55)
		{
		watchdog();
		temp=Recover_b();
		if(temp == SHMETRO_CE_INVALIDCARD)
			{return SHMETRO_CE_INVALIDCARD;}
		if(temp!=0)
			{goto recov;}
		else
			{goto recov_ok;}
		}
	else if(gUserGlobal.B12[0]!=0x00&&gUserGlobal.B12[1]!=0x00&&gUserGlobal.B12[2]!=0x00)
		{temp=Recover_c();
		if(temp == SHMETRO_CE_INVALIDCARD)
			{return SHMETRO_CE_INVALIDCARD;}
		goto recov;
		}
recov_ok:
	watchdog();
	if(gUserGlobal.Cpuflag == 0x01)
		{
		return 1;
		}
	else if(gUserGlobal.Cpuflag == 0x02)//未解锁的卡
		{
		watchdog();
		temp=HL_Activecpu();
		watchdog();
		if(temp != 0)
			{return 1;}
		gUserGlobal.CityNo[0] = 0x00;
		gUserGlobal.CityNo[1] = 0x00;
		temp = Cpu_sel1001();
		if(temp==1)
			{return 1;}
		gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
		return 0;
		}
		
	temp = mcml_read(13,gUserGlobal.B13);
	if(temp != 0x00)
		{return 1;}
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.B13,16);
		gUserGlobal.metro_prot.send_bytes = 16;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = Chk_xor_sum(gUserGlobal.B13,16);
	if(temp != 0)
		{return SHMETRO_CE_INVALIDCARD;}
	if(gUserGlobal.B13[11]!=0x02)
		{return SHMETRO_CE_INVALIDCARD;}
	gUserGlobal.CardPoint = gUserGlobal.B13[12];
	gUserGlobal.M1CardCnt[0] = gUserGlobal.B13[13];
	gUserGlobal.M1CardCnt[1] = gUserGlobal.B13[14];
	gUserGlobal.CardCount = (gUserGlobal.B13[13]<<8)+gUserGlobal.B13[14];
	if(gUserGlobal.CardPoint>9||gUserGlobal.CardCount==0xffff)
		{return SHMETRO_CE_INVALIDCARD;}
	temp=Rd_sector7();
	if(temp!=0)
		{return 1;}
	temp = mcml_load_key(0x00,0X00,0X01,gUserGlobal.KeyA1);
	if(temp != 0x00)
		{return 1;}
	temp = mcmh_read_s50(4,gUserGlobal.B4,1,0);
	if(temp != 0x00)
		{return 1;}
	temp = Chk_xor_sum(gUserGlobal.B4,16);
	if(temp != 0)
		{return SHMETRO_CE_INVALIDCARD;}
	gUserGlobal.CityNo[0] = gUserGlobal.B4[0];
	gUserGlobal.CityNo[1] = gUserGlobal.B4[1];
	gUserGlobal.CardType = gUserGlobal.B4[9];
	gUserGlobal.Cardversion = gUserGlobal.B4[8];
	temp = mcml_read(5,gUserGlobal.B5);
	if(temp != 0x00)
		{return 1;}
	temp=Chk_balanceblk(gUserGlobal.B5);
	if(temp!=0)
		{return SHMETRO_CE_INVALIDCARD;}
	gUserGlobal.Blk5val=(gUserGlobal.B5[7]<<24)+(gUserGlobal.B5[6]<<16)+(gUserGlobal.B5[5]<<8)+gUserGlobal.B5[4];
	temp = mcml_read(6,gUserGlobal.B6);
	if(temp != 0x00)
		{return 1;}
	temp=Chk_balanceblk(gUserGlobal.B6);
	gUserGlobal.Blk6val=(gUserGlobal.B6[7]<<24)+(gUserGlobal.B6[6]<<16)+(gUserGlobal.B6[5]<<8)+gUserGlobal.B6[4];
	if(temp!=0)
		{
		temp = mcml_restore(5);
		if(temp != 0x00)
			{return 1;}
		temp = mcml_transfer(6);
		if(temp != 0x00)
			{return 1;}
		temp = mcml_read(6,gUserGlobal.B6);
		if(temp != 0x00)
			{return 1;}
		gUserGlobal.Blk6val=(gUserGlobal.B6[7]<<24)+(gUserGlobal.B6[6]<<16)+(gUserGlobal.B6[5]<<8)+gUserGlobal.B6[4];
		temp = compareBlk(gUserGlobal.B6,gUserGlobal.B5);
		if(temp != 0)
			{return 1;}
		}
	else
		{
		temp = memcmp(gUserGlobal.CardBak,gUserGlobal.CardSnr,4);
		if(gUserGlobal.Err17Flag == 0x01&&temp == 0)
			{
			if(gUserGlobal.CardPoint==0)
				{temp=0x09;}
			else
				{temp=gUserGlobal.CardPoint-1;}
			temp1 = (gUserGlobal.CardBak[12]<<8)+gUserGlobal.CardBak[13]+1;
//			if((p->CardBak[0]==p->CardSnr[0])&&(p->CardBak[1]==p->CardSnr[1])&&(p->CardBak[2]==p->CardSnr[2])&&(p->CardBak[3]==p->CardSnr[3]))
			if((temp1== p->CardCount)&&(p->CardBak[28]==temp))
				{
				balance=(gUserGlobal.CardBak[7]<<24)+(gUserGlobal.CardBak[6]<<16)+(gUserGlobal.CardBak[5]<<8)+gUserGlobal.CardBak[4];
				balance = balance - gUserGlobal.Decval;
				balance = ~balance;
				if(balance == gUserGlobal.Blk5val)
					{gUserGlobal.sameflag = 0;
					temp = Sam_Tac();
					gUserGlobal.Err17Flag = 0x00;
					}
				}
			}
		}
	if(gUserGlobal.Blk5val!=gUserGlobal.Blk6val)
		{
		temp = mcml_restore(5);
		if(temp != 0x00)
			{return 1;}
		temp = mcml_transfer(6);
		if(temp != 0x00)
			{return 1;}
		temp = mcml_read(6,gUserGlobal.B6);
		if(temp != 0x00)
			{return 1;}
		gUserGlobal.Blk6val=(gUserGlobal.B6[7]<<24)+(gUserGlobal.B6[6]<<16)+(gUserGlobal.B6[5]<<8)+gUserGlobal.B6[4];
		temp = compareBlk(gUserGlobal.B6,gUserGlobal.B5);
		if(temp != 0)
			{return 1;}
		}
	temp = mcml_load_key(0x00,0X00,0X02,gUserGlobal.KeyA2);
	if(temp != 0x00)
		{return 1;}
	temp = mcmh_read_s50(8,gUserGlobal.B8,1,0);
	if(temp != 0x00)
		{return 1;}
	Add_balance();
	temp=Chk_balance();
	if(temp!=0)
		{return SHMETRO_CE_INVALIDCARD;}
	memcpy(gUserGlobal.CardExpiredDate,gUserGlobal.B8+12,4);
	if(samkey)
		{
		memcpy(buf,gUserGlobal.CardSnr,4);
		memcpy(buf+4,gUserGlobal.B8,4);
		des_encode(gUserGlobal.Keyauth,buf,encry);
		temp = Chk_des(gUserGlobal.B8+4,encry);
		}
	else
		{temp = fm_card_valcheck();}
	if(temp != 0)
		{return SHMETRO_CE_INVALIDCARD;}
	if(gUserGlobal.ReadMode==0x2)
		{//Tvmsend();
		gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
		return 0;
		}
	temp = mcml_load_key(0x00,0X00,12,gUserGlobal.KeyA8);
	if(temp != 0x00)
		{return 1;}
	temp = mcmh_read_s50(48,gUserGlobal.B32,1,0);
	if(temp != 0x00)
		{return 1;}
	temp = mcml_read(49,gUserGlobal.B33);
	if(temp != 0x00)
		{return 1;}
	temp = mcml_read(50,gUserGlobal.B34);
	if(temp != 0x00)
		{return 1;}
	temp=Send_io();
	if(temp!=0)
		{return 1;}
	gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
	return 0;
label_CPU:
	watchdog();
	gUserGlobal.CityNo[0] = 0x00;
	gUserGlobal.CityNo[1] = 0x00;
	temp = Cpu_sel1001();
	if(temp==1)
		{return 1;}
	gUserGlobal.Cpuflag = 0;
	if(gUserGlobal.SW1SW2 == 0x9303)
		{return SHMETRO_CE_INVALIDCARD;}
	temp1 = (gUserGlobal.CityNo[0]<<8)+gUserGlobal.CityNo[1];
	if(gUserGlobal.Cardversion == 0x02&&temp1 == 0x2000)
		{if(gUserGlobal.SW1SW2 == 0x6a81)
			{
			watchdog();
			temp=HL_Active2();		//未解锁
			if(temp == 2)
				{return SHMETRO_CE_INVALIDCARD;}
			else if(temp!=0)
				{return temp;}
			gUserGlobal.TagType=0x00;//当做m1卡处理
			watchdog();
			goto m1_step;
			}
		else if(gUserGlobal.SW1SW2 == 0x9000)
//*****************************************需要锁cpu
			{temp = p->Sam_errflag&0x01;
			if(temp != 0 )
				{return SHMETRO_CE_INVALIDCARD;}
			goto readlock;
			}
		else
			{return SHMETRO_CE_INVALIDCARD;}
		}
	if(gUserGlobal.Cardversion == 0x82&&temp1 == 0x2000)
		{
		watchdog();
		temp=HL_Active2();		//未解锁
		if(temp == 2)
			{return SHMETRO_CE_INVALIDCARD;}
		else if(temp!=0)
			{return temp;}
		gUserGlobal.TagType=0x00;//当做m1卡处理
		watchdog();
		goto m1_step;
		}
	if(gUserGlobal.SW1SW2 == 0x9000)
		{goto label_CPU0;}
	else if(gUserGlobal.SW1SW2 == 0x6a81)
		{
		watchdog();
		temp=HL_Active2();		//未解锁
		if(temp == 2)
			{return SHMETRO_CE_INVALIDCARD;}
		else if(temp!=0)
			{return temp;}
		gUserGlobal.Cpuflag = 0x02;
		watchdog();
		goto m1_step;
		}
	else
		{goto pufa_flow;}

label_CPU0:
	gUserGlobal.SW1SW2=0x00;
	gUserGlobal.TransValue[0]=0x00;
	gUserGlobal.TransValue[1]=0x00;
	gUserGlobal.TransValue[2]=0x00;
	gUserGlobal.TransValue[3]=0x00;
	watchdog();
	temp = Ini_purchase_ADF1();
	if(temp==1)
		{return 1;}
	if(gUserGlobal.SW1SW2==0x6281)
		{temp=HL_Active2();
		if(temp == 2)
			{return SHMETRO_CE_INVALIDCARD;}
		else if(temp!=0)
			{return temp;}
		gUserGlobal.Cpuflag = 0x01;
		goto m1_step;
		}
	if(temp == 2)
		{goto label_carderr;}
	temp = Chk_balance();
	if(temp!=0)
		{return SHMETRO_CE_INVALIDCARD;}
	if(gUserGlobal.sameflag	==1&&gUserGlobal.Err17Flag == 0x11)
		{
		temp = Geproof();
		if(temp == 1)
			{return 1;}
		}
	if(gUserGlobal.sameflag	==1&&gUserGlobal.Err17Flag == 0x12)
		{
		temp = Cpu_getprove();
//		if(temp == 1)
//			{return 1;}
		}
	if(gUserGlobal.ReadMode==0x2)
		{//Tvmsend();
		gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
		return 0;
		}
	if(gUserGlobal.CityNo[0]==0x21&&gUserGlobal.CityNo[1]==0x53)
		{goto cpuflow3;}
	if(gUserGlobal.CityNo[0]!=0x20||gUserGlobal.CityNo[1]!=0x00)
		{goto othercity;}
cpuflow3:
	watchdog();
	temp=CpuRd_sector7();
	if(temp!=0)
		{return temp;}
	watchdog();
	temp=Cpu_rdsec8(gUserGlobal.B34,0x06);
	if(temp!=0)
		{return 1;}
	temp=Cpu_rdsec8(gUserGlobal.B32,0x04);
	if(temp!=0)
		{return 1;}
	temp=Cpu_rdsec8(gUserGlobal.B33,0x05);
	if(temp!=0)
		{return 1;}
	temp = Send_io();
	gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
	return 0;

othercity:
	temp=Cpu_rd0015();
	if(temp!=0)
		{return 1;}
	temp=Cpu_rd0017(gUserGlobal.ADF1_file0017);
	if(temp!=0)
		{return 1;}

	gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
	return 0;
readlock:
	temp = Cpu_getrandom(4);
	if(temp!=0)
		{return 1;}
	sam_select(CPU_SAM);
	temp = Sam_select1001();
	if(temp!=0)
		{return SHMETRO_CE_PSAMERR;}
	gUserGlobal.Ride[1] = 0x01;
	temp = Sam_Lockmac();
	if(temp!=0)
		{return SHMETRO_CE_PSAMERR;}
	temp = Cpu_Lockcard();
	if(temp!=0)
		{return 1;}
	gUserGlobal.device_state = gUserGlobal.device_state&0xe0;
	return SHMETRO_CE_INVALIDCARD;
	
label_SJT:
	temp=mcml_read(0x00,gUserGlobal.Sjtbl0);
	if(temp!=0)
		{return 1;}
//	temp=tocken_read(0x01,gUserGlobal.Sjtbl1);
//	if(temp!=0)
//		{return 1;}
//	gUserGlobal.CardSnr[0]=gUserGlobal.Sjtbl1[0];
//	gUserGlobal.CardSnr[1]=gUserGlobal.Sjtbl1[1];
//	gUserGlobal.CardSnr[2]=gUserGlobal.Sjtbl1[2];
//	gUserGlobal.CardSnr[3]=gUserGlobal.Sjtbl1[3];
//	if(debugflag)
//		{
//		watchdog();
//		memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.Sjtbl1,4);
//		gUserGlobal.metro_prot.send_bytes = 6;
//		gUserGlobal.metro_prot.send_buf[4] = temp;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		delay_ms(100);
//		}

	temp=mcml_read(0x02,gUserGlobal.Sjtbl2);
	if(temp!=0)
		{return 1;}
	temp=mcml_read(0x03,gUserGlobal.Sjtbl3);
	if(temp!=0)
		{return 1;}
	temp=mcml_read(0x04,gUserGlobal.Sjtbl4);
	if(temp!=0)
		{return 1;}
	sam_select(SJT_SAM);
//	temp = Sam_select0003();
	temp = Sam_SjtMac();
	if(temp!=0)
		{return temp;}
	temp = compareBlk4(gUserGlobal.Sjtbl4,gUserGlobal.Mac);
	if(temp != 0)
		{return 1;}

	temp=mcml_read(0x05,gUserGlobal.Sjtbl5);//SALE pos&card type
	if(temp!=0)
		{return 1;}
	gUserGlobal.CardType=gUserGlobal.Sjtbl5[3];
	temp=mcml_read(0x06,gUserGlobal.Sjtbl6);//SALE DATE
	if(temp!=0)
		{return 1;}
	temp=mcml_read(0x07,gUserGlobal.Sjtbl7);
	if(temp!=0)
		{return 1;}
	gUserGlobal.BalanceM=gUserGlobal.Sjtbl7[0];
	gUserGlobal.BalanceL=gUserGlobal.Sjtbl7[1];
	gUserGlobal.Sjtbkflag=0;
//Balance=BalanceM*0x100+BalanceL;

	temp = Sam_KeyA1();
	if(temp!=0)
		{return temp;}
//	memcpy(gUserGlobal.KeyA1,"\xff\xff\xff\xff\x00\x00",6);
//	mcml_load_key(0,0,1,gUserGlobal.KeyA1);
//	temp = mcml_authentication(0,0,1);
	temp = fm_mcmh_authentication(0,0,gUserGlobal.KeyA1);
	if(debugflag)
		{
		watchdog();
		gUserGlobal.metro_prot.send_buf[0] = temp;
		gUserGlobal.metro_prot.send_bytes = 1;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	if(temp!=0)
		{return 1;}
	temp=mcml_read(14,gUserGlobal.Sjtbl14);
	if(temp!=0)
		{return 1;}
	temp=mcml_read(0x09,gUserGlobal.Sjtbl9);
	if(temp!=0)
		{return 1;}
	gUserGlobal.SjtCount[0]=gUserGlobal.Sjtbl9[1];
	gUserGlobal.SjtCount[1]=gUserGlobal.Sjtbl9[2];
	gUserGlobal.CardCount=(gUserGlobal.SjtCount[0]<<8)+gUserGlobal.SjtCount[1];
	Para1=0;
	temp=gUserGlobal.Sjtbl9[0]^gUserGlobal.Sjtbl9[1]^gUserGlobal.Sjtbl9[2]^gUserGlobal.Sjtbl9[3];
	if(temp!=0||gUserGlobal.Sjtbl9[0]==0xff)
		{Para1=1;}
	gUserGlobal.Psw=gUserGlobal.Sjtbl9[0];
	temp=Chk_psw();
	if(temp!=0)
		{Para1=1;}
	if(Para1==0)
		{gUserGlobal.Metrosta=gUserGlobal.Sjtbl9[0];}
	temp = Chk_sjtc();
	if(temp ==1)
		{Para1=1;}

	if(Para1!=0)//blk 9 wrong
		{temp=tocken_read(0x0f,gUserGlobal.Sjtbl15);
		if(temp!=0)
			{return 1;}
		temp=gUserGlobal.Sjtbl15[0]^gUserGlobal.Sjtbl15[1]^gUserGlobal.Sjtbl15[2]^gUserGlobal.Sjtbl15[3];
		if(temp!=0)
			{return SHMETRO_CE_INVALIDCARD;}
		gUserGlobal.Psw=gUserGlobal.Sjtbl15[0];
		temp=Chk_psw();
	//	if(temp!=0||Sjtbl15[0]==0x00||Sjtbl15[0]==0xff)
		if(temp!=0)
			{return SHMETRO_CE_INVALIDCARD;}
		gUserGlobal.Sjtbkflag=1;
		gUserGlobal.Metrosta=gUserGlobal.Sjtbl15[0];
		gUserGlobal.BalanceM=gUserGlobal.Sjtbl15[1];
		gUserGlobal.BalanceL=gUserGlobal.Sjtbl15[2];
		//Balance=BalanceM*0x100+BalanceL;
		}
	Para1=gUserGlobal.Metrosta&0x0e;
	if(Para1==0x02)		//sale
		{
		gUserGlobal.Last[0]=gUserGlobal.Sjtbl6[3];	//time
		gUserGlobal.Last[1]=gUserGlobal.Sjtbl6[2];
		gUserGlobal.Last[2]=gUserGlobal.Sjtbl6[1];
		gUserGlobal.Last[3]=gUserGlobal.Sjtbl6[0];
		gUserGlobal.Last[4]=gUserGlobal.Sjtbl5[0];	//LAST STATION
		gUserGlobal.Last[5]=gUserGlobal.Sjtbl5[1];
		gUserGlobal.Last[6]=gUserGlobal.Sjtbl5[2];
		gUserGlobal.Laststation[0]=gUserGlobal.Sjtbl5[0];
		gUserGlobal.Laststation[1]=gUserGlobal.Sjtbl5[1];
		}
	else if(Para1==0x0c)	//exit
		{temp=tocken_read(12,gUserGlobal.Sjtbl12);
		if(temp!=0)
			{return 1;}
		temp=tocken_read(13,gUserGlobal.Sjtbl13);
		if(temp!=0)
			{return 1;}
		gUserGlobal.Last[0]=gUserGlobal.Sjtbl12[3];	//time
		gUserGlobal.Last[1]=gUserGlobal.Sjtbl12[2];
		gUserGlobal.Last[2]=gUserGlobal.Sjtbl12[1];
		gUserGlobal.Last[3]=gUserGlobal.Sjtbl12[0];
		gUserGlobal.Last[4]=gUserGlobal.Sjtbl13[0];	//LAST STATION
		gUserGlobal.Last[5]=gUserGlobal.Sjtbl13[1];
		gUserGlobal.Last[6]=gUserGlobal.Sjtbl13[2];
		gUserGlobal.Laststation[0]=gUserGlobal.Sjtbl13[0];
		gUserGlobal.Laststation[1]=gUserGlobal.Sjtbl13[1];
		}
	else{temp=tocken_read(10,gUserGlobal.Sjtbl10);
		if(temp!=0)
			{return 1;}
		temp=tocken_read(11,gUserGlobal.Sjtbl11);
		if(temp!=0)
			{return 1;}
		gUserGlobal.Last[0]=gUserGlobal.Sjtbl10[3];	//time
		gUserGlobal.Last[1]=gUserGlobal.Sjtbl10[2];
		gUserGlobal.Last[2]=gUserGlobal.Sjtbl10[1];
		gUserGlobal.Last[3]=gUserGlobal.Sjtbl10[0];
		gUserGlobal.Last[4]=gUserGlobal.Sjtbl11[0];	//LAST STATION
		gUserGlobal.Last[5]=gUserGlobal.Sjtbl11[1];
		gUserGlobal.Last[6]=gUserGlobal.Sjtbl11[2];
		gUserGlobal.Laststation[0]=gUserGlobal.Sjtbl11[0];
		gUserGlobal.Laststation[1]=gUserGlobal.Sjtbl11[1];
		}
	gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
	return 0;
pufa_flow:
	temp = pf_selectpay();
	if(temp!=0)
		{return 1;}
	temp = pf_selectfile();
	if(temp!=0)
		{return 1;}
	temp = gUserGlobal.CappFlag&0x7f;
	if(temp!=0x01&&temp!=0x02)
		{return SHMETRO_CE_INVALIDCARD;}
	sam_select(SJT_SAM);
//	watchdog();
	temp=pf_rd0015(1,gUserGlobal.Pboc_0015);
	if(temp!=0)
		{return 1;}
	if(gUserGlobal.Pboc_0015[3]!=0x01||gUserGlobal.Pboc_0015[4]!=0x01||gUserGlobal.Pboc_0015[5]!=0x00)
		{return SHMETRO_CE_INVALIDCARD;}
	temp = gUserGlobal.Pboc_0015[19];
	if(temp==0x90)
		{goto et_flow;}
	memcpy(gUserGlobal.MT_UID,gUserGlobal.Pboc_0015+6,14);
	gUserGlobal.PAN = gUserGlobal.Pboc_0015[16];
	memcpy(gUserGlobal.PbocStartDate,gUserGlobal.Pboc_0015+20,4);
	memcpy(gUserGlobal.PbocExpDate,gUserGlobal.Pboc_0015+24,4);
	
	temp=gUserGlobal.CappFlag&0x80;
	if((gUserGlobal.r_mac!=0)&&(temp==0x80))
		{
		temp = Sam_select1002();
		if(temp != 0)
			{return SHMETRO_CE_NOISAM;}
		temp = pboc_cappmac4(1,gUserGlobal.Pboc_0015);
		if(temp != 0)
			{return SHMETRO_CE_NOISAM;}
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.RMac,4);
			memcpy(gUserGlobal.metro_prot.send_buf+4,gUserGlobal.Mac,4);
			gUserGlobal.metro_prot.send_bytes = 8;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			
			}
		
		if((gUserGlobal.Mac[0]!=gUserGlobal.RMac[0])||(gUserGlobal.Mac[1]!=gUserGlobal.RMac[1])||(gUserGlobal.Mac[2]!=gUserGlobal.RMac[2])||(gUserGlobal.Mac[3]!=gUserGlobal.RMac[3]))
			{return SHMETRO_CE_INVALIDCARD;}
		}

	if(gUserGlobal.flag_lian==1)
		{
		temp=pf_rd0015(3,gUserGlobal.Pboc_sec7);
		if(temp!=0)
			{return 1;}
		if(gUserGlobal.Pboc_sec7[3]!=0x01||gUserGlobal.Pboc_sec7[4]!=0x01||gUserGlobal.Pboc_sec7[5]!=0x00)
			{return SHMETRO_CE_INVALIDCARD;}
		memcpy(gUserGlobal.Huancheng+3,gUserGlobal.Pboc_sec7+9,5);
		temp=gUserGlobal.CappFlag&0x80;
		if((gUserGlobal.r_mac!=0)&&(temp==0x80))
			{
			temp=pboc_cappmac4(3,gUserGlobal.Pboc_sec7);
			if(temp!=0)
				{return SHMETRO_CE_NOISAM;}
			if((gUserGlobal.Mac[0]!=gUserGlobal.RMac[0])||(gUserGlobal.Mac[1]!=gUserGlobal.RMac[1])||(gUserGlobal.Mac[2]!=gUserGlobal.RMac[2])||(gUserGlobal.Mac[3]!=gUserGlobal.RMac[3]))
				{return SHMETRO_CE_INVALIDCARD;}
			}
		}
	else
		{memcpy(gUserGlobal.Huancheng+3,"\x0\x0\x0\x0\x0",5);}

	temp=pf_rd0015(2,gUserGlobal.Pboc_sec8);//entry & exit message
	if(temp!=0)
		{return 1;}
	if(gUserGlobal.Pboc_sec8[3]!=0x01||gUserGlobal.Pboc_sec8[4]!=0x01||gUserGlobal.Pboc_sec8[5]!=0x00)
		{return SHMETRO_CE_INVALIDCARD;}
	gUserGlobal.Metrosta = gUserGlobal.Pboc_sec8[6];
	temp=gUserGlobal.CappFlag&0x80;
	if((gUserGlobal.r_mac!=0)&&(temp==0x80))
		{
		temp = pboc_cappmac4(2,gUserGlobal.Pboc_sec8);
		if(temp != 0)
			{return SHMETRO_CE_NOISAM;}
		if((gUserGlobal.Mac[0]!=gUserGlobal.RMac[0])||(gUserGlobal.Mac[1]!=gUserGlobal.RMac[1])||(gUserGlobal.Mac[2]!=gUserGlobal.RMac[2])||(gUserGlobal.Mac[3]!=gUserGlobal.RMac[3]))
			{return SHMETRO_CE_INVALIDCARD;}
		}
	
	temp = pf_getbalance();
	if(temp != 0)
		{return 1;}
	temp = pf_gettouzi();
	if(temp != 0)
		{return 1;}
	if(gUserGlobal.Blk5val == 0)
		{return SHMETRO_CE_INVALIDCARD;}
	if(gUserGlobal.Balance == 0)
		{
		temp =  pf_gettouzi1();
		if(temp != 0)
			{return 1;}
		
		}
	gUserGlobal.TagType	= 0x04;//浦发卡
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.Pboc_sec7,64);
			gUserGlobal.metro_prot.send_bytes = 64;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}

	gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
	return 0;
et_flow:
	gUserGlobal.TagType = 0x05;
		if(debugflag)
			{
//			memcpy(gUserGlobal.metro_prot.send_buf+4,gUserGlobal.CardSnr,4);
			watchdog();
			gUserGlobal.metro_prot.send_buf[0] = temp;
			gUserGlobal.metro_prot.send_buf[1] = 0X56;
			gUserGlobal.metro_prot.send_bytes = 2;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
	temp = metro_select();
	if(temp != 0)
		{return 1;}
	temp=metro_rd0015();
	if(temp!=0)
		{return 1;}
	temp=metro_rd0006();
	if(temp!=0)
		{return 1;}
	temp=metro_rd0019();
	if(temp!=0)
		{return 1;}

	gUserGlobal.Metrosta=gUserGlobal.metro0019[3];
	
	temp = metro_getbalance();
	if(temp!=0)
		{return 1;}
	if(gUserGlobal.sameflag	==1&&gUserGlobal.Err17Flag == 0x11)
		{
		temp = Geproof();
		if(temp == 1)
			{return 1;}
		}
//	gUserGlobal.TagType = 0x05;//员工卡
	if(gUserGlobal.metro0019[2]!=0x00)
		{gUserGlobal.device_state = gUserGlobal.device_state&0xe0;
		return SHMETRO_CE_INVALIDCARD;
		}

	gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
	return 0;
label_carderr:
	gUserGlobal.device_state = gUserGlobal.device_state&0xe0;
	return SHMETRO_CE_INVALIDCARD;

}


/*=======================================================================================
函数：
功能：2.1  初始化命令。CurrentDate yyyymmdd
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_INITREADER_input(UBYTE *ibuf,UBYTE ibytes)
{
//USER_GLOBAL *p = &gUserGlobal;
//
	UBYTE Para1;
	gUserGlobal.Posid[0]=ibuf[2];//线路
	gUserGlobal.Posid[1]=ibuf[3];//车站
	gUserGlobal.Posid[3]=ibuf[4];//设备代码
	Para1= 0x00;
	switch(ibuf[13])
		{
//		case 0x20:gUserGlobal.Posid[2]=0x1a;
//			gUserGlobal.device_state = 0x20;
//			gUserGlobal.Postype = 0x11;		//进站闸机
//			break;
//		case 0x40:gUserGlobal.Posid[2]=0x15;
//			gUserGlobal.device_state = 0x40;
//			gUserGlobal.Postype = 0x12;		//出站闸机
//			break;
		case 0x60:gUserGlobal.Posid[2]=0x18;
			gUserGlobal.device_state = 0x60;
			gUserGlobal.Postype = 0x13;		//BOM
			break;
//		case 0x80:gUserGlobal.Posid[2]=0x13;
//			gUserGlobal.device_state = 0xc0;
//			gUserGlobal.Postype = 0x13;		//tvm
//			break;
//		case 0xa0:gUserGlobal.Posid[2]=0x61;
//			gUserGlobal.device_state = 0x80;
//			gUserGlobal.Postype = 0x14;		//TVM
//			break;
//		case 0xa1:gUserGlobal.Posid[2]=0x60;
//			gUserGlobal.device_state = 0x80;
//			gUserGlobal.Postype = 0x14;		//TVM
//			break;
		case 0x00:
			gUserGlobal.Posid[2]=0x18;
			gUserGlobal.device_state = 0x60;
			gUserGlobal.Postype = 0x13;		//TEST MODE
			break;
		default:
			Para1=0x03;
			break;
		}
	if(Para1!=0)
		{return SHMETRO_CE_BADPARAM;}

	return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.1
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_INITREADER_proc(void)
{
	USER_GLOBAL *p = &gUserGlobal;
	UBYTE Para1,temp;

	if(gUserGlobal.device_state==0X60)
		{Para1=0x14;}
	else
		{Para1=0x11;}
	temp = p->Sam_errflag&0x01;
	if(temp != 0 )
		{return SHMETRO_CE_OK;}
	if(p->CpuSamid[2]!=Para1||p->Posid[0]!=p->CpuSamid[3]||p->Posid[1]!=p->CpuSamid[4]||p->Posid[3]!=p->CpuSamid[5])
		{sam_select(CPU_SAM);
		if(gUserGlobal.device_state==0X60)
			{p->CpuSamid[2]=0x14;}
		else
			{p->CpuSamid[2]=0x11;}
		p->CpuSamid[3]=p->Posid[0];
		p->CpuSamid[4]=p->Posid[1];
		p->CpuSamid[5]=p->Posid[3];
		temp=Sam_select3f00();
		if(temp!=0)
			{return SHMETRO_CE_OK;}//SHMETRO_CE_PSAMERR;}
		temp=Sam_changesamid();
		if(temp!=0)
			{return SHMETRO_CE_OK;}//SHMETRO_CE_PSAMERR;}
		temp=Sam_select1001();
		if(temp!=0)
			{return SHMETRO_CE_OK;}//SHMETRO_CE_PSAMERR;}
		}
	return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.1
=========================================================================================*/
void cmd_SHMETRO_OT_INITREADER_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;
//

obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
obuf[3] = gUserGlobal.device_state;
*obytes = 4;
//shmetro_enter_state(SHMETRO_IDLE_STATE);
//
return;
}



/*=======================================================================================
函数：
功能：2.2
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_RESETREADER_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.2
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_RESETREADER_proc(void)
{
	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[200];
	UBYTE len,temp;

	p->flag_lian = 0x00;
	p->r_mac = 0x00;
	p->Sam_keyversion=0x01;
	p->Sam_errflag=0x03;
	memset(gUserGlobal.CpuSamid,0,6);
	memset(gUserGlobal.SjtSamid,0,6);
//	set_card_type(ISO14443A_SH_TYPE);//设置上海模式或者m1普通模式
//	set_card_type(ISO14443A_M1_TYPE);
//	set_card_type(1);
	sam_rst_clr();
	sam_clk_disable(0);
	sam_power_off(0);
	delay_ms(100);
	sam_power_on(0);
	sam_clk_enable(0);
	sam_rst_set();
	mcml_cardtype(2);//设置上海模式
	sam_set_speed(CPU_SAM,SAM_ETU_93);		//建设部SAM
	sam_select(CPU_SAM);
	watchdog();
	fm_reset();
	temp = sam_atr(buf,&len);
	if(temp!=0)
		{goto Sam_ini1;}
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}

	if(buf[0]!=0x3b)
		{goto Sam_ini1;}
	watchdog();
	temp = Read_Sam0016(gUserGlobal.CpuSamid);
	if(temp!=0)
		{goto Sam_ini1;}
	watchdog();
	temp = Sam_select1001();
	if(temp!=0)
		{goto Sam_ini1;}
	watchdog();
	temp = Read_Keyversion();
	if(temp!=0)
		{goto Sam_ini1;}
	p->Sam_errflag=(p->Sam_errflag)&0x0e;
Sam_ini1:
//	sam_rst_clr();
//	sam_clk_disable(SJT_SAM);
//	sam_power_off(SJT_SAM);
//	delay_ms(100);
//	sam_power_on(SJT_SAM);
//	sam_clk_enable(SJT_SAM);
//	sam_rst_set();
	sam_set_speed(SJT_SAM,SAM_ETU_93);
	
	sam_select(SJT_SAM);
	watchdog();
	temp = sam_atr(buf,&len);
	if(temp!=0)
		{goto Sam_ini2;}
	if(debugflag1)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(100);
		}
	if(buf[0]!=0x3b)
		{goto Sam_ini2;}
	watchdog();
	temp = Read_Sam0015(gUserGlobal.SjtSamid+2);
	if(temp!=0)
		{goto Sam_ini2;}
	temp = Sam_select0003();
	if(temp ==2)
		{temp = Sam_select0002();}
	if(temp!=0)
		{goto Sam_ini2;}
	
	p->Sam_errflag=(p->Sam_errflag)&0x0d;
	temp = Sam_getrandom(8);
//	goto Sam_ini3;
Sam_ini2:
//	p->resp_code=p->Sam_errflag;
	gUserGlobal.device_state = 0xff;
//	if((p->Sam_errflag) == 0x03)
//		{return SHMETRO_CE_NOPSAM;}
	return SHMETRO_CE_OK;

}

/*=======================================================================================
函数：
功能：2.2
=========================================================================================*/
void cmd_SHMETRO_OT_RESETREADER_output(void)
{
USER_GLOBAL *p = &gUserGlobal;
//
	gUserGlobal.metro_prot.send_buf[0] = p->cmd;
	gUserGlobal.metro_prot.send_buf[1] = p->cmd_retry;
	gUserGlobal.metro_prot.send_buf[2] = SHMETRO_CE_OK;
//	if(gUserGlobal.Postype!=0x11||gUserGlobal.Postype!=0x12)
	gUserGlobal.Postype=0x13;
	gUserGlobal.metro_prot.send_buf[3] = gUserGlobal.Postype;
	gUserGlobal.metro_prot.send_buf[4] = SHMETRO_SOFT_VER;
	if((gUserGlobal.Sam_errflag&0x1)==0x01)
		{gUserGlobal.metro_prot.send_buf[5] = 0x02;}
	else
		{gUserGlobal.metro_prot.send_buf[5] = 0x00;}
	memcpy(gUserGlobal.metro_prot.send_buf+6,gUserGlobal.CpuSamid+2,4);
	memcpy(gUserGlobal.metro_prot.send_buf+10,gUserGlobal.SjtSamid+2,4);
	memcpy(gUserGlobal.metro_prot.send_buf+14,"\x0\x0\x0\x0\x0\x0",6);
	gUserGlobal.metro_prot.send_buf[20] = 0x00;
	gUserGlobal.metro_prot.send_bytes = 21;

//
return;
}


/*=======================================================================================
函数：
功能：2.3
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_STOPREADER_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.3
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_STOPREADER_proc(void)
{
	return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.3
=========================================================================================*/
void cmd_SHMETRO_OT_STOPREADER_output(void)
{
USER_GLOBAL *p = &gUserGlobal;
//
gUserGlobal.device_state = 0xff;
gUserGlobal.metro_prot.send_buf[0] = p->cmd;
gUserGlobal.metro_prot.send_buf[1] = p->cmd_retry;
gUserGlobal.metro_prot.send_buf[2] = SHMETRO_CE_OK;
gUserGlobal.metro_prot.send_bytes = 3;
//
return;
}




/*=======================================================================================
函数：
功能：2.5
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_DISPSIGNAL_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.5
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_DISPSIGNAL_proc(void)
{
return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.5
=========================================================================================*/
void cmd_SHMETRO_OT_DISPSIGNAL_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
obuf[3] = 0X00;
*obytes = 4;
//
return;
}


/*=======================================================================================
函数：
功能：2.6
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_READCARD_input(UBYTE *ibuf,UBYTE ibytes)//,UBYTE *obuf,UBYTE *obytes)
{
	USER_GLOBAL *p = &gUserGlobal;
	UBYTE Para1;
	if(gUserGlobal.device_state%16!=0)
		{return SHMETRO_CE_STATUSERROR;}
	gUserGlobal.ReadMode = ibuf[2];
	Para1=0;
	switch(gUserGlobal.ReadMode)
	{
	case 2:
		if(gUserGlobal.device_state==0xc0)
			{break;}
		else
			{Para1=SHMETRO_CE_BADPARAM;
			break;
			}
	case 0x80:
		if(gUserGlobal.device_state==0x20||gUserGlobal.device_state==0x40||gUserGlobal.device_state==0xC0)
			{break;}
		else
			{Para1=SHMETRO_CE_BADPARAM;
			break;
			}
	case 0x81:
		if(gUserGlobal.device_state==0x60)
			{break;}
		else
			{Para1=SHMETRO_CE_BADPARAM;
			break;
			}
	case 0x90:
		if(gUserGlobal.device_state==0x20||gUserGlobal.device_state==0x40||gUserGlobal.device_state==0xC0)
			{break;}
		else
			{Para1=SHMETRO_CE_BADPARAM;
			break;
			}
	case 0x91:
		if(gUserGlobal.device_state==0x60)
			{break;}
		else
			{Para1=SHMETRO_CE_BADPARAM;
			break;
			}
	default:
		Para1=SHMETRO_CE_BADPARAM;
		break;
	}
	if(Para1!=0)
		{return SHMETRO_CE_BADPARAM;}
	memcpy(gUserGlobal.metro_prot.send_buf,"\x30\x00\x00\x00\x00",5);
	gUserGlobal.metro_prot.send_buf[1] = p->cmd_retry;
	gUserGlobal.metro_prot.send_bytes=5;
	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
	prot_shmetro_send_process(&(p->metro_prot));
return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.6
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_READCARD_proc(void)
{
	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	gUserGlobal.device_state= gUserGlobal.device_state|0x01;

	while((gUserGlobal.device_state%16) == 1)
	{
	prot_shmetro_rece_process(&(p->metro_prot));
	if(gUserGlobal.metro_prot.rece_state == PROT_SHMETRO_RECE_WAIT_PROCESS)
		{
//		memcpy(gUserGlobal.metro_prot.last_rece_buf,gUserGlobal.metro_prot.rece_buf,gUserGlobal.metro_prot.rece_bytes);
		p->cmd = gUserGlobal.metro_prot.rece_buf[0];
		p->cmd_retry = gUserGlobal.metro_prot.rece_buf[1];

		switch(p->cmd){
			case SHMETRO_OT_INITREADER://0x20
				temp = cmd_SHMETRO_OT_INITREADER_input(gUserGlobal.metro_prot.rece_buf,gUserGlobal.metro_prot.rece_bytes);
   	    		if(temp!=0)
   	    			{p->resp_code=temp;
   	    			send_err_resp();
   	    			gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
					}
   	    		else
   	    			{gUserGlobal.metro_prot.send_buf[0] = p->cmd;
					gUserGlobal.metro_prot.send_buf[1] = p->cmd_retry;
					gUserGlobal.metro_prot.send_buf[2] = SHMETRO_CE_OK;
					gUserGlobal.metro_prot.send_buf[3] = gUserGlobal.device_state;
					gUserGlobal.metro_prot.send_bytes = 4;
					gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
					}
				break;
			case SHMETRO_OT_RESETREADER://0x21
				temp = cmd_SHMETRO_OT_RESETREADER_proc();
				if(temp!=0)
					{p->resp_code=temp;
   	    			send_err_resp();
   	    			gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
   	    			}
   	    		else
   	    			{cmd_SHMETRO_OT_RESETREADER_output();}
   	    		break;
   	    	case SHMETRO_OT_STOPREADER://0x22
   	    		cmd_SHMETRO_OT_STOPREADER_output();
   	    		break;
			case SHMETRO_OT_READERSTATUS://0x23
				gUserGlobal.metro_prot.send_buf[0] = p->cmd;
				gUserGlobal.metro_prot.send_buf[1] = p->cmd_retry;
				gUserGlobal.metro_prot.send_buf[2] = SHMETRO_CE_OK;
				gUserGlobal.metro_prot.send_buf[3] = gUserGlobal.device_state;
				gUserGlobal.metro_prot.send_bytes=4;
				gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
				prot_shmetro_send_process(&(p->metro_prot));
				gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
				p->cmd = SHMETRO_OT_READCARD;
				break;
			case SHMETRO_OT_ABORT://0x3f
				if(gUserGlobal.device_state == 0XFF)
					{p->resp_code=SHMETRO_CE_STATUSERROR;
   	    			send_err_resp();
   	    			gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
   	    			}
   	    		else
   	    			{cmd_SHMETRO_OT_ABORT_output();
   	    			}
   	    		break;
			default:
				p->resp_code=SHMETRO_CE_STATUSERROR;
   	    		send_err_resp();
   	    		gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
				break;
			}
//
		temp = gUserGlobal.device_state&0x01;
		if(gUserGlobal.device_state==0xff||temp == 0)
			{
			return SHMETRO_CE_OK;
			}
		}

	temp = Readcard();
	if(temp == SHMETRO_CE_INVALIDCARD||temp == SHMETRO_CE_NOPSAM||temp == SHMETRO_CE_PSAMERR||temp == 0x23)
		{gUserGlobal.device_state = gUserGlobal.device_state&0xe0;
		return temp;
		}
	}
return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.6
=========================================================================================*/
void cmd_SHMETRO_OT_READCARD_output(UBYTE *obuf,UBYTE *obytes)
{
	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	if((p->cmd)!=0x30)
		{return;}
	if(gUserGlobal.Cpuflag == 0x02&&gUserGlobal.TagType==2)
		{obuf[0] = p->cmd;
		obuf[1] = p->cmd_retry;
		obuf[2] = SHMETRO_CE_OK;
		obuf[3] = 0x91;
		obuf[4] = gUserGlobal.CardSnr[0];//应用序列号后4字节
		obuf[5] = gUserGlobal.CardSnr[1];
		obuf[6] = gUserGlobal.CardSnr[2];
		obuf[7] = gUserGlobal.CardSnr[3];
		obuf[8] = gUserGlobal.ADF1_file0015[12];//应用序列号前4字节
		obuf[9] = gUserGlobal.ADF1_file0015[13];
		obuf[10] = gUserGlobal.ADF1_file0015[14];
		obuf[11] = gUserGlobal.ADF1_file0015[15];
		obuf[12] = gUserGlobal.CityNo[0];		//城市代码
		obuf[13] = gUserGlobal.CityNo[1];
		obuf[14] = gUserGlobal.CardType;//卡类型低字节
		gUserGlobal.metro_prot.send_bytes=15;
		gUserGlobal.Cpuflag = 0x00;
		return ;
		}
	gUserGlobal.Cpuflag = 0x00;
	obuf[0] = p->cmd;
	obuf[1] = p->cmd_retry;
	obuf[2] = SHMETRO_CE_OK;
	if(gUserGlobal.TagType==2)		//cpu 卡
		{obuf[3] = 0x11;
		obuf[4] = gUserGlobal.ADF1_file0015[16];//应用序列号后4字节
		obuf[5] = gUserGlobal.ADF1_file0015[17];
		obuf[6] = gUserGlobal.ADF1_file0015[18];
		obuf[7] = gUserGlobal.ADF1_file0015[19];
		obuf[8] = gUserGlobal.CityNo[0];		//城市代码
		obuf[9] = gUserGlobal.CityNo[1];
		obuf[10] = gUserGlobal.CardType;//卡类型低字节
		obuf[11] = gUserGlobal.ADF1_file0015[24];//有效期
		obuf[12] = gUserGlobal.ADF1_file0015[25];
		obuf[13] = gUserGlobal.ADF1_file0015[26];
		obuf[14] = gUserGlobal.ADF1_file0015[27];
		obuf[15] = gUserGlobal.ADF1_Balance[3];	//余额
		obuf[16] = gUserGlobal.ADF1_Balance[2];
		obuf[17] = gUserGlobal.ADF1_Balance[1];
		obuf[18] = gUserGlobal.ADF1_Balance[0];
		obuf[19] = gUserGlobal.ADF1_CardOverDraft[3];//透支限额
		obuf[20] = gUserGlobal.ADF1_CardOverDraft[2];
		obuf[21] = gUserGlobal.ADF1_CardOverDraft[1];
		obuf[22] = 0x02;
		obuf[23] = gUserGlobal.ADF1_Cnt[0];//计数器
		obuf[24] = gUserGlobal.ADF1_Cnt[1];
		obuf[25] = gUserGlobal.ADF1_file0015[9];//0015文件应用版本
		memcpy(obuf+26,gUserGlobal.Huancheng+3,10);
		obuf[36] = gUserGlobal.Metrosta;
		obuf[37] = gUserGlobal.ADF1_file0015[12];//应用序列号前4字节
		obuf[38] = gUserGlobal.ADF1_file0015[13];
		obuf[39] = gUserGlobal.ADF1_file0015[14];
		obuf[40] = gUserGlobal.ADF1_file0015[15];
		obuf[41] = gUserGlobal.ADF1_Algorithm;
		memcpy(obuf+42,gUserGlobal.ADF1_file0015+20,4);//应用启动日期
		obuf[46] = gUserGlobal.ADF1_file0015[29];//押金
//		if(gUserGlobal.ADF1_file0015[8]&0x80==0x80)
//			{obuf[47] = 0x01;}
		obuf[47] = gUserGlobal.ADF1_file0015[8];
		gUserGlobal.metro_prot.send_bytes=48;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
//*******************************
//block 2
//*******************************
		obuf[0] = p->cmd;
		obuf[1] = p->cmd_retry;
		obuf[2] = SHMETRO_CE_OK;
		obuf[3] = 0x12;
		memcpy(obuf+4,gUserGlobal.B32,16);
		gUserGlobal.metro_prot.send_bytes=20;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
//*******************************
//block 3
//*******************************
		obuf[0] = p->cmd;
		obuf[1] = p->cmd_retry;
		obuf[2] = SHMETRO_CE_OK;
		obuf[3] = 0x93;
		memcpy(obuf+4,gUserGlobal.B33,16);
		gUserGlobal.metro_prot.send_bytes=20;
//		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//		prot_shmetro_send_process(&(p->metro_prot));
//		gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
		}
	else if(gUserGlobal.TagType < 2)//m1
		{obuf[3] = 0x01;
		obuf[4] = gUserGlobal.CardSnr[0];//序列号
		obuf[5] = gUserGlobal.CardSnr[1];
		obuf[6] = gUserGlobal.CardSnr[2];
		obuf[7] = gUserGlobal.CardSnr[3];
		obuf[8] = gUserGlobal.CityNo[0];		//城市代码
		obuf[9] = gUserGlobal.CityNo[1];
		obuf[10] = gUserGlobal.CardType;//卡类型低字节
		obuf[11] = gUserGlobal.B8[12];//有效期
		obuf[12] = gUserGlobal.B8[13];
		obuf[13] = gUserGlobal.B8[14];
		obuf[14] = gUserGlobal.B8[15];
		obuf[15] = gUserGlobal.BalanceL;	//余额 低字节在前
		obuf[16] = gUserGlobal.BalanceM;
		obuf[17] = gUserGlobal.BalanceH;
		obuf[18] = 0x02;//卡状态??
		obuf[19] = gUserGlobal.M1CardCnt[0];//卡计数器
		obuf[20] = gUserGlobal.M1CardCnt[1];
		obuf[21] = gUserGlobal.Cardversion;//卡版本
		memcpy(obuf+22,gUserGlobal.Huancheng+3,10);
		obuf[32] = 0x00;
		obuf[33] = 0x00;
		if(gUserGlobal.Metrosta==0xcc||gUserGlobal.Metrosta==0xdd)
			{obuf[34]=gUserGlobal.Metrosta;
			obuf[35]=0;
			}
		else
			{obuf[34]=0;
			obuf[35]=1;
			}
		gUserGlobal.metro_prot.send_bytes=36;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
//		gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
//*******************************
//block 2 bom
//*******************************
		obuf[0] = p->cmd;
		obuf[1] = p->cmd_retry;
		obuf[2] = SHMETRO_CE_OK;
		obuf[3] = 0x02;
		memcpy(obuf+4,gUserGlobal.B32,16);
		gUserGlobal.Amountyear=obuf[5];
		gUserGlobal.Amountmonth=obuf[6];
		gUserGlobal.metro_prot.send_bytes=20;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
//*******************************
//block 2 bom
//*******************************
		obuf[0] = p->cmd;
		obuf[1] = p->cmd_retry;
		obuf[2] = SHMETRO_CE_OK;
		obuf[3] = 0x83;
		memcpy(obuf+4,gUserGlobal.B33,16);
		gUserGlobal.metro_prot.send_bytes=20;
		}

	else if(gUserGlobal.TagType	== 0x03)
		{obuf[0] = 0xc0;
		obuf[3] = 0x81;
		obuf[4] = gUserGlobal.Sjtbl0[3];//制造商代码
		obuf[5] = gUserGlobal.Sjtbl0[2];
		obuf[6] = gUserGlobal.CardSnr[3];
		obuf[7] = gUserGlobal.CardSnr[2];
		obuf[8] = gUserGlobal.CardSnr[1];
		obuf[9] = gUserGlobal.CardSnr[0];
		obuf[10]= gUserGlobal.CardType;
		obuf[11]= gUserGlobal.BalanceL;
		obuf[12]= gUserGlobal.BalanceM;
		obuf[13]= gUserGlobal.Metrosta;
		obuf[14]= gUserGlobal.SjtCount[1];
		obuf[15]= gUserGlobal.SjtCount[0];
		memcpy(obuf+16,gUserGlobal.Last,7);
		obuf[23]= gUserGlobal.Sjtbl6[3];//售票时间
		obuf[24]= gUserGlobal.Sjtbl6[2];
		obuf[25]= gUserGlobal.Sjtbl6[1];
		obuf[26]= gUserGlobal.Sjtbl6[0];
		obuf[27]= gUserGlobal.Sjtbl5[2];//售票设备
		obuf[28]= gUserGlobal.Sjtbl5[1];
		obuf[29]= gUserGlobal.Sjtbl5[0];
		obuf[30]= gUserGlobal.Sjtbl7[3];
		obuf[31]= gUserGlobal.Sjtbl7[2];
		gUserGlobal.Operater[0] = gUserGlobal.Sjtbl7[2];
		gUserGlobal.Operater[1] = gUserGlobal.Sjtbl7[3];
		temp = gUserGlobal.device_state&0xe0;
		if(temp==0xc0)			//tvm mode
			{gUserGlobal.metro_prot.send_bytes=32;}
		else{
			if(gUserGlobal.ReadMode==0x00||gUserGlobal.ReadMode==0x01)
				{gUserGlobal.metro_prot.send_bytes=32;}
			else{
				obuf[32]=gUserGlobal.Sjtbl14[3];
				obuf[33]=gUserGlobal.Sjtbl14[2];
				obuf[34]=gUserGlobal.Sjtbl14[1];
				obuf[35]=gUserGlobal.Sjtbl14[0];
				gUserGlobal.metro_prot.send_bytes=36;
				}
			}
		}
	else if(gUserGlobal.TagType	== 0x04)
		{obuf[0] = 0x11;
		obuf[3] = 0x01;
		memcpy(obuf+4,gUserGlobal.MT_UID,14);//主帐号10byte,主帐号序列号1byte,发卡行代码 2byte,卡业务类型
		memcpy(obuf+18,gUserGlobal.Pboc_0015+24,4);//有效期
		obuf[22] = gUserGlobal.BalanceL;	//余额 低字节在前
		obuf[23] = gUserGlobal.BalanceM;
		obuf[24] = gUserGlobal.BalanceH;
		obuf[25] = gUserGlobal.BalanceHH;
		obuf[26] = gUserGlobal.ADF1_CardOverDraft[3];
		obuf[27] = gUserGlobal.ADF1_CardOverDraft[2];
		obuf[28] = gUserGlobal.ADF1_CardOverDraft[1];
		gUserGlobal.metro_prot.send_bytes=29;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
//		gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
//*******************************
//block 2
//*******************************
		obuf[0] = 0x11;
		obuf[1] = p->cmd_retry;
		obuf[2] = SHMETRO_CE_OK;
		obuf[3] = 0x82;
		memcpy(obuf+4,gUserGlobal.Pboc_sec8+6,41);
		memcpy(obuf+45,gUserGlobal.Huancheng+3,5);
		memcpy(obuf+50,gUserGlobal.AID,17);
		temp = obuf[38];
		obuf[38] = obuf[40];
		obuf[40] = temp;
		temp = obuf[43];
		obuf[43] = obuf[44];
		obuf[44] = temp;
		obuf[67] = 0x00;
		obuf[68] = 0x00;
		obuf[69] = 0x00;
		gUserGlobal.metro_prot.send_bytes=70;
		}
	else if(gUserGlobal.TagType	== 0x05)
		{
		obuf[0] = 0x1f;
		obuf[3] = 0x01;
		memcpy(obuf+4,gUserGlobal.metro0015+16,4);//
		memcpy(obuf+8,gUserGlobal.metro0015+10,10);
		obuf[18] = gUserGlobal.metro0015[28];//cardtype
		memcpy(obuf+19,gUserGlobal.metro0015+24,4);//
		obuf[23] = gUserGlobal.ADF1_Balance[3];
		obuf[24] = gUserGlobal.ADF1_Balance[2];
		obuf[25] = gUserGlobal.ADF1_Balance[1];
		obuf[26] = gUserGlobal.ADF1_Balance[0];
		memcpy(obuf+27,gUserGlobal.metro0006+12,19);	//范围标志1.线路位图6.车站位图12
		gUserGlobal.metro_prot.send_bytes=46;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
//		gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
//*******************************
//block 2
//*******************************
		obuf[0] = 0x1f;
		obuf[1] = p->cmd_retry;
		obuf[2] = SHMETRO_CE_OK;
		obuf[3] = 0x82;
		memcpy(obuf+4,gUserGlobal.metro0019+3,41);
		temp = obuf[38];
		obuf[38] = obuf[40];
		obuf[40] = temp;
		temp = obuf[43];
		obuf[43] = obuf[44];
		obuf[44] = temp;
		obuf[45] = 0x00;
		obuf[46] = 0x00;
		obuf[47] = 0x00;
		obuf[48] = 0x00;
		gUserGlobal.metro_prot.send_bytes=49;
		}
	else
		{gUserGlobal.device_state = gUserGlobal.device_state&0xe0;
		obuf[0] = p->cmd;
		obuf[1] = p->cmd_retry;
		obuf[2] = SHMETRO_CE_INVALIDCARD;
		gUserGlobal.metro_prot.send_bytes=3;
		return ;
		}
	gUserGlobal.device_state = (gUserGlobal.device_state&0xe0)+2;
	return;
}





/*=======================================================================================
函数：
功能：2.7
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_ABORT_input(UBYTE *ibuf,UBYTE ibytes)
{

return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.7
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_ABORT_proc(void)
{
	if(gUserGlobal.device_state == 0xff)
		{return SHMETRO_CE_STATUSERROR;}
	return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.7
=========================================================================================*/
void cmd_SHMETRO_OT_ABORT_output(void)
{
USER_GLOBAL *p = &gUserGlobal;
//
gUserGlobal.metro_prot.send_buf[0] = p->cmd;
gUserGlobal.metro_prot.send_buf[1] = p->cmd_retry;
gUserGlobal.metro_prot.send_buf[2] = SHMETRO_CE_OK;
gUserGlobal.metro_prot.send_buf[3] = gUserGlobal.device_state;
gUserGlobal.metro_prot.send_bytes = 4;
//shmetro_enter_state(SHMETRO_IDLE_STATE);
	gUserGlobal.device_state = gUserGlobal.device_state&0xe0;
//
return;
}


/*=======================================================================================
函数：
功能：2.8
=========================================================================================*/
//UBYTE cmd_SHMETRO_ST_ENTRY_input(UBYTE *ibuf,UBYTE ibytes)
//{
//	USER_GLOBAL *p = &gUserGlobal;
//	UBYTE temp;
//	gUserGlobal.Decval=0;
//	gUserGlobal.TransValue[2]=0;
//	gUserGlobal.TransValue[3]=0;
//	if(gUserGlobal.device_state == 0x22)
//		{gUserGlobal.Err17Flag = 0;
//		gUserGlobal.Psw=ibuf[10];
//		temp = Chk_psw();
//		if(temp !=0)
//			{return SHMETRO_CE_BADPARAM;}
//		else
//			{gUserGlobal.Psw=ibuf[10];
//			gUserGlobal.Date[0]=ibuf[5];
//			gUserGlobal.Date[1]=ibuf[4];
//			gUserGlobal.Date[2]=ibuf[3];
//			gUserGlobal.Date[3]=ibuf[2];
//			gUserGlobal.PosCnt[0]=ibuf[9];
//			gUserGlobal.PosCnt[1]=ibuf[8];
//			gUserGlobal.PosCnt[2]=ibuf[7];
//			gUserGlobal.PosCnt[3]=ibuf[6];
////			gUserGlobal.Sjtremainl=ibuf[11];
////			gUserGlobal.Sjtremainh=ibuf[12];
////			Ins=ibuf[13];
//			return 0;
//			}
//		}
//	else
//		{return SHMETRO_CE_STATUSERROR;}
//}

/*=======================================================================================
函数：
功能：2.8
=========================================================================================*/
//UBYTE cmd_SHMETRO_ST_ENTRY_proc(void)
//{	USER_GLOBAL *p = &gUserGlobal;
//	UBYTE temp,rdbuf[16];
//	if(gUserGlobal.Sjtbkflag==0)
//		{gUserGlobal.Sjtbl15[0]=gUserGlobal.Metrosta;
//		gUserGlobal.Sjtbl15[1]=gUserGlobal.BalanceM;
//		gUserGlobal.Sjtbl15[2]=gUserGlobal.BalanceL;
//		gUserGlobal.Sjtbl15[3]=gUserGlobal.Metrosta^gUserGlobal.BalanceM^gUserGlobal.BalanceL;
//		temp = Sjt_Write(gUserGlobal.Sjtbl15,0x0f);
//		if(temp!=0)
//			{return SHMETRO_CE_WRITEERROR;}
//		}
//	memcpy(rdbuf,gUserGlobal.Date,4);
//	temp = Sjt_Write(rdbuf,10);
//	if(temp!=0)
//		{return SHMETRO_CE_WRITEERROR;}
//	rdbuf[4] = gUserGlobal.Posid[0];
//	rdbuf[5] = gUserGlobal.Posid[1];
//	rdbuf[6] = gUserGlobal.Posid[3];
//	lrc_gen2(rdbuf,7);
//	temp = Sjt_Write(rdbuf+4,11);
//	if(temp!=0)
//		{return SHMETRO_CE_WRITEERROR;}
//	rdbuf[0] = gUserGlobal.Psw;
//	rdbuf[1] = (gUserGlobal.CardCount+1)/0x100;
//	rdbuf[2] = (gUserGlobal.CardCount+1)%0x100;
//	rdbuf[3] = rdbuf[0]^rdbuf[1]^rdbuf[2];
//	Backupsjt();
//	temp = Sjt_Write(rdbuf,9);
//	if(temp!=0)
//		{return SHMETRO_CE_WRITEERROR;}
//	gUserGlobal.Err17Flag = 0;
//
//	sam_select(SJT_SAM);
//	temp = Sjt_Tac();
//	return SHMETRO_CE_OK;
//}

/*=======================================================================================
函数：
功能：2.8
=========================================================================================*/
//void cmd_SHMETRO_ST_ENTRY_output(UBYTE *obuf,UBYTE *obytes)
//{
//USER_GLOBAL *p = &gUserGlobal;
////
//obuf[0] = p->cmd;
//obuf[1] = p->cmd_retry;
//obuf[2] = SHMETRO_CE_OK;
//obuf[3] = p->SjtCount[1];
//obuf[4] = p->SjtCount[0];
//memcpy(obuf+5,gUserGlobal.Tac,4);
//
//*obytes = 9;
////
//return;
//}



/*=======================================================================================
函数：
功能：2.10
=========================================================================================*/
//UBYTE cmd_SHMETRO_ST_EXIT_input(UBYTE *ibuf,UBYTE ibytes)
//{
//	USER_GLOBAL *p = &gUserGlobal;
//	UBYTE temp;
////	gUserGlobal.Para1 = 0;
//	gUserGlobal.Psw=ibuf[14];
//	temp = Chk_psw();
//	if(temp!=0)
//		{return SHMETRO_CE_BADPARAM;}
//	if(gUserGlobal.device_state==0x42)
//		{gUserGlobal.Err17Flag = 0;
//		gUserGlobal.Psw=ibuf[14];
//		gUserGlobal.Date[0]=ibuf[5];//交易日期时间
//		gUserGlobal.Date[1]=ibuf[4];
//		gUserGlobal.Date[2]=ibuf[3];
//		gUserGlobal.Date[3]=ibuf[2];
//		gUserGlobal.PosCnt[0]=ibuf[9];//交易流水号
//		gUserGlobal.PosCnt[1]=ibuf[8];
//		gUserGlobal.PosCnt[2]=ibuf[7];
//		gUserGlobal.PosCnt[3]=ibuf[6];
//		gUserGlobal.TransValue[0] = 0x00;
//		gUserGlobal.TransValue[1] = 0x00;
//		gUserGlobal.TransValue[2] = ibuf[11];//交易金额高字节，通讯中低字节先传
//		gUserGlobal.TransValue[3] = ibuf[10];//交易金额低字节
//
//		gUserGlobal.Sjtremainl=ibuf[12];
//		gUserGlobal.Sjtremainh=ibuf[13];
//		gUserGlobal.Para2=ibuf[15];
//
//		return 0;
//		}
//	else
//		{return SHMETRO_CE_STATUSERROR;}
//}

/*=======================================================================================
函数：
功能：2.10
=========================================================================================*/
//UBYTE cmd_SHMETRO_ST_EXIT_proc(void)
//{	USER_GLOBAL *p = &gUserGlobal;
//	UBYTE temp,Rdbuf[16];
//
//	temp=Sjt_backup();
//	if(temp!=0)
//		{return SHMETRO_CE_WRITEERROR;}
//
//	Rdbuf[0] = gUserGlobal.Date[0];
//	Rdbuf[1] = gUserGlobal.Date[1];
//	Rdbuf[2] = gUserGlobal.Date[2];
//	Rdbuf[3] = gUserGlobal.Date[3];
//	temp = Sjt_Write(Rdbuf,12);
//	if(temp!=0)
//		{return SHMETRO_CE_WRITEERROR;}
//
//	memcpy(Rdbuf+4,gUserGlobal.Sjtbl14,4);
//	gUserGlobal.Para2 = gUserGlobal.Para2&0x01;
//	if(gUserGlobal.Para2==0x00)
//		{Rdbuf[4] = gUserGlobal.TransValue[2];
//		Rdbuf[5] = gUserGlobal.TransValue[3];
//		temp=Sjt_Write(Rdbuf+4,14);
//		if(temp!=0)
//			{return SHMETRO_CE_WRITEERROR;}
//		}
//	Rdbuf[8] = gUserGlobal.Posid[0];
//	Rdbuf[9] = gUserGlobal.Posid[1];
//	Rdbuf[10] = gUserGlobal.Posid[3];
//	lrc_gen2(Rdbuf,11);
//	temp=Sjt_Write(Rdbuf+8,13);
//	if(temp!=0)
//		{return SHMETRO_CE_WRITEERROR;}
////if(Sjtbkflag==0)
////	{
////	}
//	Rdbuf[0] = gUserGlobal.Sjtremainh;
//	Rdbuf[1] = gUserGlobal.Sjtremainl;
//	Rdbuf[2] = gUserGlobal.Operater[0];
//	Rdbuf[3] = gUserGlobal.Operater[1];
//	temp=Sjt_Write(Rdbuf,7);
//	if(temp!=0)
//		{return SHMETRO_CE_WRITEERROR;}
//
//	Backupsjt();
//	Rdbuf[0] = gUserGlobal.Psw;
//	Rdbuf[1] =(gUserGlobal.CardCount+1)/0x100;
//	Rdbuf[2] =(gUserGlobal.CardCount+1)%0x100;
//	Rdbuf[3] =Rdbuf[0]^Rdbuf[1]^Rdbuf[2];
//	temp=tocken_write(9,Rdbuf);
//	if(temp!=0)
//		{return SHMETRO_CE_WRITEERROR;}
//	gUserGlobal.Err17Flag=0;
//	sam_select(SJT_SAM);
//	temp = Sjt_Tac();
//	return SHMETRO_CE_OK;
//}

/*=======================================================================================
函数：
功能：2.10
=========================================================================================*/
//void cmd_SHMETRO_ST_EXIT_output(UBYTE *obuf,UBYTE *obytes)
//{
//USER_GLOBAL *p = &gUserGlobal;
////
//obuf[0] = p->cmd;
//obuf[1] = p->cmd_retry;
//obuf[2] = SHMETRO_CE_OK;
//obuf[3] = p->SjtCount[1];
//obuf[4] = p->SjtCount[0];
//obuf[5] = p->Sjtremainl;
//obuf[6] = p->Sjtremainh;
//memcpy(obuf+7,gUserGlobal.Tac,4);
//*obytes = 11;
////
//return;
//}


/*=======================================================================================
函数：
功能：2.13
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_ADDVALUE_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;
}

/*=======================================================================================
函数：
功能：2.13
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_ADDVALUE_proc(void)
{
return SHMETRO_CE_BADREQUEST;
}

/*=======================================================================================
函数：
功能：2.13
=========================================================================================*/
void cmd_SHMETRO_ST_ADDVALUE_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_UNKNOWN;
*obytes = 3;
//
return;
}


/*=======================================================================================
函数：
功能：2.11
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_SALECARD_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp=gUserGlobal.device_state&0x1f;
	if(temp == 0x02)
		{gUserGlobal.Psw = ibuf[12];
		temp = Chk_psw();
		if(temp!=0)
			{return SHMETRO_CE_BADPARAM;}
		else
			{gUserGlobal.Psw = ibuf[12];
			gUserGlobal.Date[0] = ibuf[5];
			gUserGlobal.Date[1] = ibuf[4];
			gUserGlobal.Date[2] = ibuf[3];
			gUserGlobal.Date[3] = ibuf[2];
			gUserGlobal.PosCnt[0] = ibuf[9];
			gUserGlobal.PosCnt[1] = ibuf[8];
			gUserGlobal.PosCnt[2] = ibuf[7];
			gUserGlobal.PosCnt[3] = ibuf[6];
			gUserGlobal.Operater[1] = ibuf[10];
			gUserGlobal.Operater[0] = ibuf[11];
			gUserGlobal.CardType = ibuf[13];
			gUserGlobal.TransValue[3] = ibuf[14];
			gUserGlobal.TransValue[2] = ibuf[15];
			gUserGlobal.Err17Flag = 0;
			return 0;
			}
		}
	else
		{return SHMETRO_CE_STATUSERROR;}
}

/*=======================================================================================
函数：
功能：2.11
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_SALECARD_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,Rdbuf[16];
	temp=Sjt_backup();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	temp=Sjt_clrblk();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	memcpy(Rdbuf,gUserGlobal.Date,4);
	temp=Sjt_Write(Rdbuf,6);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}

	Rdbuf[0] = gUserGlobal.Posid[0];
	Rdbuf[1] = gUserGlobal.Posid[1];
	Rdbuf[2] = gUserGlobal.Posid[3];
	Rdbuf[3] = gUserGlobal.CardType;
	temp=Sjt_Write(Rdbuf,5);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}

	Rdbuf[0] = gUserGlobal.TransValue[2];
	Rdbuf[1] = gUserGlobal.TransValue[3];
	Rdbuf[2] = gUserGlobal.Operater[0];
	Rdbuf[3] = gUserGlobal.Operater[1];
	temp=Sjt_Write(Rdbuf,7);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	Backupsjt();
	Rdbuf[0] = gUserGlobal.Psw;
	Rdbuf[1] = (gUserGlobal.CardCount+1)/0x100;
	Rdbuf[2] = (gUserGlobal.CardCount+1)%0x100;
	Rdbuf[3] = Rdbuf[0]^Rdbuf[1]^Rdbuf[2];
	temp=tocken_write(9,Rdbuf);
	if(temp!=0)
		{return SHMETRO_CE_PUTBACKCARD1;}
	gUserGlobal.Err17Flag=0;
	sam_select(SJT_SAM);
	temp = Sjt_Tac();
	return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.11
=========================================================================================*/
void cmd_SHMETRO_ST_SALECARD_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
obuf[3] = p->SjtCount[1];
obuf[4] = p->SjtCount[0];
obuf[5] = p->TransValue[3];
obuf[6] = p->TransValue[2];
memcpy(obuf+7,gUserGlobal.Tac,4);
*obytes = 11;
return;
}


/*=======================================================================================
函数：
功能：2.12
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_UPDATE_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
//	temp=gUserGlobal.device_state&0x1f;
	if(gUserGlobal.device_state == 0x62)
		{
		if(ibuf[2]>3)
			{return SHMETRO_CE_BADPARAM;}
		gUserGlobal.Psw = ibuf[3];
		temp = Chk_psw();
		if(temp!=0)
			{return SHMETRO_CE_BADPARAM;}
		gUserGlobal.Err17Flag = 0;
		gUserGlobal.Para1 = ibuf[2];		//更新模式
		gUserGlobal.Psw = ibuf[3];
		gUserGlobal.Station[0] = ibuf[4];		//线路编码
		gUserGlobal.Station[1] = ibuf[5];		//车站代码
		gUserGlobal.Station[2] = ibuf[6];		//设备
		gUserGlobal.Date[0] = ibuf[10];
		gUserGlobal.Date[1] = ibuf[9];
		gUserGlobal.Date[2] = ibuf[8];
		gUserGlobal.Date[3] = ibuf[7];
		gUserGlobal.PosCnt[0] = ibuf[14];
		gUserGlobal.PosCnt[1] = ibuf[13];
		gUserGlobal.PosCnt[2] = ibuf[12];
		gUserGlobal.PosCnt[3] = ibuf[11];
		gUserGlobal.TransValue[3] = ibuf[15];
		gUserGlobal.TransValue[2] = ibuf[16];
		gUserGlobal.Sjtremainl = ibuf[17];
		gUserGlobal.Sjtremainh = ibuf[18];
		gUserGlobal.Operater[1] = ibuf[19];
		gUserGlobal.Operater[0] = ibuf[20];
		gUserGlobal.Para2 = ibuf[21];
		return 0;

		}
	else
		{return SHMETRO_CE_STATUSERROR;}
}


UBYTE Sjt_uplog(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,Rdbuf[16];
	temp=gUserGlobal.Para2&0x01;
	if(temp==0)
		{temp=tocken_read(7,Rdbuf);
		if(temp!=0)
			{return 1;}
		Rdbuf[2]=gUserGlobal.Operater[0];
		Rdbuf[3]=gUserGlobal.Operater[1];
		temp=Sjt_Write(Rdbuf,0x07);
		if(temp!=0)
			{return 1;}
		}
	temp=gUserGlobal.Para2&0x04;
	if(temp==0x04)
		{Rdbuf[0]=gUserGlobal.Date[0];
		Rdbuf[1]=gUserGlobal.Date[1];
		Rdbuf[2]=gUserGlobal.Date[2];
		Rdbuf[3]=gUserGlobal.Date[3];
		temp=Sjt_Write(Rdbuf,0x0e);
		if(temp!=0)
			{return 1;}
		temp=tocken_read(0x0c,Rdbuf+4);
		if(temp!=0)
			{return 1;}
		temp=tocken_read(0x0d,Rdbuf+8);
		if(temp!=0)
			{return 1;}
		lrc_gen2(Rdbuf,11);
		temp=Sjt_Write(Rdbuf+8,0x0d);
		return temp;
		}
	temp=gUserGlobal.Para2&0x02;
	if(temp==0)
		{temp=tocken_read(0x0e,Rdbuf);
		if(temp!=0)
			{return 1;}
		Rdbuf[0]=gUserGlobal.TransValue[2];
		Rdbuf[1]=gUserGlobal.TransValue[3];
		if((gUserGlobal.Para1==0x02)&&(gUserGlobal.TransValue[2]==0x00)&&(gUserGlobal.TransValue[3]==0x00))
			{Rdbuf[2]=0x00;
			Rdbuf[3]=0x00;
			}
		temp=Sjt_Write(Rdbuf,0x0e);
		if(temp!=0)
			{return 1;}
		temp=tocken_read(0x0c,Rdbuf+4);
		if(temp!=0)
			{return 1;}
		temp=tocken_read(0x0d,Rdbuf+8);
		if(temp!=0)
			{return 1;}
		lrc_gen2(Rdbuf,11);
		temp=Sjt_Write(Rdbuf+8,0x0d);
		return temp;
		}
	return 0;
}
/*=======================================================================================
函数：
功能：2.12
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_UPDATE_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,Rdbuf[16];
	temp=Sjt_backup();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}

	memcpy(Rdbuf,gUserGlobal.Sjtbl7,4);
	Rdbuf[0] = gUserGlobal.Sjtremainh;
	Rdbuf[1] = gUserGlobal.Sjtremainl;
	temp=Sjt_Write(Rdbuf,0x07);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}

	if(gUserGlobal.Para1==0)
sjt_up0:
		{temp=Sjt_uplog();
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		Rdbuf[0] = gUserGlobal.Psw;
		Rdbuf[1] = (gUserGlobal.CardCount+1)/0x100;
		Rdbuf[2] = (gUserGlobal.CardCount+1)%0x100;
		Rdbuf[3] = Rdbuf[0]^Rdbuf[1]^Rdbuf[2];
		Backupsjt();
		temp = Sjt_Write(Rdbuf,9);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		gUserGlobal.Err17Flag = 0;
		sam_select(SJT_SAM);
		temp = Sjt_Tac();
		return SHMETRO_CE_OK;
		}
	else if(gUserGlobal.Para1==0x01)			//sale record
		{Rdbuf[0] = gUserGlobal.Station[0];
		Rdbuf[1] = gUserGlobal.Station[1];
		Rdbuf[2] = gUserGlobal.Station[2];
		Rdbuf[3] = gUserGlobal.CardType;
		temp=Sjt_Write(Rdbuf,5);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		Rdbuf[0] = gUserGlobal.Date[0];
		Rdbuf[1] = gUserGlobal.Date[1];
		Rdbuf[2] = gUserGlobal.Date[2];
		Rdbuf[3] = gUserGlobal.Date[3];
		temp=Sjt_Write(Rdbuf,6);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		goto sjt_up0;
		}
	else if(gUserGlobal.Para1 == 0x02)		//entry record
		{Rdbuf[0] = gUserGlobal.Date[0];
		Rdbuf[1] = gUserGlobal.Date[1];
		Rdbuf[2] = gUserGlobal.Date[2];
		Rdbuf[3] = gUserGlobal.Date[3];
		temp=Sjt_Write(Rdbuf,10);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		Rdbuf[4] = gUserGlobal.Station[0];
		Rdbuf[5] = gUserGlobal.Station[1];
		Rdbuf[6] = gUserGlobal.Station[2];
		lrc_gen2(Rdbuf,0x07);
		temp=Sjt_Write(Rdbuf+4,11);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		goto sjt_up0;
		}
	else
		{Rdbuf[0] = gUserGlobal.Date[0];
		Rdbuf[1] = gUserGlobal.Date[1];
		Rdbuf[2] = gUserGlobal.Date[2];
		Rdbuf[3] = gUserGlobal.Date[3];
		temp=Sjt_Write(Rdbuf,12);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		temp=tocken_read(14,Rdbuf+4);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		Rdbuf[8] = gUserGlobal.Station[0];
		Rdbuf[9] = gUserGlobal.Station[1];
		Rdbuf[10] = gUserGlobal.Station[2];
		lrc_gen2(Rdbuf,11);
		temp=Sjt_Write(Rdbuf+8,13);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		goto sjt_up0;
		}
}

/*=======================================================================================
函数：
功能：2.12
=========================================================================================*/
void cmd_SHMETRO_ST_UPDATE_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
obuf[3] = p->SjtCount[1];
obuf[4] = p->SjtCount[0];
obuf[5] = p->Sjtremainl;
obuf[6] = p->Sjtremainh;
memcpy(obuf+7,gUserGlobal.Tac,4);
*obytes = 11;
//
return;
}


/*=======================================================================================
函数：
功能：2.14
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_EXPIREDATE_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;
}

/*=======================================================================================
函数：
功能：2.14
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_EXPIREDATE_proc(void)
{
return SHMETRO_CE_BADREQUEST;
}

/*=======================================================================================
函数：
功能：2.14
=========================================================================================*/
void cmd_SHMETRO_ST_EXPIREDATE_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_UNKNOWN;
*obytes = 3;
//
return;
}





/*=======================================================================================
函数：
功能：2.15
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_READRECORD_input(UBYTE *ibuf,UBYTE ibytes)
{
//	USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state==0xff)
		{return SHMETRO_CE_STATUSERROR;}
	if(ibuf[2]>7||ibuf[2] == 0)
		{return SHMETRO_CE_BADPARAM;}
	gUserGlobal.Para1 = ibuf[2];
	return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.15
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_READRECORD_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;

	temp=tocken_read(5,gUserGlobal.Sjtbl5);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}
	temp=tocken_read(6,gUserGlobal.Sjtbl6);//(Rdbuf+4,0x06);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}
	temp=tocken_read(7,gUserGlobal.Sjtbl7);//(Rdbuf+8,0x07);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}
	temp=tocken_read(10,gUserGlobal.Sjtbl10);//(Rdbuf+12,0x0a);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}
	temp=tocken_read(11,gUserGlobal.Sjtbl11);//(Rdbkbuf,0x0b);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}
	temp=tocken_read(12,gUserGlobal.Sjtbl12);//(Rdbkbuf+4,0x0c);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}
	temp=tocken_read(13,gUserGlobal.Sjtbl13);//(Rdbkbuf+8,0x0d);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}
	temp=tocken_read(14,gUserGlobal.Sjtbl14);//(Rdbkbuf+12,0x0e);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}

	return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：2.15
=========================================================================================*/
void cmd_SHMETRO_ST_READRECORD_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;
	UBYTE len;
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;


	switch(gUserGlobal.Para1)
		{case 0x01:
			memcpy(obuf+3,gUserGlobal.Sjtbl5,4);
			memcpy(obuf+7,gUserGlobal.Sjtbl6,4);
			memcpy(obuf+11,gUserGlobal.Sjtbl7,4);
			len = 15;
			break;
		case 0x02:
			memcpy(obuf+3,gUserGlobal.Sjtbl10,4);
			memcpy(obuf+7,gUserGlobal.Sjtbl11,4);
			len = 11;
			break;
		case 0x03:
			memcpy(obuf+3,gUserGlobal.Sjtbl5,4);
			memcpy(obuf+7,gUserGlobal.Sjtbl6,4);
			memcpy(obuf+11,gUserGlobal.Sjtbl7,4);
			memcpy(obuf+15,gUserGlobal.Sjtbl10,4);
			memcpy(obuf+19,gUserGlobal.Sjtbl11,4);
			len = 23;
			break;
		case 0x04:
			memcpy(obuf+3,gUserGlobal.Sjtbl12,4);
			memcpy(obuf+7,gUserGlobal.Sjtbl13,4);
			memcpy(obuf+11,gUserGlobal.Sjtbl4,4);
			len = 15;
			break;
		case 0x05:
			memcpy(obuf+3,gUserGlobal.Sjtbl5,4);
			memcpy(o                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      