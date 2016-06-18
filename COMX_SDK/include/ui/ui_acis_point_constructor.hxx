/*HEAD ui_acis_point_constructor HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date            Name            Description of Change
2006/06/03		Zheng Guojun	Create...
$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_UI_ACIS_POINT_CONSTRUCTOR_H_20086375523
#define _KMAS_COMX_UI_ACIS_POINT_CONSTRUCTOR_H_20086375523

#include "base\root.hxx"
#include "base\factory.hxx"
#include "acis\acis_core.hxx"
#include "position.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {E62E65B6-B767-41ed-86CD-3E666F307DF6}
 const MUID IID_IUiAcisPointConstructor = 
{ 0xe62e65b6, 0xb767, 0x41ed, { 0x86, 0xcd, 0x3e, 0x66, 0x6f, 0x30, 0x7d, 0xf6 } };

interface IUiAcisPointConstructor : public IRoot
{
	virtual TStatus STDCALL Activate(bool dlg_show = true) = 0;
	virtual TStatus STDCALL SetPosition(int pos) = 0;
	virtual TStatus STDCALL GetPosition(int& pos) = 0;
	virtual TStatus STDCALL Closure() = 0;
	virtual TStatus STDCALL AddPreviewPoint(SPAposition pt) = 0;
	virtual TStatus STDCALL ClearPreviewPoints() = 0;
	virtual TStatus STDCALL AddPoint(SPAposition pt) = 0;
	virtual TStatus STDCALL DeletePoint(const ENTITY_LIST &elist) = 0;
};

// {F848D7A2-1B27-4ee7-952C-9EA306FF40E1}
 const MTYPEID CLSID_IUiAcisPointConstructor = 
{ 0xf848d7a2, 0x1b27, 0x4ee7, { 0x95, 0x2c, 0x9e, 0xa3, 0x6, 0xff, 0x40, 0xe1 } };


enum TACISPointConstructorManagerType
{
	ACIS_POINT_CONSTRUCTOR_INIT = 0,
	ACIS_POINT_CONSTRUCTOR_EXIT = 1
};

static IUiAcisPointConstructor* ManagerACISPointConstructor(IAcisCore *p_acis_core, TACISPointConstructorManagerType type)
{
	static TConnectManager tm;
	
	IUiAcisPointConstructor *p_point_constructor = NULL;
	TFactory factory;
	factory.QueryService(CLSID_IUiAcisPointConstructor, IID_IUiAcisPointConstructor, (void**)&p_point_constructor);
	assert(p_point_constructor);
	
	if (type == ACIS_POINT_CONSTRUCTOR_INIT)
	{
		tm.ReleaseConnects();
		p_point_constructor->Closure();
		tm.AddConnect(p_point_constructor, IID_IAcisCore, p_acis_core);
	}
	else if (type == ACIS_POINT_CONSTRUCTOR_EXIT)
	{
		tm.ReleaseConnects();
		p_point_constructor->Closure();
	}
	
	p_point_constructor->Release();
	
	return p_point_constructor;
}


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

