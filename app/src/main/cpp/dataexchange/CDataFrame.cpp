//
// Created by wenha_000 on 2017-09-29.
//

#include "CDataFrame.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CDataFrame::CDataFrame(int fieldCount)
:m_FieldCount(fieldCount),m_FieldDef(new CFieldDef[fieldCount])
{
}

CDataFrame::CDataFrame(int fieldCount,CFieldDef* fieldDef)
        :m_FieldCount(fieldCount),m_FieldDef(new CFieldDef[fieldCount])
{

}

CDataFrame::~CDataFrame()
{
    if(m_Buffer!=0)
    {
        delete [] m_Buffer;
    }
    if(m_FieldDef!=0)
    {
        delete [] m_FieldDef;
    }
}

CFieldDef *CDataFrame::getM_FieldDef() const {
    return m_FieldDef;
}

void CDataFrame::setM_FieldDef(CFieldDef *m_FieldDef) {
    CDataFrame::m_FieldDef = m_FieldDef;
}

unsigned short CDataFrame::GetFrameLength()
{
    unsigned short result=0;
    for(int i=0;i<m_FieldCount;i++){
        result+=m_FieldDef[i].GetLength();
    }

    return result;
}

void CDataFrame::Refresh()
{
    for(int i=0;i<m_FieldCount;i++)
    {
        m_FieldDef[i].SetIndex(i);
    }
    if(m_Buffer!=0){
        delete [] m_Buffer;
        m_Buffer=0;
    }
    unsigned short length=GetFrameLength();
    if(length>0){
        m_Buffer=new char[length];
        memset(m_Buffer,'0',length);
    }
}

int CDataFrame::GetFieldPos(int index)
{
    int result=0;
    for(int i=0;i<index;i++)
    {
        result+=m_FieldDef[i].GetLength();
    }
    return result;
}

void CDataFrame::SetFieldData(int index,int data)
{
    char fmtStr[32];
    sprintf(fmtStr,"%%0%dd",m_FieldDef[index].GetLength());
    char dataStr[256];
    sprintf(dataStr,fmtStr,data);
    SetFieldData(index,dataStr);
}

void CDataFrame::SetFieldData(int index, char* data)
{
    char fmtStr[32];
    sprintf(fmtStr,"%%- %ds",m_FieldDef[index].GetLength());
    char dataStr[256];
    sprintf(dataStr,fmtStr,data);
    memcpy(&m_Buffer[GetFieldPos(index)],dataStr,m_FieldDef[index].GetLength());
}

int CDataFrame::GetFieldInt(int index)
{
    char buffer[256];
    GetFieldStr(index,buffer);
    return atoi(buffer);
}

void CDataFrame::GetFieldStr(int index,char* result)
{
    int length=m_FieldDef[index].GetLength();
    memcpy(result,m_Buffer,length);
    result[length]=0x00;

    //去掉后补的空格
    for(int i=length-1;i>=0;i--)
    {
        if(result[i]==0x20){
            result[i]=0x00;
        } else{
            break;
        }
    }
}


