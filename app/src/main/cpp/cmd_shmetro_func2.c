//cmd_shmetro_func2.c

#ifndef _CMD_SHMETRO_FUNC2_C_
#define _CMD_SHMETRO_FUNC2_C_
//start of file

#include <string.h>
#include "shmetro_common.h"
#include "user_global.h"
#include "shmetro_def.h"

/*=======================================================================================
函数：通讯
功能：0.2 发送测试信息
=========================================================================================*/

void send_log(UBYTE recNo,UBYTE len,UBYTE *inbuf)
{
	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	gUserGlobal.metro_prot.send_buf[0] = p->cmd;
	gUserGlobal.metro_prot.send_buf[1] = p->cmd_retry;
	gUserGlobal.metro_prot.send_buf[2] = SHMETRO_CE_OK;
	gUserGlobal.metro_prot.send_buf[3] = recNo;
	memcpy(gUserGlobal.metro_prot.send_buf+4,inbuf,len);

	gUserGlobal.metro_prot.send_bytes = len+4;
	temp = recNo&0x80;
	if(temp == 0x80)
		{gUserGlobal.metro_prot.send_bytes = 4;}
	temp = recNo&0x3f;
	if(temp == gUserGlobal.LogNo)
		{return;}
	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
	prot_shmetro_send_process(&(p->metro_prot));
	gUserGlobal.metro_prot.rece_state = PROT_SHMETRO_RECE_WAIT_STX;
	return;

}
/*=======================================================================================
函数：sam卡函数
功能：1.4 SAM卡计算续期MAC
=========================================================================================*/
UBYTE Sam_xuqimac(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x80\x1a\x45\x01\x10",5);
	memcpy(buf+5,gUserGlobal.ADF1_file0015+12,8);//卡号
	memcpy(buf+13,"\x20\x00\xff\x00\x00\x00\x00\x00",8);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,21,buf,&len);
	if(debugflag)
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
	if(sw1sw2 != 0x9000)
		{return SHMETRO_CE_PSAMERR;}
	if(sw1sw2 == 0x9000)
		{
		memcpy(buf,"\x80\xfa\x05\x00\x10",5);
		memcpy(buf+5,gUserGlobal.Random,4);
		memcpy(buf+9,"\x00\x00\x00\x00\x04\xd6\x95\x18\x08\x80\x0\x0",12);//\x04\xD6\x95\x18\x08
		ret = sam_apdu(buf,21,buf,&len);
		if(ret!=0)
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
			memcpy(gUserGlobal.Mac,buf,4);
			return 0;
			}
		else if(sw1sw2 == 0x6104)
			{memcpy(buf,"\x00\xc0\x00\x00\x04",5);
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
函数：sam卡函数
功能：1.4 SAM卡计算解锁MAC
=========================================================================================*/
UBYTE Sam_Unblockmac(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x80\x1a\x45\x01\x10",5);
	memcpy(buf+5,gUserGlobal.ADF1_file0015+12,8);//卡号
	memcpy(buf+13,"\x20\x00\xff\x00\x00\x00\x00\x00",8);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,21,buf,&len);
	if(debugflag)
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
	if(sw1sw2 != 0x9000)
		{return SHMETRO_CE_PSAMERR;}
	if(sw1sw2 == 0x9000)
		{
		memcpy(buf,"\x80\xfa\x05\x00\x10",5);
		memcpy(buf+5,gUserGlobal.Random,4);
		memcpy(buf+9,"\x00\x00\x00\x00\x84\x18\x00\x00\x04\x80\x0\x0",12);
		ret = sam_apdu(buf,21,buf,&len);
		if(ret!=0)
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
			memcpy(gUserGlobal.Mac,buf,4);
			return 0;
			}
		else if(sw1sw2 == 0x6104)
			{memcpy(buf,"\x00\xc0\x00\x00\x04",5);
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
函数：sam卡函数
功能：1.4 SAM卡计算写0015文件MAC
=========================================================================================*/
UBYTE Sam_up0015mac(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x80\x1a\x45\x01\x10",5);
	memcpy(buf+5,gUserGlobal.ADF1_file0015+12,8);//卡号
	memcpy(buf+13,"\x20\x00\xff\x00\x00\x00\x00\x00",8);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,21,buf,&len);
	if(debugflag)
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
	if(sw1sw2 != 0x9000)
		{return SHMETRO_CE_PSAMERR;}
	if(sw1sw2 == 0x9000)
		{
		memcpy(buf,"\x80\xfa\x05\x00\x10",5);
		memcpy(buf+5,gUserGlobal.Random,4);
		memcpy(buf+9,"\x00\x00\x00\x00\x04\xd6\x95\x08\x05\x80\x80\x0",12);
		buf[18] = gUserGlobal.ADF1_file0015[8]|0x80;
		ret = sam_apdu(buf,21,buf,&len);
		if(ret!=0)
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
			memcpy(gUserGlobal.Mac,buf,4);
			return 0;
			}
		else if(sw1sw2 == 0x6104)
			{memcpy(buf,"\x00\xc0\x00\x00\x04",5);
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
函数：sam卡函数
功能：1.4 SAM卡计算锁M1  MAC
=========================================================================================*/
UBYTE Sam_forbidmac(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],ret,len;
	UWORD sw1sw2;
	memcpy(buf,"\x80\x1a\x45\x01\x10",5);
	memcpy(buf+5,gUserGlobal.ADF1_file0015+12,8);//卡号
	memcpy(buf+13,"\x20\x00\xff\x00\x00\x00\x00\x00",8);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,21);
		gUserGlobal.metro_prot.send_bytes = 21;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	ret = sam_apdu(buf,21,buf,&len);
	if(debugflag)
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
	if(sw1sw2 != 0x9000)
		{return SHMETRO_CE_PSAMERR;}
	if(sw1sw2 == 0x9000)
		{
		memcpy(buf,"\x80\xfa\x05\x00\x10",5);
		memcpy(buf+5,gUserGlobal.Random,4);
		memcpy(buf+9,"\x00\x00\x00\x00\x84\xCD\x00\x00\x04\x80\x0\x0",12);
		ret = sam_apdu(buf,21,buf,&len);
		if(ret!=0)
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
			memcpy(gUserGlobal.Mac,buf,4);
			return 0;
			}
		else if(sw1sw2 == 0x6104)
			{memcpy(buf,"\x00\xc0\x00\x00\x04",5);
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
函数：非接函数
功能：0.11 cpu卡读0007循环记录文件，交易明细记录
=========================================================================================*/
UBYTE Cpu_rd0007(UBYTE point,UBYTE *outbuf)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x00\xb2\x01\x3c\x10",5);
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
函数：非接cpu卡函数
功能：0.4 unblock
=========================================================================================*/
UBYTE Cpu_8418(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],len,temp;

	memcpy(buf,"\x84\x18\x00\x00\x04",5);
	memcpy(buf+5,gUserGlobal.Mac,4);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,9);
		gUserGlobal.metro_prot.send_bytes = 9;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(buf,9,buf,&len);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	if(debugflag)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000)
		{return 0;}
	else if(gUserGlobal.SW1SW2 == 0X9302)
		{return SHMETRO_CE_MAC2ERR;}
	else
		{return SHMETRO_CE_WRITEERROR;}
}
/*=======================================================================================
函数：非接cpu卡函数
功能：0.4 unblock
=========================================================================================*/
UBYTE Cpu_forbid(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],len,temp;

	memcpy(buf,"\x84\xCD\x00\x00\x04",5);
	memcpy(buf+5,gUserGlobal.Mac,4);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,9);
		gUserGlobal.metro_prot.send_bytes = 9;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(buf,9,buf,&len);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	if(debugflag)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000)
		{return 0;}
	else if(gUserGlobal.SW1SW2 == 0X9302)
		{return SHMETRO_CE_MAC2ERR;}
	else
		{return SHMETRO_CE_WRITEERROR;}
}

/*=======================================================================================
函数：非接cpu卡函数
功能：0.4 写0015文件
=========================================================================================*/
UBYTE cpu_expiredate(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],len,temp;

	memcpy(buf,"\x04\xD6\x95\x18\x08",5);
	memcpy(buf+5,gUserGlobal.CardExpiredDate,4);
	memcpy(buf+9,gUserGlobal.Mac,4);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,13);
		gUserGlobal.metro_prot.send_bytes = 13;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(buf,13,buf,&len);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	if(debugflag)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000)
		{return 0;}
	else if(gUserGlobal.SW1SW2 == 0X9302)
		{return SHMETRO_CE_MAC2ERR;}
	else
		{return SHMETRO_CE_WRITEERROR;}
}

/*=======================================================================================
函数：非接cpu卡函数
功能：0.4 写0015文件
=========================================================================================*/
UBYTE cpu_wr0015(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[256],len,temp;

	memcpy(buf,"\x04\xD6\x95\x8\x05",5);
	buf[5] = gUserGlobal.ADF1_file0015[8]|0x80;
	memcpy(buf+6,gUserGlobal.Mac,4);
	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,buf,10);
		gUserGlobal.metro_prot.send_bytes = 10;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	temp = mifpro_icmd(buf,10,buf,&len);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	if(debugflag)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,buf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (buf[len-2]<<8)+buf[len-1];
	if(gUserGlobal.SW1SW2 == 0x9000)
		{return 0;}
	else if(gUserGlobal.SW1SW2 == 0X9302)
		{return SHMETRO_CE_MAC2ERR;}
	else
		{return SHMETRO_CE_WRITEERROR;}
}

/*=======================================================================================
函数：非接卡函数
功能：0.6 复合消费 8052
=========================================================================================*/
UBYTE Credit_forload(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE rdbuf[256],len,temp;
	memcpy(rdbuf,"\x80\x52\x00\x00\x0b",5);
	memcpy(rdbuf+5,gUserGlobal.Date,7);		//交易日期
	memcpy(rdbuf+12,gUserGlobal.Mac,4);		//mac
	rdbuf[16] = 0x04;

	if(debugflag)
		{
		watchdog();
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,17);
		gUserGlobal.metro_prot.send_bytes = 17;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}

	temp = mifpro_icmd(rdbuf,21,rdbuf,&len);

	if(temp!=0)
		{return 1;}
	if(debugflag)
		{
		memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
		gUserGlobal.metro_prot.send_bytes = len;
		gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
		prot_shmetro_send_process(&(p->metro_prot));
		delay_ms(10);
		}
	gUserGlobal.SW1SW2 = (rdbuf[len-2]<<8)+rdbuf[len-1];
	if(len == 2)
		{
		if(gUserGlobal.SW1SW2 ==0x9302)
			{return SHMETRO_CE_MAC2ERR;}
		else if(gUserGlobal.SW1SW2 ==0x6901)
			{return 0x26;}
		else
			{return SHMETRO_CE_WRITEERROR;}
		}

//	memcpy(gUserGlobal.metro_prot.send_buf,rdbuf,len);
//	gUserGlobal.metro_prot.send_bytes = len;
//	gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
//	prot_shmetro_send_process(&(p->metro_prot));
	else if(len == 6)
		{
		if(gUserGlobal.SW1SW2 ==0x9000)
			{
			memcpy(gUserGlobal.Tac,rdbuf,4);//TAC
			return 0;
			}
		else
			{return 1;}
		}
	else 
		{return 1;}
}

