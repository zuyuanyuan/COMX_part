/*HEAD fem_shape_heart_display HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_FEM_SHAPE_HEART_DISPLAY_H_200731421650
#define _KMAS_COMX_FEM_SHAPE_HEART_DISPLAY_H_200731421650

#include "base\root.hxx"
#include <gl/gl_common_display.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {485954F4-0C77-4701-930A-D3494A73E494}
 const MUID IID_IFemEleCenterPointDisplay = 
{ 0x485954f4, 0xc77, 0x4701, { 0x93, 0xa, 0xd3, 0x49, 0x4a, 0x73, 0xe4, 0x94 } };

interface IFemEleCenterPointDisplay : public IRoot
{
     virtual TStatus STDCALL ActiveDisplay() = 0;
     virtual TStatus STDCALL AddGroup(int* eleIds, int len, const COLOR_VALUE& color) = 0;
     virtual TStatus STDCALL Reset() = 0;
};

// {D1623FF8-BFD4-4722-AEE1-6FA27366A680}
 const MTYPEID CLSID_IFemEleCenterPointDisplay = 
{ 0xd1623ff8, 0xbfd4, 0x4722, { 0xae, 0xe1, 0x6f, 0xa2, 0x73, 0x66, 0xa6, 0x80 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

