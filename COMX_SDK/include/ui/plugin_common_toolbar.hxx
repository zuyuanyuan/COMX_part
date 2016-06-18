/*HEAD plugin_common HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_PLUGIN_COMMON_H_200611183451
#define _KMAS_COMX_PLUGIN_COMMON_H_200611183451

#include "base\root.hxx"


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {E3AB4DF4-D0B7-47f9-80EE-698AD9850294}
 const MUID IID_IPluginCommonToolbarFileEvent = 
{ 0xe3ab4df4, 0xd0b7, 0x47f9, { 0x80, 0xee, 0x69, 0x8a, 0xd9, 0x85, 0x2, 0x94 } };

interface IPluginCommonToolbarFileEvent : public IRoot
{
     virtual TStatus STDCALL OnOpen() = 0;
     virtual TStatus STDCALL OnSave() = 0;
	   virtual TStatus STDCALL OnPrint() = 0;
};

// {A0B35FD9-6B84-4ea4-97E6-552EE0B25282}
 const MUID IID_IPluginCommonToolbarEditEvent = 
{ 0xa0b35fd9, 0x6b84, 0x4ea4, { 0x97, 0xe6, 0x55, 0x2e, 0xe0, 0xb2, 0x52, 0x82 } };

interface IPluginCommonToolbarEditEvent : public IRoot
{
     virtual TStatus STDCALL OnCut() = 0;
     virtual TStatus STDCALL OnCopy() = 0;
     virtual TStatus STDCALL OnPaste() = 0;
};

// {E9B5F192-EA08-4766-B6BC-89DAF551F9B2}
 const MTYPEID CLSID_IPluginCommonToolbar = 
{ 0xe9b5f192, 0xea08, 0x4766, { 0xb6, 0xbc, 0x89, 0xda, 0xf5, 0x51, 0xf9, 0xb2 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

