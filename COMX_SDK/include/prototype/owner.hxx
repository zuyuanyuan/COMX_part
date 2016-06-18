/*HEAD owner HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_OWNER_H_200751884612
#define _KMAS_COMX_OWNER_H_200751884612

#include <base\root.hxx>


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {C7AE82B5-66A2-4cf4-8429-5B40F366D1B3}
 const MUID IID_IOwner = 
{ 0xc7ae82b5, 0x66a2, 0x4cf4, { 0x84, 0x29, 0x5b, 0x40, 0xf3, 0x66, 0xd1, 0xb3 } };

interface IOwner : public IRoot
{
     virtual TStatus STDCALL OwnerTest() = 0;
};

// {D722185A-20B9-4cfc-9E3D-735B3E602787}
 const MTYPEID CLSID_IOwner = 
{ 0xd722185a, 0x20b9, 0x4cfc, { 0x9e, 0x3d, 0x73, 0x5b, 0x3e, 0x60, 0x27, 0x87 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

