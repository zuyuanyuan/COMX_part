/////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');

function AutoSaveKUL()
{
	if(binding.kul_doc_filename != "")
	{
		 var sci_kul_wnd = new TScintillaWnd(binding.kul_doc_hdr);

		 if(sci_kul_wnd.IsDirty() == true)
		 {
			if(comx.ui.CallYesNoDialog("KUL document has been modified, do you want to save it?", "Notify"))
			{
				SaveKULDocTo(binding.kul_doc_filename);
			}
		 }
	}
}

function OpenKULDocFrom(filename)
{
	 AutoSaveKUL();

     comx.sys.CreateFileHandler("kul_doc", filename);
     binding.kul_doc = comx.file.kul_doc;
     comx.sys.CloseFileHandler("kul_doc");
	 binding.kul_doc_filename = filename;
}

function AutoSaveJS()
{
	 if(binding.js_doc_filename != "")
	 {
		 var sci_js_wnd = new TScintillaWnd(binding.js_doc_hdr);

		 if(sci_js_wnd.IsDirty() == true)
		 {
			if(comx.ui.CallYesNoDialog("JavaScript document has been modified, do you want to save it?", "Notify"))
			{
				SaveJSDocTo(binding.js_doc_filename);
			}
		 }
	 }
}

function AutoSaveJSSilent()
{
	 if(binding.js_doc_filename != "")
	 {
		 var sci_js_wnd = new TScintillaWnd(binding.js_doc_hdr);

		 if(sci_js_wnd.IsDirty() == true)
		 {
			SaveJSDocTo(binding.js_doc_filename);
		 }
	 }
}

function OpenJSDocFrom(filename)
{
	 AutoSaveJS();
     comx.sys.CreateFileHandler("js_doc", filename);
     binding.js_doc = comx.file.js_doc;

     comx.sys.CloseFileHandler("js_doc");
 	 binding.js_doc_filename = filename;
 
	 SetTimeout(0, function(nTimerID){
         KillTimeout(nTimerID);
         RefreshJSInfo();
         SyncAssociatedKULFile();
     });
}

function IsKULFile(filename)
{
	return (filename.indexOf(".kul") != -1);
}

function IsJSFile(filename)
{

	return (filename.indexOf(".js") != -1);

}

function OnOpen()
{
	OnOpenByNewDlg();
	return;
}

function ClearJSDocDirtyFlag()
{
	var sci_wnd = new TScintillaWnd(binding.js_doc_hdr);

    sci_wnd.ClearDirtyFlag();
}

function SaveJSDocTo(filename)
{
    comx.sys.CreateFileHandler("js_doc", filename);
    comx.file.js_doc = binding.js_doc;
    comx.sys.CloseFileHandler("js_doc");

    binding.js_doc_filename = filename;

    ClearJSDocDirtyFlag();
    
    RefreshJSInfo();
}

function ClearKULDocDirtyFlag()
{
	var sci_wnd = new TScintillaWnd(binding.kul_doc_hdr);

    sci_wnd.ClearDirtyFlag();
}

function RegisterKUL(unit_name, kul_filepath)
{
	comx.shell.kulreg(unit_name, kul_filepath);
}

function SaveKULDocTo(filename)
{
    comx.sys.CreateFileHandler("kul_doc", filename);
    comx.file.kul_doc = binding.kul_doc;
    comx.sys.CloseFileHandler("kul_doc");
    binding.kul_doc_filename = filename;
    ClearKULDocDirtyFlag();

    //binding.preview_window_enable = IsBlockPackage(binding.kul_doc_filename);

    var err_msg = comx.shell.kul_validate(binding.kul_doc_filename);
	binding.validate_info = comx.shell.kul_validate_err_msg(err_msg);

	var is_pass = comx.shell.is_kul_validate_pass(err_msg);

	if(is_pass == "false")
	{
		binding.top_menu_index = 1;
		var err_line = comx.shell.kul_validate_err_line(err_msg);
		if(err_line != "-1")
		{
			binding.tabctrl_index = 0;
			binding.kul_doc_index.index1 = parseInt(err_line) - 1;
		}

		return false;
	}
	else
	{
		binding.top_menu_index = 0;

		if(IsBlockPackage(binding.kul_doc_filename))
		{
			RegisterKUL(comx.gf.kul_editor_current_unit_name, binding.kul_doc_filename);
		}

		return true;
	}
}

function OnSaveAs()
{
    if(binding.tabctrl_index == 0)
    {
        var filter = new Array();
        filter[0] = "KUL Files(*.kul)";
        filter[1] = "All Files(*.*)";

        var filename = comx.ui.CallSaveFileDialog(filter);
        if(filename != "")
        {
            SaveKULDocTo(filename);
        }
    }

    if(binding.tabctrl_index == 1)
    {
        var filter = new Array();
        filter[0] = "JavaScript Files(*.js)";
        filter[1] = "All Files(*.*)";

        var filename = comx.ui.CallSaveFileDialog(filter);
        if(filename != "")
        {
            SaveJSDocTo(filename);
        }
    }
}

function OnSave()
{
    if(binding.tabctrl_index == 0 && binding.kul_doc_filename != "")
    {
        SaveKULDocTo(binding.kul_doc_filename);
        return;
    }

    if(binding.tabctrl_index == 1 && binding.js_doc_filename != "")
    {
        SaveJSDocTo(binding.js_doc_filename);
        return;
    }

    //OnSaveAs();
}

function OnOpenByNewDlg()
{
	comx.ui.PopupSystemModalForm("0x3ac8d876-0x6cec-0x46a7-0x9a-0x5d-0x56-0x46-0xe2-0x07-0xdc-0x1e");

	var page_index = 0;

	if(comx.gf.kul_editor_open_file_dlg_valid == "true")
	{
		var ret_obj = JSON.parse(comx.gf.kul_editor_open_dlg_ret);

		var is_auto_load = false;
		if(ret_obj.kul_filepath != undefined)
		{
			page_index = 0;
			is_auto_load = true;

			OpenKULDocFrom(ret_obj.kul_filepath);
			ClearKULDocDirtyFlag();

			kul_doc_first_visible_line = 0;
			kul_doc_position = 0;
		}

		if(ret_obj.js_filepath != undefined)
		{
			if(is_auto_load == false)
			{
				page_index = 1;
			}

			OpenJSDocFrom(ret_obj.js_filepath);
			ClearJSDocDirtyFlag();
		}

		binding.tabctrl_index = page_index;        
	}
}

function OnHotKey(id)
{
	if(id == 1002)
	{
		OnSave();
	}

	if(id == 1001)
	{
		OnOpenByNewDlg();
	}

	if(id == 1003)
	{
		if(binding.tabctrl_index == 0)
		{
			comx.gf.kul_search_target_hdr = binding.kul_doc_hdr;
		}
		else
		{
			comx.gf.kul_search_target_hdr = binding.js_doc_hdr;
		}

		comx.ui.PopupSystemModalessForm("0xeb3d74cc-0xee2e-0x47c3-0x9c-0xaa-0xb2-0x0f-0x64-0x70-0xfd-0xa7");
	}

	if(id == 1004)
	{
		OnNewDocumentNew();
	}

	if(id == 1005)
	{
		OnPreviewBlock();
	}

	if(id == 1006)
	{
		OnUnitToolkit();
	}

	if(id == 1007)
	{
		OnPkgToolkit();
	}

	if(id == 1008)
	{
		pane.Close();
	}

	if(id == 1009)
	{
		OnNewFrame();
	}
}

var kul_doc_first_visible_line = 0;
var kul_doc_position = 0;

var js_doc_first_visible_line = 0;
var js_doc_position = 0;

function OnSwitchDoc()
{
	if(comx.gf.tabctrl_index == 0)
	{
		widget.scintilla.SetPosition(binding.kul_doc_hdr, kul_doc_position);
		widget.scintilla.SetFirstVisibleLine(binding.kul_doc_hdr, kul_doc_first_visible_line);

		js_doc_position = widget.scintilla.GetPosition(binding.js_doc_hdr);
		js_doc_first_visible_line = widget.scintilla.GetFirstVisibleLine(binding.js_doc_hdr);

	}
	else
	{
		kul_doc_position = widget.scintilla.GetPosition(binding.kul_doc_hdr);
		kul_doc_first_visible_line = widget.scintilla.GetFirstVisibleLine(binding.kul_doc_hdr);

		widget.scintilla.SetPosition(binding.js_doc_hdr, js_doc_position);
		widget.scintilla.SetFirstVisibleLine(binding.js_doc_hdr, js_doc_first_visible_line);
	}
}

function OnCloseForm()
{
	SaveConfig();

	AutoSaveKUL();
	AutoSaveJS();

	websocket.Stop();
}

function OnAddWidget()
{
	if(binding.kul_doc_filename == "")
	{
		return;
	}

	comx.gf.kul_doc_file = binding.kul_doc_filename;
	comx.ui.PopupSystemModalForm("0x94a39440-0xaddb-0x4a60-0x9b-0xc4-0xdf-0x29-0xca-0x5f-0xef-0xad");
	if(comx.gf.insert_to_document == "true" && comx.gf.add_widget_okay == "true")
	{
		binding.tabctrl_index = 0;
		var sci_wnd = new TScintillaWnd(binding.kul_doc_hdr);
		sci_wnd.Paste();
		OnBeautify();
	}
}

function OnBeautify()
{
	if(binding.kul_doc_filename == "")
	{
		return;
	}

	while(binding.kul_doc_filename == "")
	{
		OnSaveAs();
	}
	if(binding.kul_doc_filename != "")
	{
		var index = binding.kul_doc_index.index1;

        if(SaveKULDocTo(binding.kul_doc_filename) == false)
			return;

		comx.shell.beautify_xml(binding.kul_doc_filename);
		OpenKULDocFrom(binding.kul_doc_filename);
		binding.kul_doc_index.index1 = index;
	}
}

function OnSimpleMode()
{
	if(binding.kul_doc_filename == "")
	{
		return;
	}

	while(binding.kul_doc_filename == "")
	{
		OnSaveAs();
	}

	if(SaveKULDocTo(binding.kul_doc_filename) == false)
		return;

	comx.shell.kulprop(binding.kul_doc_filename, 1);
	OpenKULDocFrom(binding.kul_doc_filename);
}

function OnFullMode()
{
	if(binding.kul_doc_filename == "")
	{
		return;
	}

	while(binding.kul_doc_filename == "")
	{
		OnSaveAs();
	}

	if(SaveKULDocTo(binding.kul_doc_filename)== false)
		return;

	comx.shell.kulprop(binding.kul_doc_filename, 2);
	OpenKULDocFrom(binding.kul_doc_filename);
}

function OnRegister()
{
	if(binding.kul_doc_filename != "")
	{
		if(SaveKULDocTo(binding.kul_doc_filename) == false)
			return;

		comx.gf.g_kul_filename = binding.kul_doc_filename;
		comx.ui.PopupSystemModalForm("0x2b76e4d2-0xfdc9-0x4b30-0xbf-0x77-0xe7-0x12-0x70-0xca-0xf8-0x2f");

		if(binding.kul_doc_filename != comx.gf.g_kul_filename)
			OpenKULDocFrom(comx.gf.g_kul_filename);
		//OnBeautify();
	}
}

function OnExecute()
{
	comx.ui.PopupSystemModalForm("0xa5b43ef4-0x7f7a-0x4992-0x8c-0x0e-0xbe-0x77-0x1c-0xa8-0x2a-0x0e");
}

function OnNewDocument()
{
	OnNewDocumentNew();
	return;
	if(binding.tabctrl_index == 0)
	{
		if(binding.kul_doc_filename != "")
			AutoSaveKUL();
		binding.kul_doc = "";
		binding.kul_doc_filename = "";
	}
	if(binding.tabctrl_index == 1)
	{
		if(binding.js_doc_filename != "")
			AutoSaveJS();
		binding.js_doc = "";
		binding.js_doc_filename = "";
	}

	//binding.preview_window_enable = false;
}

function OnNewDocumentNew()
{
	comx.ui.PopupSystemModalForm("0x7f3c409a-0x7093-0x40c5-0xa4-0xbd-0x0e-0x1f-0x06-0x7a-0x06-0x76");

	if(comx.gf.kul_editor_open_file_dlg_valid == "true")
	{
		var ret_obj = JSON.parse(comx.gf.kul_editor_open_dlg_ret);

		if(ret_obj.kul_filepath != undefined)
		{
			OpenKULDocFrom(ret_obj.kul_filepath);
			ClearKULDocDirtyFlag();

			kul_doc_first_visible_line = 0;
			kul_doc_position = 0;

			OnBeautify();
		}

		if(ret_obj.js_filepath != undefined)
		{
			OpenJSDocFrom(ret_obj.js_filepath);
			ClearJSDocDirtyFlag();
		}

		binding.tabctrl_index = 0;
	}
}

