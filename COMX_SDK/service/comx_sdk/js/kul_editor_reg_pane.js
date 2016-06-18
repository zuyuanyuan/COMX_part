
function GetIndexByName(unit_name)
{
	var unit_name_list = binding.unit_list.split(',');
	
	for(var loop = 0; loop < unit_name_list.length; ++loop)
	{
		if(unit_name_list[loop] == unit_name)
		{
			return loop - 1;
		}
	}

	return -1;
}

function OnInitializeData()
{
 binding.unit_list = comx.sys.GetUnitNameList(true);
 var index = GetIndexByName(comx.gf.kul_editor_current_unit_name);  
 if(index != -1)
 {
     binding.unit_index = index;
 }      
}

function OnClose()
{
	pane.Close();
}

function OnHotKey(id)
{
	if(id == 1001)
	{
		OnClose();
	}
}

function OnOkay()
{
	comx.gf.g_kul_filename = comx.shell.kulreg(binding.unit_name, comx.gf.g_kul_filename);
	pane.Close();
}
