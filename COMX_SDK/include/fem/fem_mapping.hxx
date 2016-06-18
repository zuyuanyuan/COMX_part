/*HEAD fem_mapping HXX ONESTEP */

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

#ifndef FEM_MAPPING_H
#define FEM_MAPPING_H

#include <base\root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TMappingContentEnum
{
    POS,
    SCALAR,
    VECTOR
};

// {C6017396-B880-4cac-9952-6E31D17D411C}
const MUID IID_IFemMapping = 
{ 0xc6017396, 0xb880, 0x4cac, { 0x99, 0x52, 0x6e, 0x31, 0xd1, 0x7d, 0x41, 0x1c } };

interface IFemMapping : public IRoot
{
    virtual TStatus STDCALL Mapping(
        double*  pInputPhysical, 
        int dimension, 
        TMappingContentEnum  means, 
        double*  pOutputPhysical) = 0;
};

// thick meshes is known, and thin meshes is unknown
// {842416A3-1591-470c-867D-E6296035E26A}
const MUID IID_IFemMapping2 = 
{ 0x842416a3, 0x1591, 0x470c, { 0x86, 0x7d, 0xe6, 0x29, 0x60, 0x35, 0xe2, 0x6a } };

interface IFemMapping2 : public IFemMapping
{
	virtual TStatus STDCALL MappingScalarByElement(
		double *p_input, 
		double *p_output) = 0;
};

// {70D8C2D3-F105-4ece-9B8A-B46299B09205}
const MUID IID_IFemMeshConnect = 
{ 0x70d8c2d3, 0xf105, 0x4ece, { 0x9b, 0x8a, 0xb4, 0x62, 0x99, 0xb0, 0x92, 0x5 } };

interface IFemMeshConnect : public IRoot
{
    virtual TStatus STDCALL AttachMesh(
        IFemCore2 *&p_thick_mesh, 
        IFemCore2 *&p_thin_mesh) = 0;
    virtual TStatus STDCALL DetachMesh() = 0;
};

// {5F9C0C68-DB66-4c33-A881-B5C7C31E5F31}
const MUID CLSID_IFemMapping = 
{ 0x5f9c0c68, 0xdb66, 0x4c33, { 0xa8, 0x81, 0xb5, 0xc7, 0xc3, 0x1e, 0x5f, 0x31 } };
    

} // namespace comx
} // namespace Die_maker
} // namespace KMAS


#endif