function OnModeChange()
{
	if(binding.kul_mode_index == 0)
		OnSimpleMode();
	if(binding.kul_mode_index == 1)
		OnFullMode();
}

function OnInitializeData()
{
    G_js_func_info = false;
	G_js_variable_info = false;

	websocket.Startup(false/*debug_mode*/);

	binding.copyright_info = "Warning: This computer program is protected by copyright law and international treaties.Unauthorized reproduction or distribution of this program, or any portion of it, may result in severe civil and criminal penalties,and will be prosecuted under the law.";
	comx.gf.unit_index = 0;
	//binding.preview_window_enable = false;

	var kul_editor_cfg_file = "y:\\comx_sdk\\kul\\kul_editor_cfg.json";
	if(comx.sys.IsFileExist(kul_editor_cfg_file))
	{
		comx.sys.CreateFileHandler("keditor_cfg", kul_editor_cfg_file);
		var keditor_cfg_obj = JSON.parse(comx.file.keditor_cfg);
		comx.sys.CloseFileHandler("keditor_cfg");

		comx.gf.kul_editor_current_unit_name = keditor_cfg_obj.current_unit_name;
	}
	else
	{
		comx.gf.kul_editor_current_unit_name = "comx_sdk";
	}
    
    SetTimeout(1000, function(nTimerID){
        AutoSaveJSSilent();
    });
    
    SetTimeout(2000, function(){
        RefreshBingInfo();
    });
}

function SaveConfig()
{
	if(comx.gf.kul_editor_current_unit_name == undefined)
	{
		return;
	}

	var kul_editor_cfg_file = "y:\\comx_sdk\\kul\\kul_editor_cfg.json";

	comx.sys.CreateFileHandler("keditor_cfg", kul_editor_cfg_file);
	comx.file.keditor_cfg = JSON.stringify({"current_unit_name" : comx.gf.kul_editor_current_unit_name});
	comx.sys.CloseFileHandler("keditor_cfg");
}

function OnBind()
{
	comx.dev.CallBindingBrowser();
}

function OnEvent()
{
    comx.dev.CallEventBrowser();
}

function OnGlobal()
{
    comx.dev.CallGlobalFieldBrowser();
}

function OnNamespace()
{
    comx.dev.CallNamespaceBrowser();
}

function OnCopy()
{
	if(binding.tabctrl_index == 0)
	{
		var sci_wnd = new TScintillaWnd(binding.kul_doc_hdr);
		sci_wnd.Copy();
	}
	else
	{
		var sci_wnd = new TScintillaWnd(binding.js_doc_hdr);
		sci_wnd.Copy();
	}
}

function OnCut()
{
	if(binding.tabctrl_index == 0)
	{
		var sci_wnd = new TScintillaWnd(binding.kul_doc_hdr);
		sci_wnd.Cut();
	}
	else
	{
		var sci_wnd = new TScintillaWnd(binding.js_doc_hdr);
		sci_wnd.Cut();
	}
}

function OnPaste()
{
	if(binding.tabctrl_index == 0)
	{
		var sci_wnd = new TScintillaWnd(binding.kul_doc_hdr);
		sci_wnd.Paste();
	}
	else
	{
		var sci_wnd = new TScintillaWnd(binding.js_doc_hdr);
		sci_wnd.Paste();
	}
}

function OnUndo()
{
	if(binding.tabctrl_index == 0)
	{
		var sci_wnd = new TScintillaWnd(binding.kul_doc_hdr);
		sci_wnd.Undo();
	}
	else
	{
		var sci_wnd = new TScintillaWnd(binding.js_doc_hdr);
		sci_wnd.Undo();
	}
}

function OnRedo()
{
	if(binding.tabctrl_index == 0)
	{
		var sci_wnd = new TScintillaWnd(binding.kul_doc_hdr);
		sci_wnd.Redo();
	}
	else
	{
		var sci_wnd = new TScintillaWnd(binding.js_doc_hdr);
		sci_wnd.Redo();
	}
}

