/*HEAD plugin_common HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_PLUGIN_FUNC_H_200711183451
#define _KMAS_COMX_PLUGIN_FUNC_H_200711183451

#include <base/factory.hxx>

#include <string>
using namespace std;

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

inline void COMX_GetToolbar(const char* tbname, TCOMXPluginToolbar *&tbs)
{
	IComxUiPluginServer *p_plugin_server = NULL;
	KMAS::Die_maker::comx::TFactory factory;
	factory.QueryService(CLSID_IPlugServer, IID_IComxUiPluginServer, (void**)&p_plugin_server);
	assert(p_plugin_server);
	int plugins_num = 0;
	p_plugin_server->GetPluginCount(plugins_num);
	for (int index = 0; index < plugins_num; ++index)
	{
		IComxUiPlugin *p_plugin = NULL;
		p_plugin_server->GetPlugin(index, p_plugin);
		int tb_num = 0;
		p_plugin->GetToolbarCount(tb_num);
		
		for (int loop = 0; loop < tb_num; ++loop)
		{
			TCOMXPluginToolbar *pToolbar = NULL;
			p_plugin->GetToolbar(loop, pToolbar);
			if (strcmp(pToolbar->name, tbname) == 0)
			{
				tbs = pToolbar;

				p_plugin_server->Release();
				return;
			}
		}
	}

	tbs = NULL;
	p_plugin_server->Release();
}

inline void COMX_ShowToolbar(const char* tbname, bool flag)
{
	TCOMXPluginToolbar *pToolbar = NULL;
	COMX_GetToolbar(tbname, pToolbar);
	if (pToolbar)
	{
		pToolbar->is_show = flag;
	}
}

inline void COMX_ShowToolbars(string *tbnames, const int num, bool flag)
{
	for (int loop = 0; loop < num; ++loop)
	{
		COMX_ShowToolbar(tbnames[loop].c_str(), flag);
	}
}

inline void COMX_RelayoutToolbar()
{
	TFactory factory;
	QUERY_SERVICE(
		factory,
		CLSID_IPlugServer,
		IComxUiPluginServerFrm,
		p_comx_ui_plugin_server_frm);
	
	if (p_comx_ui_plugin_server_frm != NULL)
	{
		p_comx_ui_plugin_server_frm->Relayout();
		p_comx_ui_plugin_server_frm->Release();
    }
}

inline void COMX_GetToolbarItem(const char* tbname, int btn_index, TCOMXPluginToolbarItem &tbs_item)
{
	TCOMXPluginToolbar *tbs = NULL;
	COMX_GetToolbar(tbname, tbs);
	if (tbs)
	{
	    assert(btn_index >= 0 && btn_index < tbs->size);
		tbs_item = tbs->items[btn_index];
	}
}

inline void COMX_SetToolbarItem(const char* tbname, int btn_index, TCOMXPluginToolbarItem &tbs_item)
{
	TCOMXPluginToolbar *tbs = NULL;
	COMX_GetToolbar(tbname, tbs);
	if (tbs)
	{
		assert(btn_index >= 0 && btn_index < tbs->size);
		tbs->items[btn_index] = tbs_item;
	}
}

inline void COMX_ActiveToolbarItem(const char* tbname, int btn_index)
{
	IComxUiPluginServer *p_plugin_server = NULL;
	KMAS::Die_maker::comx::TFactory factory;
	factory.QueryService(CLSID_IPlugServer, IID_IComxUiPluginServer, (void**)&p_plugin_server);
	assert(p_plugin_server);
	int plugins_num = 0;
	p_plugin_server->GetPluginCount(plugins_num);
	for (int index = 0; index < plugins_num; ++index)
	{
		IComxUiPlugin *p_plugin = NULL;
		p_plugin_server->GetPlugin(index, p_plugin);
		int tb_num = 0;
		p_plugin->GetToolbarCount(tb_num);

		for (int loop = 0; loop < tb_num; ++loop)
		{
			TCOMXPluginToolbar *pToolbar = NULL;
			p_plugin->GetToolbar(loop, pToolbar);
			if (strcmp(pToolbar->name, tbname) == 0)
			{
				IComxUiPluginButton *p_plugin_btn = NULL;
				p_plugin->QueryInterface(IID_IComxUiPluginButton, (void**)&p_plugin_btn);
				if (p_plugin_btn)
				{
				    p_plugin_btn->ActiveButton(loop, btn_index);
					p_plugin_btn->Release();
				}
				p_plugin_server->Release();

				return;
			}
		}
	}
	p_plugin_server->Release();
}

inline bool COMX_IsToolbarItemIsCheck(const char* tbname, int btn_index)
{
	TCOMXPluginToolbarItem tbs_item;
	COMX_GetToolbarItem(tbname, btn_index, tbs_item);
	if (tbs_item.nFlags&PLUGIN_CHECK)
	{
		return true;
	}
	return false;
}

inline bool COMX_CheckToolbarItem(const char* tbname, int btn_index, bool flag)
{
	TCOMXPluginToolbar *tbs = NULL;
	COMX_GetToolbar(tbname, tbs);

	if (flag)
	{
		tbs->items[btn_index].nFlags |= PLUGIN_CHECK;
	}
	else
	{
		tbs->items[btn_index].nFlags &= ~PLUGIN_CHECK;
	}

	return true;
}

inline bool COMX_IsToolbarItemIsEnable(const char* tbname, int btn_index)
{
	TCOMXPluginToolbarItem tbs_item;
	COMX_GetToolbarItem(tbname, btn_index, tbs_item);
	if (tbs_item.nFlags&PLUGIN_ENABLE)
	{
		return true;
	}
	return false;
}

inline bool COMX_EnableToolbarItem(const char* tbname, int btn_index, bool flag)
{
	TCOMXPluginToolbar *tbs = NULL;
	COMX_GetToolbar(tbname, tbs);
	
	if (flag)
	{
		tbs->items[btn_index].nFlags |= PLUGIN_ENABLE;
	}
	else
	{
		tbs->items[btn_index].nFlags &= ~PLUGIN_ENABLE;
	}
	
	return true;
}

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif