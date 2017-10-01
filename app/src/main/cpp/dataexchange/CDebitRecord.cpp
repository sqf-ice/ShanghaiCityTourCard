//
// Created by wenha_000 on 2017-09-29.
//

#include <memory.h>
#include "CDebitRecord.h"

CDebitRecord::CDebitRecord()
:m_TradeSpec(0)
{}

CDebitRecord::CDebitRecord(char *corpID, unsigned long localSeq, unsigned char txnAttr,
                           char *stationID, char *posOprID, char *posCarrID,
                           unsigned char txnType, unsigned long posSeq, char *cityCode,
                           char *cardFaceNum, unsigned char cardKind,
                           unsigned long balanceBef, unsigned long amount, char *dateTime,
                           unsigned int cardCounter, char *posID, char *tac,
                           unsigned char cardVerNo, char *tradeSpec)
{
    strcpy(m_CorpID,corpID);
    m_LocalSeq=localSeq;
    m_TxnAttr=txnAttr;
    strcpy(m_StationID,stationID);
    strcpy(m_PosOprID,posOprID);
    strcpy(m_PosCarrID,posCarrID);
    m_TxnType=txnType;
    m_PosSeq=posSeq;
    strcpy(m_CityCode,cityCode);
    strcpy(m_CardFaceNum,cardFaceNum);
    m_CardKind=cardKind;
    m_BalanceBef=balanceBef;
    m_Amount=amount;
    strcpy(m_DateTime,dateTime);
    m_CardCounter=cardCounter;
    strcpy(m_PosID,posID);
    strcpy(m_Tac,tac);
    m_CardVerNo=cardVerNo;
    //memcpy(m_TradeSpec,tradeSpec,100);
}

CDebitRecord::~CDebitRecord() {
    if(m_TradeSpec!=0){
        delete [] m_TradeSpec;
    }
}

const char *CDebitRecord::getM_CorpID() const {
    return m_CorpID;
}

unsigned long CDebitRecord::getM_LocalSeq() const {
    return m_LocalSeq;
}

void CDebitRecord::setM_LocalSeq(unsigned long m_LocalSeq) {
    CDebitRecord::m_LocalSeq = m_LocalSeq;
}

unsigned char CDebitRecord::getM_TxnAttr() const {
    return m_TxnAttr;
}

void CDebitRecord::setM_TxnAttr(unsigned char m_TxnAttr) {
    CDebitRecord::m_TxnAttr = m_TxnAttr;
}

const char *CDebitRecord::getM_StationID() const {
    return m_StationID;
}

const char *CDebitRecord::getM_PosOprID() const {
    return m_PosOprID;
}

const char *CDebitRecord::getM_PosCarrID() const {
    return m_PosCarrID;
}

unsigned char CDebitRecord::getM_TxnType() const {
    return m_TxnType;
}

void CDebitRecord::setM_TxnType(unsigned char m_TxnType) {
    CDebitRecord::m_TxnType = m_TxnType;
}

unsigned long CDebitRecord::getM_PosSeq() const {
    return m_PosSeq;
}

void CDebitRecord::setM_PosSeq(unsigned long m_PosSeq) {
    CDebitRecord::m_PosSeq = m_PosSeq;
}

const char *CDebitRecord::getM_CityCode() const {
    return m_CityCode;
}

const char *CDebitRecord::getM_CardFaceNum() const {
    return m_CardFaceNum;
}

unsigned char CDebitRecord::getM_CardKind() const {
    return m_CardKind;
}

void CDebitRecord::setM_CardKind(unsigned char m_CardKind) {
    CDebitRecord::m_CardKind = m_CardKind;
}

unsigned long CDebitRecord::getM_BalanceBef() const {
    return m_BalanceBef;
}

void CDebitRecord::setM_BalanceBef(unsigned long m_BalanceBef) {
    CDebitRecord::m_BalanceBef = m_BalanceBef;
}

unsigned long CDebitRecord::getM_Amount() const {
    return m_Amount;
}

void CDebitRecord::setM_Amount(unsigned long m_Amount) {
    CDebitRecord::m_Amount = m_Amount;
}

const char *CDebitRecord::getM_DateTime() const {
    return m_DateTime;
}

unsigned int CDebitRecord::getM_CardCounter() const {
    return m_CardCounter;
}

void CDebitRecord::setM_CardCounter(unsigned int m_CardCounter) {
    CDebitRecord::m_CardCounter = m_CardCounter;
}

const char *CDebitRecord::getM_PosID() const {
    return m_PosID;
}

const char *CDebitRecord::getM_Tac() const {
    return m_Tac;
}

unsigned char CDebitRecord::getM_CardVerNo() const {
    return m_CardVerNo;
}

void CDebitRecord::setM_CardVerNo(unsigned char m_CardVerNo) {
    CDebitRecord::m_CardVerNo = m_CardVerNo;
}

char *CDebitRecord::getM_TradeSpec() const {
    return m_TradeSpec;
}

void CDebitRecord::setM_TradeSpec(char *m_TradeSpec) {
    CDebitRecord::m_TradeSpec = m_TradeSpec;
}
