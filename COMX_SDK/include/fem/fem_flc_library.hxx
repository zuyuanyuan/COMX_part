/*HEAD fem_flc_library HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_FEM_FLC_LIBRARY_H_2007331153429
#define _KMAS_COMX_FEM_FLC_LIBRARY_H_2007331153429

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {D20B32CD-AD1B-4a7b-B5DA-2D841CA6BBEA}
const MUID IID_IFemFlcLibrary = 
{ 0xd20b32cd, 0xad1b, 0x4a7b, { 0xb5, 0xda, 0x2d, 0x84, 0x1c, 0xa6, 0xbb, 0xea } };

interface IFemFlcLibrary : public IRoot
{
     virtual TStatus STDCALL Activate() = 0;
     virtual TStatus STDCALL Closure() = 0;
     virtual TStatus STDCALL GetCurrentFLCCurve(double*& points, int &num) = 0;
	 virtual TStatus STDCALL SetCurrentFLCCurve(double* points, int num) = 0;
	 virtual TStatus STDCALL SetPointsVisible(const bool flag) = 0;
	 virtual TStatus STDCALL AddPoints(const double *e1, const double *e2, const int num) = 0;

	 virtual TStatus STDCALL SetCurrentSafeCurve(double* points, int num) = 0;
	 virtual TStatus STDCALL GetCurrentSafeCurve(double*& points, int &num) = 0;
	 virtual TStatus STDCALL SetSafeFact(const double safe_fact) = 0;
	 virtual TStatus STDCALL GetSafeFact( double &safe_fact) = 0;
};

// {3C429D9C-34E3-4522-89DF-73C4A3A9489A}
const MUID IID_IFemFLDGrade = 
{ 0x3c429d9c, 0x34e3, 0x4522, { 0x89, 0xdf, 0x73, 0xc4, 0xa3, 0xa9, 0x48, 0x9a } };

enum TFemFLDGrade
{
	FLD_CRACK = 3,
	FLD_CRACK_RISK = 2,
	FLD_SAFE = 1,
	FLD_INSUFFICIENT = 0,
	FLD_WRINKLE = 5,
	FLD_WRINKLE_RISK = 4
};

interface IFemFLDGrade : public IRoot
{
     virtual TStatus STDCALL GetElementGrade(const double e1, const double e2, TFemFLDGrade& grade) = 0;
     virtual TStatus STDCALL GetElementsGrade(const double* e1, const double* e2, const int num, TFemFLDGrade* grade) = 0;
};

// {BEE4F473-0494-44e2-BB02-C02FE57E8E7B}
const MTYPEID CLSID_IFemFlcLibrary = 
{ 0xbee4f473, 0x494, 0x44e2, { 0xbb, 0x2, 0xc0, 0x2f, 0xe5, 0x7e, 0x8e, 0x7b } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

