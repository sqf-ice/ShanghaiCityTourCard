//
// Created by wenha_000 on 2017-09-29.
//

#ifndef SHANGHAICITYTOURCARD_CDEBITRECORD_H
#define SHANGHAICITYTOURCARD_CDEBITRECORD_H


class CDebitRecord {
private:
    char m_CorpID[12];
    unsigned long m_LocalSeq;
    unsigned char m_TxnAttr;
    char m_StationID[7];
    char m_PosOprID[17];
    char m_PosCarrID[7];
    unsigned char m_TxnType;
    unsigned long m_PosSeq;
    char m_CityCode[5];
    char m_CardFaceNum[13];
    unsigned char m_CardKind;
    unsigned long m_BalanceBef;
    unsigned long m_Amount;
    char m_DateTime[15];
    unsigned int m_CardCounter;
    char m_PosID[9];
    char m_Tac[9];
    unsigned char m_CardVerNo;
    char* m_TradeSpec;

public:
    CDebitRecord();

    CDebitRecord(char *corpID, unsigned long localSeq, unsigned char txnAttr,
                 char *stationID, char *posOprID, char *posCarrID,
                 unsigned char txnType, unsigned long posSeq, char *cityCode,
                 char *cardFaceNum, unsigned char cardKind,
                 unsigned long balanceBef, unsigned long amount, char *dateTime,
                 unsigned int cardCounter, char *posID, char *tac,
                 unsigned char cardVerNo, char *tradeSpec);
    virtual ~CDebitRecord();

    const char *getM_CorpID() const;

    unsigned long getM_LocalSeq() const;

    void setM_LocalSeq(unsigned long m_LocalSeq);

    unsigned char getM_TxnAttr() const;

    void setM_TxnAttr(unsigned char m_TxnAttr);

    const char *getM_StationID() const;

    const char *getM_PosOprID() const;

    const char *getM_PosCarrID() const;

    unsigned char getM_TxnType() const;

    void setM_TxnType(unsigned char m_TxnType);

    unsigned long getM_PosSeq() const;

    void setM_PosSeq(unsigned long m_PosSeq);

    const char *getM_CityCode() const;

    const char *getM_CardFaceNum() const;

    unsigned char getM_CardKind() const;

    void setM_CardKind(unsigned char m_CardKind);

    unsigned long getM_BalanceBef() const;

    void setM_BalanceBef(unsigned long m_BalanceBef);

    unsigned long getM_Amount() const;

    void setM_Amount(unsigned long m_Amount);

    const char *getM_DateTime() const;

    unsigned int getM_CardCounter() const;

    void setM_CardCounter(unsigned int m_CardCounter);

    const char *getM_PosID() const;

    const char *getM_Tac() const;

    unsigned char getM_CardVerNo() const;

    void setM_CardVerNo(unsigned char m_CardVerNo);

    char *getM_TradeSpec() const;

    void setM_TradeSpec(char *m_TradeSpec);
};


#endif //SHANGHAICITYTOURCARD_CDEBITRECORD_H
