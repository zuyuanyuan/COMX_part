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
#ifndef _KMAS_FEM_RENDER_HEAD_FILE_DEFINE
#define _KMAS_FEM_RENDER_HEAD_FILE_DEFINE

#include "fem/fem_base.hxx"
#include "fem/fem_util.hxx"
#include "base/root.hxx"

//using namespace fem;

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

#ifndef _KMAS_FEM_DISPLAY_MODE_TYPE_DEFINE
#define _KMAS_FEM_DISPLAY_MODE_TYPE_DEFINE
enum TFemDisplayMode{
	FEM_WIRE		=	0x0001,
	FEM_LIGHT		=	0x0002,
	FEM_FILL		=	0x0004,
	FEM_HIDE_LINE	=	0x0008
};
#endif

#ifndef _KMAS_FEM_VISUAL_MODE_TYPE_DEFINE
#define _KMAS_FEM_VISUAL_MODE_TYPE_DEFINE
enum TFemVisualMode{
	FEM_FACE		=	0x0001,
	FEM_BORDER		=	0x0002,
	FEM_FREE_NODE	=	0x0004
};
#endif

#ifndef _KMAS_FEM_TEMPLATE_TYPE_DEFINE
#define _KMAS_FEM_TEMPLATE_TYPE_DEFINE
enum POSITION{
	BEGIN,
	END
};
#endif	

// {0E962D73-B070-418b-ADCF-7774A398585E}
const MUID IID_IFemRender = 
{ 0xe962d73, 0xb070, 0x418b, { 0xad, 0xcf, 0x77, 0x74, 0xa3, 0x98, 0x58, 0x5e } };

interface IFemRender : public IRoot{
	virtual TStatus STDCALL SetDisplayMode(const int &mode) = 0;
	virtual TStatus STDCALL GetDisplayMode(int &mode) = 0;
	virtual TStatus STDCALL SetVisualMode(const int &mode) = 0;
	virtual TStatus STDCALL GetVisualMode(int &mode) = 0;
	virtual TStatus STDCALL SetFaceColor(
		const double &red,
		const double &green,
		const double &blue,
		const double &alpha) = 0;
	virtual TStatus STDCALL SetWireColor(
		const double &red,
		const double &green,
		const double &blue,
		const double &alpha) = 0;
	virtual TStatus STDCALL SetNodeColor(
		const double &red,
		const double &green,
		const double &blue,
		const double &alpha) = 0;
	virtual TStatus STDCALL SetBorderColor(
		const double &red,
		const double &green,
		const double &blue,
		const double &alpha) = 0;
	virtual TStatus STDCALL SetHighColor(
		const double &red,
		const double &green,
		const double &blue,
		const double &alpha) = 0; 
};

// {47D72AA6-E65B-4c2b-9694-D7FEF5859713}
const MUID IID_IFemRender2 = 
{ 0x47d72aa6, 0xe65b, 0x4c2b, { 0x96, 0x94, 0xd7, 0xfe, 0xf5, 0x85, 0x97, 0x13 } };

interface IFemRender2 : public IFemRender
{
	virtual TStatus STDCALL GetFaceColor(
		double &red, 
		double &green, 
		double &blue, 
		double &alpha) = 0;
	virtual TStatus STDCALL GetWireColor(
		double &red, 
		double &green, 
		double &blue, 
		double &alpha) = 0;
	virtual TStatus STDCALL GetNodeColor(
		double &red, 
		double &green, 
		double &blue, 
		double &alpha) = 0;
	virtual TStatus STDCALL GetBorderColor(
		double &red, 
		double &green, 
		double &blue, 
		double &alpha) = 0;
	virtual TStatus STDCALL GetHighColor(
		double &red, 
		double &green, 
		double &blue, 
		double &alpha) = 0;
};

// {2854F790-5759-42b1-95CE-BB567C5A90E2}
const MUID IID_IFemRender3 = 
{ 0x2854f790, 0x5759, 0x42b1, { 0x95, 0xce, 0xbb, 0x56, 0x7c, 0x5a, 0x90, 0xe2 } };

interface IFemRender3 : public IFemRender2
{
	virtual TStatus STDCALL SetVisibility(const bool flag) = 0;
	virtual TStatus STDCALL GetVisibility(bool &flag) = 0;
};

enum TFemRenderMode
{
	FEM_RENDER_COMMON_MODE,
	FEM_RENDER_MARK_MODE,
	FEM_RENDER_HIGHLIGHT_MODE
};

// {3F62D86B-D2FA-4330-AA85-7968BF1E11FB}
const MUID IID_IFemRender4 = 
{ 0x3f62d86b, 0xd2fa, 0x4330, { 0xaa, 0x85, 0x79, 0x68, 0xbf, 0x1e, 0x11, 0xfb } };

interface IFemRender4 : public IFemRender3
{
	virtual TStatus STDCALL SetMarkColor(
		const double red,
		const double green,
		const double blue,
		const double alpha) = 0;
	virtual TStatus STDCALL GetMarkColor(
		double &red,
		double &green,
		double &blue,
		double &alpha) = 0;
	virtual TStatus STDCALL SetRenderMode(const TFemRenderMode mode) = 0;
	virtual TStatus STDCALL GetRenderMode(TFemRenderMode &mode) = 0;
};

enum TFemRenderColorType
{
     RENDER_NODE,
     RENDER_ELEMENT
};

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

// {AF0EA0F6-6691-4e15-9000-402DA3D52A3F}
const MUID CLSID_IFemRender = 
{ 0xaf0ea0f6, 0x6691, 0x4e15, { 0x90, 0x0, 0x40, 0x2d, 0xa3, 0xd5, 0x2a, 0x3f } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif