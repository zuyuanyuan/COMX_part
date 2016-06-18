/*HEAD ui_acis_selection_toolkit HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_UI_ACIS_SELECTION_TOOLKIT_H_200852821636
#define _KMAS_COMX_UI_ACIS_SELECTION_TOOLKIT_H_200852821636

#include "base\root.hxx"
#include "base\factory.hxx"
#include "acis\acis_core.hxx"

#include <base\tag_util.hxx>
#include <base\tag_code.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum DLG_INIT_POSITION
{
     DLG_INIT_POS_LEFT_TOP			=		1,
     DLG_INIT_POS_LEFT_DOWN			=		2,
     DLG_INIT_POS_RIGHT_TOP			=		3,
     DLG_INIT_POS_RIGHT_DOWN		=		4
};

enum TUiAcisMaskType
{
     ACIS_CONTENT_ANY_MASK		    =		0x0001,
     ACIS_CONTENT_POINT_MASK		=		0x0002,
     ACIS_CONTENT_VERTEX_MASK	    =		0x0004,
     ACIS_CONTENT_CURVE_MASK		=		0x0008,
	 ACIS_CONTENT_EDGE_MASK		    =		0x0010,
	 ACIS_CONTENT_FACE_MASK		    =		0x0020,
     ACIS_OPERATION_POINT_MASK		=		0x0040,
     ACIS_OPERATION_RECTANGLE_MASK	=		0x0080,
     ACIS_OPERATION_POLYGON_MASK	=		0x0100,
     ACIS_RANGE_MULTI_MASK			=		0x0200,
     ACIS_RANGE_SINGLE_MASK			=		0x0400,
	 ACIS_CONTENT_POINT_ON_FACE     =       0x0800,
	 ACIS_CONTENT_CURVE_ON_FACE     =       0X1000
};

const int DEFAULT_ACIS_PICKUP_INIT_MASK = 
     ACIS_CONTENT_ANY_MASK|
     ACIS_CONTENT_POINT_MASK|
     ACIS_CONTENT_VERTEX_MASK|
     ACIS_CONTENT_CURVE_MASK|
	 ACIS_CONTENT_EDGE_MASK|
	 ACIS_CONTENT_FACE_MASK|
	 ACIS_CONTENT_POINT_ON_FACE|
	 ACIS_CONTENT_CURVE_ON_FACE|
     ACIS_OPERATION_POINT_MASK|
     ACIS_OPERATION_RECTANGLE_MASK|
     ACIS_OPERATION_POLYGON_MASK|
     ACIS_RANGE_MULTI_MASK|
     ACIS_RANGE_SINGLE_MASK;

const int DEFAULT_ACIS_PICKUP_CONTENT_MASK = 
	 ACIS_CONTENT_ANY_MASK|
     ACIS_CONTENT_POINT_MASK|
     ACIS_CONTENT_VERTEX_MASK|
     ACIS_CONTENT_CURVE_MASK|
	 ACIS_CONTENT_EDGE_MASK|
	 ACIS_CONTENT_FACE_MASK|
	 ACIS_CONTENT_POINT_ON_FACE|
	 ACIS_CONTENT_CURVE_ON_FACE;

const int DEFAULT_ACIS_PICKUP_OPERATION_MASK = 
	 ACIS_OPERATION_POINT_MASK|
     ACIS_OPERATION_RECTANGLE_MASK;

const int DEFAULT_ACIS_PICKUP_RANGE_MASK = 
     ACIS_RANGE_MULTI_MASK|
     ACIS_RANGE_SINGLE_MASK;

// {31A55662-318C-49fb-AAF6-B1EAC5577DC8}
const MUID IID_IUiAcisSelectionToolkit = 
{ 0x31a55662, 0x318c, 0x49fb, { 0xaa, 0xf6, 0xb1, 0xea, 0xc5, 0x57, 0x7d, 0xc8 } };

interface IUiAcisSelectionToolkit : public IRoot
{
     virtual TStatus STDCALL Activate(int init_mask, bool dlg_show = true) = 0;
     virtual TStatus STDCALL SetPosition(int x, int y) = 0;
	 virtual TStatus STDCALL SetPosition(int pos = DLG_INIT_POS_LEFT_TOP) = 0;
     virtual TStatus STDCALL GetPosition(int& x, int& y) = 0;
	 virtual TStatus STDCALL GetPosition(int &pos) = 0;
     virtual TStatus STDCALL Closure() = 0;
     virtual TStatus STDCALL SetVisualMask(int mask) = 0;
     virtual TStatus STDCALL GetVisualMask(int& mask) = 0;
	 virtual TStatus STDCALL SetActiveMask(int mask) = 0;
	 virtual TStatus STDCALL GetActiveMask(int& mask) = 0;
     virtual TStatus STDCALL Clear() = 0;
     virtual TStatus STDCALL DisplayCheckRet(bool dis) = 0;
     virtual TStatus STDCALL GetPickupResult(tag_t *&res, int &cnt) = 0;
};


// {774F7071-5DBB-413c-A1D0-AE4E4C5F96CB}
const MUID IID_IUiAcisSelectionToolkitNotify = 
{ 0x774f7071, 0x5dbb, 0x413c, { 0xa1, 0xd0, 0xae, 0x4e, 0x4c, 0x5f, 0x96, 0xcb } };

interface IUiAcisSelectionToolkitNotify : public IRoot
{
	virtual TStatus STDCALL PickupNotify() = 0;
};

// {52FA0B89-3D9D-4573-AECF-14D4C0040CB5}
const MTYPEID CLSID_IUiAcisSelectionToolkit = 
{ 0x52fa0b89, 0x3d9d, 0x4573, { 0xae, 0xcf, 0x14, 0xd4, 0xc0, 0x4, 0xc, 0xb5 } };

enum TACISPickupManagerType
{
	ACIS_PICKUP_INIT = 0,
	ACIS_PICKUP_EXIT = 1
};

static IUiAcisSelectionToolkit* ManagerACISPickup(
	IAcisCore *p_acis_core,
	TACISPickupManagerType type,
	IUiAcisSelectionToolkitNotify *p_sel_notify = NULL)
{
	static TConnectManager tm;
	
	IUiAcisSelectionToolkit *p_pickup_toolkit = NULL;
	TFactory factory;
	factory.QueryService(
		CLSID_IUiAcisSelectionToolkit, 
		IID_IUiAcisSelectionToolkit, 
		(void**)&p_pickup_toolkit);
	assert(p_pickup_toolkit);
	
	if (type == ACIS_PICKUP_INIT)
	{
		tm.ReleaseConnects();
		p_pickup_toolkit->Closure();
		tm.AddConnect(p_pickup_toolkit, IID_IAcisCore, p_acis_core);
		if (p_sel_notify != NULL)
		{
		    tm.AddConnect(
				p_pickup_toolkit, 
				IID_IUiAcisSelectionToolkitNotify, 
				p_sel_notify);
		}
	}
	else if (type == ACIS_PICKUP_EXIT)
	{
		tm.ReleaseConnects();
		p_pickup_toolkit->Closure();
	}
	
	p_pickup_toolkit->Release();
	
	return p_pickup_toolkit;
}

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