function OnKulValidate()
{
	if(binding.kul_doc_filename == "")
		return;

	comx.sys.CreateFileHandler("kul_doc", binding.kul_doc_filename);
    comx.file.kul_doc = binding.kul_doc;
    comx.sys.CloseFileHandler("kul_doc");
    ClearKULDocDirtyFlag();

	var err_msg = comx.shell.kul_validate(binding.kul_doc_filename);
	binding.validate_info = comx.shell.kul_validate_err_msg(err_msg);

	var is_pass = comx.shell.is_kul_validate_pass(err_msg);

	if(is_pass == "false")
	{
		var err_line = comx.shell.kul_validate_err_line(err_msg);
		if(err_line != "-1")
		{
			binding.tabctrl_index = 0;
			binding.kul_doc_index.index1 = parseInt(err_line) - 1;
		}
	}
}

function OnUnitToolkit()
{
	var cmd = "y:\\comx_sdk\\distribute\\KUnitApp.exe";
	comx.shell.run(cmd, true);
}

function OnPkgToolkit()
{
	comx.ui.PopupSystemModalessForm(unit.FORMID_IPkgMgrEntry);
}

function OnNewFrame()
{
	SaveConfig();

	var cmd = "y:\\comx_sdk\\bin\\kdesktop.exe -e 0x2f10d808-0x266b-0x4207-0xb3-0x25-0x2a-0xc6-0xec-0xb3-0xd8-0xcb -ucomx_sdk";
	comx.shell.run(cmd, false);
}

function IsBlockPackage(kul_fname)
{
	return (comx.shell.IsBlockPackage(kul_fname) == "true");
}

function OnPreviewBlock()
{
	if(binding.kul_doc_filename == "")
	{
		return;
	}

	if(IsBlockPackage(binding.kul_doc_filename))
	{
		comx.shell.RefreshBlockPreviewBy(binding.kul_doc_filename);
		comx.ui.PopupSystemModalessForm(comx.entry.FORMID_IKulBlockPreviewPane);
	}
	else
	{
		if(comx.shell.IsNullKulFile(binding.kul_doc_filename) == "true")
		{
			comx.ui.InformationBox("The current KUL file don't include any subctrl!");
			return;
		}

		var unit_name = comx.shell.GetUnitNameOfKUL(binding.kul_doc_filename);
		var formid = GetIdFromKulFilename(binding.kul_doc_filename, function(formid){
			var cmd = "y:\\comx_sdk\\bin\\kdesktop.exe -u " + unit_name + " -e " + formid;
			comx.shell.run(cmd, false);
		});
	}
}

function GetIdFromKulFilename(kul_filename, callback)
{
	var kul_fpath_arr = kul_filename.split("");
	for(var index = 0; index < kul_fpath_arr.length; ++index)
	{
	    if(kul_fpath_arr[index] == "\\")
	    {
			kul_fpath_arr[index] = "/";
	    }
	}

	kul_filename = kul_fpath_arr.join("");

	var unit_name = comx.shell.GetUnitNameOfKUL(binding.kul_doc_filename);

	websocket.Invoke('GetKulEntryInfo', {"unit_name" : unit_name}, function(data){
		var json_obj = data.parameters;

		for(var loop = 0; loop < json_obj.length; ++loop)
		{
			if(json_obj[loop].kul_filepath == kul_filename)
			{
				callback(json_obj[loop].id);
			}
		}
	});
}

function TransFilePathToUnixFmt(filepath)
{
	filepath = filepath.split('\\');
	return filepath.join('/');
}

function GetRegexFuncSrcCacheFilePath()
{
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'data\\cache\\';
	return TransFilePathToUnixFmt(cache_dir) + 'regex_src.func.cache';
}

function GetRegexFuncPatternCacheFilePath()
{
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'data\\cache\\';
	return TransFilePathToUnixFmt(cache_dir) + 'regex_pattern.func.cache';
}

function SaveJSFuncRegexInfoToCache()
{
	var src_filepath = GetRegexFuncSrcCacheFilePath();
	var pattern_filepath = GetRegexFuncPatternCacheFilePath();

	comx.sys.CreateFileHandler('regex_src', src_filepath);
	comx.file.regex_src = binding.js_doc;
	comx.sys.CloseFileHandler('regex_src');

	comx.sys.CreateFileHandler('regex_pattern', pattern_filepath);
	comx.file.regex_pattern = '^[ \t]*function[ \t]*([a-zA-Z\_]{1}[a-zA-Z0-9\_]*)';
	comx.sys.CloseFileHandler('regex_pattern');
}

