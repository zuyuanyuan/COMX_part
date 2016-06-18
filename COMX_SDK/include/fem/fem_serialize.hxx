/*HEAD fem_serialize HXX ONESTEP */
/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --
A dialog about check and modify overlap condition .
================================================================================
Date            Name                    Description of Change
                Zhang Xiang-kui 	      Create

$HISTORY$
================================================================================*/
#ifndef _KMAS_FEM_SERIALIZE_H
#define _KMAS_FEM_SERIALIZE_H

#include "fem/fem_base.hxx"
#include "base/root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {47F9C9E2-B4A4-4632-9ED1-64A61CA44D2A}
const MUID IID_IFemSerialize = 
{ 0x47f9c9e2, 0xb4a4, 0x4632, { 0x9e, 0xd1, 0x64, 0xa6, 0x1c, 0xa4, 0x4d, 0x2a } };

interface IFemSerialize : public IRoot{
    virtual TStatus STDCALL Load(const char *fnam) = 0;
    virtual TStatus STDCALL Save(const char *fnam) = 0;
    virtual TStatus STDCALL Import(const char *fnam) = 0;
    virtual TStatus STDCALL Export(const char *fnam) = 0;
};

// {D8BFD169-4815-484c-8317-7BDD5B57A0DA}
const MUID IID_IFemSerialize2 = 
{ 0xd8bfd169, 0x4815, 0x484c, { 0x83, 0x17, 0x7b, 0xdd, 0x5b, 0x57, 0xa0, 0xda } };

interface IFemSerialize2 : public IFemSerialize{
    virtual TStatus STDCALL ExportBorder(const char *fnam) = 0;
};

// {5721BEA1-968E-4c33-B967-5F87885C8569}
const MUID IID_IFemSerialize3 = 
{ 0x5721bea1, 0x968e, 0x4c33, { 0xb9, 0x67, 0x5f, 0x87, 0x88, 0x5c, 0x85, 0x69 } };

interface IFemSerialize3 : public IFemSerialize2
{
     virtual TStatus STDCALL ImportResult(const char *fnam) = 0;
};

// {AB2DE17D-ED96-4258-A608-28BF072DBB46}
const MUID IID_IFemSerialize4 = 
{ 0xab2de17d, 0xed96, 0x4258, { 0xa6, 0x8, 0x28, 0xbf, 0x7, 0x2d, 0xbb, 0x46 } };

interface IFemSerialize4 : public IFemSerialize3
{
	virtual TStatus STDCALL ExportBorderWithIgesFormat(const char *fname) = 0;
};

// {7C6FED1B-1F51-4d07-87A4-D857D6411291}
const MUID CLSID_IFemSerialize = 
{ 0x7c6fed1b, 0x1f51, 0x4d07, { 0x87, 0xa4, 0xd8, 0x57, 0xd6, 0x41, 0x12, 0x91 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif


