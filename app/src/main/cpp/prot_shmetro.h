//prot_shmetro.h
//编制：邓建华
//时间：20160814

#ifndef _PROT_SHMETRO_H_
#define _PROT_SHMETRO_H_
//start of file

#define PROT_SHMETRO_STX   0x02
#define PROT_SHMETRO_ETX   0x03
#define PROT_SHMETRO_DLE   0x10

#define PROT_SHMETRO_SEND_OK             0
#define PROT_SHMETRO_SEND_START          1

#define PROT_SHMETRO_RECE_WAIT_STX       0
#define PROT_SHMETRO_RECE_WAIT_LEN_DLE   1
#define PROT_SHMETRO_RECE_WAIT_LEN       2
#define PROT_SHMETRO_RECE_WAIT_DATA_DLE  3
#define PROT_SHMETRO_RECE_WAIT_DATA      4
#define PROT_SHMETRO_RECE_WAIT_LRC_DLE   5
#define PROT_SHMETRO_RECE_WAIT_LRC       6
#define PROT_SHMETRO_RECE_WAIT_ETX       7
#define PROT_SHMETRO_RECE_WAIT_PROCESS   8

#define PROT_SHMETRO_WAIT_ACK_TIMEOUT        150  //150ms必须有应答
#define PROT_SHMETRO_RECE_WAIT_CHAR_TIMEOUT  50      
#define PROT_SHMETRO_MAX_RETRY          1

typedef struct{
	UBYTE send_state;
	UBYTE send_bytes;
	UBYTE rece_state;
	UBYTE rece_bytes;
	UBYTE rece_ptr;
	UDWORD send_timer_cnt;
	UDWORD rece_timer_cnt;
	UBYTE rece_buf[256];
  UBYTE send_buf[256];
	//
  UBYTE uart_index;
	//
	UBYTE send_retry;
	UBYTE last_rece_buf[256];
	UBYTE last_rece_bytes;
  } SHMETRO_PROT;

//
void prot_shmetro_init(SHMETRO_PROT *p,UBYTE uart_index);
void uart_put_byte_prot_shmetro(UBYTE uart_index,UBYTE inbyte);
void prot_shmetro_send_block(SHMETRO_PROT *p);
void prot_shmetro_send_process(SHMETRO_PROT *p);
void prot_shmetro_rece_process(SHMETRO_PROT *p);
void prot_shmetro_cmd_process(SHMETRO_PROT *p);
UBYTE prot_shmetro_is_wait_process(SHMETRO_PROT *p);
void prot_shmetro_processed(SHMETRO_PROT *p);
//
UBYTE is_prot_shmetro_head(UBYTE ch);
UBYTE prot_shmetro_is_busy(SHMETRO_PROT *p);

//end of file
#endif


