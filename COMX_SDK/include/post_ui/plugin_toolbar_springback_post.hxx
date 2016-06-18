/*HEAD plugin_toolbar_springback_post HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_PLUGIN_TOOLBAR_SPRINGBACK_POST_H_200837799
#define _KMAS_COMX_PLUGIN_TOOLBAR_SPRINGBACK_POST_H_200837799

#include <base/root.hxx>


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {C0ED872A-FDB9-426b-ABDF-0CAF91ABE979}
 const MUID IID_IPluginToolbarSpringbackPost = 
{ 0xc0ed872a, 0xfdb9, 0x426b, { 0xab, 0xdf, 0xc, 0xaf, 0x91, 0xab, 0xe9, 0x79 } };

interface IPluginToolbarSpringbackPost : public IRoot
{
	virtual TStatus STDCALL ShowOrHideToolbarIcon( bool show_hide_flag ) = 0;
};

// {247535A5-A757-41e1-929C-BAD841D884F0}
 const MTYPEID CLSID_IPluginToolbarSpringbackPost = 
{ 0x247535a5, 0xa757, 0x41e1, { 0x92, 0x9c, 0xba, 0xd8, 0x41, 0xd8, 0x84, 0xf0 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

