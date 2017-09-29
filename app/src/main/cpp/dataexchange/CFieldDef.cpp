//
// Created by wenha_000 on 2017-09-29.
//

#include "CFieldDef.h"

CFieldDef::CFieldDef()
:m_Index(0),m_Length(0),m_Type(DATA_INT)
{

}

CFieldDef::CFieldDef(int length,DataType type)
:m_Index(0),m_Length(length),m_Type(type)
{

}

void CFieldDef::SetFieldDef(int length,DataType type)
{
    m_Length=length;
    m_Type=type;
}

int CFieldDef::GetIndex()
{
    return m_Index;
}
void CFieldDef::SetIndex(int index)
{
    m_Index=index;
}
int CFieldDef::GetLength()
{
    return m_Length;
}
void CFieldDef::SetLength(int length)
{
    m_Length=length;
}
CFieldDef::DataType CFieldDef::GetType()
{
    return m_Type;
}
void CFieldDef::SetType(DataType type)
{
    m_Type=type;
}
