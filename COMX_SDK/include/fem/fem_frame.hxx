/*HEAD fem_frame HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date            Name            Description of Change
2007/06/05			YanKK					create

$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_FEM_FRAME_H_20076172445
#define _KMAS_COMX_FEM_FRAME_H_20076172445

#include "base\root.hxx"
#include <fem/fem_render.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {25623850-34D6-4789-B07F-78EEB6F3A002}
const MUID IID_IFemFrame = 
{ 0x25623850, 0x34d6, 0x4789, { 0xb0, 0x7f, 0x78, 0xee, 0xb6, 0xf3, 0xa0, 0x2 } };

interface IFemFrame : public IRoot
{
	virtual TStatus STDCALL ImportPsyVal(const char* title, const char* unit, const TFemRenderColorType &type, const double* vals, const int clen ) = 0;
   	virtual TStatus STDCALL InsertNullPsyVal() = 0;
	virtual TStatus STDCALL InsertFldPsyVal() = 0;

//	virtual TStatus STDCALL GetCurFemcore(IFemFrame *& p_cur_femcore) = 0;

	virtual TStatus STDCALL GetCurExtremum(const char* title , double &max_p , double &min_p , char * unit) = 0;
	virtual TStatus STDCALL ActivePsyVal(const char* title) = 0;
	virtual	TStatus STDCALL SetDisplayMode(const TFemRenderColorType &type) = 0;
	virtual TStatus STDCALL GetCurActivedPsyVal(char * title ) = 0;
	virtual TStatus STDCALL GetCurDisplayMode(TFemRenderColorType &type) = 0;

	virtual TStatus STDCALL GetSBColor(TFemRenderColorInfo &sb_color) = 0;// ActivePsyVal( )∫Û π”√

	virtual TStatus STDCALL GetAllEleOrNodePsyVal( const char* title , const TFemRenderColorType &type,  double * para_val , int &array_len ) = 0;
	virtual TStatus STDCALL GetCurSinglePsyVal( const char* title , const TFemRenderColorType &type, const int ele_node_id , double  &para_val  ) = 0;

	virtual TStatus STDCALL GetCirleRangeELE( IFemCore *&pfemcore , const int centerID , const double radii, int * &eleids, int &len) = 0;

	virtual TStatus STDCALL Clear() = 0;
};

// {5AAC3B23-85B9-48cb-8E9D-C6C4F8B4A142}
const MUID EID_IFemcoreCache = 
{ 0x5aac3b23, 0x85b9, 0x48cb, { 0x8e, 0x9d, 0xc6, 0xc4, 0xf8, 0xb4, 0xa1, 0x42 } };


// {7666B9B1-2753-4ece-88BA-69A7650C85FF}
const MTYPEID CLSID_IFemFrame = 
{ 0x7666b9b1, 0x2753, 0x4ece, { 0x88, 0xba, 0x69, 0xa7, 0x65, 0xc, 0x85, 0xff } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

