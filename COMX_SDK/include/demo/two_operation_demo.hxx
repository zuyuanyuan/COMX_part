/*HEAD two_operation_demo HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_TWO_OPERATION_DEMO_H_20084272056
#define _KMAS_COMX_TWO_OPERATION_DEMO_H_20084272056

#include "base\root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TEnumDemo
{
     TYPE1		=		0,
     TYPE2		=		1,
};

struct TStructDemo
{
     int age;
};

// {3579B92B-2C98-42c5-A9A4-BEAC5665A06B}
 const MUID IID_ITwoOperationDemo = 
{ 0x3579b92b, 0x2c98, 0x42c5, { 0xa9, 0xa4, 0xbe, 0xac, 0x56, 0x65, 0xa0, 0x6b } };

interface ITwoOperationDemo : public IRoot
{
     virtual TStatus STDCALL operation2(double left, double right, double& res) = 0;
};

// {F3E78BF4-3692-4a4d-8B14-65A3668C51D4}
const MUID IID_ITwoOperationDemoEx = 
{ 0xf3e78bf4, 0x3692, 0x4a4d, { 0x8b, 0x14, 0x65, 0xa3, 0x66, 0x8c, 0x51, 0xd4 } };

interface ITwoOperationDemoEx : public ITwoOperationDemo
{
	virtual TStatus STDCALL operation2_ex() = 0;
};

// {BD81EA82-7597-4c03-B621-0BD48B03E1DC}
 const MUID IID_ITwoOperationDemoPara = 
{ 0xbd81ea82, 0x7597, 0x4c03, { 0xb6, 0x21, 0xb, 0xd4, 0x8b, 0x3, 0xe1, 0xdc } };

interface ITwoOperationDemoPara : public IRoot
{
     virtual TStatus STDCALL SetEfficient(double val) = 0;
};

// {4A45DBFC-A5D5-415c-98F8-65C7475024E3}
 const MTYPEID CLSID_ITwoOperationDemo = 
{ 0x4a45dbfc, 0xa5d5, 0x415c, { 0x98, 0xf8, 0x65, 0xc7, 0x47, 0x50, 0x24, 0xe3 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

