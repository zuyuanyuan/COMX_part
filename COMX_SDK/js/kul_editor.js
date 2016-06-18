/////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');

function IsFunctionDeclareLine()
{
    var line = widget.scintilla.GetLine(binding.js_doc_hdr, binding.js_doc_index.index1);

    var re = /function[ \t]+[0-9A-Za-z]+\(.*\)/g;
    var s = line.match(re);
    
    if(s)
    {
        return true;
    }
    else
    {
        return false;
    }
}

function OnDistribute()
{
    comx.shell.run_ex('kdesktop.exe -u comx_sdk -e 0xd286e193-0x0479-0x42db-0x9e-0x9d-0x97-0x2a-0xe3-0x4e-0xb2-0xf2', false, false);
}

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
         G_js_global_variable_info_res = false;
         G_refresh_js_info_lock = false;
         G_js_doc_backup = false;
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
        alert(1);
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
        
        comx.ui.ClosePaneByFormID('0xeb3d74cc-0xee2e-0x47c3-0x9c-0xaa-0xb2-0x0f-0x64-0x70-0xfd-0xa7');
		G_is_search_window_active = false;
	}
}

var G_is_search_window_active = false;

function OnResizeForm()
{
	if(G_is_search_window_active == true)
	{
		comx.ui.ClosePaneByFormID('0xeb3d74cc-0xee2e-0x47c3-0x9c-0xaa-0xb2-0x0f-0x64-0x70-0xfd-0xa7');
		G_is_search_window_active = false;
		SetTimeout(10, function(nTimerID){
			KillTimeout(nTimerID);
			comx.ui.PopupSystemModalessForm("0xeb3d74cc-0xee2e-0x47c3-0x9c-0xaa-0xb2-0x0f-0x64-0x70-0xfd-0xa7", binding.kul_doc_hdr);
			G_is_search_window_active = true;
		});
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
        CallSearchDialog();
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
    
    if(id == 1010)
    {
        OnExecute();
    }
}

function CallSearchDialog()
{
    if(binding.tabctrl_index == 0)
	{
		comx.gf.kul_search_target_hdr = binding.kul_doc_hdr;
	}
	else
	{
		comx.gf.kul_search_target_hdr = binding.js_doc_hdr;
	}

    if(G_is_search_window_active)
    {
        G_is_search_window_active = false;
        comx.ui.ClosePaneByFormID("0xeb3d74cc-0xee2e-0x47c3-0x9c-0xaa-0xb2-0x0f-0x64-0x70-0xfd-0xa7");
    }
    else
    {
        G_is_search_window_active = true;
        comx.ui.PopupSystemModalessForm("0xeb3d74cc-0xee2e-0x47c3-0x9c-0xaa-0xb2-0x0f-0x64-0x70-0xfd-0xa7", binding.kul_doc_hdr);
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
    
    if(binding.tabctrl_index == 0)
	{
		comx.gf.kul_search_target_hdr = binding.kul_doc_hdr;
	}
	else
	{
		comx.gf.kul_search_target_hdr = binding.js_doc_hdr;
	}

    OnResizeForm();
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
    comx.gf.new_document_integrate_mode = true;
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

function get_xpm_data(filename)
{
    var file_path = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'picture/' + filename;
     
    if(comx.sys.IsFileExist(file_path))
    {
        comx.sys.CreateFileHandler('xpm_data', file_path);
        var ret = comx.file.xpm_data;
        comx.sys.CloseFileHandler('xpm_data');
        
        return ret;
    }
    else
    {
        return false;
    }
}

function RegisterACImage(type, xpmFile)
{
    var xpmData = get_xpm_data(xpmFile);
    if(xpmData)
    {
        widget.scintilla.RegisterImage(binding.js_doc_hdr, type, xpmData);
    }
}

function OnInitializeData()
{
    G_js_func_info = false;
	G_js_variable_info = false;
    G_js_global_variable_info = false;

	websocket.Startup(websocket.IsDebug()/*debug_mode*/);

	binding.copyright_info = "Warning: This computer program is protected by copyright law and international treaties.Unauthorized reproduction or distribution of this program, or any portion of it, may result in severe civil and criminal penalties,and will be prosecuted under the law.";
	comx.gf.unit_index = 0;
	//binding.preview_window_enable = false;

/*
	SetTimeout(500, function(nTimerID){
        //alert(nTimerID);
        if(!IsFunctionDeclareLine())
        {
            AutoSaveJSSilent();
        }
    });
    
    
    SetTimeout(500, function(nTimerID){
        //alert(nTimerID);
        if(!IsFunctionDeclareLine())
        {
            RefreshBingInfo();
        }
    });
  */  
    RegisterACImage(1, 'variable.xpm');
    RegisterACImage(2, 'object.xpm');
    RegisterACImage(3, 'function.xpm');
    RegisterACImage(4, 'key.xpm');
    RegisterACImage(5, 'invoke.xpm');
    RegisterACImage(6, 'message.xpm');
    
    InitializePredefineTipInfo();
    
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
    
    /*websocket.Invoke('GetLessonList', {}, function(data){
        binding.lesson_list = data.parameters.lesson_list;
    });*/
    
    //binding.lesson_list = comx.dev.GetManualGifList();
    
    var manual_index_file = comx.sys.GetEnvironmentVariables('COMX_SDK') + 'manual/index.json';
    comx.sys.CreateFileHandler('manual_index', manual_index_file);
    var manual_index_str = comx.file.manual_index;
    comx.sys.CloseFileHandler('manual_index');
    
    var manual_index = JSON.parse(manual_index_str);

    var lesson_list = '' + manual_index.length;
    for(var loop = 0; loop < manual_index.length; ++loop)
    {
        lesson_list += ',';
        lesson_list += manual_index[loop].name;
    }
    
    binding.lesson_list = lesson_list;
    
    SetTimeout(100, function(nID){
        if(g_is_lesson_playing)
        {
            binding.lesson_progress = binding.lesson_bmp;
        }
    });
}

function GetLessonFileByIndex(index)
{
    var manual_index_file = comx.sys.GetEnvironmentVariables('COMX_SDK') + 'manual/index.json';
    comx.sys.CreateFileHandler('manual_index', manual_index_file);
    var manual_index_str = comx.file.manual_index;
    comx.sys.CloseFileHandler('manual_index');
    
    var manual_index = JSON.parse(manual_index_str);
    
    return manual_index[index].file;
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
		var json_obj = data.parameters.json_content;

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

function GetRegexFuncResultCacheFilePath()
{
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'data\\cache\\';
	return TransFilePathToUnixFmt(cache_dir) + 'regex_res.func.cache';
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
	comx.file.regex_src = /*binding.js_doc*/G_js_doc_backup;
	comx.sys.CloseFileHandler('regex_src');

	comx.sys.CreateFileHandler('regex_pattern', pattern_filepath);
	comx.file.regex_pattern = '^[ \t]*(function[ \t]*([a-zA-Z\_]{1}[a-zA-Z0-9\_]*)[ \t]*\(.*\)).*[\r\n]*(\{)';//'^[ \t]*function[ \t]*([a-zA-Z\_]{1}[a-zA-Z0-9\_]*)';
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

function GetRegexVarResultCacheFilePath()
{
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'data\\cache\\';
	return TransFilePathToUnixFmt(cache_dir) + 'regex_res.var.cache';
}

function GetRegexGlobalVarSrcCacheFilePath()
{
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'data\\cache\\';
	return TransFilePathToUnixFmt(cache_dir) + 'regex_src.global.var.cache';
}

function GetRegexGlobalVarPatternCacheFilePath()
{
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'data\\cache\\';
	return TransFilePathToUnixFmt(cache_dir) + 'regex_pattern.global.var.cache';
}

function GetRegexGlobalVarResultCacheFilePath()
{
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + 'data\\cache\\';
	return TransFilePathToUnixFmt(cache_dir) + 'regex_res.global.var.cache';
}

function SaveJSVarRegexInfoToCache()
{
	var src_filepath = GetRegexVarSrcCacheFilePath();
	var pattern_filepath = GetRegexVarPatternCacheFilePath();

	comx.sys.CreateFileHandler('regex_src', src_filepath);
	comx.file.regex_src = /*binding.js_doc*/G_js_doc_backup;
	comx.sys.CloseFileHandler('regex_src');

	comx.sys.CreateFileHandler('regex_pattern', pattern_filepath);
	comx.file.regex_pattern = 'websocket\\.(Invoke|on)\\([\'"]{1}([^\',{]*)[\'"]{1}';
	comx.sys.CloseFileHandler('regex_pattern');
}

function SaveJSGlobalVarRegexInfoToCache()
{
	var src_filepath = GetRegexGlobalVarSrcCacheFilePath();
	var pattern_filepath = GetRegexGlobalVarPatternCacheFilePath();

	comx.sys.CreateFileHandler('regex_src', src_filepath);
	comx.file.regex_src = /*binding.js_doc*/G_js_doc_backup;
	comx.sys.CloseFileHandler('regex_src');

	comx.sys.CreateFileHandler('regex_pattern', pattern_filepath);
	comx.file.regex_pattern = '^[ \t]*var[ \t]*([Gg]{1}_[a-zA-Z0-9\_]*)';
	comx.sys.CloseFileHandler('regex_pattern');
}

var G_js_func_info = false;
var G_js_variable_info = false;

var G_js_func_name = false;
var G_js_var_name = false;

var G_js_global_variable_info = false;

var G_js_func_tip_info = {};
var G_js_predefine_func_tip_info = {};

var G_js_doc_backup = false;

var G_refresh_js_info_lock = false;

function InitializePredefineTipInfo()
{
    G_js_predefine_func_tip_info["TScintillaWnd"]   = "function TScintillaWnd(hdr)";
    G_js_predefine_func_tip_info["TBitmapWnd"]      = "function TBitmapWnd(hdr)";
    G_js_predefine_func_tip_info["TToolbar"]        = "function TToolbar(toolbarid)";
    G_js_predefine_func_tip_info["TXmlDocument"]    = "function TXmlDocument(encode)";
    G_js_predefine_func_tip_info["TOpenGLRecord"]   = "function TOpenGLRecord(db_hdr, db_rec)";
    G_js_predefine_func_tip_info["TOpenGLSensor"]   = "function TOpenGLSensor(window_hdr)";
    G_js_predefine_func_tip_info["TOpenglDatabase"] = "function TOpenglDatabase(window_hdr, db_hdr)";
    G_js_predefine_func_tip_info["require"]         = "function require(js_file)";
    G_js_predefine_func_tip_info["SetTimeout"]      = "function SetTimeout(uElapse, fn)";
    G_js_predefine_func_tip_info["KillTimeout"]     = "function KillTimeout(nTimerID)";
    G_js_predefine_func_tip_info["PlayGif"]         = "function PlayGif(gif_fpath, uElapse, cb)";
    G_js_predefine_func_tip_info["PlayGifRollBack"] = "function PlayGifRollBack(gif_fpath, uElapse, cb)";
    G_js_predefine_func_tip_info["StopGif"]         = "function StopGif(nID)";
    G_js_predefine_func_tip_info["GetNilBitmap"]    = "function GetNilBitmap()";
    G_js_predefine_func_tip_info["GetGifFullPath"]  = "function GetGifFullPath(gif_fname)";
    G_js_predefine_func_tip_info["alert"]           = "function alert(str);//popup modal alert information dialog.";
    
    G_js_predefine_func_tip_info["TOpenglDatabase.Refresh"]         = "function TOpenglDatabase.Refresh();";
    G_js_predefine_func_tip_info["TOpenglDatabase.CreateRecord"]    = "function TOpenglDatabase.CreateRecord(type, sub_type)";
    G_js_predefine_func_tip_info["TOpenglDatabase.Begin"]           = "function TOpenglDatabase.Begin(obj_db_rec)";
    G_js_predefine_func_tip_info["TOpenglDatabase.End"]             = "function TOpenglDatabase.End()";
    G_js_predefine_func_tip_info["TOpenglDatabase.BeginPrimitive"]  = "function TOpenglDatabase.BeginPrimitive(type)";
    G_js_predefine_func_tip_info["TOpenglDatabase.EndPrimitive"]    = "function TOpenglDatabase.EndPrimitive()";
    G_js_predefine_func_tip_info["TOpenglDatabase.InputVertex"]     = "function TOpenglDatabase.InputVertex(x, y, z)";
    G_js_predefine_func_tip_info["TOpenglDatabase.InputNormal"]     = "function TOpenglDatabase.InputNormal(x, y, z)";
    
    G_js_predefine_func_tip_info["TOpenGLSensor.Activate"] = "function TOpenGLSensor.Activate(sensor_type)";
    
    G_js_predefine_func_tip_info["TOpenGLRecord.GetKey"]            = "function TOpenGLRecord.GetKey();";
    G_js_predefine_func_tip_info["TOpenGLRecord.GetDisplayList"]    = "function TOpenGLRecord.GetDisplayList();";
    G_js_predefine_func_tip_info["TOpenGLRecord.GetRenderMode"]     = "function TOpenGLRecord.GetRenderMode();";
    G_js_predefine_func_tip_info["TOpenGLRecord.SetRenderMode"]     = "function TOpenGLRecord.SetRenderMode(mode) // 0 - common; 1 - highlight; 2 - mark.;";
    G_js_predefine_func_tip_info["TOpenGLRecord.GetColor"]          = "function TOpenGLRecord.GetColor() // \"r, g, b, a\", such as : \"255, 255, 0, 0\".;";
    G_js_predefine_func_tip_info["TOpenGLRecord.SetColor"]          = "function TOpenGLRecord.SetColor(clr);";
    G_js_predefine_func_tip_info["TOpenGLRecord.GetHightLightColor"]= "function TOpenGLRecord.GetHightLightColor() // \"r, g, b, a\", such as : \"255, 255, 0, 0\".;";
    G_js_predefine_func_tip_info["TOpenGLRecord.SetHightLightColor"]= "function TOpenGLRecord.SetHightLightColor(clr);";
    G_js_predefine_func_tip_info["TOpenGLRecord.GetMarkColor"]      = "function TOpenGLRecord.GetMarkColor() // \"r, g, b, a\", such as : \"255, 255, 0, 0\".;";
    G_js_predefine_func_tip_info["TOpenGLRecord.SetMarkColor"]      = "function TOpenGLRecord.SetMarkColor(clr)";
    G_js_predefine_func_tip_info["TOpenGLRecord.GetPolygonMode"]    = "function TOpenGLRecord.GetPolygonMode() // 0 - Wire, 1 - Face, 2 - Vertex.";
    G_js_predefine_func_tip_info["TOpenGLRecord.SetPolygonMode"]    = "function TOpenGLRecord.SetPolygonMode(mode)";
    G_js_predefine_func_tip_info["TOpenGLRecord.GetLineWidth"]      = "function TOpenGLRecord.GetLineWidth()";
    G_js_predefine_func_tip_info["TOpenGLRecord.SetLineWidth"]      = "function TOpenGLRecord.SetLineWidth(line_width)";
    G_js_predefine_func_tip_info["TOpenGLRecord.GetPointSize"]      = "function TOpenGLRecord.GetPointSize()";
    G_js_predefine_func_tip_info["TOpenGLRecord.SetPointSize"]      = "function TOpenGLRecord.SetPointSize(point_size)";
    G_js_predefine_func_tip_info["TOpenGLRecord.IsLight"]           = "function TOpenGLRecord.IsLight()";
    G_js_predefine_func_tip_info["TOpenGLRecord.EnableLight"]       = "function TOpenGLRecord.EnableLight(flag)";
    G_js_predefine_func_tip_info["TOpenGLRecord.IsBlend"]           = "function TOpenGLRecord.IsBlend()";
    G_js_predefine_func_tip_info["TOpenGLRecord.EnableBlend"]       = "function TOpenGLRecord.EnableBlend(flag)";
    G_js_predefine_func_tip_info["TOpenGLRecord.IsVisible"]         = "function TOpenGLRecord.IsVisible()";
    G_js_predefine_func_tip_info["TOpenGLRecord.EnableVisible"]     = "function TOpenGLRecord.EnableVisible(flag)";
    G_js_predefine_func_tip_info["TOpenGLRecord.IsBosseyed"]        = "function TOpenGLRecord.IsBosseyed()";
    G_js_predefine_func_tip_info["TOpenGLRecord.EnableBosseyed"]    = "function TOpenGLRecord.EnableBosseyed(flag)";
    G_js_predefine_func_tip_info["TOpenGLRecord.IsPlotMode"]        = "function TOpenGLRecord.IsPlotMode()";
    G_js_predefine_func_tip_info["TOpenGLRecord.EnablePlotMode"]    = "function TOpenGLRecord.EnablePlotMode(flag)";
    
    G_js_predefine_func_tip_info["TXmlDocument.Dispose"]          = "function TXmlDocument.Dispose()";
    G_js_predefine_func_tip_info["TXmlDocument.Load"]             = "function TXmlDocument.Load(file_path)";
    G_js_predefine_func_tip_info["TXmlDocument.Save"]             = "function TXmlDocument.Save(file_path)";
    G_js_predefine_func_tip_info["TXmlDocument.Beautify"]         = "function TXmlDocument.Beautify(file_path)";
    G_js_predefine_func_tip_info["TXmlDocument.Search"]           = "function TXmlDocument.Search(key)";
    G_js_predefine_func_tip_info["TXmlDocument.ClearRecords"]     = "function TXmlDocument.ClearResults()";
    G_js_predefine_func_tip_info["TXmlDocument.GetRecordsNumber"] = "function TXmlDocument.GetRecordsNumber()";
    G_js_predefine_func_tip_info["TXmlDocument.GetRecord"]        = "function TXmlDocument.GetRecord(index)";
    G_js_predefine_func_tip_info["TXmlDocument.ResetPos"]         = "function TXmlDocument.ResetPos()";
    G_js_predefine_func_tip_info["TXmlDocument.SavePos"]          = "function TXmlDocument.SavePos(name)";
    G_js_predefine_func_tip_info["TXmlDocument.RestorePos"]       = "function TXmlDocument.RestorePos(name)";
    G_js_predefine_func_tip_info["TXmlDocument.FindElem"]         = "function TXmlDocument.FindElem(name)";
    G_js_predefine_func_tip_info["TXmlDocument.IntoElem"]         = "function TXmlDocument.IntoElem()";
    G_js_predefine_func_tip_info["TXmlDocument.OutOfElem"]        = "function TXmlDocument.OutOfElem()";
    G_js_predefine_func_tip_info["TXmlDocument.GetData"]          = "function TXmlDocument.GetData()";
    G_js_predefine_func_tip_info["TXmlDocument.GetAttrib"]        = "function TXmlDocument.GetAttrib()";
    G_js_predefine_func_tip_info["TXmlDocument.SetData"]          = "function TXmlDocument.SetData(data)";
    G_js_predefine_func_tip_info["TXmlDocument.SetAttrib"]        = "function TXmlDocument.SetAttrib(data)";
    G_js_predefine_func_tip_info["TXmlDocument.AddElem"]          = "function TXmlDocument.AddElem(elem_name)";
    G_js_predefine_func_tip_info["TXmlDocument.InserElem"]        = "function TXmlDocument.InserElem(elem_name)";
    G_js_predefine_func_tip_info["TXmlDocument.IsWellFormat"]     = "function TXmlDocument.IsWellFormat()";
    G_js_predefine_func_tip_info["TXmlDocument.RemoveElem"]       = "function TXmlDocument.RemoveElem()";
                                                                                
    G_js_predefine_func_tip_info["TToolbar.Activate"] = "function TToolbar.Activate(index)";      
    G_js_predefine_func_tip_info["TToolbar.Check"]    = "function TToolbar.Check(index, flag)";
    G_js_predefine_func_tip_info["TToolbar.IsCheck"]  = "function TToolbar.IsCheck(index)";
    G_js_predefine_func_tip_info["TToolbar.Enable"]   = "function TToolbar.Enable(index, flag)";
    G_js_predefine_func_tip_info["TToolbar.IsEnable"] = "function TToolbar.IsEnable(index)";
    G_js_predefine_func_tip_info["TToolbar.Relayout"] = "function TToolbar.Relayout()";
    G_js_predefine_func_tip_info["TToolbar.Hide"]     = "function TToolbar.Hide()";
    G_js_predefine_func_tip_info["TToolbar.Show"]     = "function TToolbar.Show()";
    
    G_js_predefine_func_tip_info["TBitmapWnd.SaveAs"]                = "function TBitmapWnd.SaveAs(fpath)";
    G_js_predefine_func_tip_info["TBitmapWnd.SetDisplayMode"]        = "function TBitmapWnd.SetDisplayMode(mode)";
    G_js_predefine_func_tip_info["TBitmapWnd.SetBackColor"]          = "function TBitmapWnd.SetBackColor(red, green, blue)";
    G_js_predefine_func_tip_info["TBitmapWnd.SetTransparentColor"]   = "function TBitmapWnd.SetTransparentColor(red, green, blue)";
    G_js_predefine_func_tip_info["TBitmapWnd.EnableTransparentMode"] = "function TBitmapWnd.EnableTransparentMode(flag)";
    G_js_predefine_func_tip_info["TBitmapWnd.AddPoint"]              = "function TBitmapWnd.AddPoint(x, y)"; 
    G_js_predefine_func_tip_info["TBitmapWnd.ClearPoints"]           = "function TBitmapWnd.ClearPoints()";

    G_js_predefine_func_tip_info["TScintillaWnd.Paste"]               = "function TScintillaWnd.Paste()";
    G_js_predefine_func_tip_info["TScintillaWnd.Copy"]                = "function TScintillaWnd.Copy()";
    G_js_predefine_func_tip_info["TScintillaWnd.GetFirstVisibleLine"] = "function TScintillaWnd.GetFirstVisibleLine()";
    G_js_predefine_func_tip_info["TScintillaWnd.SetFirstVisibleLine"] = "function TScintillaWnd.SetFirstVisibleLine(line_no)";
    G_js_predefine_func_tip_info["TScintillaWnd.Undo"]                = "function TScintillaWnd.Undo()";
    G_js_predefine_func_tip_info["TScintillaWnd.Redo"]                = "function TScintillaWnd.Redo()";
    G_js_predefine_func_tip_info["TScintillaWnd.Cut"]                 = "function TScintillaWnd.Cut()";
    G_js_predefine_func_tip_info["TScintillaWnd.Clear"]               = "function TScintillaWnd.Clear()";
    G_js_predefine_func_tip_info["TScintillaWnd.GetSize"]             = "function TScintillaWnd.GetSize()";
    G_js_predefine_func_tip_info["TScintillaWnd.GetLineCount"]        = "function TScintillaWnd.GetLineCount()";
    G_js_predefine_func_tip_info["TScintillaWnd.IsDirty"]             = "function TScintillaWnd.IsDirty()";
    G_js_predefine_func_tip_info["TScintillaWnd.ClearDirtyFlag"]      = "function TScintillaWnd.ClearDirtyFlag()";
    G_js_predefine_func_tip_info["TScintillaWnd.GetPosition"]         = "function TScintillaWnd.GetPosition()";
    G_js_predefine_func_tip_info["TScintillaWnd.SetPosition"]         = "function TScintillaWnd.SetPosition(pos)";

    G_js_predefine_func_tip_info["websocket.InvokeSandbox"]         = "function websocket.InvokeSandbox(formid, cache, callback)";
}   

function RefreshJSInfo()
{
    if(G_refresh_js_info_lock)
    {
        return;
    }
    else
    {
        G_refresh_js_info_lock = true;
    }
    
    G_js_func_info = false;
    G_js_variable_info = false;
    G_js_global_variable_info = false;
    G_js_doc_backup = binding.js_doc;
    
    //binding.js_func_list = "";
    //binding.js_variable_list = "";
    
    G_js_func_name = binding.js_func_val;
    G_js_var_name = binding.js_variable_val;
    
    //binding.js_func_val = "parsing...";
    //binding.js_variable_val = "parsing...";
    
	//SetTimeout(0, function(nTimerID){
	//	KillTimeout(nTimerID);
		SaveJSFuncRegexInfoToCache();
		websocket.Invoke('GetFunctionListOfJavaScript', {'src' : GetRegexFuncSrcCacheFilePath() , 'pattern' : GetRegexFuncPatternCacheFilePath(), 'res' : GetRegexFuncResultCacheFilePath()}, function(data){
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
            
            SaveJSVarRegexInfoToCache();
			websocket.Invoke('GetVariableListOfJavaScript', {'src' : GetRegexVarSrcCacheFilePath() , 'pattern' : GetRegexVarPatternCacheFilePath(), 'res' : GetRegexVarResultCacheFilePath()}, function(data){
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
			
				SaveJSGlobalVarRegexInfoToCache();
				websocket.Invoke('GetGlobalVariableListOfJavaScript', {'src' : GetRegexGlobalVarSrcCacheFilePath() , 'pattern' : GetRegexGlobalVarPatternCacheFilePath(), 'res' : GetRegexGlobalVarResultCacheFilePath()}, function(data){
					if(data.parameters.type == 'ok')
					{
						G_js_global_variable_info = data.parameters;
					}

					RefreshGlobalVariableInfoRes();
                    G_refresh_js_info_lock = false;
				});
			});  

 		});
 	//});
}

function get_brace_end_pos(strDoc, start_pos)
{
    if(strDoc[start_pos] != '{')
    {
        return false;
    }
    
    var cnt = 0;
    
    for(var loop = start_pos + 1; loop < strDoc.length; ++loop)
    {
        if(strDoc[loop] == '{')
        {
            cnt++;
        }
        
        if(strDoc[loop] == '}')
        {
            if(cnt == 0)
            {
                return loop;
            }
            else
            {
                cnt--;
            }
        }
    }
    
    return false;
}

var G_func_pos_list = [];

function RefreshJSFuncList()
{
    if(G_js_func_info == false)
		return;
        
    var result = G_js_func_info.result;
    var func_list = '';

	var js_func_name = G_js_func_name;//binding.js_func_val;
    
    G_js_func_tip_info = {};
    G_func_pos_list = [];
    
    var index = 0;
    for(var i = 0; i < result.length; ++i)
	{
        var item_name = /*binding.js_doc*/G_js_doc_backup.substr(result[i][1].start, result[i][1].end - result[i][1].start);

        var tip_info = /*binding.js_doc.*/G_js_doc_backup.substr(result[i][0].start, result[i][0].end - result[i][0].start);
        
        var start_pos = result[i][3].start;
        var end_pos = get_brace_end_pos(G_js_doc_backup, start_pos);
        //comx.ui.InformationBox(G_js_doc_backup.substr(start_pos, end_pos - start_pos + 1));
        var func_content = G_js_doc_backup.substr(result[i][0].start, end_pos - result[i][0].start + 1);
        G_func_pos_list.push({'start' : start_pos, 'end' : end_pos, 'func_content' : func_content});
        
        G_js_func_tip_info[item_name] = tip_info;
        
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
        var type = /*binding.js_doc*/G_js_doc_backup.substr(result[i][0].start, result[i][0].end - result[i][0].start)
        var name = /*binding.js_doc*/G_js_doc_backup.substr(result[i][1].start, result[i][1].end - result[i][1].start);
        
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
    if(G_ac_insert_str != false)
    {
        var text = G_ac_insert_str;
        G_ac_insert_str = false;
        
        SetTimeout(50, function(nID){
            KillTimeout(nID);
            
            if(text.indexOf('websocket.on') != -1)
            {
                var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - text.length);;
                var insert_text = 'websocket.on(\'[name]\'\, function(data){\n'  + generate_space_str(offset)  + '\n' + generate_space_str(offset) + '});';
                widget.scintilla.FindPrevious(binding.js_doc_hdr, text);
                widget.scintilla.Cut(binding.js_doc_hdr);
                comx.util.CopyToClipboard(insert_text);
                widget.scintilla.Paste(binding.js_doc_hdr);
                
                widget.scintilla.FindPrevious(binding.js_doc_hdr, '[name]');
                widget.scintilla.Cut(binding.js_doc_hdr);
            }
            
            if(text.indexOf('websocket.Invoke') != -1)
            {
                var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - text.length);;
                var insert_text = 'websocket.Invoke(\'[name]\'\, function(data){\n'  + generate_space_str(offset)  + '\n' + generate_space_str(offset) + '});';
                widget.scintilla.FindPrevious(binding.js_doc_hdr, text);
                widget.scintilla.Cut(binding.js_doc_hdr);
                comx.util.CopyToClipboard(insert_text);
                widget.scintilla.Paste(binding.js_doc_hdr);
                
                widget.scintilla.FindPrevious(binding.js_doc_hdr, '[name]');
                widget.scintilla.Cut(binding.js_doc_hdr);
            }
            
            if(text == 'if(){...}')
            {
                var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - text.length);;
                var insert_text = 'if([condition])\n'  + generate_space_str(offset)  + '{\n' + generate_space_str(offset) + '}';
                widget.scintilla.FindPrevious(binding.js_doc_hdr, text);
                comx.util.CopyToClipboard(insert_text);
                widget.scintilla.Paste(binding.js_doc_hdr);
                
                widget.scintilla.FindPrevious(binding.js_doc_hdr, '[condition]');
                widget.scintilla.Cut(binding.js_doc_hdr);
            }
            
            if(text == 'if(){...}else{}')
            {
                var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - text.length);;
                var insert_text = 'if([condition])\n'  + 
                                    generate_space_str(offset)  + '{\n' + 
                                    generate_space_str(offset) + '}' + '\n' + 
                                    generate_space_str(offset) + 'else\n' + 
                                    generate_space_str(offset)+ '{\n' + 
                                    generate_space_str(offset) + '}';
                widget.scintilla.FindPrevious(binding.js_doc_hdr, text);
                comx.util.CopyToClipboard(insert_text);
                widget.scintilla.Paste(binding.js_doc_hdr);
                
                widget.scintilla.FindPrevious(binding.js_doc_hdr, '[condition]');
                widget.scintilla.Cut(binding.js_doc_hdr);
            }
            
            if(text == 'for(var key in [obj]){...}')
            {
                var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - text.length);;
                var insert_text = 'for(var key in [obj])\n' + generate_space_str(offset)  + '{\n' + generate_space_str(offset) + '}';
                widget.scintilla.FindPrevious(binding.js_doc_hdr, text);
                widget.scintilla.Cut(binding.js_doc_hdr);
                comx.util.CopyToClipboard(insert_text);
                widget.scintilla.Paste(binding.js_doc_hdr);
                
                widget.scintilla.FindPrevious(binding.js_doc_hdr, '[obj]');
                widget.scintilla.Cut(binding.js_doc_hdr);
            }
            
            if(text == 'for(){...}')
            {
                var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - text.length);;
                var insert_text = 'for(var loop = 0; loop < [num]; ++loop)\n' + generate_space_str(offset)  + '{\n' + generate_space_str(offset) + '}';
                widget.scintilla.FindPrevious(binding.js_doc_hdr, text);
                widget.scintilla.Cut(binding.js_doc_hdr);
                comx.util.CopyToClipboard(insert_text);
                widget.scintilla.Paste(binding.js_doc_hdr);
                
                widget.scintilla.FindPrevious(binding.js_doc_hdr, '[num]');
                widget.scintilla.Cut(binding.js_doc_hdr);
            }
        });
    }
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
    var keywords = ['break','case','catch','continue','default','delete','do','else','finally',
                'function','in','instanceof','new','return','switch','this','throw','try','typeof',
                'var','void','while','with','abstract','boolean','byte','char','class','const','debugger',
                'double','enum','export','extends','final','float','goto','implements','import','int','undefined',
                'interface','long','native','package','private','protected','public','short','static','true', 'false','eval',
                'if(){...}','if(){...}else{}','for(var key in [obj]){...}', 'for(){...}', 
                'super','synchronized','throws','transient','volatile'];
                
    var ret = [];
    for(var loop = 0; loop < keywords.length; ++loop)
    {
        ret.push(keywords[loop] + '?4');
    }
    
    ret.push('comx?2');
    ret.push('binding?2');
    ret.push('pane?2');
    ret.push('unit?2');
    ret.push('widget?2');
    ret.push('gl?2');
 
    var func_list = binding.js_func_list.split(',');
    for(var loop = 1; loop < func_list.length; ++loop)
    {
        ret.push(func_list[loop] + '?3');
    }
    
    ret.push('websocket?2');
    ret.push('websocket.on(...)?6');
    ret.push('websocket.Invoke(...)?5');
    ret.push('websocket.InvokeEx(...)?5');
    
    ret.push('TScintillaWnd?2');
    ret.push('TBitmapWnd?2');
    ret.push('TToolbar?2');
    ret.push('TXmlDocument?2');
    ret.push('TOpenGLRecord?2');
    ret.push('TOpenGLSensor?2');
    ret.push('TOpenglDatabase?2');
    
    ret.push('require?3');
    ret.push('alert?3');
    ret.push('SetTimeout?3');
    ret.push('KillTimeout?3');
    ret.push('PlayGif?3');
    ret.push('PlayGifRollBack?3');
    ret.push('StopGif?3');
    ret.push('GetNilBitmap?3');
    ret.push('GetGifFullPath?3');
    
    
    var global_val_list = get_global_var_list();
    
    for(var loop = 0; loop < global_val_list.length; ++loop)
    {
        ret.push(global_val_list[loop] + '?1');
    }
    
    var local_var_list = get_local_var_list();

    for(var loop = 0; loop < local_var_list.length; ++loop)
    {
        ret.push(local_var_list[loop] + '?1');
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
        if(ac_list[loop] != 'gf?2' && ac_list[loop] != 'file?2')
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
        if(ac_list[loop] != 'formid?1')
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
		var json_obj = data.parameters.json_content;

		for(var loop = 0; loop < json_obj.length; ++loop)
		{
			if(json_obj[loop].js_filepath == binding.js_doc_filename)
			{
				G_associated_kul_fname = json_obj[loop].kul_filepath;
			}
		}
        
        //alert(JSON.stringify(json_obj));
        
        RefreshBingInfo();
	});
}

