/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --
	1. Make all of the elements' normal consistency .
	2. Make all of the elements' normal reverse with a correlative direction .
================================================================================
Date            Name            Description of Change
2007/03/19		  Zheng guojun	  Create
$HISTORY$
================================================================================
*/
#ifndef _KMAS_COMX_FEM_DISPLAY_NORMAL_H
#define _KMAS_COMX_FEM_DISPLAY_NORMAL_H

#include <fem/fem_core.hxx>
#include <fem\fem_render.hxx>

enum NORMAL_DISPLAY_TYPE
{
	USE_ARROW,
	USE_COLOR
};

namespace KMAS
{
namespace Die_maker
{
namespace comx
{
// {E8F4746B-7C5D-4b76-8025-2A7C59E61E55}
const MUID IID_IFemDisplayNormal = 
{ 0xe8f4746b, 0x7c5d, 0x4b76, { 0x80, 0x25, 0x2a, 0x7c, 0x59, 0xe6, 0x1e, 0x55 } };

interface IFemDisplayNormal : public IRoot
{
	virtual TStatus STDCALL DisplayNormal(IFemCore *&pFemCore, IFemRender *&pFemRender) = 0;
	virtual TStatus STDCALL ClearDisplay() = 0;
	virtual TStatus STDCALL DisplayType(NORMAL_DISPLAY_TYPE type) = 0;
};

// {41DC74AB-FD9B-4442-AB6F-5C60EC029FF1}
const MUID CLSID_IFemDisplayNormal = 
{ 0x41dc74ab, 0xfd9b, 0x4442, { 0xab, 0x6f, 0x5c, 0x60, 0xec, 0x2, 0x9f, 0xf1 } };

} // namespace comx
} // namespace Die_maker
} // namespace KMAS

#endif