/*=======================================================================================
函数：非接卡函数
功能：0.6 复合消费 8052
=========================================================================================*/
UBYTE cpu_getbalance(void)
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
	if(gUserGlobal.SW1SW2==0x9000)
		{return 0;}
	return 2;
}



//=======================================================================================
//=========================================================================================*/
UBYTE Wr_exit(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,buf[16];
	UDWORD temp1;
	
	temp = gUserGlobal.B32[0]&0x20;
	gUserGlobal.Psw=gUserGlobal.Ride[1]|temp;
	Form_psw();
	gUserGlobal.B33[0]=gUserGlobal.Psw;
	memcpy(gUserGlobal.B33+1,gUserGlobal.Date+1,5);
	gUserGlobal.B33[6] = gUserGlobal.Posid[0];
	gUserGlobal.B33[7] = gUserGlobal.Posid[1];
	gUserGlobal.B33[8] = gUserGlobal.Posid[3];
	temp1 = gUserGlobal.B32[9]+(gUserGlobal.B32[10]<<8);
	temp1 = temp1+gUserGlobal.Decval;
	gUserGlobal.B33[9] = temp1%0x100;
	gUserGlobal.B33[10] = (temp1>>8)%0x100;
	gUserGlobal.B33[11]=gUserGlobal.Amountcount;
	gUserGlobal.B33[12]=gUserGlobal.AmountbalanceL;
	gUserGlobal.B33[13]=gUserGlobal.AmountbalanceM;
	gUserGlobal.B33[14]=gUserGlobal.AmountbalanceH;
	lrc_gen2(gUserGlobal.B33,15);
	temp = mcml_load_key(0x00,0X00,12,gUserGlobal.KeyA8);
	if(temp != 0x00)
		{return 1;}
	temp = fm_mcmh_write(49,gUserGlobal.B33,1,0);
	if(temp != 0x00)
		{return 1;}
	temp = mcml_read(49,buf);
	if(temp != 0x00)
		{return 1;}
	temp = compareBlk(buf,gUserGlobal.B33);
	if(temp!=0)
		{return 1;}
	memset(gUserGlobal.B34,0,16);
	gUserGlobal.B34[0]=0xdd;
	gUserGlobal.B34[15]=0xdd;
	temp=mcml_write(50,gUserGlobal.B34);
	if(temp!=0)
		{return 1;}
	temp = mcml_read(50,buf);
	if(temp != 0x00)
		{return 1;}
	temp = compareBlk(buf,gUserGlobal.B34);
	if(temp!=0)
		{return 1;}
	else
		{return 0;}
}
/*=======================================================================================
函数：
功能：0.90 形成卡上公共信息记录
=========================================================================================*/
//void form_0017buf(void)
//{
//	USER_GLOBAL *p = &gUserGlobal;
//	if(gUserGlobal.BalanceType == 00)//普通钱包消费
//		{gUserGlobal.ADF1_file0017[2]=0x00;//锁定标志位
//		gUserGlobal.ADF1_file0017[3]=0x00;//记录版本
//		
//		}
//	else
//		{
//		}
//	return;
//}






/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_LOCKCARD_input(UBYTE *ibuf,UBYTE ibytes)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = gUserGlobal.device_state &0x1f;
	if(temp != 0x02)
		{return SHMETRO_CE_STATUSERROR;}
	
	memcpy(gUserGlobal.Date,ibuf+2,7);//交易日期时间
	gUserGlobal.PosCnt[0] = 0x00;
	gUserGlobal.PosCnt[1] = ibuf[9];//交易流水号
	gUserGlobal.PosCnt[2] = ibuf[10];
	gUserGlobal.PosCnt[3] = ibuf[11];
	gUserGlobal.Ride[1] = ibuf[12];
	if(gUserGlobal.Ride[1]==0)
		{gUserGlobal.Ride[0]=0x12;}
	else
		{gUserGlobal.Ride[1]=0x01;
		gUserGlobal.Ride[0]=0x1F;
		}
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_LOCKCARD_proc(void)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,buf[16];
	if(gUserGlobal.TagType == 0)//m1卡
		{
		temp = WrRecord();
		if(temp !=0)
			{return SHMETRO_CE_WRITEERROR;}
		memset(gUserGlobal.B13,0,16);
		gUserGlobal.B13[11] = 0x03;
		gUserGlobal.B13[12]=(gUserGlobal.CardPoint+1)%0x0a;
		gUserGlobal.B13[13]=(gUserGlobal.CardCount+1)/0x100;
		gUserGlobal.B13[14]=(gUserGlobal.CardCount+1)%0x100;
		lrc_gen2(gUserGlobal.B13,15);
		temp = mcml_load_key(0x00,0X00,0x03,gUserGlobal.KeyA3);
		if(temp != 0x00)
			{return SHMETRO_CE_WRITEERROR;}
		temp = fm_mcmh_write(13,gUserGlobal.B13,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_WRITEERROR;}
		gUserGlobal.Para2 = 0x01;
		if(gUserGlobal.Ride[1] != 0)
			{
			temp = mcml_load_key(0x00,0X00,0x01,gUserGlobal.KeyA1);
			if(temp != 0x00)
				{return 0;}
			temp = mcmh_read_s50(5,buf,1,0);
			if(temp != 0x00)
				{return 0;}
			gUserGlobal.Decval = buf[0]+(buf[1]<<8)+(buf[2]<<16)+(buf[3]<<24);
			temp = mcml_decrement(5,gUserGlobal.Decval);
			if(temp != 0x00)
				{return 0;}
			temp = mcml_transfer(5);
			if(temp != 0x00)
				{return 0;}
			temp = mcml_restore(5);
			if(temp != 0x00)
				{return 0;}
			temp = mcml_transfer(6);
			if(temp != 0x00)
				{return 0;}
			gUserGlobal.Para2 = 0x03;
			}
		return SHMETRO_CE_OK;
		}
	else//CPU卡
		{if(gUserGlobal.Ride[1]==0)
			{gUserGlobal.Para2 = 0x01;}
		else
			{gUserGlobal.Para2 = 0x03;}
		temp = Cpu_getrandom(4);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		sam_select(CPU_SAM);
		temp = Sam_select1001();
		if(temp!=0)
			{return SHMETRO_CE_PSAMERR;}
		temp = Sam_Lockmac();
		if(temp!=0)
			{return SHMETRO_CE_PSAMERR;}
		temp = Cpu_Lockcard();
		if(temp == 0)
			{return SHMETRO_CE_OK;}
		else if(temp == 1)
			{return SHMETRO_CE_CARDREMOVED;}
		else
			{return SHMETRO_CE_WRITEERROR;}
		}
//	return 0;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_LOCKCARD_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
obuf[3] = gUserGlobal.Para2;
*obytes = 4;
//
return;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_LOCKCARD1_input(UBYTE *ibuf,UBYTE ibytes)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = gUserGlobal.device_state &0x1f;
	if(temp != 0x02)
		{return SHMETRO_CE_STATUSERROR;}
	
	memcpy(gUserGlobal.Mac,ibuf+2,4);//MAC
	gUserGlobal.Ride[1] = ibuf[6];
	if(gUserGlobal.Ride[1] != 0)
		{gUserGlobal.Ride[1] = 0x01;}
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_LOCKCARD1_proc(void)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	if((gUserGlobal.Mac[0]==0x00)&&(gUserGlobal.Mac[1]==0x00)&&(gUserGlobal.Mac[2]==0x00)&&(gUserGlobal.Mac[3]==0x00))
		{
		temp = Cpu_getrandom(4);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		sam_select(CPU_SAM);
		temp = Sam_select1001();
		if(temp!=0)
			{return SHMETRO_CE_PSAMERR;}
		temp = Sam_Lockmac();
		if(temp!=0)
			{return SHMETRO_CE_PSAMERR;}
		temp = Cpu_Lockcard();
		if(temp == 0)
			{return SHMETRO_CE_OK;}
		if(gUserGlobal.SW1SW2 == 0X6988)
			{return SHMETRO_CE_MAC2ERR;}	
		return SHMETRO_CE_WRITEERROR;
		}
	else
		{
		temp = Cpu_Lockcard();
		if(temp == 0)
			{return SHMETRO_CE_OK;}
		if(gUserGlobal.SW1SW2 == 0X6988)
			{return SHMETRO_CE_MAC2ERR;}
		return SHMETRO_CE_WRITEERROR;
		}

}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_LOCKCARD1_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
//obuf[3] = 0X00;
*obytes = 3;
//
return;
}



/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_ADDVALUE_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_ADDVALUE_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_ADDVALUE_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_ENABLECARD_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_ENABLECARD_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_ENABLECARD_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_UPDATE_input(UBYTE *ibuf,UBYTE ibytes)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = ibuf[2];
	if(temp== 0||temp > 2)
		{return SHMETRO_CE_BADPARAM;}
	if(gUserGlobal.device_state==0x62)
		{gUserGlobal.Err17Flag = 0;
		gUserGlobal.Ride[1] = ibuf[2];	//更新模式
		memcpy(gUserGlobal.Date,ibuf+3,7);//交易日期时间
		gUserGlobal.PosCnt[0] = 0x00;
		gUserGlobal.PosCnt[1] = ibuf[10];//交易流水号
		gUserGlobal.PosCnt[2] = ibuf[11];
		gUserGlobal.PosCnt[3] = ibuf[12];
		gUserGlobal.TransValue[0] = 0x00;
		gUserGlobal.TransValue[1] = ibuf[15];//交易金额高字节，通讯中低字节先传
		gUserGlobal.TransValue[2] = ibuf[14];
		gUserGlobal.TransValue[3] = ibuf[13];//交易金额低字节
		gUserGlobal.Decval = ibuf[13]+(ibuf[14]<<8)+(ibuf[15]<<16);
		memcpy(gUserGlobal.Updatebuf,ibuf+16,16);
		gUserGlobal.Ride[0] = 0x14;
		
		return 0;
		}
	else 
		{return SHMETRO_CE_STATUSERROR;}	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_UPDATE_proc(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,Rdbuf[16];
	sam_select(CPU_SAM);
	if(gUserGlobal.TagType==2)
		{goto cpu_update;}
	temp = Wrbackup();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	
	temp = WrRecord();
	if(temp !=0)
		{return SHMETRO_CE_WRITEERROR;}
	if(gUserGlobal.Decval!=0)
		{
		temp = mcml_load_key(0x00,0X00,0x01,gUserGlobal.KeyA1);
		if(temp != 0)
			{return SHMETRO_CE_WRITEERROR;}
		temp = fm_mcmh_decrement(0,5,gUserGlobal.Decval,Rdbuf);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
//		temp = mcml_transfer(5);
//		if(temp != 0x00)
//			{return 0x03;}//SHMETRO_CE_WRITEERROR;}	
		}
	if(gUserGlobal.Ride[1] == 1)
		{
		temp = mcml_load_key(0x00,0X00,12,gUserGlobal.KeyA8);
		if(temp != 0)
			{return SHMETRO_CE_WRITEERROR;}
		temp = fm_mcmh_write(48,gUserGlobal.Updatebuf,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_WRITEERROR;}	
		memset(gUserGlobal.B34,0,16);
		gUserGlobal.B34[0]=0xcc;
		gUserGlobal.B34[15]=0xcc;
		temp=mcml_write(50,gUserGlobal.B34);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}	
		}
	else
		{
		temp = mcml_load_key(0x00,0X00,12,gUserGlobal.KeyA8);
		if(temp != 0)
			{return SHMETRO_CE_WRITEERROR;}
		temp = fm_mcmh_write(49,gUserGlobal.Updatebuf,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_WRITEERROR;}	
		memset(gUserGlobal.B34,0,16);
		gUserGlobal.B34[0]=0xdd;
		gUserGlobal.B34[15]=0xdd;
		temp=mcml_write(50,gUserGlobal.B34);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}	
		}

	memset(gUserGlobal.B13,0,16);
	gUserGlobal.B13[11] = 0x02;
	gUserGlobal.B13[12]=(gUserGlobal.CardPoint+1)%0x0a;
	gUserGlobal.B13[13]=(gUserGlobal.CardCount+1)/0x100;
	gUserGlobal.B13[14]=(gUserGlobal.CardCount+1)%0x100;
	lrc_gen2(gUserGlobal.B13,15);
	temp = mcml_load_key(0x00,0X00,0x03,gUserGlobal.KeyA3);
	if(temp != 0)
		{return SHMETRO_CE_WRITEERROR;}
	temp = fm_mcmh_write(13,gUserGlobal.B13,1,0);
	if(temp != 0x00)
		{return SHMETRO_CE_WRITEERROR;}
	temp = mcml_read(13,Rdbuf);
	if(temp != 0x00)
		{return SHMETRO_CE_WRITEERROR;}
	temp = compareBlk(Rdbuf,gUserGlobal.B13);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	Backup();
	gUserGlobal.Err17Flag = 0x01;
	watchdog();
	delay_ms(500);
	watchdog();
	memset(gUserGlobal.B12,0,16);
	temp = mcml_write(12,gUserGlobal.B12);
	if(temp != 0x00)
		{return SHMETRO_CE_PUTBACKCARD1;}
	temp = mcml_read(12,Rdbuf);
	if(temp != 0x00)
		{return SHMETRO_CE_PUTBACKCARD1;}
	temp = compareBlk(Rdbuf,gUserGlobal.B12);
	if(temp!=0)
		{return SHMETRO_CE_PUTBACKCARD1;}
	gUserGlobal.Err17Flag = 0x00;
