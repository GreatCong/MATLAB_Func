#ifndef MASTERWRAPPER_H
#define MASTERWRAPPER_H

#include "MemShare_Win.h"
#include "Mex.h"

#include <stdint.h>

#define MY_DEBUG_PRINT mexPrintf

typedef struct{
    int16_t test_num;
    int16_t slave_count;
    int16_t renewState;
}EtherCAT_MemParam;

class MasterWrapper
{
public:
    MasterWrapper();
    ~MasterWrapper();
public:
    typedef enum{
        MTYPE_NULL,
        MTYPE_UINT8,
        MTYPE_UINT16,
        MTYPE_UINT32,
        MTYPE_INT8,
        MTYPE_INT16,
        MTYPE_INT32
    }MType_enum;

    typedef enum{
        MCMD_Init,
        MCMD_Destroy,
        MCMD_Get_ParamSys,
        MCMD_Set_ParamSys,
        MCMD_Get_ParamPDO,
        MCMD_Set_ParamPDO
    }MCmd_enum;

    bool Wrapper_Init_cores();
    bool Wrapper_Destroy_cores();

    bool Wrapper_isAttachMaster();

    double Wrapper_get_Data(int offset,MType_enum type);
    void Wrapper_set_Data(int offset,MType_enum type,double data);

    EtherCAT_MemParam Wrapper_get_MemParamSys();
    void Wrapper_set_MemParamSys(const EtherCAT_MemParam param);
    MType_enum Wrapper_Parse_type(const mxArray *pa, bool isDebug);
private:
    MemShare_Win m_MasterMem_Handle;
    int16_t m_test_num;
    EtherCAT_MemParam *m_paramSys;

    const int m_pdo_offset = 1024;
protected:
    template <class T>
    T get_pdo_value(char *address,int offset){
        T *address_tmp = (T *)(address+offset);
        return (*address_tmp);
    }

    template <class T>
    void set_pdo_value(char *address,int offset,T value){
        T *address_tmp = (T *)(address+offset);
        *address_tmp = value;
//        printf("value = %d\n",*address_tmp);
    }
};

#endif // MASTERWRAPPER_H