function GetRegexVarSrcCacheFilePath()
{
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'data\\cache\\';
	return TransFilePathToUnixFmt(cache_dir) + 'regex_src.var.cache';
}

function GetRegexVarPatternCacheFilePath()
{
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'data\\cache\\';
	return TransFilePathToUnixFmt(cache_dir) + 'regex_pattern.var.cache';
}

function SaveJSVarRegexInfoToCache()
{
	var src_filepath = GetRegexVarSrcCacheFilePath();
	var pattern_filepath = GetRegexVarPatternCacheFilePath();

	comx.sys.CreateFileHandler('regex_src', src_filepath);
	comx.file.regex_src = binding.js_doc;
	comx.sys.CloseFileHandler('regex_src');

	comx.sys.CreateFileHandler('regex_pattern', pattern_filepath);
	comx.file.regex_pattern = 'websocket\\.(Invoke|on)\\([\'"]{1}([^\',{]*)[\'"]{1}';
	comx.sys.CloseFileHandler('regex_pattern');
}

var G_js_func_info = false;
var G_js_variable_info = false;

var G_js_func_name = false;
var G_js_var_name = false;

function RefreshJSInfo()
{
    G_js_func_info = false;
    G_js_variable_info = false;
    
    //binding.js_func_list = "";
    //binding.js_variable_list = "";
    
    G_js_func_name = binding.js_func_val;
    G_js_var_name = binding.js_variable_val;
    
    //binding.js_func_val = "parsing...";
    //binding.js_variable_val = "parsing...";
    
	//SetTimeout(0, function(nTimerID){
	//	KillTimeout(nTimerID);
		SaveJSFuncRegexInfoToCache();
		websocket.Invoke('GetFunctionListOfJavaScript', {'src' : GetRegexFuncSrcCacheFilePath() , 'pattern' : GetRegexFuncPatternCacheFilePath()}, function(data){
			if(data.parameters.type == 'ok')
			{
				G_js_func_info = data.parameters;
                RefreshJSFuncList();
			}
            else
            {
                binding.js_func_list = "";
                binding.js_func_val = "";
            }
		});
        
        SaveJSVarRegexInfoToCache();
		websocket.Invoke('GetVariableListOfJavaScript', {'src' : GetRegexVarSrcCacheFilePath() , 'pattern' : GetRegexVarPatternCacheFilePath()}, function(data){
			if(data.parameters.type == 'ok')
			{
				G_js_variable_info = data.parameters;
                RefreshJSVarList();
			}
            else
            {
                binding.js_variable_list = "";
                binding.js_variable_val = "";
            }
		});
	//});
}

function RefreshJSFuncList()
{
    if(G_js_func_info == false)
		return;
        
    var result = G_js_func_info.result;
    var func_list = '';

	var js_func_name = G_js_func_name;//binding.js_func_val;
    
    var index = 0;
    for(var i = 0; i < result.length; ++i)
	{
        var item_name = binding.js_doc.substr(result[i][0].start, result[i][0].end - result[i][0].start);
		if(js_func_name == item_name)
        {
            index = i;
        }        
        func_list += (',' + item_name);
	}

    func_list = '' + result.length + func_list;
    
    if(binding.js_func_list != func_list)
    {
        binding.js_func_list = func_list;
		binding.js_func_list_index = index;
    }
}

function RefreshJSVarList()
{
    if(G_js_variable_info == false)
		return;
        
    var result = G_js_variable_info.result;
    var var_list = '';

	var js_var_name = G_js_var_name//binding.js_variable_val;
    
    var index = 0;
    for(var i = 0; i < result.length; ++i)
	{
        var type = binding.js_doc.substr(result[i][0].start, result[i][0].end - result[i][0].start);
        var name = binding.js_doc.substr(result[i][1].start, result[i][1].end - result[i][1].start);
        
        var item_name = '';
        if(type == 'Invoke')
        {
            item_name = name + ' : Emitter';
        }
        else
        {
            item_name = name + ' : Listener';
        }
        
		if(js_var_name == item_name)
        {
            index = i;
        }        
        var_list += (',' + item_name);
	}

    var_list = '' + result.length + var_list;
    
    if(binding.js_variable_list != var_list)
    {
        binding.js_variable_list = var_list;
		binding.js_variable_list_index = index;
    }
}

