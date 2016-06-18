/*HEAD plugin_fem_appearance HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_UI_APPEARANCE_H_20061122135953
#define _KMAS_COMX_UI_APPEARANCE_H_20061122135953

#include "base\root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {60BEB5B8-5159-4ac2-B875-53839E216144}
 const MUID IID_IUiAppearance = 
{ 0x60beb5b8, 0x5159, 0x4ac2, { 0xb8, 0x75, 0x53, 0x83, 0x9e, 0x21, 0x61, 0x44 } };

interface IUiAppearance : public IRoot
{
	virtual TStatus STDCALL SyncStatusToModel() = 0;
};

// {AAC6446D-A980-4398-ABBD-BB2F1D313858}
 const MTYPEID CLSID_IUiAppearance = 
{ 0xaac6446d, 0xa980, 0x4398, { 0xab, 0xbd, 0xbb, 0x2f, 0x1d, 0x31, 0x38, 0x58 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

