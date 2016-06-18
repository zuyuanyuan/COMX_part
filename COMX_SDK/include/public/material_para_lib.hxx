/*HEAD fem_material_para_lib HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date            Name            Description of Change

$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_FEM_MATERIAL_PARA_LIB_H_200712912405
#define _KMAS_COMX_FEM_MATERIAL_PARA_LIB_H_200712912405

#include "base\root.hxx"

#include "public\material_parameter_base.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {47A861CC-89FF-4fc5-B787-4CD11797833C}
 const MUID IID_IMaterialParaLib = 
{ 0x47a861cc, 0x89ff, 0x4fc5, { 0xb7, 0x87, 0x4c, 0xd1, 0x17, 0x97, 0x83, 0x3c } };

interface IMaterialParaLib : public IRoot
{
     virtual TStatus STDCALL Import(const char* fileName) = 0;
     virtual TStatus STDCALL Export(const char* fileName) = 0;
     virtual TStatus STDCALL AddMatGroup(const char* mat_name, const material_parameter& mat_para) = 0;
     virtual TStatus STDCALL GetMatGroup(const char* mat_name, material_parameter& mat_para) = 0;
     virtual TStatus STDCALL DelMatGroup(const char* mat_name) = 0;
     virtual TStatus STDCALL GetMatGroupNums(int& group_nums) = 0;
     virtual TStatus STDCALL GetMatNames(char mat_names[][24]) = 0;
     virtual TStatus STDCALL Clear() = 0;
};

// {1B53A3A3-B865-4886-9023-A6DDD2528774}
 const MTYPEID CLSID_IMaterialParaLib = 
{ 0x1b53a3a3, 0xb865, 0x4886, { 0x90, 0x23, 0xa6, 0xdd, 0xd2, 0x52, 0x87, 0x74 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

