#include "mex.h"

#include "MasterWrapper.h"

MasterWrapper G_MasterWrapper;

#define VERSION_DATA "Version 1.0 Beta"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
//	char error_format_data[1024];
    bool ret = false;
    int slave_wr = 0;
    int slave_offset = 0;
    int classID = 0;
    double slave_writeData = 0;
    EtherCAT_MemParam tmp_paramSys;

    if(nrhs > 4)
		mexErrMsgTxt("参数太多！\r\n");

    if(nrhs == 0){ //help
        MY_DEBUG_PRINT(VERSION_DATA);
        MY_DEBUG_PRINT("\n");

        #ifdef 	_WIN64
        MY_DEBUG_PRINT("Platform is X64\r\n");
        #else
        MY_DEBUG_PRINT("Platform is X86\r\n");
        #endif

        MY_DEBUG_PRINT("Function List:\n");
        MY_DEBUG_PRINT("%d.Init:\t Master_Interface(0)\n",MasterWrapper::MCMD_Init+1);
        MY_DEBUG_PRINT("%d.Destroy:\t Master_Interface(1)\n",MasterWrapper::MCMD_Destroy+1);
        MY_DEBUG_PRINT("%d.Get ParamSys:\t Master_Interface(2)\n",MasterWrapper::MCMD_Get_ParamSys+1);
        MY_DEBUG_PRINT("%d.Set ParamSys:\t Master_Interface(3,dataSys)\n",MasterWrapper::MCMD_Set_ParamSys+1);
        MY_DEBUG_PRINT("%d.Get ParamPDO:\t Master_Interface(4,offset,type,data)\n",MasterWrapper::MCMD_Get_ParamPDO+1);
        MY_DEBUG_PRINT("%d.Set ParamPDO:\t Master_Interface(5,offset,type,data)\n",MasterWrapper::MCMD_Set_ParamPDO+1);
	}
	else{
		int state_num = *(mxGetPr(prhs[0]));
		switch(state_num){
            case MasterWrapper::MCMD_Init://init
               ret = G_MasterWrapper.Wrapper_Init_cores();
               if(ret == false){
                   mexErrMsgTxt("Attach Master error！\r\n");
               }
               else{
                   MY_DEBUG_PRINT("Attach Master OK\n");
               }
			break;
            case MasterWrapper::MCMD_Destroy://stop
                ret = G_MasterWrapper.Wrapper_Destroy_cores();
                if(ret == false){
                    mexErrMsgTxt("Release Master error！\r\n");
                }
                else{
                    MY_DEBUG_PRINT("Release Master OK\n");
                }
			break;
            case MasterWrapper::MCMD_Get_ParamSys: //GetParam
            if(G_MasterWrapper.Wrapper_isAttachMaster() == false){
                mexErrMsgTxt("Please init first！\r\n");
            }
                plhs[0] = mxCreateDoubleMatrix(1, 3, mxREAL);//第一个输出是一个1*2的矩阵
                double *outpus_data;
                outpus_data = mxGetPr(plhs[0]);//获得矩阵的第一个元素的指针
                tmp_paramSys = G_MasterWrapper.Wrapper_get_MemParamSys();

                outpus_data[0] = tmp_paramSys.test_num;
                outpus_data[1] = tmp_paramSys.slave_count;
                outpus_data[2] = tmp_paramSys.renewState;
			break;
           case MasterWrapper::MCMD_Set_ParamSys://SetParam
            if(G_MasterWrapper.Wrapper_isAttachMaster() == false){
                mexErrMsgTxt("Please init first！\r\n");
            }
                tmp_paramSys.renewState = *(mxGetPr(prhs[1]));
                G_MasterWrapper.Wrapper_set_MemParamSys(tmp_paramSys);
            break;
           case MasterWrapper::MCMD_Get_ParamPDO:
           case MasterWrapper::MCMD_Set_ParamPDO:
            if(G_MasterWrapper.Wrapper_isAttachMaster() == false){
                mexErrMsgTxt("Please init first！\r\n");
            }
            if(nrhs == 4){
                slave_wr = *(mxGetPr(prhs[0]));//第一个参数 3为读，4为写
                slave_offset = *(mxGetPr(prhs[1]));//第二个参数
                classID = mxGetClassID(prhs[2]);//第三个参数
                slave_writeData = *(mxGetPr(prhs[3]));//第四个参数

                plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);//第一个输出是一个1*1的矩阵
                double *pdo_data;
                pdo_data = mxGetPr(plhs[0]);//获得矩阵的第一个元素的指针

                if(mxCHAR_CLASS == classID){
                    //NOTE:在matlab传递中，一个字符占2个char
                    MasterWrapper::MType_enum type =  G_MasterWrapper.Wrapper_Parse_type(prhs[2],false);//第三个参数是type
                    switch(slave_wr){
                       case MasterWrapper::MCMD_Get_ParamPDO://读
                        pdo_data[0] = G_MasterWrapper.Wrapper_get_Data(slave_offset,type);
                        break;
                       case MasterWrapper::MCMD_Set_ParamPDO://写
                        G_MasterWrapper.Wrapper_set_Data(slave_offset,type,slave_writeData);
                        pdo_data[0] = slave_writeData;
                        break;
                      default:
                        mexErrMsgTxt("参数错误！\r\n");
                        break;
                    }
                }
                else{
                    mexErrMsgTxt("参数太少！\r\n");
                }
            break;
			default:
			break;
		}


        }
	}

}
