/*HEAD fem_constrain_node HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_FEM_CONSTRAIN_NODE_H_2007257587
#define _KMAS_COMX_FEM_CONSTRAIN_NODE_H_2007257587

#include "base\root.hxx"

#include <ostp_solver.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {0389A4BA-5752-4790-BFF4-1A4830EBB449}
 const MUID IID_IConstrainNodeSet = 
{ 0x389a4ba, 0x5752, 0x4790, { 0xbf, 0xf4, 0x1a, 0x48, 0x30, 0xeb, 0xb4, 0x49 } };

interface IConstrainNodeSet : public IRoot
{
     virtual TStatus STDCALL AddGroup() = 0;
     virtual TStatus STDCALL DelGroup(const char* groupName) = 0;
     virtual TStatus STDCALL SetConstrainPara(const char* groupName, const TOnestepSolverConstraintInfor& constrain, const int* nodeIds, int len) = 0;
     virtual TStatus STDCALL GetConstrainPara(const char* groupName, TOnestepSolverConstraintInfor& constrain, int*& nodeIds, int& len) = 0;
     virtual TStatus STDCALL MinusConstrainNode(const char* groupName, const int* nodeIds, int len) = 0;
	 virtual TStatus STDCALL GetGroupNums(int& group_num) = 0;
     virtual TStatus STDCALL GetGroupNames(char names[][24]) = 0;
     virtual TStatus STDCALL Clear() = 0;
     virtual TStatus STDCALL SetConsData(int* parameters, int len) = 0;
     virtual TStatus STDCALL SetConsNodeGroup(int* groups, int len) = 0;
};

// {2058A21D-53F6-436c-B3A8-13746D56F571}
 const MTYPEID CLSID_IConstrainNodeSet = 
{ 0x2058a21d, 0x53f6, 0x436c, { 0xb3, 0xa8, 0x13, 0x74, 0x6d, 0x56, 0xf5, 0x71 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

