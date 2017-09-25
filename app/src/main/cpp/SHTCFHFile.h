//
// Created by wenha_000 on 2017-09-12.
//

#ifndef SHANGHAICITYTOURCARD_SHTCFHFILE_H
#define SHANGHAICITYTOURCARD_SHTCFHFILE_H

/**
Version	N2	版本号	16
TradeCode	N2	行业代码
        FileType	N4	文件类型	8451/8453
RecNum	N8	记录总数
        RecLength	N8	记录长度
Reserved	ANS24	保留域	全F
IsTSUsed	N1	行业特有数据启用标志	0：不用
1：启用
        SettFlag	AN1	清算标记
RtnSign	S1	回车符	’\n’
*/
struct FHFileDescription
{
    char version[2];
    char tradecode[2];
    char filetype[4];
    char recNum[8];
    char recLength[8];
    char reserved[24];
    char isTsUsed;
    char rtnSign;
};

/**
File Record Area（N）
CorpId	N11	营运单位代码
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
TradSpec	ANSVAR	行业特有数据	依据IsTSUsed而决定有无该域
RtnSign	S1	回车符	’\n’
*/
struct FHFileRecord
{
    char corpID[11];
    char localTxnSeq[8];
    char txnAttr[2];
    char stationID[6];
    char posOprID[6];
    char posCarrId[6];
    char txnType[2];
    char posSeq[12];
    char cityCode[4];
    char cardFaceNum[12];
    char cardKind[2];
    char balBef[8];
    char txnAmt[8];
    char txnDate[8];
    char txnTime[6];
    char txnCounter[6];
    char posID[8];
    char tac[8];
    char cardVerNo[2];
    char tradSpec[10];
    char rtnSign;
};

int FHFileDescriptionFromeData(FHFileDescription* stFileDesc,char* data,int length);
int FHFileDescriptionToData(char* data,FHFileDescription* stFileDesc,int length);

int FHFileRecordFromeData(FHFileRecord* stFileRecord,char* data,int length);
int FHFileRecordToData(char* data,FHFileRecord* stFileRecord,int length);

#endif //SHANGHAICITYTOURCARD_SHTCFHFILE_H
