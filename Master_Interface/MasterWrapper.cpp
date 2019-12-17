#include "MasterWrapper.h"

MasterWrapper::MasterWrapper()
{
   m_test_num = 0;
   m_MasterMem_Handle.MemShare_set_memSize(1024*5);//设置共享内存的大小,暂且设置为5K

   m_paramSys = nullptr;
}

MasterWrapper::~MasterWrapper()
{
   m_MasterMem_Handle.MemShare_Destroy_cores();
}

bool MasterWrapper::Wrapper_Init_cores()
{
    m_test_num = 0;
    if(m_MasterMem_Handle.MemShare_Init_cores()){
        m_paramSys = (EtherCAT_MemParam*)(this->m_MasterMem_Handle.MemShare_get_baseAddr());
    }
    else{
        return false;
    }

    return true;
}

bool MasterWrapper::Wrapper_Destroy_cores()
{
    m_test_num = 0;
    m_paramSys = nullptr;

    return m_MasterMem_Handle.MemShare_Destroy_cores();
}

bool MasterWrapper::Wrapper_isAttachMaster()
{
    if(m_MasterMem_Handle.MemShare_get_baseAddr() == nullptr){
        return false;
    }
    else{
        return true;
    }
}

void MasterWrapper::Wrapper_set_Data(int offset, MType_enum type, double data)
{
    switch (type) {
    case MTYPE_UINT8:
        this->set_pdo_value<uint8_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset,(uint8_t)data);
        break;
    case MTYPE_UINT16:
        this->set_pdo_value<uint16_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset,(uint16_t)data);
        break;
    case MTYPE_UINT32:
        this->set_pdo_value<uint32_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset,(uint32_t)data);
        break;
    case MTYPE_INT8:
        this->set_pdo_value<int8_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset,(int8_t)data);
        break;
    case MTYPE_INT16:
        this->set_pdo_value<int16_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset,(int16_t)data);
        break;
    case MTYPE_INT32:
        this->set_pdo_value<int32_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset,(int32_t)data);
        break;
    default:
        break;
    }
}

void MasterWrapper::Wrapper_set_MemParamSys(const EtherCAT_MemParam param)
{
    //先只是赋值一个
    m_paramSys->renewState = param.renewState;
}

EtherCAT_MemParam MasterWrapper::Wrapper_get_MemParamSys()
{
    EtherCAT_MemParam *tmp = (EtherCAT_MemParam *) (this->m_MasterMem_Handle.MemShare_get_baseAddr());
    EtherCAT_MemParam ret = *tmp;

    if(tmp->test_num != 0){
        if(m_test_num != tmp->test_num){
//            MY_DEBUG_PRINT("xx=%d\r\n",pBuf[0]);
            m_test_num = tmp->test_num;
//            MY_DEBUG_PRINT("id=%d\r\n",*(pBuf+1024+0x28));
        }
//        else{
//            MY_DEBUG_PRINT("...\r\n");
//        }
    }

    return ret;
}


double MasterWrapper::Wrapper_get_Data(int offset,MType_enum type)
{
    double ret_data = 0;

    switch (type) {
    case MTYPE_UINT8:
        ret_data = this->get_pdo_value<uint8_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset);
        break;
    case MTYPE_UINT16:
        ret_data = this->get_pdo_value<uint16_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset);
        break;
    case MTYPE_UINT32:
        ret_data = this->get_pdo_value<uint32_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset);
        break;
    case MTYPE_INT8:
        ret_data = this->get_pdo_value<int8_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset);
        break;
    case MTYPE_INT16:
        ret_data = this->get_pdo_value<int16_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset);
        break;
    case MTYPE_INT32:
        ret_data = this->get_pdo_value<int32_t>(this->m_MasterMem_Handle.MemShare_get_baseAddr()+m_pdo_offset,offset);
        break;
    default:
        break;
    }

    return ret_data;
}

MasterWrapper::MType_enum MasterWrapper::Wrapper_Parse_type(const mxArray *pa,bool isDebug){
    bool ret = true;
    MType_enum ret_type = MTYPE_NULL;
    //NOTE:在matlab传递中，一个字符占2个char
    uint16_t *array_chars = (uint16_t*)mxGetChars(pa);
    switch(array_chars[0]){
      case 'u':
        switch(array_chars[1]){
           case '8':
            if(isDebug){
               MY_DEBUG_PRINT("uint8\n");
            }
            ret_type = MTYPE_UINT8;
            break;
           case '1':
            if(array_chars[2] == '6'){
                if(isDebug){
                   MY_DEBUG_PRINT("uint16\n");
                }
                ret_type = MTYPE_UINT16;
            }
            else{
                ret = false;
            }
            break;
        case '3':
            if(array_chars[2] == '2'){
                if(isDebug){
                   MY_DEBUG_PRINT("uint32\n");
                }
                ret_type = MTYPE_UINT32;
            }
            else{
                ret = false;
            }
            break;
          default:
            ret = false;
            break;
        }
        break;
      case 'i':
        switch(array_chars[1]){
           case '8':
            if(isDebug){
               MY_DEBUG_PRINT("int8\n");
            }
            ret_type = MTYPE_INT8;
            break;
           case '1':
            if(array_chars[2] == '6'){
                if(isDebug){
                   MY_DEBUG_PRINT("int16\n");
                }
                ret_type = MTYPE_INT16;
            }
            else{
                ret = false;
            }
            break;
        case '3':
            if(array_chars[2] == '2'){
                if(isDebug){
                   MY_DEBUG_PRINT("int32\n");
                }
                ret_type = MTYPE_INT32;
            }
            else{
                ret = false;
            }
            break;
          default:
            ret = false;
            break;
        }
        break;
      default:
        ret = false;
        break;
    }

    if(ret == false){
        mexErrMsgTxt("参数类型错误！\n");
    }

    return ret_type;
}
