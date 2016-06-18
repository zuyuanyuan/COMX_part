/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --
A dialog about check and modify overlap condition .
================================================================================
Date            Name                    Description of Change
                Zhang Xiang-kui         Create
                Zheng Guojun            Modify and add IAcisCore2 interface.
$HISTORY$
================================================================================*/

#ifndef _KMAS_ACIS_CORE_H
#define _KMAS_ACIS_CORE_H

#include <base/root.hxx>
#include <lists.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {F54A84F9-5E9E-485f-B3E1-ED83E5FCC988}
const MUID IID_IAcisCore = 
{ 0xf54a84f9, 0x5e9e, 0x485f, { 0xb3, 0xe1, 0xed, 0x83, 0xe5, 0xfc, 0xc9, 0x88 } };

			
interface IAcisCore : public IRoot
{
     virtual TStatus STDCALL RefEntityList(ENTITY_LIST *&elist) = 0;
     virtual TStatus STDCALL ModifiedNotify() = 0;
     virtual TStatus STDCALL Clear() = 0;
};

// {5DB48E08-A820-48e7-8735-6C1CA56EE142}
const MUID IID_IAcisCore2 = 
{ 0x5db48e08, 0xa820, 0x48e7, { 0x87, 0x35, 0x6c, 0x1c, 0xa5, 0x6e, 0xe1, 0x42 } };

interface IAcisCore2 : public IAcisCore
{
     virtual TStatus STDCALL ModifiedNotify(ENTITY *pEntity) = 0;
};

// {475B15E2-9D9F-4fb4-A7FB-180F3B6D0ADD}
const MUID IID_IAcisCoreNotify = 
{ 0x475b15e2, 0x9d9f, 0x4fb4, { 0xa7, 0xfb, 0x18, 0xf, 0x3b, 0x6d, 0xa, 0xdd } };

interface IAcisCoreNotify : public IRoot
{
     virtual TStatus STDCALL ModifiedEvent() = 0;
     virtual TStatus STDCALL ModifiedEvent(ENTITY *pEntity) = 0;
};			
			
// {BC66FAD5-3DD7-4d21-8951-DF8B58B7807B}
const MUID CLSID_IAcisCore = 
{ 0xbc66fad5, 0x3dd7, 0x4d21, { 0x89, 0x51, 0xdf, 0x8b, 0x58, 0xb7, 0x80, 0x7b } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