//	temp = mcml_load_key(0x00,0X00,0x01,gUserGlobal.KeyA1);
//	if(temp != 0)
//		{return 1;}
	if(samkey)
		{temp = fm_mcmh_authentication(0x00,0x01,gUserGlobal.KeyA1);}
	else
		{temp = fm_mcmh_authentication(0x04,0x01,gUserGlobal.KeyA1);}
	if(temp != 0)
		{goto ot_update1;}
	temp = mcml_restore(5);
	if(temp!=0)
		{goto ot_update1;}
	temp = mcml_transfer(6);

ot_update1:
	temp = Sam_Tac();
	return SHMETRO_CE_OK;

cpu_update:
	
	temp = Sam_select1001();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
//	lastaction=0x09;
	if(gUserGlobal.CityNo[0]!=0x20&&gUserGlobal.CityNo[1]!=0x00)
		{goto cpu_up0;}
	temp=Cpu_getrandom(8);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	temp = Sam_Ext_auth();
	if(temp!=0)
		{return SHMETRO_CE_PSAMERR;}
	temp=Cpu_Ext_auth();
	if(temp==1)
		{return SHMETRO_CE_WRITEERROR;}
	if(gUserGlobal.SW1SW2==0X9000)
		{goto cpu_up0;}
	if(gUserGlobal.SW1SW2==0X6a88&&gUserGlobal.ADF1_file0015[9]==0x03)
		{goto cpu_up0;}
	else
		{return SHMETRO_CE_INVALIDCARD;}	
cpu_up0:
	temp = Cpu_80ca();
	if(temp==1)
		{return SHMETRO_CE_WRITEERROR;}
	temp = Sam_80ca();
	if(temp!=0)
		{return temp;}
	temp = Ini_capp_ADF1();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	temp = Sam_mac1_sh();
	if(temp!=0)
		{return temp;}
	if(gUserGlobal.CityNo[0]==0x20&&gUserGlobal.CityNo[1]==0x00)
		{goto up_shanghai;}
	else if(gUserGlobal.CityNo[0]==0x21&&gUserGlobal.CityNo[1]==0x53)
		{goto up_shanghai1;}
	else
		{goto up_0017;}
up_shanghai:
	form_record();
	temp = Update_capp0007();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}

up_shanghai1:
	if(gUserGlobal.Ride[1] == 1)
		{
		memcpy(gUserGlobal.B32,gUserGlobal.Updatebuf,16);
		if(debugflag)
			{
			watchdog();
			gUserGlobal.metro_prot.send_buf[0] = 0x8a;
			gUserGlobal.metro_prot.send_bytes = 1;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		temp = Update_cappS8(4);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		memset(gUserGlobal.B34,0,16);
		gUserGlobal.B34[0]=0xcc;
		gUserGlobal.B34[15]=0xcc;
		temp = Update_cappS8(6);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		}
	else
		{
		memcpy(gUserGlobal.B33,gUserGlobal.Updatebuf,16);
		if(debugflag)
			{
			watchdog();
			gUserGlobal.metro_prot.send_buf[0] = 0x8b;
			gUserGlobal.metro_prot.send_bytes = 1;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		temp = Update_cappS8(5);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		memset(gUserGlobal.B34,0,16);
		gUserGlobal.B34[0]=0xdd;
		gUserGlobal.B34[15]=0xdd;
		temp = Update_cappS8(6);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}	
		
		}
cpu_up1:
	Backup();
	p->Err17Flag=0X11;
	watchdog();
	temp = Debitcapp();
//			temp =1;
//			memcpy(gUserGlobal.Tac,"\x00\x00\x00\x00",4);
//			delay_ms(500);
	if(temp == 0)
		{goto cpu_up2;}
	else if(temp == 2)
		{p->Err17Flag=0x00;
		return SHMETRO_CE_WRITEERROR;
		}
	else
		{
		watchdog();
		mcml_pwr_off();
		delay_ms(5);
		mcml_pwr_on();
		delay_ms(5);
		watchdog();
		temp=HL_Active();
		if(temp != 0)
			{
			watchdog();
			temp=HL_Active();
			if(temp!=0)
				{return SHMETRO_CE_PUTBACKCARD1;}
			}
		temp = Cpu_sel1001();
		if(temp==1)
			{return SHMETRO_CE_PUTBACKCARD1;}
		temp = Cpu_getprove();
		if(temp==0)
			{goto cpu_up2;}
		if(temp== 2)
			{p->Err17Flag=0x00;
			return SHMETRO_CE_WRITEERROR;
			}
		else
			{return SHMETRO_CE_PUTBACKCARD1;}
		}
cpu_up2:
	p->Err17Flag=0;
	temp = Sam_mac2();
//	if(debugflag)
//		{temp=SHMETRO_CE_MAC2ERR;}
	if(temp==0)
		{return SHMETRO_CE_OK;}
	else if(temp==SHMETRO_CE_MAC2ERR)
		{
		memcpy(gUserGlobal.ErrCardSnr,gUserGlobal.CardSnr,4);
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.ErrCardSnr,4);
			memcpy(gUserGlobal.metro_prot.send_buf+4,gUserGlobal.CardSnr,4);
			gUserGlobal.metro_prot.send_bytes = 8;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		return SHMETRO_CE_MAC2ERR;
		}
	else
		{return SHMETRO_CE_PSAMERR;}
up_0017:
	gUserGlobal.ADF1_file0017[0] = 0x09;	//复合消费标志
	gUserGlobal.ADF1_file0017[1] = 0x2e;	//记录长度
	gUserGlobal.ADF1_file0017[2] = 0x00;	//应用锁定标志位
	gUserGlobal.ADF1_file0017[3] = 0x00;	//复合消费记录版本号
	gUserGlobal.ADF1_file0017[4] = 0x00;	//交易类型
	gUserGlobal.ADF1_file0017[5] = 0x20;	//终端机编号（城市代码）
	gUserGlobal.ADF1_file0017[6] = 0x00;
	memcpy(gUserGlobal.ADF1_file0017+7,gUserGlobal.Posid,4);
	memcpy(gUserGlobal.ADF1_file0017+11,"\x0\x0\x0\x0",4);//金额（用于补扣）
	memcpy(gUserGlobal.ADF1_file0017+15,gUserGlobal.Date,7);
	memcpy(gUserGlobal.ADF1_file0017+22,"\x0\x0\x0\x0",4);//通用复合消费记录MAC
	if(gUserGlobal.Ride[1] == 1)
		{
		gUserGlobal.ADF1_file0017[4] = 0x01;
		gUserGlobal.ADF1_file0017[26] = 0xcc;
		}
	else	
		{
		gUserGlobal.ADF1_file0017[4] = 0x00;
		gUserGlobal.ADF1_file0017[26] = 0xdd;
		}
	memcpy(gUserGlobal.ADF1_file0017+27,gUserGlobal.Updatebuf,16);
	temp=Update_capp0017(gUserGlobal.ADF1_file0017);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	goto cpu_up1;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_UPDATE_output(UBYTE *obuf,UBYTE *obytes)
{
	USER_GLOBAL *p = &gUserGlobal;
	UDWORD temp;
//
	obuf[0] = p->cmd;
	obuf[1] = p->cmd_retry;
	obuf[2] = SHMETRO_CE_OK;
	if(gUserGlobal.TagType == 2)//cpu卡
		{
			obuf[3] = gUserGlobal.ADF1_Cnt[0];
			obuf[4] = gUserGlobal.ADF1_Cnt[1];
			temp = gUserGlobal.Balance-gUserGlobal.Decval;
			obuf[5] = temp%0x100;
			obuf[6] = (temp>>8)%0x100;
			obuf[7] = (temp>>16)%0x100;
		}
	else
		{
			obuf[3] = gUserGlobal.M1CardCnt[0];
			obuf[4] = gUserGlobal.M1CardCnt[1];
			temp = gUserGlobal.Balance-gUserGlobal.Decval;
			obuf[5] = temp%0x100;
			obuf[6] = (temp>>8)%0x100;
			obuf[7] = (temp>>16)%0x100;
		}
	memcpy(obuf+8,gUserGlobal.Tac,4);
	*obytes = 12;
//
return;
}


