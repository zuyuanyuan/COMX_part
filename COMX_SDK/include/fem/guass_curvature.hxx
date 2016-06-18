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
#ifndef _KMAS_COMX_GUASS_CURVATURE_DECLARE_H
#define _KMAS_COMX_GUASS_CURVATURE_DECLARE_H

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

interface IGuassCurvature : public IRoot
{
     virtual TStatus STDCALL Update() = 0;
     virtual TStatus STDCALL AutoUpdate(const bool &) = 0;
     virtual TStatus STDCALL GetValue(const int elem_id , double &) = 0;
     virtual TStatus STDCALL GetRawValue(const int elem_id , double &) = 0;
     virtual TStatus STDCALL GetMaxValue(double &) = 0;
     virtual TStatus STDCALL GetMaxRawValue(double &) = 0;
};

// {6CE17D72-C718-4b3e-9013-77D2DEDBC17E}
const MUID IID_IGuassCurvature = 
{ 0x6ce17d72, 0xc718, 0x4b3e, { 0x90, 0x13, 0x77, 0xd2, 0xde, 0xdb, 0xc1, 0x7e } };

// {3885F120-19EE-4e69-ABB0-24C7F525B507}
const MUID CLSID_IGuassCurvature = 
{ 0x3885f120, 0x19ee, 0x4e69, { 0xab, 0xb0, 0x24, 0xc7, 0xf5, 0x25, 0xb5, 0x7 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
