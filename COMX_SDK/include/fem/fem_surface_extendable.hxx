/*HEAD fem_surface_extendable HXX ONESTEP */
/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --
A dialog about check and modify overlap condition .
================================================================================
Date            Name                    Description of Change
                Zhang Xiang-kui 	      Create

$HISTORY$
================================================================================*/
#ifndef _KMAS_COMX_FEM_SURFACE_EXTENDABLE_DECLARE_H
#define _KMAS_COMX_FEM_SURFACE_EXTENDABLE_DECLARE_H

#include <base/root.hxx>
#include <fem/fem_base.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

typedef vector<int>::iterator TExtendableElementsIterator;
typedef vector<int>::iterator TExtendableNodesIterator;

// {F5B578B7-2ACA-4e00-A7F0-C417DB42CFCC}
const MUID IID_IFemSurfaceExtendableUtil = 
{ 0xf5b578b7, 0x2aca, 0x4e00, { 0xa7, 0xf0, 0xc4, 0x17, 0xdb, 0x42, 0xcf, 0xcc } };

interface IFemSurfaceExtendableUtil : public IRoot
{
     virtual TStatus STDCALL GetExtendableElement(fem::POSITION pos, TExtendableElementsIterator &ee_iter) = 0;
     virtual TStatus STDCALL GetExtendableNode(fem::POSITION pos, TExtendableNodesIterator &en_iter) = 0;
     virtual TStatus STDCALL IsExtendable(bool &flag) = 0;
};

// {A31B20E7-E933-4024-B035-DA4885CC5844}
const MUID CLSID_IFemSurfaceExtendableUtil = 
{ 0xa31b20e7, 0xe933, 0x4024, { 0xb0, 0x35, 0xda, 0x48, 0x85, 0xcc, 0x58, 0x44 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif