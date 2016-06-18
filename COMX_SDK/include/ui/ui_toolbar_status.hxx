/*HEAD ui_common HXX COMX_COMPONENT_MODULE */

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
#ifndef _KMAS_COMX_UI_COMMON_H
#define _KMAS_COMX_UI_COMMON_H

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {7726D910-ED74-4828-BEFA-8BB2D38F531A}
const MUID IID_IUiToolbarStatus = 
{ 0x7726d910, 0xed74, 0x4828, { 0xbe, 0xfa, 0x8b, 0xb2, 0xd3, 0x8f, 0x53, 0x1a } };

interface IUiToolbarStatus : public IRoot
{
     virtual TStatus STDCALL UpdateUI() = 0;
};

// {75D4B0A5-F7FD-45ba-862B-45968504219C}
const MUID IID_IUiToolbarStatusDriver = 
{ 0x75d4b0a5, 0xf7fd, 0x45ba, { 0x86, 0x2b, 0x45, 0x96, 0x85, 0x4, 0x21, 0x9c } };

interface IUiToolbarStatusDriver : public IRoot
{
     virtual TStatus STDCALL UpdateUIDriver() = 0;
};

// {819ED4E6-8793-4721-86B0-76978EC0B002}
const MUID CLSID_IUiToolbarStatus = 
{ 0x819ed4e6, 0x8793, 0x4721, { 0x86, 0xb0, 0x76, 0x97, 0x8e, 0xc0, 0xb0, 0x2 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
