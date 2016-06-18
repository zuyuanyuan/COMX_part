/*HEAD plugin_toolbox_atl HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_PLUGIN_TOOLBOX_ATL_H_2006112122047
#define _KMAS_COMX_PLUGIN_TOOLBOX_ATL_H_2006112122047

#include "base\root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {18F581E1-E6FC-443c-9912-44F5816F241E}
 const MUID IID_IPluginToolbox = 
{ 0x18f581e1, 0xe6fc, 0x443c, { 0x99, 0x12, 0x44, 0xf5, 0x81, 0x6f, 0x24, 0x1e } };

interface IPluginToolbox : public IRoot
{
     virtual TStatus STDCALL WarningMessageBox(const char* msg) = 0;
     virtual TStatus STDCALL ErrorMessageBox(const char* msg) = 0;
     virtual TStatus STDCALL InfoMessageBox(const char* msg) = 0;
     virtual TStatus STDCALL OpenFile(const char* filter, char* filename) = 0;
     virtual TStatus STDCALL SaveFile(const char* filter, char* filename) = 0;
};

// {5F2ED898-BC62-461d-B4AD-9BA87A700C08}
const MUID IID_IPluginToolbox2 = 
{ 0x5f2ed898, 0xbc62, 0x461d, { 0xb4, 0xad, 0x9b, 0xa8, 0x7a, 0x70, 0xc, 0x8 } };

interface IPluginToolbox2 : public IPluginToolbox
{
	virtual TStatus STDCALL FoldFile(char *&filename) = 0;
};

// {543A849A-466E-4de6-B06C-73CACB72C1D4}
 const MTYPEID CLSID_IPluginToolbox = 
{ 0x543a849a, 0x466e, 0x4de6, { 0xb0, 0x6c, 0x73, 0xca, 0xcb, 0x72, 0xc1, 0xd4 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

