/*HEAD projection_serialize HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date            Name            Description of Change
2007/03/31		Zheng Guojun	Create
$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_PROJECTION_SERIALIZE_H_20071272655
#define _KMAS_COMX_PROJECTION_SERIALIZE_H_20071272655

#include "base\root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {E12CAFCF-CF71-4f4d-BD8E-403C6D7B2C85}
const MUID IID_IProjectionSerialize = 
{ 0x2ae5f812, 0x6563, 0x4874, { 0x9d, 0x73, 0xf7, 0x2e, 0xea, 0x33, 0x3c, 0x32 } };

interface IProjectionSerialize : public IRoot
{
	 virtual TStatus STDCALL SaveToProjection() = 0;
	 virtual TStatus STDCALL ReadFromProjection() = 0;
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

