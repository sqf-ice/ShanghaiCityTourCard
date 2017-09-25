//cmd_shmetro_func2.h
#include "shmetro_common.h"
#ifndef _CMD_SHMETRO_FUNC2_H_
#define _CMD_SHMETRO_FUNC2_H_
//start of file

#ifdef __cplusplus
extern "C" {
#endif
//2.16	单程票退票交易
UBYTE cmd_SHMETRO_ST_RETURN_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_ST_RETURN_proc(void);
void cmd_SHMETRO_ST_RETURN_output(UBYTE *obuf, UBYTE *obytes);

//2.17	锁卡交易
UBYTE cmd_SHMETRO_OT_LOCKCARD_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_LOCKCARD_proc(void);
void cmd_SHMETRO_OT_LOCKCARD_output(UBYTE *obuf, UBYTE *obytes);

//2.17	锁卡交易
UBYTE cmd_SHMETRO_OT_LOCKCARD1_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_LOCKCARD1_proc(void);
void cmd_SHMETRO_OT_LOCKCARD1_output(UBYTE *obuf, UBYTE *obytes);

//2.18	取单程票交易认证码（TAC）
UBYTE cmd_SHMETRO_ST_GETPROOF_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_ST_GETPROOF_proc(void);
void cmd_SHMETRO_ST_GETPROOF_output(UBYTE *obuf, UBYTE *obytes);

//2.19	格式化单程票
UBYTE cmd_SHMETRO_ST_FORMATCARD_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_ST_FORMATCARD_proc(void);
void cmd_SHMETRO_ST_FORMATCARD_output(UBYTE *obuf, UBYTE *obytes);

//2.20	应急票售票交易
UBYTE cmd_SHMETRO_ST_SALEenmergencyCARD_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_ST_SALEenmergencyCARD_proc(void);
void cmd_SHMETRO_ST_SALEenmergencyCARD_output(UBYTE *obuf, UBYTE *obytes);

//2.21	计算应急票申请授权码
UBYTE cmd_SHMETRO_ST_AUTHCODE_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_ST_AUTHCODE_proc(void);
void cmd_SHMETRO_ST_AUTHCODE_output(UBYTE *obuf, UBYTE *obytes);


UBYTE cmd_SHMETRO_OT_GETKEY_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_GETKEY_proc(void);
void cmd_SHMETRO_OT_GETKEY_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_UNBLOCK_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_UNBLOCK_proc(void);
void cmd_SHMETRO_OT_UNBLOCK_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_GETRAN_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_GETRAN_proc(void);
void cmd_SHMETRO_OT_GETRAN_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_EXPIREDATE1_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_EXPIREDATE1_proc(void);
void cmd_SHMETRO_OT_EXPIREDATE1_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_FORBID_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_FORBID_proc(void);
void cmd_SHMETRO_OT_FORBID_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_CREDITINI_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_CREDITINI_proc(void);
void cmd_SHMETRO_OT_CREDITINI_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_CREDIT_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_CREDIT_proc(void);
void cmd_SHMETRO_OT_CREDIT_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_ADDVALUE1_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_ADDVALUE1_proc(void);
void cmd_SHMETRO_OT_ADDVALUE1_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_CalcMAC_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_CalcMAC_proc(void);
void cmd_SHMETRO_OT_CalcMAC_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_CheckMAC_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_CheckMAC_proc(void);
void cmd_SHMETRO_OT_CheckMAC_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_CREDITINI1_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_CREDITINI1_proc(void);
void cmd_SHMETRO_OT_CREDITINI1_output(UBYTE *obuf, UBYTE *obytes);

#ifdef __cplusplus
}
#endif
//end of file
#endif


