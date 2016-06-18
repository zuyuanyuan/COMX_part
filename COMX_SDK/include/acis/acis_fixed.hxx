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

#ifndef _KMAS_COMX_ACIS_FIXED
#define _KMAS_COMX_ACIS_FIXED

#include <base/root.hxx>
#include <lists.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

interface IAcisFixed : public IRoot
{
     virtual TStatus STDCALL ExecuteFixed(int &error_numnber, char *log_file = NULL) = 0;
};

// {2CE88D05-685B-47a4-B229-C3A9C7C580B9}
const MUID IID_IAcisFixed = 
{ 0x2ce88d05, 0x685b, 0x47a4, { 0xb2, 0x29, 0xc3, 0xa9, 0xc7, 0xc5, 0x80, 0xb9 } };

// {5C52A210-204E-4800-B240-089580E8556B}
const MUID CLSID_IAcisFixed = 
{ 0x5c52a210, 0x204e, 0x4800, { 0xb2, 0x40, 0x8, 0x95, 0x80, 0xe8, 0x55, 0x6b } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif