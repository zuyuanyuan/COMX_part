/*HEAD fem_onestep_material HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_FEM_ONESTEP_MATERIAL_H_20071272655
#define _KMAS_COMX_FEM_ONESTEP_MATERIAL_H_20071272655

#include "base\root.hxx"
#include <public\material_parameter_base.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum SETTING_MAT_TYPE
{
     SET_ALL_ELES			=		0x0001,
     SET_CHOOSEED_ELES		=		0x0002,
     SET_REMAINDER_ELES		=		0x0004
};

// {E12CAFCF-CF71-4f4d-BD8E-403C6D7B2C85}
const MUID IID_IMaterialParaSet = 
{ 0xe12cafcf, 0xcf71, 0x4f4d, { 0xbd, 0x8e, 0x40, 0x3c, 0x6d, 0x7b, 0x2c, 0x85 } };

interface IMaterialParaSet : public IRoot
{
	 virtual TStatus STDCALL Initialize() = 0;
	 virtual TStatus STDCALL SetMatParameter(const char* mat_name, const material_parameter& mat_par, int set_type = 0, const int* eleIds = NULL, int len = 0) = 0;
     virtual TStatus STDCALL GetMatParameter(const char* mat_name, material_parameter& mat_par, int*& eleIds, int& len) = 0;
     virtual TStatus STDCALL DelMatParameter(const char* mat_name) = 0;
	 virtual TStatus STDCALL GetMatGroupNums(int& group_num) = 0;
     virtual TStatus STDCALL GetMatNames(char names[][24]) = 0;
     virtual TStatus STDCALL EleGroupId(int eleId, int& groupId) = 0;
     virtual TStatus STDCALL SetMatData(float* parameters, int groupSN) = 0;
     virtual TStatus STDCALL SetMatEleGroup(int* groups, int eleCounts) = 0;
	 virtual TStatus STDCALL GetOperatedMode(int &mode) = 0;
     virtual TStatus STDCALL Clear() = 0;
};

// {64F002F2-BE04-4169-AFDE-837F943BC669}
const MTYPEID CLSID_IMaterialParaSet = 
{ 0x64f002f2, 0xbe04, 0x4169, { 0xaf, 0xde, 0x83, 0x7f, 0x94, 0x3b, 0xc6, 0x69 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

