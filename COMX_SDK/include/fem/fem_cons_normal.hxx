/*HEAD fem_cons_normal HXX COMX_COMPONENT_MODULE */

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
2007/03/19      Zheng guojun    Create
$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_FEM_CONS_NORMAL_H_200731962038
#define _KMAS_COMX_FEM_CONS_NORMAL_H_200731962038

#include "base\root.hxx"

#include "fem\fem_core.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {72C91C73-471A-4e8f-8C4A-BF7C1400E256}
const MUID IID_IFemConsNormal = 
{ 0x72c91c73, 0x471a, 0x4e8f, { 0x8c, 0x4a, 0xbf, 0x7c, 0x14, 0x0, 0xe2, 0x56 } };

interface IFemConsNormal : public IRoot
{
     virtual TStatus STDCALL ConsNormal(IFemCore *&pFemCore) = 0;
     virtual TStatus STDCALL ReverseNormal(
         IFemCore *&pFemCore, 
         bool sameToRefe, 
         const fem::TNormal &refeNormal) = 0;
};

// {B2177542-64BE-4cf6-8CE7-45A40B65813D}
const MUID IID_IFemConsNormal2 = 
{ 0xb2177542, 0x64be, 0x4cf6, { 0x8c, 0xe7, 0x45, 0xa4, 0xb, 0x65, 0x81, 0x3d } };

interface IFemConsNormal2 : public IFemConsNormal
{
     virtual TStatus STDCALL EnableProgressbar(const bool flag = true) = 0;
};

// {28AEE16D-EA63-4ceb-A335-22975F9CDA61}
const MTYPEID CLSID_IFemConsNormal = 
{ 0x28aee16d, 0xea63, 0x4ceb, { 0xa3, 0x35, 0x22, 0x97, 0x5f, 0x9c, 0xda, 0x61 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif


