/*HEAD fem_core1d HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_FEM_CORE1D_H_200932615641
#define _KMAS_COMX_FEM_CORE1D_H_200932615641

#include <base\root.hxx>

#include <fem\fem_base.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {3CD550BC-6C13-4c47-9D25-06925F8B687A}
 const MUID IID_IFemCore1d = 
{ 0x3cd550bc, 0x6c13, 0x4c47, { 0x9d, 0x25, 0x6, 0x92, 0x5f, 0x8b, 0x68, 0x7a } };

interface IFemCore1d : public IRoot
{
     virtual TStatus STDCALL InsertNode(const int pos, const fem::TPoint &pt) = 0;
     virtual TStatus STDCALL InsertNode(const int &pos, const fem::TPoint *&pts, const int &clen) = 0;
     virtual TStatus STDCALL InsertElement(const int &pos, const int &n1, const int &n2) = 0;
     virtual TStatus STDCALL InsertElement(const int &pos, const int *elements, const int &clen) = 0;
     virtual TStatus STDCALL GetNodeCount(int &count) = 0;
     virtual TStatus STDCALL GetNode(const int &pos, fem::TPoint &pt) = 0;
     virtual TStatus STDCALL GetNode(const int &spos, const int &epos, fem::TPoint *points) = 0;
     virtual TStatus STDCALL GetElementCount(int &count) = 0;
     virtual TStatus STDCALL GetElement(const int &pos, int &n1, int &n2) = 0;
     virtual TStatus STDCALL GetElement(const int &spos, const int &epos, int *elements) = 0;
     virtual TStatus STDCALL DeleteNode(const int &pos) = 0;
     virtual TStatus STDCALL DeleteNode(const int &spos, const int &epos) = 0;
     virtual TStatus STDCALL DeleteNode(int *node_ids, const int &clen) = 0;
     virtual TStatus STDCALL DeleteElement(const int &pos) = 0;
     virtual TStatus STDCALL DeleteElement(const int &spos, const int &epos) = 0;
     virtual TStatus STDCALL DeleteElement(int *element_ids, const int &clen) = 0;
     virtual TStatus STDCALL UpdateNode(const int &pos, const fem::TPoint &pt) = 0;
     virtual TStatus STDCALL Clear() = 0;
     virtual TStatus STDCALL IsInitialize(bool &is_init) = 0;
};

// {C2ACFC8B-2EA3-48eb-B13D-8C04FB391B25}
 const MUID IID_IFemCore1dCache = 
{ 0xc2acfc8b, 0x2ea3, 0x48eb, { 0xb1, 0x3d, 0x8c, 0x4, 0xfb, 0x39, 0x1b, 0x25 } };

interface IFemCore1dCache : public IRoot
{
	virtual TStatus STDCALL GetCacheInfo(fem::TFemCore1dCacheBase *&cache_info) = 0;
	virtual TStatus STDCALL RefreshCache() = 0;
};

// {C22E60C9-6520-4967-B254-E70FA14C2724}
 const MUID IID_IFemCore1dModifiednotify = 
{ 0xc22e60c9, 0x6520, 0x4967, { 0xb2, 0x54, 0xe7, 0xf, 0xa1, 0x4c, 0x27, 0x24 } };

interface IFemCore1dModifiednotify : public IRoot
{
     virtual TStatus STDCALL Notify() = 0;
};

// {536B0E7E-0BD4-40f1-BA6A-ED83CB34A91A}
const MUID IID_IPropertyField = 
{ 0x536b0e7e, 0xbd4, 0x40f1, { 0xba, 0x6a, 0xed, 0x83, 0xcb, 0x34, 0xa9, 0x1a } };

interface IPropertyField : public IRoot
{
	 virtual TStatus STDCALL AddPropertyMetaInfo(const MUID &pid, const char *description, int mode) = 0;
	 virtual TStatus STDCALL GetFirstPropertyMetaInfo(MUID &pid, char *description, int mode) = 0;
	 virtual TStatus STDCALL GetNextPropertyMetaInfo(MUID &pid, char *description, int mode) = 0;
	 virtual TStatus STDCALL SetNodePropertyValue(const MUID &pid, const int index, void *p_data) = 0;
	 virtual TStatus STDCALL GetNodePropertyValue(const MUID &pid, const int index, void *&p_data) = 0;
	 virtual TStatus STDCALL SetElementPropertyValue(const MUID &pid, const int index, void *p_data) = 0;
	 virtual TStatus STDCALL GetElementPropertyValue(const MUID &pid, const int index, void *&p_data) = 0;
};

// {F47B8D3E-15DC-4d18-AE78-C3E991033B92}
const MUID IID_IFemCore1dSerialize = 
{ 0xf47b8d3e, 0x15dc, 0x4d18, { 0xae, 0x78, 0xc3, 0xe9, 0x91, 0x3, 0x3b, 0x92 } };

interface IFemCore1dSerialize : public IRoot
{
	 virtual TStatus STDCALL Open(const char *fname) = 0;
	 virtual TStatus STDCALL Save(const char *fname) = 0;
	 virtual TStatus STDCALL Import(const char *fname) = 0;
	 virtual TStatus STDCALL Export(const char *fname) = 0;
};

// {E3EA64CC-9AA6-4f19-AC19-9905B585DC63}
const MUID IID_IFemCoreBridge = 
{ 0xe3ea64cc, 0x9aa6, 0x4f19, { 0xac, 0x19, 0x99, 0x5, 0xb5, 0x85, 0xdc, 0x63 } };

interface IFemCoreBridge : public IRoot
{
	 virtual TStatus STDCALL SetNodeBaseAddress(const int base_address) = 0;
	 virtual TStatus STDCALL GetNodeBaseAddress(int &base_address) = 0;
	 virtual TStatus STDCALL SetElementBaseAddress(const int base_address) = 0;
	 virtual TStatus STDCALL GetElementBaseAddress(int &base_address) = 0;
};

// {42D09FE7-8F3F-4534-93A1-F84010FF47A0}
const MUID IID_IFemCore1dPost = 
{ 0x42d09fe7, 0x8f3f, 0x4534, { 0x93, 0xa1, 0xf8, 0x40, 0x10, 0xff, 0x47, 0xa0 } };

interface IFemCore1dPost : public IRoot
{
	 virtual TStatus STDCALL Enable(const bool flag) = 0;
	 virtual TStatus STDCALL AddValues(const char *title, const int mode, const char *unit, double *vals) = 0;
	 virtual TStatus STDCALL GetValues(const char *title, double *&vals) = 0;
	 virtual TStatus STDCALL GetValue(const char *title, const int index, double &val) = 0;
	 virtual TStatus STDCALL Activate(const char *title) = 0;
};

// {2CB4D2C5-FF43-40df-8121-34F3E5876593}
 const MTYPEID CLSID_IFemCore1d = 
{ 0x2cb4d2c5, 0xff43, 0x40df, { 0x81, 0x21, 0x34, 0xf3, 0xe5, 0x87, 0x65, 0x93 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

