/*HEAD cad_function_wrap HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_CAD_FUNCTION_WRAP_H_20081058443
#define _KMAS_COMX_CAD_FUNCTION_WRAP_H_20081058443

#include "base\root.hxx"
#include "base\tag_util.hxx"
#include "base\tag_code.hxx"
#include "base\factory.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

extern "C"
{
	struct cad_func_util_t
	{
		char* (*get_func_name)(const tag_t tag_fun);
		char* (*get_func_description)(const tag_t tag_fun);
		void* (*get_func_value)(const tag_t tag_fun);
	};
}

// {DD9F1C0A-3045-4105-BB69-5DE1E125B56D}
 const MUID IID_ICADFunctionWrap = 
{ 0xdd9f1c0a, 0x3045, 0x4105, { 0xbb, 0x69, 0x5d, 0xe1, 0xe1, 0x25, 0xb5, 0x6d } };

interface ICADFunctionWrap : public IRoot
{
	virtual TStatus STDCALL GetFunc(const char *name, tag_t &tag) = 0;
	virtual TStatus STDCALL GetFirstFunc(tag_t &tag) = 0;
	virtual TStatus STDCALL GetNextFunc(tag_t &tag, bool &is_end) = 0;
};

// {2D13A8C0-5FDF-4fbc-A0D8-E74DD4C50EC6}
const MUID IID_ICADFunctionWrapUtil = 
{ 0x2d13a8c0, 0x5fdf, 0x4fbc, { 0xa0, 0xd8, 0xe7, 0x4d, 0xd4, 0xc5, 0xe, 0xc6 } };

interface ICADFunctionWrapUtil : public IRoot
{
	virtual TStatus STDCALL GetFuncUtil(cad_func_util_t &cfu) = 0;
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

