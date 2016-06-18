/////////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');

function OnClose()
{
	pane.Close();
	comx.gf.kul_editor_open_file_dlg_valid = false;
}

function OnCloseForm()
{
    websocket.Dispose();
}

function OnInitializeData()
{
    websocket.Connect();
	binding.is_auto_load_js = true;
    
    //var cur_unit_name = comx.gf.kul_editor_current_unit_name;
	RefreshCategoryList();
    
    //SetTimeout(100, function(nID){
    //    KillTimeout(nID);
    //    comx.gf.kul_editor_current_unit_name = cur_unit_name;
        RefreshPackageList();   
    //});
}



function OnHotKey(id)
{
	if(id == 1001)
	{
		OnClose();
	}
}

function RefreshCategoryList()
{
	var unit_list = comx.sys.GetUnitNameList(false);
	var category_list = "0, comx_sdk";

	
	var unit_list_arr = unit_list.split(',');

	for(var loop = 1; loop < unit_list_arr.length; ++loop)
	{
		category_list += ",1,";
		category_list += unit_list_arr[loop];
	}
 
	binding.category_table = category_list;
}

function OnCategoryClick()
{
    if(!comx.gf.g_bCategoryClickFlag)
    {
        comx.gf.g_bCategoryClickFlag = true;
        return;
    }
    
	binding.category_index.index2 = 0;
	comx.gf.kul_editor_current_unit_name = binding.category_val;
	RefreshPackageList();
	//OnFileListClick();
}

var js_file_map = new Object;

function TransFilePathToUnixFmt(filepath)
{
	filepath = filepath.split('\\');
	return filepath.join('/');
}

function RefreshPackageList()
{
	var unit_name = comx.gf.kul_editor_current_unit_name;
	binding.current_unit_name = unit_name;
    
    websocket.Invoke('GetKulEntryInfo', {"unit_name" : unit_name}, function(data){
	    var kul_entry_obj = data.parameters;
	    var file_list = "";
	    for(var loop = 0; loop < kul_entry_obj.length; ++loop)
	    {
		file_list += "0,";
		file_list += kul_entry_obj[loop].name;
		file_list += ",";
		file_list += kul_entry_obj[loop].is_block;
		file_list += ",";
		file_list += kul_entry_obj[loop].kul_filepath;
		file_list += ",";

		if(kul_entry_obj[loop].is_block == false)
		{
			js_file_map[kul_entry_obj[loop].kul_filepath] = kul_entry_obj[loop].js_filepath;
		}
	    }

	    for(var loop = 0; loop < kul_entry_obj.length; ++loop)
	    {
		if(kul_entry_obj[loop].is_block == false)
		{
			file_list += "1,";
			file_list += kul_entry_obj[loop].name;
			file_list += ",";
			file_list += "-,";
			file_list += kul_entry_obj[loop].js_filepath;
			file_list += ",";
		}
	    }
        
        file_list += "1,";
			file_list += 'websocket';
			file_list += ",";
			file_list += "-,";
			file_list += TransFilePathToUnixFmt(comx.sys.GetEnvironmentVariables(unit_name.toUpperCase()) + 'js/websocket.js');
			file_list += ",";

	    binding.filelist_table = file_list;

	    OnFileListClick();
	});
}

function OnFileListClick()
{
	var index = binding.filelist_index.index1;

	var table = binding.filelist_table;
	var table_arr = table.split(',');

	if(table_arr[index * 4 + 2] == "false")
	{
		binding.is_auto_load_js_enable = true;
	}
	else
	{
		binding.is_auto_load_js_enable = false;
	}
}

function OnInitializePost()
{
	OnFileListClick();
    comx.shell.SetFocusCtrl(binding.category_hdr);
}

function OnOpen()
{
	var index = binding.filelist_index.index1;

	var table = binding.filelist_table;
	var table_arr = table.split(',');

	if(table_arr[index * 4 + 2] == "false")
	{
		if(binding.is_auto_load_js == "true")
		{
			comx.gf.kul_editor_open_dlg_ret = JSON.stringify({"kul_filepath" : table_arr[index * 4 + 3], "js_filepath" : js_file_map[table_arr[index * 4 + 3]]});
		}
		else
		{
			comx.gf.kul_editor_open_dlg_ret = JSON.stringify({"kul_filepath" : table_arr[index * 4 + 3]});
		}
	}
	else
	{
		if(table_arr[index * 4 + 0] == "0")
		{
			comx.gf.kul_editor_open_dlg_ret = JSON.stringify({"kul_filepath" : table_arr[index * 4 + 3]});
		}
		else
		{
			comx.gf.kul_editor_open_dlg_ret = JSON.stringify({"js_filepath" : table_arr[index * 4 + 3]});
		}
	}

	comx.gf.kul_editor_open_file_dlg_valid = true;
	pane.Close();
}

function OnFileListDbClick()
{
	OnOpen();
}
