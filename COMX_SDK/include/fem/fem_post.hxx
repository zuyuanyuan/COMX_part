/*HEAD fem_post HXX ONESTEP */
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
#ifndef _KMAS_COMX_FEM_POST_HEAD
#define _KMAS_COMX_FEM_POST_HEAD

#include <base/root.hxx>

#ifndef ONESTEP_INTEGERATE_TO_UG_NX

#include <fem/fem_render.hxx>
#include <fem/fem_core.hxx>

#endif

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

#ifdef ONESTEP_INTEGERATE_TO_UG_NX
enum TFemRenderColorType
{
     RENDER_NODE,
     RENDER_ELEMENT
};
#endif
// {B75B0905-3F1D-47ac-B5A4-554DE35D18F3}
const MUID IID_IFemPost = 
{ 0xb75b0905, 0x3f1d, 0x47ac, { 0xb5, 0xa4, 0x55, 0x4d, 0xe3, 0x5d, 0x18, 0xf3 } };

interface IFemPost : public IRoot
{
     virtual TStatus STDCALL Import(const char *title, const char *unit, 
          const TFemRenderColorType &type,
          double *vals, const int &clen) = 0;
     virtual TStatus STDCALL Activate(const char *title, const bool &is_inverse = false) = 0;
     virtual TStatus STDCALL GetActivateTitle(char *&title) = 0;
     virtual TStatus STDCALL IsValidTitle(const char *title, bool &flag) = 0;
     virtual TStatus STDCALL Clear() = 0;
     virtual TStatus STDCALL GetValue(const char *title, const int &index, double &val) = 0;
};

// {AC0D86D5-A745-4720-AC70-A0FA3AEEFED2}
const MUID IID_IFemPost2 = 
{ 0xac0d86d5, 0xa745, 0x4720, { 0xac, 0x70, 0xa0, 0xfa, 0x3a, 0xee, 0xfe, 0xd2 } };

interface IFemPost2 : public IFemPost
{
	virtual TStatus STDCALL GetValue(const char *title, double *&val, int &len) = 0;
};

#ifdef ONESTEP_INTEGERATE_TO_UG_NX
struct TFemRenderColorInfo
{
     TFemRenderColorType type;
     double              *clrs;
     bool                is_valid;
     int                 length;
};

// {7411C631-AF0B-4db2-B4DA-48A67F85FE95}
const MUID IID_IFemRenderColor = 
{ 0x7411c631, 0xaf0b, 0x4db2, { 0xb4, 0xda, 0x48, 0xa6, 0x7f, 0x85, 0xfe, 0x95 } };

interface IFemRenderColor : public IRoot
{
     virtual TStatus STDCALL GetColorCache(TFemRenderColorInfo *&cache) = 0;
};

class TFemRenderColorCache
{
     TFemRenderColorInfo *_cache;
public:
     struct Item
     {
          double red;
          double green;
          double blue;
     };
public:
     TFemRenderColorCache(TFemRenderColorInfo *cache) : _cache(cache)
     {
          assert(_cache);
     }
public:
     const Item &operator[](const int &index)
     {
          assert(index >= 0 && index < _cache->length);
          Item *ptr_clr = (Item*)(_cache->clrs + index * 3);
          return *ptr_clr;
     }
     const int &Count(){return _cache->length;}
     const TFemRenderColorType &Type(){return _cache->type;}
     const bool &IsValid(){return _cache->is_valid;}
};
#endif
// {010FA776-3826-4f41-B50F-73C9FEE5768F}
const MUID CLSID_IFemPost = 
{ 0x10fa776, 0x3826, 0x4f41, { 0xb5, 0xf, 0x73, 0xc9, 0xfe, 0xe5, 0x76, 0x8f } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif