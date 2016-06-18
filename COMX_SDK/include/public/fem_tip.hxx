/*HEAD fem_tip HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_FEM_ONESTEP_TIP_H_200712012854
#define _KMAS_COMX_FEM_ONESTEP_TIP_H_200712012854

#include "base\root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum ROTATE_AXIS
{
     X_AXIS		=		0,
     Y_AXIS		=		1,
     Z_AXIS		=		2,
};

// {A11E3DFD-3A2F-4a3a-AF0E-6CFE190F0D79}
const MUID IID_IFemTip = 
{ 0xa11e3dfd, 0x3a2f, 0x4a3a, { 0xaf, 0xe, 0x6c, 0xfe, 0x19, 0xf, 0xd, 0x79 } };

interface IFemTip : public IRoot
{
     virtual TStatus STDCALL CheckUnderCuting(int*& redEleId, int& redEldIdLen, int*& yellowEleId, int& yellowEleIdLen, int*& greenEleId, int& greenEleIdLen) = 0;
     virtual TStatus STDCALL ManualTip(const ROTATE_AXIS axis, const float angle) = 0;
	 virtual TStatus STDCALL SetTipAxis(ROTATE_AXIS axis) = 0;
     virtual TStatus STDCALL ResetTip() = 0;
     virtual TStatus STDCALL AutoTipAverageNorma() = 0;
     virtual TStatus STDCALL AutoTipMaxAera() = 0;
};

// {5E5260ED-2AA1-48c6-8172-5C1315674CE7}
const MTYPEID CLSID_IFemTip = 
{ 0x5e5260ed, 0x2aa1, 0x48c6, { 0x81, 0x72, 0x5c, 0x13, 0x15, 0x67, 0x4c, 0xe7 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

