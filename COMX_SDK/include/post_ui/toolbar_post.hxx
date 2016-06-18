/*HEAD toolbar_post HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_TOOLBAR_POST_H_200761882058
#define _KMAS_COMX_TOOLBAR_POST_H_200761882058

#include "base\root.hxx"


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {07E45C9C-15D2-437a-85E7-B63922F2B55F}
 const MUID IID_IToolbarPost = 
{ 0x7e45c9c, 0x15d2, 0x437a, { 0x85, 0xe7, 0xb6, 0x39, 0x22, 0xf2, 0xb5, 0x5f } };

interface IToolbarPost : public IRoot
{
};

// {1441582C-5426-4265-9526-8C665202CAA6}
 const MTYPEID CLSID_IToolbarPost = 
{ 0x1441582c, 0x5426, 0x4265, { 0x95, 0x26, 0x8c, 0x66, 0x52, 0x2, 0xca, 0xa6 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

