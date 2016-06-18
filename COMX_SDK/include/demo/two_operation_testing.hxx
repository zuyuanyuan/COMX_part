/*HEAD two_operation_testing HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_TWO_OPERATION_TESTING_H_2008428016
#define _KMAS_COMX_TWO_OPERATION_TESTING_H_2008428016

#include "base\root.hxx"


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {D2DF60E2-24FA-4e67-B9EB-3FFEE8954286}
 const MUID IID_ITwoOperationTesting = 
{ 0xd2df60e2, 0x24fa, 0x4e67, { 0xb9, 0xeb, 0x3f, 0xfe, 0xe8, 0x95, 0x42, 0x86 } };

interface ITwoOperationTesting : public IRoot
{
     virtual TStatus STDCALL Execute() = 0;
};

// {52C46E53-55C0-47d0-AD9F-E309AA9D6E74}
 const MTYPEID CLSID_ITwoOperationTesting = 
{ 0x52c46e53, 0x55c0, 0x47d0, { 0xad, 0x9f, 0xe3, 0x9, 0xaa, 0x9d, 0x6e, 0x74 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