function OnJavaScriptDocChanged()
{
    //RefreshJSInfo();
}

function OnVariableInfoChanged()
{
    SetTimeout(10, function(nTimerID){
        KillTimeout(nTimerID);
        
        if(G_js_variable_info == false)
        {
            return;
        }
        
        var index = binding.js_variable_list_index;
        var result = G_js_variable_info.result;
        
        if(result[index] == undefined)
        {
            return;
        }
        
        var pos = result[index][0].start;
        var end = result[index][1].end;
        widget.scintilla.SetPosition(binding.js_doc_hdr, pos + 10000);
        widget.scintilla.SetPosition(binding.js_doc_hdr, pos);
        if(binding.js_doc.substr(pos, result[index][0].end - pos) == 'Invoke')
        { 
			widget.scintilla.FindPrevious(binding.js_doc_hdr, 'function');
        }
        widget.scintilla.FindNext(binding.js_doc_hdr, binding.js_doc.substr(result[index][1].start, end - result[index][1].start));
        
        comx.shell.SetFocusCtrl(binding.js_doc_hdr);
    });
}

function OnFunctionInfoChanged()
{
    SetTimeout(10, function(nTimerID){
		KillTimeout(nTimerID);
        
        if(G_js_func_info == false)
        {
            return;
        }
        
        var index = binding.js_func_list_index;
        var result = G_js_func_info.result;
        
        if(result[index] == undefined)
        {
            return;
        }
        
        var pos = result[index][0].start;
        var end = result[index][0].end;
        widget.scintilla.SetPosition(binding.js_doc_hdr, pos + 10000);
        widget.scintilla.SetPosition(binding.js_doc_hdr, pos);
        widget.scintilla.FindNext(binding.js_doc_hdr, binding.js_doc.substr(pos, end - pos));
        
        comx.shell.SetFocusCtrl(binding.js_doc_hdr);
    });
}

function pickup_ns_from_prefix(prefix)
{
    prefix = prefix.split('.');
    
    if(prefix.length == 1)
    {
        return "";
    }
    else
    {
        var ret = "";
        for(var loop = 0; loop < prefix.length - 1; ++loop)
        {
            if(ret != "")
            {
                ret += '.';
            }
            
            ret += prefix[loop];    
        }
        
        return ret;
    }
}

function pickup_ac_str_from_prefix(prefix)
{
    prefix = prefix.split('.');
    return prefix[prefix.length - 1];
}

function generate_space_str(space_num)
{
    var ret = [];
    for(var loop = 0; loop < space_num; ++loop)
    {
        ret.push(' ');
    }
    return ret.join('');
}

function get_top_ac_str_list(start_pos)
{
    var ret = ['break','case','catch','continue','default','delete','do','else','finally',
				'for(var key in [obj])\n' + generate_space_str(start_pos)  + '{\n' + generate_space_str(start_pos) + '}',
                'for(var loop = 0; loop < [num]; ++loop)\n' + generate_space_str(start_pos)  + '{\n' + generate_space_str(start_pos) + '}',
                'function','in','instanceof','new','return','switch','this','throw','try','typeof',
                'if([condition])\n'  + generate_space_str(start_pos)  + '{\n' + generate_space_str(start_pos) + '}',
                'if([condition])\n'  + generate_space_str(start_pos)  + '{\n' + generate_space_str(start_pos) + '}' + '\n' + generate_space_str(start_pos) + 'else\n' + generate_space_str(start_pos)+ '{\n' + generate_space_str(start_pos) + '}',
                'var','void','while','with','abstract','boolean','byte','char','class','const','debugger',
                'double','enum','export','extends','final','float','goto','implements','import','int','gl','widget','websocket',
                'interface','long','native','package','private','protected','public','short','static','true', 'false','eval','pane', 
                'super','synchronized','throws','transient','volatile'];
    
    ret.push('comx');
    ret.push('binding');
    
    var func_list = binding.js_func_list.split(',');
    for(var loop = 1; loop < func_list.length; ++loop)
    {
        ret.push(func_list[loop]);
    }
    
    return ret;
}

