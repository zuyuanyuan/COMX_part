/////////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');
var player = require('class/gif_animate.js');   

var is_new_block = false;

function GenerateGUID()
{
    var guid = "";
    for (var i = 1; i <= 32; i++){
        var n = Math.floor(Math.random()*16.0).toString(16);
        guid +=   n;
    }
    return guid;
}

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
    if(comx.gf.new_document_integrate_mode == 'true')
    {
        websocket.Connect();
    }
    else
    {
        websocket.Startup(false/*debug_mode*/);
        binding.tabctrl_index = 1;
        binding.notify_bmp = comx.sys.GetEnvironmentVariables('COMX_SDK') + 'picture/nil.bmp';
        SetTimeout(0, function(nID){
            KillTimeout(nID);
            binding.notify_label = false;
        });
    }
    
	binding.unit_name_list = comx.sys.GetUnitNameList(true);
	var index = GetIndexByName(comx.gf.kul_editor_current_unit_name);
    
    comx.shell.kulclean(binding.unit_name);

	if(index != -1)
	{
		binding.unit_name_index = index;
	}

	comx.gf.kul_editor_open_file_dlg_valid = false;
    
    binding.pkg_unit_name_list = comx.sys.GetUnitNameList(true);
    binding.pkg_unit_name = '';
    
    binding.js_ext_unit_list = comx.sys.GetUnitNameList(true);
    binding.js_ext_use_top_ns = true;
    binding.js_ext_top_ns_enable = false;
    binding.js_ext_support_sample_codes = true;
    
    var js_ext_ul_arr = binding.js_ext_unit_list.split(',');
    for(var loop = 1; loop < js_ext_ul_arr.length; ++loop)
    {
        if(js_ext_ul_arr[loop] == comx.gf.kul_editor_current_unit_name)
        {
            binding.js_ext_unit_index = loop - 1;
        }
    }
    
    binding.js_ext_method_unit_list = comx.sys.GetUnitNameList(true);
    binding.js_ext_method_unit_index = binding.js_ext_unit_index;
    
    OnJSExtMethodUnitChanged();
    OnCheckDeamonService();
}

function OnCheckDeamonService()
{
    if(binding.is_deamon_service_unit == 'true')
    {
        binding.deamon_service_name_enable = true;
    }
    else
    {
        binding.deamon_service_name_enable = false;
    }
}

function OnJSExtMethodUnitChanged()
{
    websocket.Invoke('GetJSExtProjList', {'unit_name' : binding.js_ext_method_unit_name}, function(data){
        var proj_list = data.parameters;
        binding.js_ext_method_proj_list = '' + proj_list.length + ',' + proj_list.join(',');
        
        var sub_folder = "js.ext";
        if(binding.js_ext_method_unit_name == 'comx_sdk')
        {
            sub_folder = "js_ext";
        }
        var cpp_fpath = comx.sys.GetEnvironmentVariables(binding.js_ext_method_unit_name.toUpperCase()) + 
            'src/' + sub_folder + '/' +
            binding.js_ext_method_proj_name + '/' + 
            binding.js_ext_method_proj_name + '.cpp';
        
        var method_list_str = comx.shell.GetJSExtMethodList(cpp_fpath);    
        var method_list_arr = method_list_str.split(',');
        
        binding.js_ext_method_list = '' + method_list_arr.length + ',' + method_list_arr.join(',');
        
        binding.js_ext_method_name = '';
    });   
}

