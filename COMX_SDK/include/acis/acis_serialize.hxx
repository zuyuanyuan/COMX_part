/*==============================================================================

  Copyright 2006 Jilin Kingmesh Corp.
  All rights reserved
  
	================================================================================ 
	File description:
	
	  -- Please append file description informations here --
	  ================================================================================
	  Date            Name                    Description of Change
	  
	$HISTORY$
================================================================================*/

#ifndef _ACIS_SERIALIZE_H
#define _ACIS_SERIALIZE_H

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TAcisSerializeIOType{
     ACIS_IO_IGES,
     ACIS_IO_STEP,
     ACIS_IO_VDA,
     ACIS_IO_CATIA,
     ACIS_IO_UG,
     ACIS_IO_PROE,
	 ACIS_IO_SAT
};

interface IAcisSerialize : public IRoot
{
     virtual TStatus STDCALL Open(const char *szfnam) = 0;
     virtual TStatus STDCALL Save(const char *szfnam) = 0;
     virtual TStatus STDCALL Import(const char *szfnam , const TAcisSerializeIOType &type) = 0;
     virtual TStatus STDCALL Export(const char *szfnam , const TAcisSerializeIOType &type) = 0; 
};

// {254AE833-929D-4862-A3FF-655967CE2539}
const MUID IID_IAcisSerialize = 
{ 0x254ae833, 0x929d, 0x4862, { 0xa3, 0xff, 0x65, 0x59, 0x67, 0xce, 0x25, 0x39 } };

// {b4cc07fa-de85-4c87-a7d6-dba8feb526bc}
// const MUID IID_IAcisSerialize2 =
// {0xb4cc07fa, 0xde85, 0x4c87, {0xa7, 0xd6, 0xdb, 0xa8, 0xfe, 0xb5, 0x26, 0xbc } };
// 
// interface IAcisSerialize2 : public IAcisSerialize
// {
// 	virtual TStatus STDCALL ExportStepFileSequence(const char *szfnam) = 0;
// };

// {A28CC2BB-916F-47b2-9702-A8494C65C749}
const MUID CLSID_IAcisSerialize = 
{ 0xa28cc2bb, 0x916f, 0x47b2, { 0x97, 0x2, 0xa8, 0x49, 0x4c, 0x65, 0xc7, 0x49 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif