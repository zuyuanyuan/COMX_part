/////////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');

var is_new_block = false;

function OnBlock()
{
	binding.label_index = 1;
	is_new_block = true;

	binding.websocket_enable = false;
	binding.websocket_service_enable = false;
}

function OnPackage()
{
	binding.label_index = 0;
	is_new_block = false;

	binding.websocket_enable = true;
	binding.websocket_service_enable = (binding.websocket_check == "true");
}

function OnInitializeData()
{
        websocket.Connect();

	binding.unit_name_list = comx.sys.GetUnitNameList(true);
	var index = GetIndexByName(comx.gf.kul_editor_current_unit_name);

	if(index != -1)
	{
		binding.unit_name_index = index;
	}

	comx.gf.kul_editor_open_file_dlg_valid = false;
}

function OnInitializePost()
{
	comx.shell.SetFocusCtrl(binding.pkg_name_hdr);
}

function GetIndexByName(unit_name)
{
	var unit_name_list = binding.unit_name_list.split(',');
	
	for(var loop = 0; loop < unit_name_list.length; ++loop)
	{
		if(unit_name_list[loop] == unit_name)
		{
			return loop - 1;
		}
	}

	return -1;
}

function RegisterKUL(unit_name, kul_filepath)
{
	comx.shell.kulreg(unit_name, kul_filepath);
}

function OnOkay()
{
	//if(CheckPackageName(binding.pkg_name))
        CheckPackageName(binding.pkg_name, function(ret){
	    if(ret)
	    {
		comx.gf.kul_editor_current_unit_name = binding.unit_name;
		
		var kul_filepath = "y:\\" + binding.unit_name + "\\kul\\" + binding.pkg_name + ".kul";
		var js_filepath = "y:\\" + binding.unit_name + "\\js\\" + binding.pkg_name + "_pane.js";
		
		var is_need_support_websocket = (binding.websocket_check == "true" && is_new_block == false);
		var is_need_support_service = is_need_support_websocket && (binding.websocket_service_check == "true");

		comx.shell.CreateKULPackage(binding.unit_name, binding.pkg_name, is_new_block, is_need_support_websocket, is_need_support_service);
		RegisterKUL(binding.unit_name, kul_filepath);

		if(is_new_block == false)
		{
			comx.gf.kul_editor_open_dlg_ret = JSON.stringify({"kul_filepath" : kul_filepath, "js_filepath" : js_filepath});
		}
		else
		{
			comx.gf.kul_editor_open_dlg_ret = JSON.stringify({"kul_filepath" : kul_filepath});
		}

		comx.gf.kul_editor_open_file_dlg_valid = true;

		pane.Close();
	    }
	});
}

function OnClose()
{
	pane.Close();
}

function OnCloseForm()
{
        websocket.Dispose();
}

function CheckPackageName(pkg_name, cb)
{
	var ret = true;

	if(pkg_name == "")
	{
		comx.ui.InformationBox("The New Package Name Can't be NULL!");
		ret = false;
	        cb(ret)
;
	        return;
	}

	var unit_name = binding.unit_name;
	if(comx.sys.GetEnvironmentVariables(unit_name.toUpperCase()) == undefined)
	{
		comx.ui.InformationBox("the environment variable: '" + unit_name.toUpperCase() + "' isn't exist, \nperhaps you should restart your computer!");
		ret = false;

		//return ret;
	        cb(ret);

	        return;
	}

        websocket.Invoke('GetKulEntryInfo', {"unit_name" : unit_name}, function(data){
	    var kul_entry_obj = data.parameters;

	    for(var loop = 0; loop < kul_entry_obj.length; ++loop)
	    {
		if(kul_entry_obj[loop].name == pkg_name)
		{
		    comx.ui.InformationBox("The name '" + pkg_name + "' has been used!");
		    ret = false;
		    cb(ret);
		    break;
		}
	    }
	    if(ret == true)
	    {
		cb(ret);
	    }
	});
}

function sleep(n)
{
    var start=new Date().getTime();
    while(true) if(new Date().getTime()-start>n) break;
}

function OnUnitNameChanged()
{
	//Do nothing.
}

function OnHotKey(id)
{
	if(id == 1001)
	{
		OnClose();
	}
}

function OnWebSocketCheck()
{
	binding.websocket_service_enable = (binding.websocket_check == "true")
}