/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_EXPIREDATE_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_EXPIREDATE_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_EXPIREDATE_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_READRECORD_input(UBYTE *ibuf,UBYTE ibytes)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	if(gUserGlobal.device_state == 0xff)
		{return SHMETRO_CE_BADPARAM;}
	temp = ibuf[2]&0xc0;
	if(temp != 0)
		{return SHMETRO_CE_BADPARAM;}
	gUserGlobal.Ride[0] = ibuf[2];
	gUserGlobal.Ride[1] = ibuf[3];
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_READRECORD_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,point,i,secno;
	if(gUserGlobal.TagType==2)
		{goto cpu_log;}
	memset(gUserGlobal.Logbuf,0,368);
	temp=gUserGlobal.Ride[1]&0x01;
	if(temp==0x01)
		{
		temp = mcml_load_key(0x00,0X00,0x02,gUserGlobal.KeyA2);
		if(temp != 0)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(9,gUserGlobal.B9,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		memcpy(gUserGlobal.Logbuf,gUserGlobal.B9,14);
		temp = mcml_read(10,gUserGlobal.B10);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		memcpy(gUserGlobal.Logbuf+14,gUserGlobal.B10,8);
		memcpy(gUserGlobal.Logbuf+22,gUserGlobal.B1+1,5);//押金
		memcpy(gUserGlobal.Logbuf+27,gUserGlobal.B10+8,4);
		gUserGlobal.Logbuf[31] = 0x00;
		gUserGlobal.LogNo = 0x00;
		}
	temp=gUserGlobal.Ride[1]&0x02;
	if(temp==0x02)
		{point = (gUserGlobal.CardPoint+9)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+32,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+32,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+32,0,16);}
		gUserGlobal.LogNo = 0x01;
		}
	temp=gUserGlobal.Ride[1]&0x04;
	if(temp==0x04)
		{point = (gUserGlobal.CardPoint+8)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+48,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+48,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+48,0,16);}
		gUserGlobal.LogNo = 0x02;
		}
	temp=gUserGlobal.Ride[1]&0x08;
	if(temp==0x08)
		{point = (gUserGlobal.CardPoint+7)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+64,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+64,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+64,0,16);}
		gUserGlobal.LogNo = 0x03;
		}
	temp=gUserGlobal.Ride[1]&0x10;
	if(temp==0x10)
		{point = (gUserGlobal.CardPoint+6)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+80,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+80,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+80,0,16);}
		gUserGlobal.LogNo = 0x04;
		}
	temp=gUserGlobal.Ride[1]&0x20;
	if(temp==0x20)
		{point = (gUserGlobal.CardPoint+5)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+96,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+96,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+96,0,16);}
		gUserGlobal.LogNo = 0x05;
		}
	temp=gUserGlobal.Ride[1]&0x40;
	if(temp==0x40)
		{point = (gUserGlobal.CardPoint+4)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+112,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+112,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+112,0,16);}
		gUserGlobal.LogNo = 0x06;
		}
	temp=gUserGlobal.Ride[1]&0x80;
	if(temp==0x80)
		{point = (gUserGlobal.CardPoint+3)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+128,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+128,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+128,0,16);}
		gUserGlobal.LogNo = 0x07;
		}
	temp=gUserGlobal.Ride[0]&0x01;
	if(temp==0x01)
		{point = (gUserGlobal.CardPoint+2)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+144,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+144,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+144,0,16);}
		gUserGlobal.LogNo = 0x08;
		}
	temp=gUserGlobal.Ride[0]&0x02;
	if(temp==0x02)
		{point = (gUserGlobal.CardPoint+1)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+160,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+160,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+160,0,16);}
		gUserGlobal.LogNo = 0x09;
		}
	temp=gUserGlobal.Ride[0]&0x04;
	if(temp==0x04)
		{point = (gUserGlobal.CardPoint)%10;
		CardPoint_to_blkno(point);
		secno = gUserGlobal.BlkNo/4;
		if(secno == 0x03)
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA3);}
		else
			{temp = mcml_load_key(0x00,0X00,secno,gUserGlobal.KeyA4);}
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(gUserGlobal.BlkNo,gUserGlobal.Logbuf+176,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.Logbuf+176,16);
		if(temp!=0)
			{memset(gUserGlobal.Logbuf+176,0,16);}
		gUserGlobal.LogNo = 0x0a;
		}
	temp=gUserGlobal.Ride[0]&0x08;
	if(temp==0x08)
		{
		temp = mcml_load_key(0x00,0X00,0x07,gUserGlobal.KeyA7);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(28,gUserGlobal.B28,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp=0;
		for(i=0;i<16;i++)
			{temp=gUserGlobal.B28[i]^temp;}
//		temp=temp^0xaa;
		if(temp != 0xaa)
			{gUserGlobal.Logbuf[192]= 0xcb;}
		else
			{
			sam_select(CPU_SAM);
			temp = Sam_select1003();
			if(temp!=0)
				{return SHMETRO_CE_PSAMERR;}
			temp = Calc_Tac();
			if(temp != 0x00)
				{return SHMETRO_CE_PSAMERR;}
			if(gUserGlobal.Tac[0]!=gUserGlobal.B28[13]||gUserGlobal.Tac[1]!=gUserGlobal.B28[14])
				{gUserGlobal.Logbuf[192]= 0xcb;}
			else
				{gUserGlobal.Logbuf[192]= 0x00;}
			}
		gUserGlobal.LogNo = 0x0b;
		}
	temp=gUserGlobal.Ride[0]&0x10;
	if(temp==0x10)
		{
		temp = mcml_load_key(0x00,0X00,0x07,gUserGlobal.KeyA7);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(28,gUserGlobal.B28,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp=0;
		for(i=0;i<16;i++)
			{temp=gUserGlobal.B28[i]^temp;}
//		temp=temp^0xaa;
		if(temp != 0xaa)
			{gUserGlobal.Logbuf[193]= 0x8c;}
		else
			{gUserGlobal.Logbuf[193]= 0x00;}
		gUserGlobal.LogNo = 0x0c;
		}
	temp=gUserGlobal.Ride[0]&0x20;
	if(temp==0x20)
		{
		temp = mcml_load_key(0x00,0X00,0x07,gUserGlobal.KeyA7);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = mcmh_read_s50(29,gUserGlobal.B29,1,0);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp = Chk_xor_sum(gUserGlobal.B29,16);
		if(temp!=0)
			{memset(gUserGlobal.B29,0,16);}
		gUserGlobal.LogNo = 0x0d;
		}
	return SHMETRO_CE_OK;
cpu_log:
	temp=gUserGlobal.Ride[1]&0x01;
	if(temp==0x01)
		{return SHMETRO_CE_BADPARAM;}
	temp=gUserGlobal.Ride[1]&0x02;
	if(temp==0x02)
		{temp=Cpu_rd0007(0x01,gUserGlobal.Logbuf+32);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x01;
		}
	temp=gUserGlobal.Ride[1]&0x04;
	if(temp==0x04)
		{temp=Cpu_rd0007(0x02,gUserGlobal.Logbuf+48);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x02;
		}
	temp=gUserGlobal.Ride[1]&0x08;
	if(temp==0x08)
		{temp=Cpu_rd0007(0x03,gUserGlobal.Logbuf+64);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x03;
		}
	temp=gUserGlobal.Ride[1]&0x10;
	if(temp==0x10)
		{temp=Cpu_rd0007(0x04,gUserGlobal.Logbuf+80);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x04;
		}
	temp=gUserGlobal.Ride[1]&0x20;
	if(temp==0x20)
		{temp=Cpu_rd0007(0x05,gUserGlobal.Logbuf+96);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x05;
		}
	temp=gUserGlobal.Ride[1]&0x40;
	if(temp==0x40)
		{temp=Cpu_rd0007(0x06,gUserGlobal.Logbuf+112);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x06;
		}
	temp=gUserGlobal.Ride[1]&0x80;
	if(temp==0x80)
		{temp=Cpu_rd0007(0x07,gUserGlobal.Logbuf+128);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x07;
		}
	temp=gUserGlobal.Ride[0]&0x01;
	if(temp==0x01)
		{temp=Cpu_rd0007(0x08,gUserGlobal.Logbuf+144);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x08;
		}
	temp=gUserGlobal.Ride[0]&0x02;
	if(temp==0x02)
		{temp=Cpu_rd0007(0x09,gUserGlobal.Logbuf+160);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x09;
		}
	temp=gUserGlobal.Ride[0]&0x04;
	if(temp==0x04)
		{temp=Cpu_rd0007(0x0a,gUserGlobal.Logbuf+176);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		gUserGlobal.LogNo = 0x0A;
		}
	return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_READRECORD_output(UBYTE *obuf,UBYTE *obytes)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp=gUserGlobal.Ride[1]&0x01;
	if(temp==0x01)
		{send_log(0,32,gUserGlobal.Logbuf);}
	temp=gUserGlobal.Ride[1]&0x02;
	if(temp==0x02)
		{send_log(1,16,gUserGlobal.Logbuf+32);}
	temp=gUserGlobal.Ride[1]&0x04;
	if(temp==0x04)
		{send_log(2,16,gUserGlobal.Logbuf+48);}
	temp=gUserGlobal.Ride[1]&0x08;
	if(temp==0x08)
		{send_log(3,16,gUserGlobal.Logbuf+64);}
	temp=gUserGlobal.Ride[1]&0x10;
	if(temp==0x10)
		{send_log(4,16,gUserGlobal.Logbuf+80);}
	temp=gUserGlobal.Ride[1]&0x20;
	if(temp==0x20)
		{send_log(5,16,gUserGlobal.Logbuf+96);}
	temp=gUserGlobal.Ride[1]&0x40;
	if(temp==0x40)
		{send_log(6,16,gUserGlobal.Logbuf+112);}
	temp=gUserGlobal.Ride[1]&0x80;
	if(temp==0x80)
		{send_log(7,16,gUserGlobal.Logbuf+128);}
	temp=gUserGlobal.Ride[0]&0x01;
	if(temp==0x01)
		{send_log(8,16,gUserGlobal.Logbuf+144);}
	temp=gUserGlobal.Ride[0]&0x02;
	if(temp==0x02)
		{send_log(9,16,gUserGlobal.Logbuf+160);}
	temp=gUserGlobal.Ride[0]&0x04;
	if(temp==0x04)
		{send_log(10,16,gUserGlobal.Logbuf+176);}
	temp=gUserGlobal.Ride[0]&0x08;
	if(temp==0x08)
		{
		if(gUserGlobal.Logbuf[192]!=0)
			{gUserGlobal.Logbuf[192]=0;
			send_log(0xcb,16,gUserGlobal.Logbuf+192);
			}
		else
			{send_log(11,16,gUserGlobal.B28);}
		}
	temp=gUserGlobal.Ride[0]&0x10;
	if(temp==0x10)
		{
		if(gUserGlobal.Logbuf[193]!=0)
			{gUserGlobal.Logbuf[193]=0;
			send_log(0x8c,16,gUserGlobal.Logbuf+192);
			}
		else
			{send_log(12,16,gUserGlobal.B28);}
		}
	temp=gUserGlobal.Ride[0]&0x20;
	if(temp==0x20)
		{send_log(13,16,gUserGlobal.B29);}
return;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_READRECORD1_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state == 0xff)
		{return SHMETRO_CE_BADPARAM;}
	if(ibuf[2]==0x00&&ibuf[3]==0x00)
		{return SHMETRO_CE_BADPARAM;}
	gUserGlobal.Ride[0] = ibuf[2];
	gUserGlobal.Ride[1] = ibuf[3];
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_READRECORD1_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp, i;

//send_log(UBYTE recNo,UBYTE len,UBYTE *inbuf)
	memset(gUserGlobal.Logbuf,0,512);	
	temp=gUserGlobal.Ride[1]&0x01;
	if(temp==0x01)
		{temp=Cpu_rd0006(0x01,gUserGlobal.Logbuf);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x00;
		}
	temp=gUserGlobal.Ride[1]&0x02;
	if(temp==0x02)
		{temp=Cpu_rd0006(0x02,gUserGlobal.Logbuf+23);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x01;
		}
	temp=gUserGlobal.Ride[1]&0x04;
	if(temp==0x04)
		{temp=Cpu_rd0006(0x03,gUserGlobal.Logbuf+46);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x02;
		}
	temp=gUserGlobal.Ride[1]&0x08;
	if(temp==0x08)
		{temp=Cpu_rd0006(0x04,gUserGlobal.Logbuf+69);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x03;
		}
	temp=gUserGlobal.Ride[1]&0x10;
	if(temp==0x10)
		{temp=Cpu_rd0018(0x01,gUserGlobal.Logbuf+92);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x04;
		}
	temp=gUserGlobal.Ride[1]&0x20;
	if(temp==0x20)
		{temp=Cpu_rd0018(0x02,gUserGlobal.Logbuf+115);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x05;
		}
	temp=gUserGlobal.Ride[1]&0x40;
	if(temp==0x40)
		{temp=Cpu_rd0018(0x03,gUserGlobal.Logbuf+138);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x06;
		}
	temp=gUserGlobal.Ride[1]&0x80;
	if(temp==0x80)
		{temp=Cpu_rd0018(0x04,gUserGlobal.Logbuf+161);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x07;
		}
	temp=gUserGlobal.Ride[0]&0x01;
	if(temp==0x01)
		{temp=Cpu_rd0018(0x05,gUserGlobal.Logbuf+184);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x08;
		}
	temp=gUserGlobal.Ride[0]&0x02;
	if(temp==0x02)
		{temp=Cpu_rd0018(0x06,gUserGlobal.Logbuf+207);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x09;
		}
	temp=gUserGlobal.Ride[0]&0x04;
	if(temp==0x04)
		{temp=Cpu_rd0018(0x07,gUserGlobal.Logbuf+230);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x0a;
		}
	temp=gUserGlobal.Ride[0]&0x08;
	if(temp==0x08)
		{temp=Cpu_rd0018(0x08,gUserGlobal.Logbuf+253);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x0b;
		}
	temp=gUserGlobal.Ride[0]&0x10;
	if(temp==0x10)
		{temp=Cpu_rd0018(0x09,gUserGlobal.Logbuf+276);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x0c;
		}
	temp=gUserGlobal.Ride[0]&0x20;
	if(temp==0x20)
			{temp=Cpu_rd0018(0x0a,gUserGlobal.Logbuf+299);
		if(temp!=0)
			{goto Ot_rdlogerr;}
		gUserGlobal.LogNo = 0x0d;
		}
	temp=gUserGlobal.Ride[0]&0x40;
	if(temp==0x40)
		{
		temp=Cpu_rdsec7(gUserGlobal.B28,0x01);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp=0;
		for(i=0;i<16;i++)
			{temp=gUserGlobal.B28[i]^temp;}
//		temp=temp^0xaa;
		if(temp != 0xaa)
			{gUserGlobal.Logbuf[322]= 0xcb;}
		else
			{
			sam_select(CPU_SAM);
			temp = Calc_Tac();
			if(temp != 0x00)
				{return SHMETRO_CE_PSAMERR;}
			if(gUserGlobal.Tac[0]!=gUserGlobal.B28[13]||gUserGlobal.Tac[1]!=gUserGlobal.B28[14])
				{gUserGlobal.Logbuf[322]= 0xcb;}
			else
				{gUserGlobal.Logbuf[322]= 0x00;}	
			}	
		gUserGlobal.LogNo = 0x0e;
		}
	temp=gUserGlobal.Ride[0]&0x80;
	if(temp==0x80)
		{
		temp=Cpu_rdsec7(gUserGlobal.B28,0x01);
		if(temp != 0x00)
			{return SHMETRO_CE_CARDREMOVED;}
		temp=0;
		for(i=0;i<16;i++)
			{temp=gUserGlobal.B28[i]^temp;}
//		temp=temp^0xaa;
		if(temp != 0xaa)
			{gUserGlobal.Logbuf[322]= 0x8c;}
		else
			{gUserGlobal.Logbuf[322]= 0x00;}
		gUserGlobal.LogNo = 0x0f;
		}
	return SHMETRO_CE_OK;
Ot_rdlogerr:
	return SHMETRO_CE_CARDREMOVED;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_READRECORD1_output(UBYTE *obuf,UBYTE *obytes)
{
	USER_GLOBAL *p = &gUserGlobal;	
//
	UBYTE temp;
	temp=gUserGlobal.Ride[1]&0x01;
	if(temp==0x01)
		{send_log(0,23,gUserGlobal.Logbuf);}
	temp=gUserGlobal.Ride[1]&0x02;
	if(temp==0x02)
		{send_log(1,23,gUserGlobal.Logbuf+23);}
	temp=gUserGlobal.Ride[1]&0x04;
	if(temp==0x04)
		{send_log(2,23,gUserGlobal.Logbuf+46);}
	temp=gUserGlobal.Ride[1]&0x08;
	if(temp==0x08)
		{send_log(3,23,gUserGlobal.Logbuf+69);}
	temp=gUserGlobal.Ride[1]&0x10;
	if(temp==0x10)
		{send_log(4,23,gUserGlobal.Logbuf+92);}
	temp=gUserGlobal.Ride[1]&0x20;
	if(temp==0x20)
		{send_log(5,23,gUserGlobal.Logbuf+115);}
	temp=gUserGlobal.Ride[1]&0x40;
	if(temp==0x40)
		{send_log(6,23,gUserGlobal.Logbuf+138);}
	temp=gUserGlobal.Ride[1]&0x80;
	if(temp==0x80)
		{send_log(7,23,gUserGlobal.Logbuf+161);}
	temp=gUserGlobal.Ride[0]&0x01;
	if(temp==0x01)
		{send_log(8,23,gUserGlobal.Logbuf+184);}
	temp=gUserGlobal.Ride[0]&0x02;
	if(temp==0x02)
		{send_log(9,23,gUserGlobal.Logbuf+207);}
	temp=gUserGlobal.Ride[0]&0x04;
	if(temp==0x04)
		{send_log(10,23,gUserGlobal.Logbuf+230);}
	temp=gUserGlobal.Ride[0]&0x08;
	if(temp==0x08)
		{send_log(11,23,gUserGlobal.Logbuf+253);}
	temp=gUserGlobal.Ride[0]&0x10;
	if(temp==0x10)
		{send_log(12,23,gUserGlobal.Logbuf+276);}
	temp=gUserGlobal.Ride[0]&0x20;
	if(temp==0x20)
		{send_log(13,23,gUserGlobal.Logbuf+299);}
	temp=gUserGlobal.Ride[0]&0x40;
	if(temp==0x40)
		{
		if(gUserGlobal.Logbuf[322]!=0)
			{obuf[0] = p->cmd;
			obuf[1] = p->cmd_retry;
			obuf[2] = 0x14;
			*obytes = 3;
			return;
			}
		else
			{send_log(14,16,gUserGlobal.B28);}
		}
	temp=gUserGlobal.Ride[0]&0x80;
	if(temp==0x80)
		{
		if(gUserGlobal.Logbuf[322]!=0)
			{obuf[0] = p->cmd;
			obuf[1] = p->cmd_retry;
			obuf[2] = 0x14;
			*obytes = 3;
			return;
			}
		else
			{send_log(15,16,gUserGlobal.B28);}
		}
return;
}
/*=======================================================================================
函数：
功能：
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
UBYTE cmd_SHMETRO_OT_GETPROOF_input(UBYTE *ibuf,UBYTE ibytes)
{
	USER_GLOBAL *p = &gUserGlobal;
//	UBYTE temp;
	if(gUserGlobal.device_state == 0xff||gUserGlobal.device_state == 0x00)
		{return SHMETRO_CE_STATUSERROR;}

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
	if(gUserGlobal.CardBak[18]== 0x02)	//充值交易，不判断流水号
		{goto Getproof2;}
	if(ibuf[2]!=gUserGlobal.PosCnt[1])	//交易流水号
		{goto Getproof1ok;}
	if(ibuf[3]!=gUserGlobal.PosCnt[2])
		{goto Getproof1ok;}
	if(ibuf[4]!=gUserGlobal.PosCnt[3])
		{goto Getproof1ok;}
Getproof2:
	if(ibuf[5]!=gUserGlobal.CardBak[44])		//卡号
		{goto Getproof1ok;}
	if(ibuf[6]!=gUserGlobal.CardBak[45])
		{goto Getproof1ok;}
	if(ibuf[7]!=gUserGlobal.CardBak[46])
		{goto Getproof1ok;}
	if(ibuf[8]!=gUserGlobal.CardBak[47])
		{goto Getproof1ok;}
	if(ibuf[9]!=gUserGlobal.CardBak[12])		//卡计数器
		{goto Getproof1ok;}
	if(ibuf[10]!=gUserGlobal.CardBak[13])
		{goto Getproof1ok;}
	return 0;
Getproof1ok:
	return SHMETRO_CE_NORECORD;
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_GETPROOF_proc(void)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,temp1;
	temp = memcmp(gUserGlobal.Tac,"\x00\x00\x00\x00",4);
	if(temp != 0)
		{return 0;}
	temp1 = gUserGlobal.device_state;
	if(temp == 0 &&gUserGlobal.Err17Flag!=0)
		{
		temp = Readcard();
		if(temp != 0)
			{gUserGlobal.device_state = temp1;
			return SHMETRO_CE_NORECORD;
			}
		temp = memcmp(gUserGlobal.Tac,"\x00\x00\x00\x00",4);
		if(temp == 0)
			{gUserGlobal.device_state = temp1;
			return SHMETRO_CE_NORECORD;
			}
		else
			return 0;
		}
	else
		return SHMETRO_CE_NORECORD;	
//	return 0;
//	return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
//CardBak[64];
  //CardBak内容：
  //CardSnr 4字节(0~3)，Balance 交易前金额4字节(4~7)(high--low(M1卡为Block6的前4字节))
  //随机数4字节(8~11)，卡计数器2字节(12\13)，交易金额4字节(14~17)(high--low)
  //(cpu卡交易类型,18)，交易日期时间7字节(19~25)
  //cpu卡消费初始化的密钥版本1字节(26)、算法标示1字节(27)，
//CardPoint(28),Metrosta(29),gUserGlobal.BalanceL~H(30~32);
=========================================================================================*/	
void cmd_SHMETRO_OT_GETPROOF_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
//	UDWORD temp,temp1;
	obuf[0] = p->cmd;
	obuf[1] = p->cmd_retry;
	obuf[2] = 0x00;
	memcpy(obuf+3,gUserGlobal.Tac,4);
//	memcpy(obuf+7,gUserGlobal.PosCnt,4);
//	if(gUserGlobal.TagType == 0x02)
//		{temp = gUserGlobal.CardBak[7]+(gUserGlobal.CardBak[6]<<8)+(gUserGlobal.CardBak[5]<<16)+(gUserGlobal.CardBak[4]<<24);}
//	else
//		{temp = gUserGlobal.CardBak[30]+(gUserGlobal.CardBak[31]<<8)+(gUserGlobal.CardBak[32]<<16);}
//	temp1 = gUserGlobal.CardBak[17]+(gUserGlobal.CardBak[16]<<8)+(gUserGlobal.CardBak[15]<<16)+(gUserGlobal.CardBak[14]<<24);
//	temp = temp-temp1;
//	obuf[11] = temp%0x100;
//	obuf[12] = (temp>>8)%0x100;
//	obuf[13] = (temp>>16)%0x100;
	*obytes = 7;
//
return;
}



