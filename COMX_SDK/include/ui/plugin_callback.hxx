#ifndef _KMAS_COMX_PLUGIN_CALLBACK_HXX
#define _KMAS_COMX_PLUGIN_CALLBACK_HXX

#include <base/type_wrap.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{
    extern "C"
    {
        typedef void (*fn_comx_plugin_callback_t)(int code, KVARIANT *paras);
    }

enum TPluginCallbackCodeEnum
{
    COMX_SIDEBAR_LV_CLICK = 1,
    COMX_SIDEBAR_LV_CLOSE = 2,
	COMX_SIDEBAR_LV_SORT  = 3
};

}//end namespace comx
}//end namespace Die_maker
}//end namespace KMAS

#endif