function OnJSExtMethodProjChanged()
{
    var sub_folder = "js.ext";
    if(binding.js_ext_method_unit_name == 'comx_sdk')
    {
        sub_folder = "js_ext";
    }
    var cpp_fpath = comx.sys.GetEnvironmentVariables(binding.js_ext_method_unit_name.toUpperCase()) + 
        'src/' + sub_folder + '/' +
        binding.js_ext_method_proj_name + '/' + 
        binding.js_ext_method_proj_name + '.cpp';
    
    var method_list_str = comx.shell.GetJSExtMethodList(cpp_fpath);    
    var method_list_arr = method_list_str.split(',');
    
    binding.js_ext_method_list = '' + method_list_arr.length + ',' + method_list_arr.join(',');
    
    binding.js_ext_method_name = '';
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
    if(binding.tabctrl_index == 0)
    {
        CheckPackageName(binding.pkg_name, function(ret){
            if(ret)
            {
                comx.gf.kul_editor_current_unit_name = binding.unit_name;
                
                var kul_filepath = "y:\\" + binding.unit_name + "\\kul\\" + binding.pkg_name + ".kul";
                var js_filepath = "y:\\" + binding.unit_name + "\\js\\" + binding.pkg_name + "_pane.js";
                
                var is_need_support_websocket = (binding.websocket_check == "true" && is_new_block == false);
                var is_need_support_service = is_need_support_websocket && (binding.websocket_service_check == "true");
                var is_need_support_sandbox = is_need_support_websocket && (binding.websocket_sandbox_check == "true");

                comx.shell.CreateKULPackage(
                    binding.unit_name, 
                    binding.pkg_name, 
                    is_new_block, 
                    is_need_support_websocket, 
                    is_need_support_service,
                    is_need_support_sandbox);
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
    else if(binding.tabctrl_index == 1)
    {
        if(binding.is_deamon_service_unit == 'true' && binding.deamon_service_name == '')
        {
            alert('Deamon Service Name can\'t be NULL');
            return; 
        }
            
        if(binding.pkg_unit_name)
        {
            /*var nID = player.Play('progress.gif', 50, function(fname){
                binding.progbar_bmp = fname;
            });*/
            
            

            websocket.Invoke('CreateUnit', {'unit_name' : binding.pkg_unit_name}, function(data){
                //player.Stop(nID);
                if(g_create_unit_mode == 0)
                {
                    websocket.Invoke('SimpleUnit', {'unit_name' : binding.pkg_unit_name}, function(data){
                        //nothing.
                    });
                }
                var unit_list = comx.sys.GetUnitNameList(false);
    
                var unit_arr = unit_list.split(',');
                var bat_content = "";
                for(var loop = 1; loop < unit_arr.length; ++loop)
                {
                    var unit_name = unit_arr[loop];
                    bat_content += "set " + unit_name.toUpperCase() + "=y:\\" + unit_name + "\\\r\n";
                }
                
                comx.sys.CreateFileHandler('unit_reg_hdr', 'y:\\comx_sdk\\__.bat');
                comx.file.unit_reg_hdr = bat_content;
                comx.sys.CloseFileHandler('unit_reg_hdr');
                
                if(binding.is_deamon_service_unit == 'true')
                {
                    comx.shell.InitDeamonUnit(binding.pkg_unit_name);
                    var deamon_para = {'name' : binding.deamon_service_name,
                                       'id' : comx.shell.GenerateGUID(),
                                       'pkgid' : comx.shell.GenerateGUID(),
                                       'formid' : comx.shell.GenerateGUID(),
                                       'refid' : comx.shell.GenerateGUID(),
                                       'unit_name' : binding.pkg_unit_name};
                    websocket.Invoke('UpdateDeamonUnit', deamon_para, function(data){

                        var main_kul_fpath = "y:/" + binding.pkg_unit_name + "/kul/main.kul";
                        var ui_block_fpath = "y:/" + binding.pkg_unit_name + "/kul/ui_block.kul";
                        RegisterKUL(binding.pkg_unit_name, main_kul_fpath);
                        RegisterKUL(binding.pkg_unit_name, ui_block_fpath);
                        
                        pane.Close();
                    });
                }
                else
                {
                    pane.Close();
                }
            });
        }
        else
        {
            alert("New Unit Name can't be NULL!");
        }
    }
    else if(comx.gf.tabctrl_index == 2)
    {
        var js_ext_unit_name = binding.js_ext_unit_name;
        var js_ext_fun_ns = binding.js_ext_fun_ns;
        var js_ext_top_ns = binding.js_ext_top_ns;

        if(js_ext_fun_ns == '')
        {
            alert('Function Namspace can\'t be NULL!');
            return;
        }

        if(binding.js_ext_use_top_ns == "true")
        {
            if(js_ext_unit_name == 'comx_sdk')
            {
                js_ext_top_ns = 'comx';
            }
            else
            {
                js_ext_top_ns = js_ext_unit_name;
            }
        }
        else
        {
            if(js_ext_top_ns == '')
            {
                alert('Top Namespace can\'t be NULL!');
                return;
            }
        }
        
        var js_ext_proj_name = 'js_ext_' + js_ext_top_ns + '_' + js_ext_fun_ns;
        var js_ext_proj_dir = comx.sys.GetEnvironmentVariables(js_ext_unit_name.toUpperCase()) + 'src/js.ext/' + js_ext_proj_name;
        
        if(comx.sys.IsFileExist(js_ext_proj_dir))
        {
            alert(js_ext_proj_name + ' had been exist!');
            return;
        }
        
        var template_fpath = comx.sys.GetEnvironmentVariables('comx_sdk') + 'data/js.ext/template/template.json';
 
        comx.sys.CreateFileHandler('template_fpath', template_fpath);
        var template_content = comx.file.template_fpath;
        comx.sys.CloseFileHandler('template_fpath');
        
        var template = JSON.parse(template_content);
        
        var transform_tmpl = [];
        transform_tmpl.push({'from' : template.proj_name, 'to' : js_ext_proj_name});
        transform_tmpl.push({'from' : template.unit_name, 'to' : js_ext_unit_name.toUpperCase()});
        transform_tmpl.push({'from' : template.top_ns, 'to' : js_ext_top_ns});
        transform_tmpl.push({'from' : template.fun_ns, 'to' : js_ext_fun_ns});
        
        var transform_tmpl_fname = GenerateGUID() + '.json';
        var transform_tmpl_fpath = comx.sys.GetEnvironmentVariables('COMX_SDK') + 'data/js.ext/template/' + transform_tmpl_fname;
        
        //comx.ui.InformationBox(transform_tmpl_fpath);
        
        comx.sys.CreateFileHandler('transform_tmpl_fpath', transform_tmpl_fpath);
        comx.file.transform_tmpl_fpath = JSON.stringify(transform_tmpl);
        comx.sys.CloseFileHandler('transform_tmpl_fpath');
        
        var tmpl_type = '';
        
        var is_support_comx = eval(binding.js_ext_support_comx);
        var is_support_acis = eval(binding.js_ext_support_acis);
        
        if(!is_support_acis && !is_support_comx)
        {
            tmpl_type = 'default';
        }
        else if(is_support_comx && is_support_acis)
        {
            tmpl_type = 'both';
        }
        else if(is_support_comx)
        {
            tmpl_type = 'comx';
        }
        else
        {
            tmpl_type = 'acis';
        }
        
        var parameters = {
            'rule_file' : transform_tmpl_fname, 
            'unit_name' : js_ext_unit_name, 
            'proj_name' : js_ext_proj_name,
            'tmpl_type' : tmpl_type
        };
        
        websocket.Invoke('CreateJSExt', parameters, function(data){
            //pane.Close();
            alert(js_ext_proj_name + ' is created successful!');
            OnJSExtMethodUnitChanged();
        });
        
        //pane.Close();
    }
    else
    {
        if(binding.js_ext_method_unit_name == '')
        {
            alert('Unit Name isn\'t NULL!');
            return;
        }
        
        if(binding.js_ext_method_proj_name == '')
        {
            alert('Project Name isn\'t NULL!');
            return;
        }
        
        if(binding.js_ext_method_name == '')
        {
            alert('Method name isn\'t NULL!');
            return;
        }
        
        var method_list_arr = binding.js_ext_method_list.split(',');
        for(var loop = 0; loop < method_list_arr.length; ++loop)
        {
            if(binding.js_ext_method_name == method_list_arr[loop])
            {
                alert('Method - "' + binding.js_ext_method_name + '" is exist!');
                return;
            }
        }
        
        var sub_folder = "js.ext";
        if(binding.js_ext_method_unit_name == 'comx_sdk')
        {
            sub_folder = "js_ext";
        }
        var cpp_fpath = comx.sys.GetEnvironmentVariables(binding.js_ext_method_unit_name.toUpperCase()) + 
            'src/' + sub_folder + '/' +
            binding.js_ext_method_proj_name + '/' + 
            binding.js_ext_method_proj_name + '.cpp';
            
        cpp_fpath = cpp_fpath.split('\\').join('/');
        //alert(cpp_fpath);

        websocket.Invoke('AppendJSExtMethod', {'file' : cpp_fpath, 'method' : binding.js_ext_method_name, 'num' : binding.js_ext_method_para_num}, function(data){
            //pane.Close();
            OnJSExtMethodProjChanged();
        });
    }
}

function OnOpenJSExtProj()
{
    if(!binding.js_ext_method_proj_name)
    {
        return;
    }
    
    var sub_folder = "js.ext";
    if(binding.js_ext_method_unit_name == 'comx_sdk')
    {
        sub_folder = "js_ext";
    }
    
    var sln_fpath = comx.sys.GetEnvironmentVariables(binding.js_ext_method_unit_name.toUpperCase()) + 
        'src/' + sub_folder + '/' +
        binding.js_ext_method_proj_name + '/' + 
        binding.js_ext_method_proj_name + '.sln';
        
    sln_fpath = sln_fpath.split('\\').join('/');
    
    //alert(sln_fpath);

    if(!comx.sys.GetEnvironmentVariables('VS80COMNTOOLS'))
    {
        alert("Visual Studio 2005 can't be found!!!");
        return;
    }
    
    var vs8_tools_arr = comx.sys.GetEnvironmentVariables('VS80COMNTOOLS').split('\\');
    vs8_tools_arr[vs8_tools_arr.length - 1] = '';
    vs8_tools_arr[vs8_tools_arr.length - 2] = 'IDE';
    var vs8_devenv = vs8_tools_arr.join('\\') + '\\devenv.exe';
    var cmd = '"' + vs8_devenv + '" ' + sln_fpath;
    
    //comx.ui.InformationBox(vs8_devenv);
 
    comx.shell.run_ex(cmd, false, false);
 }

function OnClose()
{
	pane.Close();
}

function OnCloseForm()
{
    if(comx.gf.new_document_integrate_mode == 'true')
    {
        websocket.Dispose();
    }
    else
    {
        websocket.Stop();
    }
}

function CheckPackageName(pkg_name, cb)
{
    comx.shell.kulclean(binding.unit_name);
    
	var ret = true;

	if(pkg_name == "")
	{
		alert("New Package Name can't be NULL!");
		ret = false;
	        cb(ret);
	        return;
	}

	var unit_name = binding.unit_name;
	if(comx.sys.GetEnvironmentVariables(unit_name.toUpperCase()) == undefined)
	{
		alert("The Env: '" + unit_name.toUpperCase() + "' is Invalid, Please Restart Your Computer!");
		ret = false;

		//return ret;
	        cb(ret);

	        return;
	}

        websocket.Invoke('GetKulEntryInfo', {"unit_name" : unit_name}, function(data){
	    var kul_entry_obj = data.parameters.json_content;

	    for(var loop = 0; loop < kul_entry_obj.length; ++loop)
	    {
		if(kul_entry_obj[loop].name == pkg_name)
		{
		    comx.ui.InformationBox("The Name '" + pkg_name + "' has been Used!");
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
    comx.shell.kulclean(binding.unit_name);
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
    
    if(binding.websocket_check == "false")
    {
        binding.websocket_sandbox_enable = false;
    }
    else if(binding.websocket_service_check == "true")
    {
        binding.websocket_sandbox_enable = false;
    }
    else
    {
        binding.websocket_sandbox_enable = true;
    }
}

function OnWebSocketServiceCheck()
{
    if(binding.websocket_service_check == "true")
    {
        binding.websocket_sandbox_enable = false;
        binding.websocket_sandbox_check = false;
    }
    else
    {
        binding.websocket_sandbox_enable = true;
    }
}

function OnPkgUnitNameChanged()
{
    var unit_name_list = comx.sys.GetUnitNameList(true);
    unit_name_list = unit_name_list.split(',');
    
    binding.ok_btn_enable = true;
    for(var loop = 1; loop < unit_name_list.length; ++loop)
    {
        if(binding.pkg_unit_name == unit_name_list[loop])
        {
            binding.ok_btn_enable = false;
            //alert('Unit - \"' + binding.pkg_unit_name + '\" is Exist, Please Input another Name!');
            comx.shell.SetFocusCtrl(binding.pkg_combobox_hdr);
            
            break;
        }
    }
}

function OnTabctrlSwitch()
{
    if(comx.gf.tabctrl_index == 0)
    {
        binding.ok_btn_enable = true;
        comx.shell.SetFocusCtrl(binding.pkg_name_hdr);
    }
    else if(comx.gf.tabctrl_index == 1)
    {
        var unit_name_list = comx.sys.GetUnitNameList(true);
        unit_name_list = unit_name_list.split(',');
        
        binding.ok_btn_enable = true;
        for(var loop = 1; loop < unit_name_list.length; ++loop)
        {
            if(binding.pkg_unit_name == unit_name_list[loop])
            {
                binding.ok_btn_enable = false;
                
                break;
            }
        }
        
        if(comx.gf.new_document_integrate_mode != 'true')
        {
            binding.notify_label = false;
        }
        
        comx.shell.SetFocusCtrl(binding.pkg_combobox_hdr);
    }
    else
    {
        comx.shell.SetFocusCtrl(binding.js_ext_fun_ns_hdr);
    }
}

var g_create_unit_mode = 0;

function OnSimpleMode()
{
    g_create_unit_mode = 0;
}

function OnFullMode()
{
    g_create_unit_mode = 1;
}

var g_use_unit_name_as_top_ns = true;

function OnUseUnitNameAsTopNS()
{
    g_use_unit_name_as_top_ns = !g_use_unit_name_as_top_ns;
    binding.js_ext_top_ns_enable = !g_use_unit_name_as_top_ns;
    comx.shell.SetFocusCtrl(binding.js_ext_fun_ns_hdr);
}
