#ifndef _COMX_PLUGIN_UTIL_H
#define _COMX_PLUGIN_UTIL_H

#include <set>
using namespace std;
#include <base/factory.hxx>
using namespace KMAS::Die_maker::comx;

struct TCOMXPluginToolbarCmdID
{
	int tb_index;
	int item_index;
};

inline bool operator<(
	  const TCOMXPluginToolbarCmdID &left, 
	  const TCOMXPluginToolbarCmdID &right)
{
	if (left.tb_index < right.tb_index)
	{
		return true;
	}
	else if(left.tb_index == right.tb_index)
	{
		if(left.item_index < right.item_index)
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

class TCOMXPluginFlagManager
{
	set<set<TCOMXPluginToolbarCmdID> > radio_group;
	set<TCOMXPluginToolbarCmdID>       check;
public:
	void AddRadioGroup(TCOMXPluginToolbarCmdID gid[], const int &gid_num)
	{
		set<TCOMXPluginToolbarCmdID> grp;
		for (int loop = 0; loop < gid_num; ++loop)
		{
			grp.insert(gid[loop]);
		}

		radio_group.insert(grp);
	}

	void AddCheck(const TCOMXPluginToolbarCmdID &id)
	{
		check.insert(id);
	}

public:
	void Enable(const TCOMXPluginToolbarCmdID &id, bool flag) //变灰
	{
		TCOMXPluginToolbar *tbs = GET_TOOLBAR_VARIATION();
		int tbs_num = GET_TOOLBAR_NUMBER();

		assert(id.tb_index >=0 && id.tb_index < tbs_num);
		assert(id.item_index >= 0 && id.tb_index < tbs[id.tb_index].size);

		if (flag)
		{
			tbs[id.tb_index].items[id.item_index].nFlags |= PLUGIN_ENABLE;
		}
		else
		{
			tbs[id.tb_index].items[id.item_index].nFlags &= (~PLUGIN_ENABLE);
		}
		
	}

	void Enable(const int &tb_index, const int &item_index, bool flag)//变灰
	{
		TCOMXPluginToolbarCmdID id;

		id.tb_index = tb_index;
		id.item_index = item_index;

		Enable(id, flag);
	}

	void Check(const TCOMXPluginToolbarCmdID &id, bool flag)
	{
		TCOMXPluginToolbar *tbs = GET_TOOLBAR_VARIATION();
		int tbs_num = GET_TOOLBAR_NUMBER();

		assert(id.tb_index >=0 && id.tb_index < tbs_num);
		assert(id.item_index >= 0 && id.tb_index < tbs[id.tb_index].size);

		if (flag)
		{
			tbs[id.tb_index].items[id.item_index].nFlags |= PLUGIN_CHECK;
		}
		else
		{
			tbs[id.tb_index].items[id.item_index].nFlags &= (~PLUGIN_CHECK);
		}

	}

	void Check(const int &tb_index, const int &item_index, bool flag)
	{
		TCOMXPluginToolbarCmdID id;

		id.tb_index = tb_index;
		id.item_index = item_index;

		Check(id, flag);
	}
	
	void Activate(const TCOMXPluginToolbarCmdID &id) //激活某一个ID
	{
		TCOMXPluginToolbar *tbs = GET_TOOLBAR_VARIATION();
		int tbs_num = GET_TOOLBAR_NUMBER();

		assert(id.tb_index >=0 && id.tb_index < tbs_num);
		assert(id.item_index >= 0 && id.item_index < tbs[id.tb_index].size);

		bool is_radio = false;
		for (set< set<TCOMXPluginToolbarCmdID> >::iterator iter_rgrp = radio_group.begin();
			iter_rgrp != radio_group.end(); ++iter_rgrp)
		{
			set<TCOMXPluginToolbarCmdID> &rgrp = *iter_rgrp;
			if (rgrp.find(id) != rgrp.end())
			{
				for (set<TCOMXPluginToolbarCmdID>::iterator iter_id = rgrp.begin();
					iter_id != rgrp.end(); ++iter_id)
				{
					tbs[iter_id->tb_index].items[iter_id->item_index].nFlags &= (~PLUGIN_RADIO);
				}
				tbs[id.tb_index].items[id.item_index].nFlags |= PLUGIN_RADIO;
				
				is_radio = true;

				break;
			}
		}

		if (!is_radio)
		{
			if (check.find(id) != check.end())
			{
				if (tbs[id.tb_index].items[id.item_index].nFlags & PLUGIN_CHECK)
				{
					tbs[id.tb_index].items[id.item_index].nFlags &= (~PLUGIN_CHECK);
				}
				else
				{
					tbs[id.tb_index].items[id.item_index].nFlags |= PLUGIN_CHECK;
				}
			}
		}
	}

	void Activate(const int &tb_index, const int &item_index)
	{
		TCOMXPluginToolbarCmdID id;

		id.tb_index = tb_index;
		id.item_index = item_index;

		Activate(id);
	}
public:
	bool IsEnable(const TCOMXPluginToolbarCmdID &id)//检测是否变灰
	{
		TCOMXPluginToolbar *tbs = GET_TOOLBAR_VARIATION();
		int tbs_num = GET_TOOLBAR_NUMBER();

		assert(id.tb_index >=0 && id.tb_index < tbs_num);
		assert(id.item_index >= 0 && id.tb_index < tbs[id.tb_index].size);

		return tbs[id.tb_index].items[id.item_index].nFlags&PLUGIN_ENABLE;
	}

	bool IsEnable(const int &tb_index, const int &item_index)
	{
		TCOMXPluginToolbarCmdID id;

		id.tb_index = tb_index;
		id.item_index = item_index;

		return IsEnable(id);
	}

	bool IsCheck(const TCOMXPluginToolbarCmdID &id)//检测是否是check
	{
		TCOMXPluginToolbar *tbs = GET_TOOLBAR_VARIATION();
		int tbs_num = GET_TOOLBAR_NUMBER();

		assert(id.tb_index >=0 && id.tb_index < tbs_num);
		assert(id.item_index >= 0 && id.tb_index < tbs[id.tb_index].size);

		return (tbs[id.tb_index].items[id.item_index].nFlags&PLUGIN_CHECK) != 0;
	}

	bool IsCheck(const int &tb_index, const int &item_index)
	{
		TCOMXPluginToolbarCmdID id;
		
		id.tb_index = tb_index;
		id.item_index = item_index;

		return IsCheck(id);
	}

	bool IsRadio(const TCOMXPluginToolbarCmdID &id)//检测是否是radio
	{
		return IsCheck(id);
	}

	bool IsRadio(const int &tb_index, const int &item_index)
	{
		return IsRadio(tb_index, item_index);
	}
};

#endif