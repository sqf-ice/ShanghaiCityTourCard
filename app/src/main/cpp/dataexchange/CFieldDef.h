//
// Created by wenha_000 on 2017-09-29.
//

#ifndef SHANGHAICITYTOURCARD_CFIELDDEF_H
#define SHANGHAICITYTOURCARD_CFIELDDEF_H

/**
 * 数据域定义类
 */
class CFieldDef {
    enum DataType{
        DATA_INT=0x00,
        DATA_STRING=0x01
    };
    int m_Index;
    int m_Length;
    DataType m_Type;

public:
    CFieldDef();
    CFieldDef(int length,DataType type);

    void SetFieldDef(int length,DataType type);

    int GetIndex();
    void SetIndex(int index);
    int GetLength();
    void SetLength(int length);
    DataType GetType();
    void SetType(DataType type);
};


#endif //SHANGHAICITYTOURCARD_CFIELDDEF_H