/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_TMOUNT_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_TMOUNT_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_TMOUNT_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_AUTH_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_AUTH_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_AUTH_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_VERIFYPIN_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_VERIFYPIN_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_VERIFYPIN_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_MODIFYPIN_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_MODIFYPIN_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_MODIFYPIN_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_PAY_input(UBYTE *ibuf,UBYTE ibytes)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = gUserGlobal.device_state&0x03;
	if(temp==0x2)
		{gUserGlobal.Err17Flag = 0;
		memcpy(gUserGlobal.Date,ibuf+2,7);//交易日期时间
		gUserGlobal.PosCnt[0] = 0x00;
		gUserGlobal.PosCnt[1] = ibuf[9];//交易流水号
		gUserGlobal.PosCnt[2] = ibuf[10];
		gUserGlobal.PosCnt[3] = ibuf[11];
		gUserGlobal.TransValue[0] = 0x00;
		gUserGlobal.TransValue[1] = ibuf[14];//交易金额高字节，通讯中低字节先传
		gUserGlobal.TransValue[2] = ibuf[13];
		gUserGlobal.TransValue[3] = ibuf[12];//交易金额低字节
		gUserGlobal.Decval=ibuf[12]+(ibuf[13]<<8)+(ibuf[14]<<16);
		return 0;
		}
	else 
		{return SHMETRO_CE_STATUSERROR;}
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_PAY_proc(void)
{
USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,buf[16];
//	UDWORD TransValue;

	sam_select(CPU_SAM);
	if(gUserGlobal.TagType==2)
		{goto cpu_pay;}
	
	temp = Wrbackup();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	temp = WrRecord();
	if(temp !=0)
		{return SHMETRO_CE_WRITEERROR;}
	if(gUserGlobal.Decval!=0)
		{
		temp = mcml_load_key(0x00,0X00,0x01,gUserGlobal.KeyA1);
		if(temp != 0x00)
			{return SHMETRO_CE_WRITEERROR;}
		temp = fm_mcmh_decrement(0,5,gUserGlobal.Decval,buf);
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
//		temp = mcml_transfer(5);
//		if(temp != 0x00)
//			{return SHMETRO_CE_WRITEERROR;}	
		}
	memset(gUserGlobal.B13,0,16);
	gUserGlobal.B13[11] = 0x02;
	gUserGlobal.B13[12]=(gUserGlobal.CardPoint+1)%0x0a;
	gUserGlobal.B13[13]=(gUserGlobal.CardCount+1)/0x100;
	gUserGlobal.B13[14]=(gUserGlobal.CardCount+1)%0x100;
	lrc_gen2(gUserGlobal.B13,15);
	temp = mcml_load_key(0x00,0X00,0x03,gUserGlobal.KeyA3);
	if(temp != 0x00)
		{return SHMETRO_CE_WRITEERROR;}
	temp = fm_mcmh_write(13,gUserGlobal.B13,1,0);
	if(temp != 0x00)
		{return SHMETRO_CE_WRITEERROR;}
	temp = mcml_read(13,buf);
	if(temp != 0x00)
		{return SHMETRO_CE_WRITEERROR;}
	temp = compareBlk(buf,gUserGlobal.B13);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	Backup();
	gUserGlobal.Err17Flag = 0x01;
	memset(gUserGlobal.B12,0,16);
	temp = mcml_write(12,gUserGlobal.B12);
	if(temp != 0x00)
		{return SHMETRO_CE_PUTBACKCARD1;}
	temp = mcml_read(12,buf);
	if(temp != 0x00)
		{return SHMETRO_CE_PUTBACKCARD1;}
	temp = compareBlk(buf,gUserGlobal.B12);
	if(temp!=0)
		{return SHMETRO_CE_PUTBACKCARD1;}
	gUserGlobal.Err17Flag = 0x00;
	temp = Sam_Tac();
	return SHMETRO_CE_OK;
	
cpu_pay://cpu卡
	temp = Sam_select1001();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
//	lastaction=0x09;
	if(gUserGlobal.CityNo[0]!=0x20&&gUserGlobal.CityNo[1]!=0x00)
		{goto cpu_pay0;}
	temp=Cpu_getrandom(8);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	temp = Sam_Ext_auth();
	if(temp!=0)
		{return SHMETRO_CE_PSAMERR;}
	temp=Cpu_Ext_auth();
	if(temp==1)
		{return SHMETRO_CE_WRITEERROR;}
	if(gUserGlobal.SW1SW2==0X9000)
		{goto cpu_pay0;}
	if(gUserGlobal.SW1SW2==0X6a88&&gUserGlobal.ADF1_file0015[9]==0x03)
		{goto cpu_pay0;}
	else
		{return SHMETRO_CE_INVALIDCARD;}	
cpu_pay0:
	temp = Cpu_80ca();
	if(temp==1)
		{return SHMETRO_CE_WRITEERROR;}
	temp = Sam_80ca();
	if(temp!=0)
		{return temp;}
	temp = Ini_capp_ADF1();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	temp = Sam_mac1_sh();
	if(temp!=0)
		{return temp;}
	if(gUserGlobal.CityNo[0]==0x20&&gUserGlobal.CityNo[1]==0x00)
		{goto pay_shanghai;}
	else if(gUserGlobal.CityNo[0]==0x21&&gUserGlobal.CityNo[1]==0x53)
		{goto pay_shanghai1;}
	else
		{goto pay_0017;}
pay_shanghai:
	form_record();
	temp = Update_capp0007();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
pay_shanghai1:
	Backup();
	p->Err17Flag=0X11;
//cpu_pay1:
	temp = Debitcapp();
//			temp =1;
	if(temp == 0)
		{goto cpu_pay2;}
	else if(temp == 2)
		{p->Err17Flag=0x00;
		return SHMETRO_CE_WRITEERROR;
		}
	else
		{
		mcml_pwr_off();
		delay_ms(5);
		mcml_pwr_on();
		delay_ms(5);
		temp=HL_Active();
		if(temp != 0)
			{
			temp=HL_Active();
			if(temp!=0)
				{return SHMETRO_CE_PUTBACKCARD1;}
			}
		temp = Cpu_sel1001();
		if(temp==1)
			{return SHMETRO_CE_PUTBACKCARD1;}
		temp = Cpu_getprove();
		if(temp==0)
			{goto cpu_pay2;}
		if(temp== 2)
			{p->Err17Flag=0x00;
			return SHMETRO_CE_WRITEERROR;
			}
		else
			{return SHMETRO_CE_PUTBACKCARD1;}
		}
cpu_pay2:
	p->Err17Flag=0;
	temp = Sam_mac2();
//	if(debugflag)
//		{temp=SHMETRO_CE_MAC2ERR;}
	if(temp==0)
		{return SHMETRO_CE_OK;}
	else if(temp==SHMETRO_CE_MAC2ERR)
		{
		memcpy(gUserGlobal.ErrCardSnr,gUserGlobal.CardSnr,4);
		if(debugflag)
			{
			watchdog();
			memcpy(gUserGlobal.metro_prot.send_buf,gUserGlobal.ErrCardSnr,4);
			memcpy(gUserGlobal.metro_prot.send_buf+4,gUserGlobal.CardSnr,4);
			gUserGlobal.metro_prot.send_bytes = 8;
			gUserGlobal.metro_prot.send_state = PROT_SHMETRO_SEND_START;
			prot_shmetro_send_process(&(p->metro_prot));
			delay_ms(10);
			}
		return SHMETRO_CE_MAC2ERR;
		}
	else
		{return SHMETRO_CE_PSAMERR;}
pay_0017:
	return SHMETRO_CE_WRITEERROR;
	
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_PAY_output(UBYTE *obuf,UBYTE *obytes)
{
	USER_GLOBAL *p = &gUserGlobal;
	UDWORD temp;
//
	obuf[0] = p->cmd;
	obuf[1] = p->cmd_retry;
	obuf[2] = SHMETRO_CE_OK;
	if(gUserGlobal.TagType == 2)//cpu卡
		{
			obuf[3] = gUserGlobal.ADF1_Cnt[0];
			obuf[4] = gUserGlobal.ADF1_Cnt[1];
			temp = gUserGlobal.Balance-gUserGlobal.Decval;
			obuf[5] = temp%0x100;
			obuf[6] = (temp>>8)%0x100;
			obuf[7] = (temp>>16)%0x100;
		}
	else
		{
			obuf[3] = gUserGlobal.M1CardCnt[0];
			obuf[4] = gUserGlobal.M1CardCnt[1];
			temp = gUserGlobal.Balance-gUserGlobal.Decval;
			obuf[5] = temp%0x100;
			obuf[6] = (temp>>8)%0x100;
			obuf[7] = (temp>>16)%0x100;
		}
	memcpy(obuf+8,gUserGlobal.Tac,4);
	*obytes = 12;
//
return;
}




/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_LOCK_input(UBYTE *ibuf,UBYTE ibytes)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = gUserGlobal.device_state&0x03;
	if(temp==0x2)
		{gUserGlobal.Err17Flag = 0;
		gUserGlobal.Psw = ibuf[12];
		temp = Chk_psw();
		if(temp!=0)
			{return SHMETRO_CE_BADPARAM;}
		gUserGlobal.Psw=ibuf[14];
		gUserGlobal.Date[0]=ibuf[5];//交易日期时间
		gUserGlobal.Date[1]=ibuf[4];
		gUserGlobal.Date[2]=ibuf[3];
		gUserGlobal.Date[3]=ibuf[2];
		gUserGlobal.PosCnt[0]=ibuf[9];//交易流水号
		gUserGlobal.PosCnt[1]=ibuf[8];
		gUserGlobal.PosCnt[2]=ibuf[7];
		gUserGlobal.PosCnt[3]=ibuf[6];
		gUserGlobal.TransValue[0] = 0x00;
		gUserGlobal.TransValue[1] = 0x00;
		gUserGlobal.TransValue[2] = ibuf[11];//交易金额高字节，通讯中低字节先传
		gUserGlobal.TransValue[3] = ibuf[10];//交易金额低字节
		
		gUserGlobal.Sjtremainl=0x00;//ibuf[12];
		gUserGlobal.Sjtremainh=0x00;//ibuf[13];
		return 0;
		}
	else 
		{return SHMETRO_CE_STATUSERROR;}
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_ST_LOCK_proc(void)
{
//	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,Rdbuf[16];
	temp=Sjt_backup();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}

	Rdbuf[0] = gUserGlobal.TransValue[2];
	Rdbuf[1] = gUserGlobal.TransValue[3];
	Rdbuf[2] = 0x00;
	Rdbuf[3] = 0x00;
	temp = Sjt_Write(Rdbuf,14);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}

	temp=mcml_read_4bytes(12,Rdbuf+4);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	temp=mcml_read_4bytes(13,Rdbuf+8);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	lrc_gen2(Rdbuf,11);
	temp=Sjt_Write(Rdbuf+8,13);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	gUserGlobal.Sjtbl7[0] = gUserGlobal.Sjtremainh;
	gUserGlobal.Sjtbl7[1] = gUserGlobal.Sjtremainl;
	temp=Sjt_Write(gUserGlobal.Sjtbl7,7);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}

	Backupsjt();
	Rdbuf[0] = gUserGlobal.Psw;
	Rdbuf[1] =(gUserGlobal.CardCount+1)/0x100;
	Rdbuf[2] =(gUserGlobal.CardCount+1)%0x100;
	Rdbuf[3] =Rdbuf[0]^Rdbuf[1]^Rdbuf[2];
	temp=mcml_write_4bytes(9,Rdbuf);
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	gUserGlobal.Err17Flag=0;
	sam_select(SJT_SAM);
	temp = Sjt_Tac();
	return SHMETRO_CE_OK;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_ST_LOCK_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_SALEenmergencyCARD_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_ST_SALEenmergencyCARD_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_ST_SALEenmergencyCARD_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_RETURN_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_ST_RETURN_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_ST_RETURN_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_GETPROOF_input(UBYTE *ibuf,UBYTE ibytes)
{
	USER_GLOBAL *p = &gUserGlobal;
//	UBYTE temp;
	if(gUserGlobal.device_state == 0xff||gUserGlobal.device_state == 0x00)
		{return SHMETRO_CE_STATUSERROR;}

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
	
	if(ibuf[2]!=gUserGlobal.PosCnt[3])	//交易流水号
		{return SHMETRO_CE_NORECORD;}
	if(ibuf[3]!=gUserGlobal.PosCnt[2])
		{return SHMETRO_CE_NORECORD;}
	if(ibuf[4]!=gUserGlobal.PosCnt[1])
		{return SHMETRO_CE_NORECORD;}
	if(ibuf[5]!=gUserGlobal.PosCnt[0])
		{return SHMETRO_CE_NORECORD;}
	if((gUserGlobal.Tac[0]==0x00)&&(gUserGlobal.Tac[1]==0x00)&&(gUserGlobal.Tac[2]==0x00)&&(gUserGlobal.Tac[3]==0x00))
		{return SHMETRO_CE_NORECORD;}
	return 0;
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_ST_GETPROOF_proc(void)
{
return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
//CardBak[64];
  //CardBak内容：
  //CardSnr 4字节(0~3)，Balance 交易前金额4字节(4~7)(high--low(M1卡为Block6的前4字节))
  //随机数4字节(8~11)，卡计数器2字节(12\13)，交易金额4字节(14~17)(high--low)
  //(cpu卡交易类型,18)，交易日期时间7字节(19~25)
  //cpu卡消费初始化的密钥版本1字节(26)、算法标示1字节(27)，
//CardPoint(28),Metrosta(29),gUserGlobal.BalanceL~H(30~32);
=========================================================================================*/	
void cmd_SHMETRO_ST_GETPROOF_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
obuf[3] = gUserGlobal.CardBak[13];
obuf[4] = gUserGlobal.CardBak[12];
memcpy(obuf+5,gUserGlobal.Tac,4);
*obytes = 9;
//
return;
}



/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_FORMATCARD_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_ST_FORMATCARD_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_ST_FORMATCARD_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_ST_AUTHCODE_input(UBYTE *ibuf,UBYTE ibytes)
{
return SHMETRO_CE_BADREQUEST;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_ST_AUTHCODE_proc(void)
{
return SHMETRO_CE_BADREQUEST;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_ST_AUTHCODE_output(UBYTE *obuf,UBYTE *obytes)
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
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_GETKEY_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state == 0xff||gUserGlobal.device_state == 0x00)
		{return SHMETRO_CE_STATUSERROR;}

	return SHMETRO_CE_OK;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_GETKEY_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE buf[16],outlen[2],temp;
	temp = fm_get_module_no(outlen,gUserGlobal.ModuleNo,buf);
	if(temp!= 0)
		{
		if(temp == 0xfe)
			{fm_reset();
//			temp = fm_get_module_no(outlen,gUserGlobal.ModuleNo,buf);
//			if(temp == 0)
//				{goto getkey1;}	
			}
		return 0x23;
		}
//getkey1:
	temp = fm_get_random(0x00,outlen,buf);
	if(temp!= 0)
		{return 0x23;}
	memcpy(gUserGlobal.Random,buf,16);
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_GETKEY_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
memcpy(obuf+3,gUserGlobal.ModuleNo,6);
memcpy(obuf+9,gUserGlobal.Random,16);
*obytes = 25;
//
return;
}	
/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_UNBLOCK_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
//	if(gUserGlobal.device_state != 0x62)
//		{return SHMETRO_CE_STATUSERROR;}
	memcpy(gUserGlobal.Mac,ibuf+2,4);
	return SHMETRO_CE_OK;	
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_UNBLOCK_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	if(gUserGlobal.Mac[0]!=0||gUserGlobal.Mac[1]!=0||gUserGlobal.Mac[2]!=0||gUserGlobal.Mac[3]!=0)
		{goto Ot_unblock1;}
	sam_select(CPU_SAM);
	temp = Sam_select1001();
	if(temp!=0)
		{return SHMETRO_CE_WRITEERROR;}
	temp = HL_Activecpu();
	temp = Cpu_sel1001();
	temp=Cpu_getrandom(4);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}
	temp = Sam_Unblockmac();
	if(temp != 0)
		{return SHMETRO_CE_PSAMERR;}	
