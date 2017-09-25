//cmd_shmetro_func1.h
#include "shmetro_common.h"
#ifndef _CMD_SHMETRO_FUNC1_H_
#define _CMD_SHMETRO_FUNC1_H_
//start of file

#ifdef __cplusplus
extern "C" {
#endif
//2.1	初始化读写器命令
UBYTE cmd_SHMETRO_OT_INITREADER_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_INITREADER_proc(void);
void cmd_SHMETRO_OT_INITREADER_output(UBYTE *obuf, UBYTE *obytes);

//2.2	复位读写器命令
UBYTE cmd_SHMETRO_OT_RESETREADER_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_RESETREADER_proc(void);
//void cmd_SHMETRO_OT_RESETREADER_output(UBYTE *obuf,UBYTE *obytes);
void cmd_SHMETRO_OT_RESETREADER_output(void);
//2.3	停止服务命令
UBYTE cmd_SHMETRO_OT_STOPREADER_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_STOPREADER_proc(void);
//void cmd_SHMETRO_OT_STOPREADER_output(UBYTE *obuf,UBYTE *obytes);
void cmd_SHMETRO_OT_STOPREADER_output(void);
//2.4	取读写器状态命令
UBYTE cmd_SHMETRO_OT_READERSTATUS_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_READERSTATUS_proc(void);
void cmd_SHMETRO_OT_READERSTATUS_output(UBYTE *obuf, UBYTE *obytes);

//2.5	显示指示信号命令
UBYTE cmd_SHMETRO_OT_DISPSIGNAL_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_DISPSIGNAL_proc(void);
void cmd_SHMETRO_OT_DISPSIGNAL_output(UBYTE *obuf, UBYTE *obytes);

//2.6	取卡信息命令
UBYTE cmd_SHMETRO_OT_READCARD_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_READCARD_proc(void);
void cmd_SHMETRO_OT_READCARD_output(UBYTE *obuf, UBYTE *obytes);

//2.7	中止命令
UBYTE cmd_SHMETRO_OT_ABORT_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_ABORT_proc(void);
//void cmd_SHMETRO_OT_ABORT_output(UBYTE *obuf,UBYTE *obytes);
void cmd_SHMETRO_OT_ABORT_output(void);

//2.8	单程票进站交易
UBYTE cmd_SHMETRO_ST_ENTRY_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_ST_ENTRY_proc(void);
void cmd_SHMETRO_ST_ENTRY_output(UBYTE *obuf, UBYTE *obytes);

//2.9	专用车票进站交易
UBYTE cmd_SHMETRO_ST_ENTRY2_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_ST_ENTRY2_proc(void);
void cmd_SHMETRO_ST_ENTRY2_output(UBYTE *obuf, UBYTE *obytes);

//2.10	单程票出站交易
UBYTE cmd_SHMETRO_ST_EXIT_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_ST_EXIT_proc(void);
void cmd_SHMETRO_ST_EXIT_output(UBYTE *obuf, UBYTE *obytes);

//2.11	单程票售票交易
UBYTE cmd_SHMETRO_ST_SALECARD_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_ST_SALECARD_proc(void);
void cmd_SHMETRO_ST_SALECARD_output(UBYTE *obuf, UBYTE *obytes);

//2.12	单程票更新交易
UBYTE cmd_SHMETRO_OT_UPDATE_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_UPDATE_proc(void);
void cmd_SHMETRO_OT_UPDATE_output(UBYTE *obuf, UBYTE *obytes);

//2.13	单程票加值交易
UBYTE cmd_SHMETRO_OT_ADDVALUE_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_ADDVALUE_proc(void);
void cmd_SHMETRO_OT_ADDVALUE_output(UBYTE *obuf, UBYTE *obytes);

//2.14	单程票续期交易
UBYTE cmd_SHMETRO_OT_EXPIREDATE_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_EXPIREDATE_proc(void);
void cmd_SHMETRO_OT_EXPIREDATE_output(UBYTE *obuf, UBYTE *obytes);

//2.15	读单交易记录
UBYTE cmd_SHMETRO_OT_READRECORD_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_READRECORD_proc(void);
void cmd_SHMETRO_OT_READRECORD_output(UBYTE *obuf, UBYTE *obytes);

UBYTE cmd_SHMETRO_OT_READRECORD1_input(UBYTE *ibuf, UBYTE ibytes);
UBYTE cmd_SHMETRO_OT_READRECORD1_proc(void);
void cmd_SHMETRO_OT_READRECORD1_output(UBYTE *obuf, UBYTE *obytes);

UBYTE HL_Active(void);
UBYTE cmd_test(UBYTE *ibuf, UBYTE ibytes, UBYTE *obuf, UBYTE *obytes);

#ifdef __cplusplus
}
#endif
//end of file
#endif


