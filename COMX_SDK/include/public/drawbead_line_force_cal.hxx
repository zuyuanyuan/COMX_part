/*HEAD fem_constrain_node HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_DRAWBEAD_FORCE_CALCULATE_H
#define _KMAS_COMX_DRAWBEAD_FORCE_CALCULATE_H

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{
// {8E195C98-0140-4b3d-BBE7-A1F476789AD5}
const MUID IID_IDrawbaedLineForceCal = 
{ 0x8e195c98, 0x140, 0x4b3d, { 0xbb, 0xe7, 0xa1, 0xf4, 0x76, 0x78, 0x9a, 0xd5 } };

interface IDrawbaedLineForceCal : public IRoot
{
	virtual TStatus STDCALL CalCircinal(float (&input)[11], float &FRestrain, float &FOpen) = 0;
	virtual TStatus STDCALL CalRectangular(float (&input)[12], float &FRestrain, float &FOpen) = 0;
};

// {1C583C53-72BC-4f0e-9436-44F2C9450883}
const MUID CLSID_IDrawbaedLineForceCal = 
{ 0x1c583c53, 0x72bc, 0x4f0e, { 0x94, 0x36, 0x44, 0xf2, 0xc9, 0x45, 0x8, 0x83 } };

} // namespace comx
} // namespace Die_maker
} // namespace KMAS


#endif