function filter_ac_list_by_str(ac_list, ac_str)
{
    var ret = [];
    for(var loop = 0; loop < ac_list.length; ++loop)
    {
        if(ac_list[loop].substr(0, ac_str.length).toLowerCase() == ac_str.toLowerCase())
        {
            if(ac_list[loop].indexOf('0xba590c050x08530x45c10x940x2b0xbe0xcc0x6c0x8d0xeb0x1b') == -1)
            {
                ret.push(ac_list[loop]);
            }
        }
    }
    
    return ret;
}

function filter_ac_list_by_str_comx(ac_list)
{
    var ret = [];
    for(var loop = 0; loop < ac_list.length; ++loop)
    {
        if(ac_list[loop] != 'gf' && ac_list[loop] != 'file')
        {
            ret.push(ac_list[loop]);
        }
    }
    
    return ret;
}

function filter_ac_list_by_str_pane(ac_list)
{
    var ret = [];
    for(var loop = 0; loop < ac_list.length; ++loop)
    {
        if(ac_list[loop] != 'formid')
        {
            ret.push(ac_list[loop]);
        }
    }
    
    return ret;
}

function GetUnitNameByJSFile(js_fname)
{
    if(js_fname != '')
    {
        js_fname = js_fname.split('\\').join('/').split('/');
		return js_fname[1];    
    }
    
    return '';
}

var G_binding_info = false;
var G_associated_kul_fname = '';

function SyncAssociatedKULFile()
{
    G_binding_info = false;
    G_associated_kul_fname = '';
    
    var unit_name = GetUnitNameByJSFile(binding.js_doc_filename);
    
    if(unit_name == '')
    {
        return;
    }
    
    websocket.Invoke('GetKulEntryInfo', {"unit_name" : unit_name}, function(data){
		var json_obj = data.parameters;

		for(var loop = 0; loop < json_obj.length; ++loop)
		{
			if(json_obj[loop].js_filepath == binding.js_doc_filename)
			{
				G_associated_kul_fname = json_obj[loop].kul_filepath;
			}
		}
        
        RefreshBingInfo();
	});
}

function RefreshBingInfo()
{
    G_binding_info = false;
    
    if(G_associated_kul_fname != '')
    {
        websocket.Invoke('GetKulBindingInfo', {'kul_fname' : G_associated_kul_fname}, function(data){
            G_binding_info = data.parameters;
        });
    }
}

function OnAutoComplete(prefix)
{
    var ns = pickup_ns_from_prefix(prefix);
    var ac_str = pickup_ac_str_from_prefix(prefix);
    
    if(ns == '')
    {
        var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - ac_str.length);;
        var top_ac_list = get_top_ac_str_list(offset);
        var ac_list = filter_ac_list_by_str(top_ac_list, ac_str);
        
        if(ac_list.length != 0)
        {
            widget.scintilla.AutoComplete(binding.js_doc_hdr, ac_list, ac_str.length);
        }
    }
    
    if(ns == 'binding')
    {
        if(G_binding_info != false)
        {
            var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - ac_str.length);;
			var ac_list = filter_ac_list_by_str(G_binding_info, ac_str);
			
			if(ac_list.length != 0)
			{
				widget.scintilla.AutoComplete(binding.js_doc_hdr, ac_list, ac_str.length);
			}
        }
    }
    else
    {
        if(ns.indexOf('comx.gf') == 0 ||
           ns.indexOf('comx.file') == 0 ||
           ns.indexOf('unit') == 0 ||
           ns.indexOf('my_math') == 0)
        {
            return;
        }
        
        var ns_obj = eval(ns);
		if(typeof(ns_obj) == 'object')
        {
            var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - ac_str.length);;

			var ac_list = [];
            for(var key in ns_obj)
            {
                ac_list.push(key);
            }
            
            var ac_list = filter_ac_list_by_str(ac_list, ac_str);
            
            if(ns == 'comx')
            {
                ac_list = filter_ac_list_by_str_comx(ac_list);
            }
            
            if(ns == 'pane')
            {
                ac_list = filter_ac_list_by_str_pane(ac_list);
            }
			
			if(ac_list.length != 0)
			{
				widget.scintilla.AutoComplete(binding.js_doc_hdr, ac_list, ac_str.length);
			}
        }
    }
}