function RefreshBingInfo()
{
    G_binding_info = false;
    
    if(G_associated_kul_fname != '')
    {
        websocket.Invoke('GetKulBindingInfo', {'kul_fname' : G_associated_kul_fname}, function(data){
            if(data.parameters)
            {
                G_binding_info = data.parameters;
            }
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
    else
    {
        var ac_list_of_obj = get_ac_list_of_obj_by_ns(ns);
        
        if(ac_list_of_obj.length != 0)
        {
            var ac_list = filter_ac_list_by_str(ac_list_of_obj, ac_str);
            widget.scintilla.AutoComplete(binding.js_doc_hdr, ac_list, ac_str.length);
        }
    }
    
    if(ns == 'unit')
    {
        if(!comx.gf.kul_editor_current_unit_name)
        {
            return;
        }
        
        var ac_list_arr = comx.sys.GetFormIDListOfUnit(comx.gf.kul_editor_current_unit_name);
        ac_list_arr = ac_list_arr.split(',');
        
        var ac_list = [];
        ac_list.push('name?1');
        for(var loop = 1; loop < ac_list_arr.length; ++loop)
        {
            ac_list.push(ac_list_arr[loop] + '?1');
        }
        
        if(ac_list.length != 0)
		{
			widget.scintilla.AutoComplete(binding.js_doc_hdr, ac_list, ac_str.length);
		}
    }
    else if(ns == 'binding')
    {
        if(G_binding_info != false)
        {
            var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - ac_str.length);;
			var ac_list_origin = filter_ac_list_by_str(G_binding_info, ac_str);
            
            var ac_list = [];
            for(var loop = 0; loop < ac_list_origin.length; ++loop)
            {
                ac_list.push(ac_list_origin[loop] + '?1');
            }
			
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
           ns.indexOf('my_math') == 0 ||
           ns.indexOf('g_') == 0 ||
           ns.indexOf('G_') == 0)
        {
            return;
        }
        
        var ns_obj = false;
        
        try
        {
			ns_obj = eval(ns);
        }
        catch(err)
        {
            ns_obj = false;
        }
        
		if(typeof(ns_obj) == 'object')
        {
            var offset = widget.scintilla.GetOffsetOfPos(binding.js_doc_hdr, widget.scintilla.GetPosition(binding.js_doc_hdr) - ac_str.length);;

			var ac_list = [];
            for(var key in ns_obj)
            {
                if(typeof(ns_obj[key]) == 'object')
                {
                    ac_list.push(key + '?2');
                }
                else if(typeof(ns_obj[key]) == 'function')
                {
                    ac_list.push(key + '?3');
                }
                else
                {
                    ac_list.push(key + '?1');
                }
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
    
    //test codes
    //var func_prototype = get_func_tip_info_by_prefix(prefix);
    //if(func_prototype)
    //{
    //    comx.ui.InformationBox(func_prototype);
    //}
}

function get_func_tip_info_by_prefix(prefix)
{
    var ns = pickup_ns_from_prefix(prefix);
    var ac_str = pickup_ac_str_from_prefix(prefix);
    
  	var ret = false;
    
    if(ns != '')
    {
        var comments = comx.sys.GetFunctionCommentsBuiltIn(ns, ac_str);
        if(comments != '-')
        {
            ret = comments;
        }
    }
    else
    {
        if(G_js_func_tip_info[ac_str] != undefined)
        {
            ret = G_js_func_tip_info[ac_str];
        }
        if(G_js_predefine_func_tip_info[ac_str] != undefined)
        {
            ret = G_js_predefine_func_tip_info[ac_str];
        }
    }
    
    if(G_js_obj_call_tip[prefix] != undefined)
    {
        ret = G_js_obj_call_tip[prefix];
    }
    
    return ret;
}

function get_seperator_of_tip_info(tip_info)
{
    var ret = [];
    for(var loop = 0; loop < tip_info.length; ++loop)
    {
        if(tip_info[loop] == '(')
        {
            ret.push(loop);
        }
        
        if(tip_info[loop] == ')')
        {
            ret.push(loop);
        }
        
        if(tip_info[loop] == ',')
        {
            ret.push(loop);
        }
    }
    
    return ret;
}

function get_start_pos_of_tip_info_para_by_index(tip_info, index)
{
    var separator_arr = get_seperator_of_tip_info(tip_info);
    return separator_arr[index] + 1;
}

function get_end_pos_of_tip_info_para_by_index(tip_info, index)
{
    var separator_arr = get_seperator_of_tip_info(tip_info);
    return separator_arr[index + 1];
}

function OnCallTip(prefix, pos, index)
{
    index = parseInt(index);
    pos = parseInt(pos);
    var tip_info = get_func_tip_info_by_prefix(prefix);

    if(tip_info)
    {
        var start_pos = get_start_pos_of_tip_info_para_by_index(tip_info, index);
        var end_pos = get_end_pos_of_tip_info_para_by_index(tip_info, index);
        
        //tip_info += ('\r\n' + start_pos + ',' + end_pos + '\r\n' + index);
        widget.scintilla.CallTip(binding.js_doc_hdr, /*widget.scintilla.GetPosition(binding.js_doc_hdr)*/pos, tip_info); 
        
        if(start_pos != undefined && end_pos != undefined)
        {
			widget.scintilla.CallTipHightLight(binding.js_doc_hdr,start_pos, end_pos);
        }
    }
}

var G_js_global_variable_info_res = false;
function RefreshGlobalVariableInfoRes()
{
    var ret = [];
    if(G_js_global_variable_info == false)
    {
        G_js_global_variable_info_res = ret;
		return;
    }
        
    var result = G_js_global_variable_info.result;
     
    for(var i = 0; i < result.length; ++i)
	{
        var name = /*binding.js_doc*/G_js_doc_backup.substr(result[i][0].start, result[i][0].end - result[i][0].start);
   
        ret.push(name);
	}
    
    G_js_global_variable_info_res = ret;
}

function get_global_var_list()
{
    return G_js_global_variable_info_res;
}

function get_local_var_list()
{
    var current_pos = parseInt(widget.scintilla.GetPosition(binding.js_doc_hdr));
    for(var loop = 0; loop < G_func_pos_list.length; ++loop)
    {
        if(current_pos > G_func_pos_list[loop].start && current_pos < G_func_pos_list[loop].end)
        {
            var cur_func_content = G_func_pos_list[loop].func_content;//G_js_doc_backup.substr(G_func_pos_list[loop].start, G_func_pos_list[loop].end - G_func_pos_list[loop].start + 1);
            var tmp_filepath = comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\data\\func_content.tmp";
            comx.sys.CreateFileHandler('func_content', tmp_filepath);
            comx.file.func_content = cur_func_content;
            comx.sys.CloseFileHandler('func_content');
            
            return JSON.parse(comx.shell.GetVarListOfJsFunc(tmp_filepath));
        }
    }
    
    return [];
}

var G_js_obj_call_tip = {};

function get_ac_list_of_obj_by_ns(ns)
{
    var tmp_filepath = comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\data\\func_content.tmp";
    comx.sys.CreateFileHandler('func_content', tmp_filepath);
    comx.file.func_content = binding.js_doc;
    comx.sys.CloseFileHandler('func_content');
            
    var js_objs = JSON.parse(comx.shell.GetObjectListFromJSFile(tmp_filepath));

    G_js_obj_call_tip = {};
    
    var ret = [];
    if(typeof(js_objs[ns]) == 'string')
    {
        var class_instance = eval(js_objs[ns] + '.prototype');
        for(var key in class_instance)
        {
            if(typeof(class_instance[key]) == 'function')
            {
                ret.push(key + '?3');
                var predefine_key = js_objs[ns] + '.' + key;
                if(G_js_predefine_func_tip_info[predefine_key] != undefined)
                {
                    G_js_obj_call_tip[ns + '.' + key] = G_js_predefine_func_tip_info[predefine_key];
                }
            }
        }
    }
    
    return ret;
}

var G_ac_insert_str = false;
function OnAutoCompleteSelect(text)
{
    G_ac_insert_str = text;
}

function OnSimpleUnit()
{
    comx.ui.PopupSystemModalForm("0x031a2227-0x850c-0x44e6-0x8b-0x9b-0xcc-0x66-0x42-0x9b-0xa3-0xf7");
}

function OnDevToolkit()
{
    comx.dev.CallDevelopmentCenter();
}

function OnUpdateUnit()
{
    websocket.Invoke('UpdateUnit', {'unit_name' : comx.gf.kul_editor_current_unit_name},function(data){
    
    });
}

var g_top_menu_index_bak = 0;

function OnTopMenuSwitch()
{
    if(parseInt(comx.gf.tabctrl_index) == 3)
    {
        binding.global_index = 1;
        binding.lesson_progress_visible1 = false;
    }
    else
    {
        g_top_menu_index_bak = binding.top_menu_index;
    }
}

var player = require('class/gif_animate.js');

var g_old_gif_path = false;

var g_is_lesson_playing = false;

function OnPlayManualGif()
{
    var gif_path = comx.sys.GetEnvironmentVariables('COMX_SDK') + '/manual/' + GetLessonFileByIndex(parseInt(binding.lesson_list_index));
    if(g_old_gif_path != gif_path)
    {
        g_old_gif_path = gif_path;
        binding.lesson_bmp = gif_path;
    }
    else
    {
        binding.lesson_bmp = "play";
    }
    
    g_is_lesson_playing = true;
    binding.lesson_progress_visible1 = g_is_lesson_playing;
    binding.gif_play_enable = false;
    binding.gif_pause_enable = binding.gif_stop_enable = true;
}

function OnPause()
{
    binding.lesson_bmp = "";
    g_is_lesson_playing = false;
    //binding.lesson_progress_visible = g_is_lesson_playing;
    binding.gif_play_enable = true;
    binding.gif_stop_enable = true;
    binding.gif_pause_enable = false;
}

function OnBackMainPane()
{
    OnPause();
    binding.top_menu_index = g_top_menu_index_bak; 
    binding.global_index = 0;
}

function GetWordPath()
{
    var ret = comx.shell.GetAppPath("winword");
    
    if(ret == 'false')
    {
        return false;
    }
    
    if(ret.split('"').length >= 2)
    {
        return ret.split('"')[1];
    }
    
    return ret;
}

function GetWPSPath()
{
    var ret = comx.shell.GetAppPath('wps');
        
    if(ret == 'false')
    {
        return false;
    }
    
    if(ret.split('"').length >= 2)
    {
        return ret.split('"')[1];
    }
    
    return ret;
}

function GetOperatorSystemVer()
{
    var os_ver_fname = comx.sys.GetEnvironmentVariables('COMX_SDK') + 'data/os.ver.txt';
    
    if(!comx.sys.IsFileExist(os_ver_fname))
    {
        return;
    }
    
    comx.sys.CreateFileHandler('os_ver_fname', os_ver_fname);
    var os_ver_content = comx.file.os_ver_fname;
    comx.sys.CloseFileHandler('os_ver_fname');
    
    return (os_ver_content.match(/[0-9]*\.[0-9]*\.[0-9]*/).toString());
}

function IsWindows80OrLater()
{
    var os_ver = GetOperatorSystemVer();
    var major_ver = os_ver.split('.')[0];
    var sub_ver = os_ver.split('.')[1];
    
    return (parseInt(major_ver) >= 6 && parseInt(sub_ver) >= 2);
}

function IsDevMode()
{
    return false;//comx.sys.IsFileExist(comx.sys.GetEnvironmentVariables('COMX_SDK') + '.gitignore');
}

function OnBook()
{
    if(!IsDevMode())
    {
        comx.shell.Execute('open', comx.sys.GetEnvironmentVariables('COMX_SDK') + 'manual\\comx.manual.pdf');
        return;
    }
    
    var word_path = GetWordPath();
    var wps_path = GetWPSPath();
    
    if(word_path && !IsWindows80OrLater())
    {
        var cmd = '"' + word_path + '" /n ' + comx.sys.GetEnvironmentVariables('COMX_SDK') + 'manual\\comx.manual.docx';
        comx.shell.run_ex(cmd, true, false);
        
        return;
    }
    
    if(wps_path)
    {
        var cmd = '"' + wps_path + '" /n ' + comx.sys.GetEnvironmentVariables('COMX_SDK') + 'manual\\comx.manual.docx';
        comx.shell.run_ex(cmd, true, false);
        
        return;
    }
 
    comx.shell.Execute('open', comx.sys.GetEnvironmentVariables('COMX_SDK') + 'manual\\comx.manual.pdf');
}

function OnStop()
{
    var nil_gif = comx.sys.GetEnvironmentVariables('COMX_SDK') + '/picture/startup.gif';

    binding.lesson_bmp = nil_gif;
    g_old_gif_path = nil_gif;
    
    g_is_lesson_playing = false;
    binding.lesson_progress_visible1 = g_is_lesson_playing;
    
    binding.gif_play_enable = true;
    binding.gif_pause_enable = binding.gif_stop_enable = false;
}

function OnLessonProgress()
{
    //binding.lesson_bmp = "progress," + binding.lesson_progress;
}
function OnLessonGifPress()
{
    if(g_is_lesson_playing)
    {
        OnPause();
    }
    else
    {
        var nil_gif = comx.sys.GetEnvironmentVariables('COMX_SDK') + '/picture/startup.gif';
        if(g_old_gif_path == false || g_old_gif_path == nil_gif)
        {
            return;
        }
        OnPlayManualGif();
    }
}
