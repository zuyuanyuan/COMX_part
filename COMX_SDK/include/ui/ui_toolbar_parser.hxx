/*HEAD ui_toolbar_parser HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_UI_TOOLBAR_PARSER_H_2009101753013
#define _KMAS_COMX_UI_TOOLBAR_PARSER_H_2009101753013

#include "base\root.hxx"
#include <ui\plugin.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {4B1A8A32-AAD4-4f32-B38A-E5419E2CE437}
 const MUID IID_IUiToolbarParser = 
{ 0x4b1a8a32, 0xaad4, 0x4f32, { 0xb3, 0x8a, 0xe5, 0x41, 0x9e, 0x2c, 0xe4, 0x37 } };

interface IUiToolbarParser : public IRoot
{
     virtual TStatus STDCALL OnDispatch(const char* tbid, const int index) = 0;
};

// {A9822E9C-6ACB-48d1-90E3-8E5EC6B60EA6}
const MUID IID_IUiToolbarInfo = 
{ 0xa9822e9c, 0x6acb, 0x48d1, { 0x90, 0xe3, 0x8e, 0x5e, 0xc6, 0xb6, 0xe, 0xa6 } };

interface IUiToolbarInfo : public IRoot
{
	 virtual TStatus STDCALL GetToolbarDataByID(const char* toolbarid, TCOMXPluginToolbar *&tb_data) = 0;
};

// {2E8A0B5B-1776-4ae0-A3C6-E071B4135653}
 const MTYPEID CLSID_IUiToolbarParser = 
{ 0x2e8a0b5b, 0x1776, 0x4ae0, { 0xa3, 0xc6, 0xe0, 0x71, 0xb4, 0x13, 0x56, 0x53 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

