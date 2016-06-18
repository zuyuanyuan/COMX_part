/*HEAD base HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_BASE_H_200751313713
#define _KMAS_COMX_BASE_H_200751313713

#include <base\root.hxx>


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {E8737B38-0499-4e0c-8480-F3EE4C97C375}
 const MUID IID_IBase = 
{ 0xe8737b38, 0x499, 0x4e0c, { 0x84, 0x80, 0xf3, 0xee, 0x4c, 0x97, 0xc3, 0x75 } };

interface IBase : public IRoot
{
     virtual TStatus STDCALL demo() = 0;
};

// {4D8F0622-D90D-4f51-9101-DB176E306AE6}
const MUID EID_Base = 
{ 0x4d8f0622, 0xd90d, 0x4f51, { 0x91, 0x1, 0xdb, 0x17, 0x6e, 0x30, 0x6a, 0xe6 } };

// {1BEA5ED1-D1E9-4bf0-B895-0220C6ED6135}
const MUID EID_Base2 = 
{ 0x1bea5ed1, 0xd1e9, 0x4bf0, { 0xb8, 0x95, 0x2, 0x20, 0xc6, 0xed, 0x61, 0x35 } };

// {358BB377-925B-4b87-B493-BEA27A16CBD8}
 const MTYPEID CLSID_IBase = 
{ 0x358bb377, 0x925b, 0x4b87, { 0xb4, 0x93, 0xbe, 0xa2, 0x7a, 0x16, 0xcb, 0xd8 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

