/*HEAD fem_info HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_FEM_INFO_H_20105259230
#define _KMAS_COMX_FEM_INFO_H_20105259230

#include "base\root.hxx"


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {2D3D2E8F-E1D1-4f8a-BDE2-9E32CC45C747}
 const MUID IID_IFemInfo = 
{ 0x2d3d2e8f, 0xe1d1, 0x4f8a, { 0xbd, 0xe2, 0x9e, 0x32, 0xcc, 0x45, 0xc7, 0x47 } };

interface IFemInfo : public IRoot
{
     virtual TStatus STDCALL Activate(const int mode) = 0;
     virtual TStatus STDCALL Closure() = 0;
};

// {34660B6D-ACB0-445e-BD8F-3F95719B3F4D}
 const MTYPEID CLSID_IFemInfo = 
{ 0x34660b6d, 0xacb0, 0x445e, { 0xbd, 0x8f, 0x3f, 0x95, 0x71, 0x9b, 0x3f, 0x4d } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