Ot_unblock1:
	temp = Cpu_8418();
	if(temp!=0)
		{return temp;}
	
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_UNBLOCK_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
*obytes = 3;
//
return;
}	
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_GETRAN_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state != 0x62)
		{return SHMETRO_CE_STATUSERROR;}
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_GETRAN_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp=Cpu_getrandom(4);
	if(temp!=0)
		{return SHMETRO_CE_CARDREMOVED;}
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_GETRAN_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
memcpy(obuf+3,gUserGlobal.Random,4);
*obytes = 7;
//
return;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_EXPIREDATE1_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state != 0x62)
		{return SHMETRO_CE_STATUSERROR;}
	memcpy(gUserGlobal.CardExpiredDate,ibuf+2,4);
	memcpy(gUserGlobal.Mac,ibuf+6,4);	
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
//
//
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_EXPIREDATE1_proc(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = memcmp(gUserGlobal.CardExpiredDate,"\x00\x00\x00\x00",4);
//	if((p->CardExpiredDate[0]==0x00)&&(p->CardExpiredDate[1]==0x00)&&(p->CardExpiredDate[2]==0x00)&&(p->CardExpiredDate[3]==0x00));
	if(temp == 0)
		{goto wr0015;}
	temp = memcmp(gUserGlobal.Mac,"\x00\x00\x00\x00",4);
	if(temp == 0)
		{temp=Cpu_getrandom(4);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		sam_select(CPU_SAM);
		temp = Sam_select1001();
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		temp = Sam_xuqimac();
		if(temp !=0)
			{return SHMETRO_CE_PSAMERR;}
		}
	temp = cpu_expiredate();
	if(temp!=0)
		{return temp;}
	return 0;
wr0015:
	if(p->Mac[0]==0x00&&p->Mac[1]==0x00&&p->Mac[2]==0x00&&p->Mac[3]==0x00);
		{temp=Cpu_getrandom(4);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		sam_select(CPU_SAM);
		temp = Sam_select1001();
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		temp = Sam_up0015mac();
		if(temp !=0)
			{return SHMETRO_CE_PSAMERR;}
		}
	temp = cpu_wr0015();
	if(temp!=0)
		{return temp;}
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_EXPIREDATE1_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
*obytes = 3;
//
return;
}
/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_FORBID_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state != 0x62)
		{return SHMETRO_CE_STATUSERROR;}
	memcpy(gUserGlobal.Mac,ibuf+2,4);	
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
//
//
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_FORBID_proc(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	if(p->Mac[0]==0x00&&p->Mac[1]==0x00&&p->Mac[2]==0x00&&p->Mac[3]==0x00);
		{temp=Cpu_getrandom(4);
		if(temp!=0)
			{return SHMETRO_CE_CARDREMOVED;}
		sam_select(CPU_SAM);
		temp = Sam_select1001();
		if(temp!=0)
			{return SHMETRO_CE_WRITEERROR;}
		temp = Sam_forbidmac();
		if(temp !=0)
			{return SHMETRO_CE_PSAMERR;}
		}
	temp = Cpu_forbid();
	if(temp!=0)
		{return temp;}
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_FORBID_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
*obytes = 3;
//
return;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_CREDITINI_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state != 0x62)
		{return SHMETRO_CE_STATUSERROR;}
	gUserGlobal.TransValue[0] = 0x00;
	gUserGlobal.TransValue[1] = ibuf[4];//交易金额高字节，通讯中低字节先传
	gUserGlobal.TransValue[2] = ibuf[3];
	gUserGlobal.TransValue[3] = ibuf[2];//交易金额低字节
	gUserGlobal.Decval = ibuf[2]+(ibuf[3]<<8)+(ibuf[4]<<16);
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
//
//
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_CREDITINI_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = Ini_forload_ADF1();
	if(temp!=0)
		{return temp;}
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_CREDITINI_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
memcpy(obuf+3,gUserGlobal.Creditbuf,16);
*obytes = 19;
//
return;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_CREDIT_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state != 0x62)
		{return SHMETRO_CE_STATUSERROR;}
	memcpy(gUserGlobal.Date,ibuf+2,7);//交易日期时间
	memcpy(gUserGlobal.Mac,ibuf+9,4);
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
//
//
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_CREDIT_proc(void)
{	USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = cpu_getbalance();
	if(temp != 0)
		{return SHMETRO_CE_WRITEERROR;}
	Backup();
	gUserGlobal.CardBak[18] = 0x02;//交易类型标识，02圈存
	p->Err17Flag=0X12;
	temp = Credit_forload();
	if(temp == 0)
		{
		p->Err17Flag=0X00;
		return SHMETRO_CE_OK;
		}
	else if(temp == 1)
		{
//		mcml_pwr_off();
//		delay_ms(5);
//		mcml_pwr_on();
//		delay_ms(5);
		temp=HL_Activecpu();
		if(temp != 0)
			{
			temp=HL_Activecpu();
			if(temp!=0)
				{return SHMETRO_CE_PUTBACKCARD1;}
			}
		temp = Cpu_sel1001();
		if(temp==1)
			{return SHMETRO_CE_PUTBACKCARD1;}
		temp = Cpu_getprove();
		if(temp==0)
			{
			p->Err17Flag=0x00;
			return 0;
			}
		if(temp== 2)
			{p->Err17Flag=0x00;
			return SHMETRO_CE_WRITEERROR;
			}
		else
			{return SHMETRO_CE_PUTBACKCARD1;}		
		}
	else
		{p->Err17Flag=0X00;
		return temp;
		}

}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_CREDIT_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
memcpy(obuf+3,gUserGlobal.Tac,4);
*obytes = 7;
//
return;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_ADDVALUE1_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state != 0x62)
		{return SHMETRO_CE_STATUSERROR;}
	gUserGlobal.Err17Flag = 0;
	gUserGlobal.Ride[0] = 0x99;
	memcpy(gUserGlobal.Date,ibuf+2,7);//交易日期时间
	gUserGlobal.PosCnt[0] = 0x00;
	gUserGlobal.PosCnt[1] = ibuf[9];//交易流水号
	gUserGlobal.PosCnt[2] = ibuf[10];
	gUserGlobal.PosCnt[3] = ibuf[11];
	gUserGlobal.TransValue[0] = 0x00;
	gUserGlobal.TransValue[1] = ibuf[14];//交易金额高字节，通讯中低字节先传
	gUserGlobal.TransValue[2] = ibuf[13];
	gUserGlobal.TransValue[3] = ibuf[12];//交易金额低字节
	gUserGlobal.Decval = ibuf[12]+(ibuf[13]<<8)+(ibuf[14]<<16);
	memcpy(gUserGlobal.Keybuf,ibuf+15,16);
	memcpy(gUserGlobal.B8,ibuf+31,16);
	memcpy(gUserGlobal.Mac,ibuf+47,4);
	return SHMETRO_CE_OK;
}
/****************************************************************************
* 名称：Ot_tacbuf()
****************************************************************************/
void addval_tacbuf(void)
{//DelayNS(150);
	
//	gUserGlobal.Logbuf
	gUserGlobal.Logbuf[5] = 0x00;
	gUserGlobal.Logbuf[6] = gUserGlobal.PosCnt[1];
	gUserGlobal.Logbuf[7] = gUserGlobal.PosCnt[2];
	gUserGlobal.Logbuf[8] = gUserGlobal.PosCnt[3];
	gUserGlobal.Logbuf[9] = gUserGlobal.CityNo[0];
	gUserGlobal.Logbuf[10] = gUserGlobal.CityNo[1];
	memcpy(gUserGlobal.Logbuf+11,gUserGlobal.CardSnr,4);
	gUserGlobal.Logbuf[15] = gUserGlobal.CardType;
	gUserGlobal.Logbuf[16] = gUserGlobal.BalanceL;		//低字节BalanceL
	gUserGlobal.Logbuf[17] = gUserGlobal.BalanceM;
	gUserGlobal.Logbuf[18] = gUserGlobal.BalanceH;
	gUserGlobal.Logbuf[19] = gUserGlobal.TransValue[3];	//交易金额低字节,交易金额4字节(14~17)(high--low)
	gUserGlobal.Logbuf[20] = gUserGlobal.TransValue[2];	//
	gUserGlobal.Logbuf[21] = gUserGlobal.TransValue[1];
	memcpy(gUserGlobal.Logbuf+22,gUserGlobal.Date,7);	//交易日期时间7字节(19~25)
	gUserGlobal.Logbuf[29] = gUserGlobal.M1CardCnt[0];	//卡计数器2字节(12\13)
	gUserGlobal.Logbuf[30] = gUserGlobal.M1CardCnt[1];
	memcpy(gUserGlobal.Logbuf+31,gUserGlobal.ModuleNo+2,4);		//CPU卡sam卡卡号后4字节
	gUserGlobal.Logbuf[35] = 0x80;
	gUserGlobal.Logbuf[36] = 0x00;
}
void formblk9(void)
{	UDWORD temp;
	memcpy(gUserGlobal.B9,gUserGlobal.Date+1,5);

	gUserGlobal.B9[5] = gUserGlobal.TransValue[3];
	gUserGlobal.B9[6] = gUserGlobal.TransValue[2];
	gUserGlobal.B9[7] = gUserGlobal.TransValue[1];
	gUserGlobal.B9[8] = gUserGlobal.BalanceL;
	gUserGlobal.B9[9] = gUserGlobal.BalanceM;
	gUserGlobal.B9[10] = gUserGlobal.BalanceH;
	temp = gUserGlobal.Balance+ gUserGlobal.Decval;
	gUserGlobal.B9[11] = temp%0x100;
	gUserGlobal.B9[12] = (temp>>8)%0x100;
	gUserGlobal.B9[13] = (temp>>16)%0x100;
	gUserGlobal.B9[14] =gUserGlobal.M1CardCnt[0];
	gUserGlobal.B9[15] =gUserGlobal.M1CardCnt[1];
}
/*=======================================================================================
函数：
//
//
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_ADDVALUE1_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp,buf[64];
	temp = Wrbackup();
	if(temp!=0)
		{goto ot_add1;}
//	outbuf[0] = 0x01;
//	debug_printf("\x0d\x0a");
//	debug_printf("%02x",outbuf[0]);
//	delay_ms(10);
	temp = WrRecord();
	if(temp !=0)
		{goto ot_add1;}
//	outbuf[0] = 0x02;
//	debug_printf("\x0d\x0a");
//	debug_printf("%02x",outbuf[0]);
//	delay_ms(10);
	temp = mcml_load_key(0x00,0X00,0x03,gUserGlobal.KeyA3);
	if(temp != 0)
		{goto ot_add1;}
	memset(gUserGlobal.B13,0,16);
	gUserGlobal.B13[11] = 0x02;
	gUserGlobal.B13[12]=(gUserGlobal.CardPoint+1)%0x0a;
	gUserGlobal.B13[13]=(gUserGlobal.CardCount+1)/0x100;
	gUserGlobal.B13[14]=(gUserGlobal.CardCount+1)%0x100;
	lrc_gen2(gUserGlobal.B13,15);
//	outbuf[0] = 0x03;
//	debug_printf("\x0d\x0a");
//	debug_printf("%02x",outbuf[0]);
//	delay_ms(10);
	temp = fm_mcmh_write(13,gUserGlobal.B13,1,0);
	if(temp != 0x00)
		{goto ot_add1;}
//	outbuf[0] = 0x04;
//	debug_printf("\x0d\x0a");
//	debug_printf("%02x",outbuf[0]);
//	delay_ms(10);
	temp = mcml_read(13,buf);
	if(temp != 0x00)
		{goto ot_add1;}
	temp = compareBlk(buf,gUserGlobal.B13);
	if(temp!=0)
		{goto ot_add1;}
//	outbuf[0] = 0x05;
//	debug_printf("\x0d\x0a");
//	debug_printf("%02x",outbuf[0]);
//	delay_ms(10);
	Backup();
//	gUserGlobal.Err17Flag = 0x01;
	memset(gUserGlobal.B12,0,16);
//	outbuf[0] = 0x06;
//	debug_printf("\x0d\x0a");
//	debug_printf("%02x",outbuf[0]);
//	delay_ms(10);
	temp = mcml_write(12,gUserGlobal.B12);
	if(temp != 0x00)
		{goto ot_add1;}
	addval_tacbuf();
	watchdog();
	temp = fm_calc_tac(0x0a,0x21,gUserGlobal.Logbuf+5,buf);
	if(temp != 0)
		{if(temp == 0xfe)
			{return 0x23;}
		watchdog();
		temp = fm_calc_tac(0x0a,0x21,gUserGlobal.Logbuf+5,buf);
		if(temp != 0)
			{goto ot_add1;}
		}
	memcpy(gUserGlobal.Tac,buf,4);
	memset(gUserGlobal.B10,0,16);
	temp = fm_mcmh_authenticationB(gUserGlobal.Keybuf);
	if(temp!=0)
		{
//		memcpy(buf,"\x55\x01",2);
//		debug_printf("\x0d\x0a");
//		for(i=0;i<2;i++)
//			{debug_printf("%02x",buf[i]);}
//		delay_ms(10);
		goto ot_add1;
		}
	memcpy(gUserGlobal.B10,gUserGlobal.Posid,4);
	memcpy(gUserGlobal.B10+4,gUserGlobal.ModuleNo+2,4);
	memcpy(gUserGlobal.B10+8,gUserGlobal.Tac,4);
	temp = mcml_write(10,gUserGlobal.B10);
	if(temp != 0)
		{goto ot_add1;}
	formblk9();
	temp = mcml_write(9,gUserGlobal.B9);
	if(temp != 0)
		{goto ot_add1;}
	watchdog();
	temp = mcml_writeMac(8,gUserGlobal.B8,gUserGlobal.Mac);
	if(temp==0)
		{gUserGlobal.Err17Flag = 0;}
ot_add1:
	gUserGlobal.Result = 0x3e;
	watchdog();
	temp = fm_get_transresult(gUserGlobal.Date+1,gUserGlobal.Tac);
	if(temp != 0)
		{
		watchdog();
		temp = fm_get_transresult(gUserGlobal.Date+1,gUserGlobal.Tac);
		if(temp !=0)
			{
			return 0x23;
			}
		}
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_ADDVALUE1_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;
UDWORD temp;
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = 0x00;
obuf[3] = gUserGlobal.Result;
temp = gUserGlobal.Balance + gUserGlobal.Decval;
obuf[4] = temp%0x100;
obuf[5] = (temp>>8)%0x100;
obuf[6] = (temp>>16)%0x100;
memcpy(obuf+7,gUserGlobal.Tac,4);
memcpy(obuf+11,gUserGlobal.Mac,4);
*obytes = 15;
//
return;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_CalcMAC_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	if(gUserGlobal.device_state == 0xff)
		{return SHMETRO_CE_STATUSERROR;}
		
	temp = fm_calc_mac(ibuf+2,gUserGlobal.Mac);
	if(temp!=0)
		{return 0x23;}
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
//
//
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_CalcMAC_proc(void)
{	
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_CalcMAC_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
memcpy(obuf+3,gUserGlobal.Mac,4);
*obytes = 7;
//
return;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_CheckMAC_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	if(gUserGlobal.device_state == 0xff)
		{return SHMETRO_CE_STATUSERROR;}

	temp = fm_check_mac(ibuf+2);
	if(temp == 0x00)
		{
		gUserGlobal.Para1 = 0x0;
		return 0;
		}
	else if(temp == 0xff)
		{return 0x22;}
	else
		{return 0x23;}
	
	
}
	
/*=======================================================================================
函数：
//
//
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_CheckMAC_proc(void)
{	
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_CheckMAC_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
memcpy(obuf+3,gUserGlobal.Mac,4);
*obytes = 7;
//
return;
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/
UBYTE cmd_SHMETRO_OT_CREDITINI1_input(UBYTE *ibuf,UBYTE ibytes)
{	//USER_GLOBAL *p = &gUserGlobal;
	if(gUserGlobal.device_state != 0x62)
		{return SHMETRO_CE_STATUSERROR;}
	return SHMETRO_CE_OK;
}
	
/*=======================================================================================
函数：
//
//
功能：
=========================================================================================*/	
UBYTE cmd_SHMETRO_OT_CREDITINI1_proc(void)
{	//USER_GLOBAL *p = &gUserGlobal;
	UBYTE temp;
	temp = mcml_load_key(0x00,0X00,0X02,gUserGlobal.KeyA2);
	if(temp != 0x00)
		{return 0x23;}//模块通讯错误
	//fm_mcmh_readMac(UBYTE authmode,UBYTE addr, UBYTE *readdata)
	temp = fm_mcmh_readMac(0,8,gUserGlobal.B8);
	if(temp == 0x4e)
		{return 0x4e;}//当前随机数无效
	if(temp != 0x00)
		{return 0x13;}//操作过程中卡移动
	return SHMETRO_CE_OK;		
}

/*=======================================================================================
函数：
功能：
=========================================================================================*/	
void cmd_SHMETRO_OT_CREDITINI1_output(UBYTE *obuf,UBYTE *obytes)
{
USER_GLOBAL *p = &gUserGlobal;	
//
obuf[0] = p->cmd;
obuf[1] = p->cmd_retry;
obuf[2] = SHMETRO_CE_OK;
memcpy(obuf+3,gUserGlobal.B1+8,3);
memcpy(obuf+6,gUserGlobal.B8,16);
memcpy(obuf+22,gUserGlobal.Mac,4);
*obytes = 26;
//
return;
}




//end of file
#endif