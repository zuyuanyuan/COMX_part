/*HEAD fem_constrain_node HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

	User draw some drawbead line on surface of the FEM with mouse .
================================================================================
Date            Name            Description of Change
2007/03/19		Zgj				Create and modify 

$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_DRAWBEAD_LINE_SET_HXX
#define _KMAS_COMX_DRAWBEAD_LINE_SET_HXX

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {8B728AA3-F283-48fd-9256-AAD195C37703}
const MUID IID_IDrawbeadLineSet = 
{ 0x8b728aa3, 0xf283, 0x48fd, { 0x92, 0x56, 0xaa, 0xd1, 0x95, 0xc3, 0x77, 0x3 } };

interface IDrawbeadLineSet : public IRoot
{
	virtual TStatus STDCALL SetZDirection(bool zDirection = true) = 0;
	virtual TStatus STDCALL CreateNew(char *drawbeadName) = 0;
	virtual TStatus STDCALL ActiveDrawbead(const char *drawbeadName) = 0;
	virtual TStatus STDCALL AddPoint() = 0;
	virtual TStatus STDCALL DeletePoint() = 0;
	virtual TStatus STDCALL MovePoint() = 0;
	virtual TStatus STDCALL DeleteAllPoint() = 0;
	virtual TStatus STDCALL ClosePoint() = 0;
	virtual TStatus STDCALL DeleteDrawbead(const char *drawbeadName) = 0;
	virtual TStatus STDCALL GetDrawbeadNames(char names[][24], int &numbers) = 0;
	virtual TStatus STDCALL IsDisplayDrawbead(bool display = true) = 0;
	virtual TStatus STDCALL End() = 0;
	virtual TStatus STDCALL GetDrawbeadSerialNum(const char *drawbeadName, int &groupId) = 0;
	virtual TStatus STDCALL GetDrawbeadLengh(const int &groupId, double &lengh) = 0;
	virtual TStatus STDCALL Clear() = 0;
	
	// First refreshData, then get numbers,nodes,elements...
	// Under interface all for output KMAS file...
	virtual TStatus STDCALL RefreshDatas() = 0;
	virtual TStatus STDCALL GetNumbers(int &groupNums, int &nodesNums, int &eleNums) = 0;
	virtual TStatus STDCALL GetNodes(double *nodes) = 0;
	virtual TStatus STDCALL GetElements(int *elements) = 0;
};

// {A84BE730-57AF-4e9f-9494-0AF2CB52E55E}
const MUID CLSID_IDrawbeadLineSet = 
{ 0xa84be730, 0x57af, 0x4e9f, { 0x94, 0x94, 0xa, 0xf2, 0xcb, 0x52, 0xe5, 0x5e } };

} // namespace comx
} // namespace Die_maker
} // namespace KMAS


#endif