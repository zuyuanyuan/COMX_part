/*HEAD mesh_generate1d HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_MESH_GENERATE1D_H_20094274449
#define _KMAS_COMX_MESH_GENERATE1D_H_20094274449

#include "base\root.hxx"
#include <lists.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {A0EA8C6A-A0A3-4238-A380-F7C72D3EE77B}
 const MUID IID_IMeshGenerate1d = 
{ 0xa0ea8c6a, 0xa0a3, 0x4238, { 0xa3, 0x80, 0xf7, 0xc7, 0x2d, 0x3e, 0xe7, 0x7b } };

interface IMeshGenerate1d : public IRoot
{
     virtual TStatus STDCALL Generate(ENTITY_LIST elist) = 0;
	 virtual TStatus STDCALL Generate(ENTITY* edge, int num) = 0;
};

// {5567E840-A094-4812-9A89-2835A184F771}
 const MTYPEID CLSID_IMeshGenerate1d = 
{ 0x5567e840, 0xa094, 0x4812, { 0x9a, 0x89, 0x28, 0x35, 0xa1, 0x84, 0xf7, 0x71 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

