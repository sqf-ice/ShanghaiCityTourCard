//
// Created by wenha_000 on 2017-09-30.
//

#include "CFHFileDebitRecord.h"

CFHFileDebitRecord::CFHFileDebitRecord() : CDataFrame(19)
{
    CFieldDef* fieldDef=getM_FieldDef();

    fieldDef[0]=CFieldDef(11,CFieldDef::DATA_STRING);
    fieldDef[1]=CFieldDef(8,CFieldDef::DATA_INT);
    fieldDef[2]=CFieldDef(2,CFieldDef::DATA_INT);
    fieldDef[3]=CFieldDef(6,CFieldDef::DATA_STRING);
    fieldDef[4]=CFieldDef(16,CFieldDef::DATA_STRING);
    fieldDef[5]=CFieldDef(6,CFieldDef::DATA_STRING);
    fieldDef[6]=CFieldDef(2,CFieldDef::DATA_STRING);
    fieldDef[7]=CFieldDef(12,CFieldDef::DATA_INT);
    fieldDef[8]=CFieldDef(4,CFieldDef::DATA_STRING);
    fieldDef[9]=CFieldDef(12,CFieldDef::DATA_STRING);
    fieldDef[10]=CFieldDef(2,CFieldDef::DATA_INT);
    fieldDef[11]=CFieldDef(8,CFieldDef::DATA_INT);
    fieldDef[12]=CFieldDef(8,CFieldDef::DATA_INT);
    fieldDef[13]=CFieldDef(14,CFieldDef::DATA_STRING);
    fieldDef[14]=CFieldDef(6,CFieldDef::DATA_INT);
    fieldDef[15]=CFieldDef(8,CFieldDef::DATA_STRING);
    fieldDef[16]=CFieldDef(8,CFieldDef::DATA_STRING);
    fieldDef[17]=CFieldDef(2,CFieldDef::DATA_INT);
    fieldDef[18]=CFieldDef(0,CFieldDef::DATA_INT);
}
