/////////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');
var player = require('class/gif_animate.js');

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

var g_prog_block = false;

function RefreshPackageList()
{
	var unit_name = comx.gf.kul_editor_current_unit_name;
	binding.current_unit_name = unit_name;
    
    var nID = 0;
    
    if(g_prog_block == false)
    {
        g_prog_block = true;
        nID = player.Play('progress.gif', 50, function(fname){
            binding.prog_bmp = fname;
        });
    }
    
    //comx.shell.kulclean(unit_name);
    
    websocket.Invoke('GetKulEntryInfo', {"unit_name" : unit_name}, function(data){
        comx.gf.kul_editor_current_unit_name = data.parameters.unit_name;
        binding.current_unit_name = data.parameters.unit_name;
	    var kul_entry_obj = data.parameters.json_content;
	    var file_list = "";
	    for(var loop = 0; loop < kul_entry_obj.length; ++loop)
	    {
            if(!comx.sys.IsFileExist(kul_entry_obj[loop].kul_filepath))
            {
                continue;
            }
            
            if(kul_entry_obj[loop].js_filepath == false)
            {
                file_list += "2,";
                var kul_file_arr = kul_entry_obj[loop].kul_filepath.split('/');
                var pane_name = (kul_file_arr[kul_file_arr.length - 1].split('.')[0]);
                var js_filepath = comx.sys.GetEnvironmentVariables(data.parameters.unit_name.toUpperCase()) + 'js/' + pane_name + '_pane.js';
                
                if(comx.sys.IsFileExist(js_filepath))
                {
                    kul_entry_obj[loop].js_filepath = (TransFilePathToUnixFmt(js_filepath));
                }
                else
                {
                    js_filepath = comx.sys.GetEnvironmentVariables(data.parameters.unit_name.toUpperCase()) + 'js/' + pane_name + '.js';
                    kul_entry_obj[loop].js_filepath = (TransFilePathToUnixFmt(js_filepath));
                }
            }
            else
            {
                file_list += "0,";
            }
            
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
                if(!comx.sys.IsFileExist(kul_entry_obj[loop].js_filepath))
                {
                    continue;
                }
                
                file_list += "1,";
                file_list += kul_entry_obj[loop].name;
                file_list += ",";
                file_list += "-,";
                file_list += kul_entry_obj[loop].js_filepath;
                file_list += ",";
            }
	    }
        
        var ws_fname = TransFilePathToUnixFmt(comx.sys.GetEnvironmentVariables(unit_name.toUpperCase()) + 'js/websocket.js');
        
        if(comx.sys.IsFileExist(ws_fname))
        {
            file_list += "1,";
			file_list += 'websocket';
			file_list += ",";
			file_list += "-,";
			file_list += ws_fname;
			file_list += ",";
        }
        
	    binding.filelist_table = file_list;
        
        OnFileListClick();
        
        g_prog_block = false;
        player.Stop(nID);
        binding.prog_bmp = comx.sys.GetEnvironmentVariables('COMX_SDK') + '/picture/nil.bmp';
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
