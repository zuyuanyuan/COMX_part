/*HEAD ui_meshes_selection_toolkit HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date            Name            Description of Change
2006/11/13		ZhengGuojun		Add Getposition(int &x, int &y);
$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_UI_MESHES_SELECTION_TOOLKIT_H_20061189342
#define _KMAS_COMX_UI_MESHES_SELECTION_TOOLKIT_H_20061189342

#include "base\root.hxx"
#include "base\factory.hxx"
#include "fem\fem_core.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TUiMeshesSMContentType
{
     MESHES_SM_NODE		=		0,
     MESHES_SM_ELEMENT	=		1,
};

enum TUiMeshesSMOperationType
{
     MESHES_SM_POINT		=		0,
     MESHES_SM_RECTANGLE	=		1,
     MESHES_SM_POLYGON		=		2,
};

enum TUiMeshesSMRangeType
{
     MESHES_SM_SINGLE		=		0,
     MESHES_SM_MULTI		=		1,
};

enum TUiMeshesSMMaskType
{
	MESHES_SM_NODE_MASK			=		0x0001,
	MESHES_SM_ELEMENT_MASK		=       0x0002,
	MESHES_SM_POINT_MASK		=       0x0004,
	MESHES_SM_RECTANGLE_MASK	=       0x0008,
	MESHES_SM_POLYGON_MASK		=       0x0010,
	MESHES_SM_SINGLE_MASK		=       0x0020,
	MESHES_SM_MULTI_MASK		=       0x0040,
	MESHES_SM_CONJUNCTION_MASK	=		0x0080,
	MESHES_SM_LIMIT_COUNTS_MASK	=		0x0100,
};

struct TUiMeshesSMStatusType
{
     TUiMeshesSMContentType   content_type;
     TUiMeshesSMRangeType     range_type;
     TUiMeshesSMOperationType operation_type;
     
     bool                     is_associate;
     double                   angle;
	 bool					  is_limit_counts;
	 int					  limit_counts;
};

// {801546DC-AF24-4300-9B12-79E2E0FF67A4}
 const MUID IID_IUiMeshesSelectionToolkit = 
{ 0x801546dc, 0xaf24, 0x4300, { 0x9b, 0x12, 0x79, 0xe2, 0xe0, 0xff, 0x67, 0xa4 } };

interface IUiMeshesSelectionToolkit : public IRoot
{
     virtual TStatus STDCALL Activate(TUiMeshesSMContentType ct, TUiMeshesSMRangeType rt, TUiMeshesSMOperationType ot) = 0;
     virtual TStatus STDCALL SetPosition(const int& x, const int& y) = 0;
	 virtual TStatus STDCALL GetPosition(int& x, int& y) = 0;
     virtual TStatus STDCALL Closure() = 0;
     virtual TStatus STDCALL GetPickupResult(int*& res, int& rlen) = 0;
	 virtual TStatus STDCALL SetMask(const int mask) = 0;
	 virtual TStatus STDCALL GetMask(int &mask) = 0;
	 virtual TStatus STDCALL SetConjunctionAngle(const double angle) = 0;
	 virtual TStatus STDCALL SetLimitCounts(const int& couts) = 0;
	 virtual TStatus STDCALL GetStatus(TUiMeshesSMStatusType &status) = 0;
	 virtual TStatus STDCALL SetStatus(const TUiMeshesSMStatusType &status) = 0;
	 virtual TStatus STDCALL Clear() = 0;
	 virtual TStatus STDCALL Reverse() = 0;
	 virtual TStatus STDCALL DisplayCheckRet(bool display = true) = 0;
};

// {DA9E016C-D8F2-4389-B387-BD1C2BD75091}
const MUID IID_IUiMeshesSelectionToolkit2 = 
{ 0xda9e016c, 0xd8f2, 0x4389, { 0xb3, 0x87, 0xbd, 0x1c, 0x2b, 0xd7, 0x50, 0x91 } };

interface IUiMeshesSelectionToolkit2 : public IUiMeshesSelectionToolkit
{
    virtual TStatus STDCALL EnablePaint(bool flag = true) = 0;
};

// {BB2F90B0-13AA-40d6-B8E0-40C342AB1FF3}
 const MUID IID_IUiMeshesSelectionToolkitNotify = 
{ 0xbb2f90b0, 0x13aa, 0x40d6, { 0xb8, 0xe0, 0x40, 0xc3, 0x42, 0xab, 0x1f, 0xf3 } };

interface IUiMeshesSelectionToolkitNotify : public IRoot
{
     virtual TStatus STDCALL PickupNotify() = 0;
};

// {54475D46-D00F-4d7a-8E68-59D302CD7C67}
 const MTYPEID CLSID_IUiMeshesSelectionToolkit = 
{ 0x54475d46, 0xd00f, 0x4d7a, { 0x8e, 0x68, 0x59, 0xd3, 0x2, 0xcd, 0x7c, 0x67 } };

 enum TFemPickupManagerType
 {
	 FEM_PICKUP_INIT = 0,
	 FEM_PICKUP_EXIT = 1
 };

 static IUiMeshesSelectionToolkit* ManagerFemPickup(IFemCore *p_fem_core, TFemPickupManagerType type)
 {
	 static DWORD dwCookie = 0;

	 IUiMeshesSelectionToolkit *p_pickup_toolkit = NULL;
	 TFactory factory;
	 factory.QueryService(CLSID_IUiMeshesSelectionToolkit, IID_IUiMeshesSelectionToolkit, (void**)&p_pickup_toolkit);
	 assert(p_pickup_toolkit);

	 if (type == FEM_PICKUP_INIT)
	 {
		 if (dwCookie != 0)
			{
				KMAS::Die_maker::comx::disconnect(p_pickup_toolkit, IID_IFemCore, dwCookie);
				p_pickup_toolkit->Closure();
				dwCookie = 0;
			}
			KMAS::Die_maker::comx::connect(p_pickup_toolkit, p_fem_core, IID_IFemCore, dwCookie);
	 }
	 else if (type == FEM_PICKUP_EXIT)
	 {
		 KMAS::Die_maker::comx::disconnect(p_pickup_toolkit, IID_IFemCore, dwCookie);
		 p_pickup_toolkit->Closure();
		 dwCookie = 0;
	 }
	 else
	 {
		 assert(false);
	 }

	 p_pickup_toolkit->Release();

	 return p_pickup_toolkit;
 }

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

