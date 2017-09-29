//
// Created by wenha_000 on 2017-09-29.
//

#ifndef SHANGHAICITYTOURCARD_CDATAFRAME_H
#define SHANGHAICITYTOURCARD_CDATAFRAME_H

#include "CFieldDef.h"

/**
 * 数据报文基类
 */
class CDataFrame {
private:
    char* m_Buffer;
    int m_FieldCount;
    CFieldDef* m_FieldDef;
public:
    CDataFrame(int fieldCount);
    CDataFrame(int fieldCount,CFieldDef* fieldDef);

    ~CDataFrame();

    //获取数据报文长度
    unsigned short GetFrameLength();
    void Refresh();

    int GetFieldPos(int index);
    void SetFieldData(int index,int data);
    void SetFieldData(int index,char* data);

    int GetFieldInt(int index);
    void GetFieldStr(int index,char* result);
};


#endif //SHANGHAICITYTOURCARD_CDATAFRAME_H
