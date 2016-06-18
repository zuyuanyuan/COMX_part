/*HEAD plugin_dlg_post_player HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_PLUGIN_DLG_POST_PLAYER_H_20077625719
#define _KMAS_COMX_PLUGIN_DLG_POST_PLAYER_H_20077625719

#include "base\root.hxx"


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {D77EAF77-4E8B-46dd-9693-74FD43656850}
const MUID IID_IPluginDlgPostPlayer = 
{ 0xd77eaf77, 0x4e8b, 0x46dd, { 0x96, 0x93, 0x74, 0xfd, 0x43, 0x65, 0x68, 0x50 } };

interface IPluginDlgPostPlayer : public IRoot
{
	 virtual TStatus STDCALL Activate() = 0;
     virtual TStatus STDCALL Closuer() = 0;
     virtual TStatus STDCALL SetPosition(int x, int y) = 0;
     virtual TStatus STDCALL GetPosition(int& x, int& y) = 0;
	 virtual TStatus STDCALL RefreshScreen() = 0;
};

// {9C914C0B-2C24-42c0-9B75-D8D27673814A}
const MTYPEID CLSID_IPluginDlgPostPlayer = 
{ 0x9c914c0b, 0x2c24, 0x42c0, { 0x9b, 0x75, 0xd8, 0xd2, 0x76, 0x73, 0x81, 0x4a } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

