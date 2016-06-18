/*HEAD acis_function_wrap HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_ACIS_FUNCTION_WRAP_H_20081058443
#define _KMAS_COMX_ACIS_FUNCTION_WRAP_H_20081058443

#include <cad\cad_function_wrap.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {F7796E66-F40D-4505-BD1E-9CEF3ED32B3B}
 const MTYPEID CLSID_IAcisFunctionWrap = 
{ 0xf7796e66, 0xf40d, 0x4505, { 0xbd, 0x1e, 0x9c, 0xef, 0x3e, 0xd3, 0x2b, 0x3b } };

class TAcisFunctionWrap
{
	tag_t _tag_fun;
	cad_func_util_t _cfu;
private:
	void init_cad_func_util()
	{
        TFactory factory;
        QUERY_SERVICE(
            factory,
            CLSID_IAcisFunctionWrap,
            ICADFunctionWrapUtil,
            p_cac_func_util);
        assert(p_cac_func_util);

        p_cac_func_util->GetFuncUtil(_cfu);

        p_cac_func_util->Release();
	}
public:
	TAcisFunctionWrap(const tag_t tag_fun)
	{
		KMAS::tag_util::TTagUtil tag_obj(tag_fun);
		assert(tag_obj.GetCode() == KMAS::tag_code::CAD_FUNCTION);

		_tag_fun = tag_fun;
		init_cad_func_util();
	}
	TAcisFunctionWrap(const char *name)
	{
		TFactory factory;
        QUERY_SERVICE(
            factory,
            CLSID_IAcisFunctionWrap,
            ICADFunctionWrap,
            p_cad_func);
        assert(p_cad_func);
		
		TStatus ret = p_cad_func->GetFunc(name, _tag_fun);
		assert(ret == M_OK);
		
		p_cad_func->Release();

		init_cad_func_util();
	}
public:
    operator tag_t()
    {
        return _tag_fun;
    }
public:
	char* GetName()
	{
		return _cfu.get_func_name(_tag_fun);
	}
	char* GetDescription()
	{
		return _cfu.get_func_description(_tag_fun);
	}
	void* GetValue()
	{
		return _cfu.get_func_value(_tag_fun);
	}
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

