function OnInitializeData()
{
	binding.unit_list = comx.sys.GetUnitNameList(true);
	binding.is_run_kdesktop = false;
	binding.is_default_option_enable = false;
	binding.unit_index = GetIndexByName(comx.gf.kul_editor_current_unit_name);//comx.gf.unit_index;

	if(binding.unit_name == "comx_sdk")
	{
		binding.is_run_kdesktop = false;
		binding.is_run_kdesktop_option_enable = false;
		binding.is_default_option_enable = true;
		binding.is_default = true;
	}
	else
	{
		binding.is_run_kdesktop = false;//comx.gf.is_run_kdesktop;
		binding.is_default = comx.gf.is_default;

		if(/*comx.gf.is_run_kdesktop == "false"*/true)
		{
			binding.is_default_option_enable = true;
		}
		else
		{
			binding.is_default_option_enable = false;
		}
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
	if(binding.is_run_kdesktop == "true")
	{
		var cmd = "y:\\comx_sdk\\bin\\kdesktop.exe -u" + binding.unit_name;
		comx.shell.run(cmd, false);
	}
	else if(binding.is_default == "true")
	{
		var cmd = "y:\\comx_sdk\\bin\\kdesktop.exe -e default -u" + binding.unit_name;
		comx.shell.run(cmd, false);
	}
	else
	{
		var cmd = "y:\\comx_sdk\\bin\\kdesktop.exe -e select -u" + binding.unit_name;
		comx.shell.run(cmd, false);
	}

	comx.gf.unit_index = binding.unit_index;

	comx.gf.is_run_kdesktop = binding.is_run_kdesktop;
	comx.gf.is_default_option_enable = binding.is_default_option_enable;
	comx.gf.is_default = binding.is_default;

	pane.Close();
}

function OnUnitChange()
{
	comx.gf.is_run_kdesktop = binding.is_run_kdesktop;
	comx.gf.is_default_option_enable = binding.is_default_option_enable;

	if(binding.unit_name == "comx_sdk")
	{
		binding.is_run_kdesktop = false;
		binding.is_run_kdesktop_option_enable = false;
		binding.is_default_option_enable = true;
		binding.is_default = true;
	}
	else
	{
		binding.is_run_kdesktop = comx.gf.is_run_kdesktop;
		binding.is_run_kdesktop_option_enable = true;
		binding.is_default_option_enable = comx.gf.is_default_option_enable;
		binding.is_default = comx.gf.is_default;
	}
}

function OnCheckRunDesktop()
{
	if(binding.is_run_kdesktop == "true")
	{
		binding.is_default_option_enable = false;
	}
	else
	{
		binding.is_default_option_enable = true;
		binding.is_default = true;
	}
}

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