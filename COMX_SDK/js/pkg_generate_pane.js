var MAX_STEP = 9;

function OnInitializeData()
{
	binding.unit_list = comx.sys.GetUnitNameList(false);
	binding.back_btn_enable = false;
	binding.create_btn_enable = false;
}

function OnAppendItemToList(selected_list, src_name)
{
	var tar_list = selected_list;

	var new_index = 0;

	if(tar_list == "")
	{
		tar_list = "1," + src_name;
	}
	else
	{
		if(tar_list.indexOf(src_name) == -1)
		{
			var tar_arr = tar_list.split(",");
			tar_arr[0] = tar_arr.push(src_name) - 1;
			tar_list = tar_arr.join();
			new_index = tar_arr.length - 2;
		}
	}

	return tar_list;
}

function OnAppendItem(selected_list, src_name, tar_index, remove_btn_enable_flag)
{
	eval(selected_list + " = OnAppendItemToList(" + selected_list + "," + src_name + ");");
	eval(tar_index + " = " + selected_list + ".split(\",\").length - 2;");
	eval(remove_btn_enable_flag + "= true;");
}

function OnRemoveItemFromList(selected_list, tar_index, remove_btn_enable_flag)
{
	eval("var tar_arr = " + selected_list + ".split(\",\");");
	eval("tar_arr.splice(parseInt(" + tar_index + ") + 1, 1);");
	eval("tar_arr[0] = tar_arr.length - 1;");

	eval("var tar_kul_index_bak = " + tar_index + ";");
	eval(selected_list + " = tar_arr.join();");
	eval(tar_index + " = tar_kul_index_bak;");

	eval("if(" + selected_list + " == \"\")" + remove_btn_enable_flag + " = false;");
}

function OnAppendKUL()
{
	OnAppendItem("binding.selected_kul_list",
        "binding.src_kul",
        "binding.tar_kul_index",
        "binding.kul_remove_btn_enable_flag");
}

function OnAppendJsExt()
{
	OnAppendItem("binding.selected_js_ext_list",
        "binding.src_js_ext",
        "binding.tar_js_ext_index",
        "binding.js_ext_remove_btn_enable_flag");
}

function OnRemoveJsExt()
{
	OnRemoveItemFromList("binding.selected_js_ext_list",
        "binding.tar_js_ext_index",
        "binding.js_ext_remove_btn_enable_flag");
}

function OnAppendModule()
{
	OnAppendItem("binding.selected_module_list",
        "binding.src_module",
        "binding.tar_module_index",
        "binding.module_remove_btn_enable_flag");
}

function OnRemoveKUL()
{	
	OnRemoveItemFromList("binding.selected_kul_list",
        "binding.tar_kul_index",
        "binding.kul_remove_btn_enable_flag");
}

function OnRemoveModule()
{	
	OnRemoveItemFromList("binding.selected_module_list",
        "binding.tar_module_index",
        "binding.module_remove_btn_enable_flag");
}

function OnAppendInclude()
{
	OnAppendItem("binding.selected_include_list",
        "binding.src_include",
        "binding.tar_include_index",
        "binding.include_remove_btn_enable_flag");
}

function OnRemoveInclude()
{	
	OnRemoveItemFromList("binding.selected_include_list",
        "binding.tar_include_index",
        "binding.include_remove_btn_enable_flag");
}

function OnAppendPicture()
{
	OnAppendItem("binding.selected_picture_list",
        "binding.src_picture",
        "binding.tar_picture_index",
        "binding.picture_remove_btn_enable_flag");
}

function OnRemovePicture()
{	
	OnRemoveItemFromList("binding.selected_picture_list",
        "binding.tar_picture_index",
        "binding.picture_remove_btn_enable_flag");
}

function OnAppendClsid()
{
	OnAppendItem("binding.selected_clsid_list",
        "binding.src_clsid",
        "binding.tar_clsid_index",
        "binding.clsid_remove_btn_enable_flag");
}

function OnRemoveClsid()
{	
	OnRemoveItemFromList("binding.selected_clsid_list",
        "binding.tar_clsid_index",
        "binding.clsid_remove_btn_enable_flag");
}

function OnAppendIid()
{
	OnAppendItem("binding.selected_iid_list",
        "binding.src_iid",
        "binding.tar_iid_index",
        "binding.iid_remove_btn_enable_flag");
}

function OnRemoveIid()
{	
	OnRemoveItemFromList("binding.selected_iid_list",
        "binding.tar_iid_index",
        "binding.iid_remove_btn_enable_flag");
}

function OnAppendData()
{
	OnAppendItem("binding.selected_data_list",
        "binding.src_data",
        "binding.tar_data_index",
        "binding.data_remove_btn_enable_flag");
}

function OnRemoveData()
{	
	OnRemoveItemFromList("binding.selected_data_list",
        "binding.tar_data_index",
        "binding.data_remove_btn_enable_flag");
}

function OnInitStepStatus(src_list_val, src_list_bind, selected_list_bind, append_btn_enable_flag, remove_btn_enable_flag)
{
	eval(src_list_bind + " = \"" + src_list_val + "\";");
	eval(selected_list_bind + " = \"\";");

	eval("if(" + src_list_bind + " == \"\"){" + append_btn_enable_flag + " = false;}else{" + append_btn_enable_flag + " = true;}");

	eval(remove_btn_enable_flag + " = false;");

}

function OnActivateStep(cur_step)
{
	if(cur_step == 1)
	{
		OnInitStepStatus(comx.package.GetKulListFromUnit(binding.unit_name),
			"binding.src_kul_list",
			"binding.selected_kul_list",
			"binding.kul_append_btn_flag",
			"binding.kul_remove_btn_enable_flag");
	}

	if(cur_step == 2)
	{
		OnInitStepStatus(comx.package.GetJsExtListFromUnit(binding.unit_name),
			"binding.src_js_ext_list",
			"binding.selected_js_ext_list",
			"binding.js_ext_append_btn_flag",
			"binding.js_ext_remove_btn_enable_flag");
	}

	if(cur_step == 3)
	{
		OnInitStepStatus(comx.package.GetModuleListFromUnit(binding.unit_name),
			"binding.src_module_list",
			"binding.selected_module_list",
			"binding.module_append_btn_flag",
			"binding.module_remove_btn_enable_flag");
	}

	if(cur_step == 4)
	{
		OnInitStepStatus(comx.package.GetIncludeListFromUnit(binding.unit_name),
			"binding.src_include_list",
			"binding.selected_include_list",
			"binding.include_append_btn_flag",
			"binding.include_remove_btn_enable_flag");
	}

	if(cur_step == 5)
	{
		OnInitStepStatus(comx.package.GetPictureListFromUnit(binding.unit_name),
			"binding.src_picture_list",
			"binding.selected_picture_list",
			"binding.picture_append_btn_flag",
			"binding.picture_remove_btn_enable_flag");
	}

	if(cur_step == 6)
	{
		OnInitStepStatus(comx.package.GetClsidListFromUnit(binding.unit_name),
			"binding.src_clsid_list",
			"binding.selected_clsid_list",
			"binding.clsid_append_btn_flag",
			"binding.clsid_remove_btn_enable_flag");
	}

	if(cur_step == 7)
	{
		OnInitStepStatus(comx.package.GetIidListFromUnit(binding.unit_name),
			"binding.src_iid_list",
			"binding.selected_iid_list",
			"binding.iid_append_btn_flag",
			"binding.iid_remove_btn_enable_flag");
	}

	if(cur_step == 8)
	{
		OnInitStepStatus(comx.package.GetDataFileListFromUnit(binding.unit_name),
			"binding.src_data_list",
			"binding.selected_data_list",
			"binding.data_append_btn_flag",
			"binding.data_remove_btn_enable_flag");
	}
}

function OnNext()
{
	//comx.ui.InformationBox(binding.unit_name);
	binding.cur_step = parseInt(binding.cur_step) + 1;
	if(binding.cur_step == MAX_STEP)
	{
		binding.create_btn_enable = true;
		binding.next_btn_enable = false;
		binding.progbar_visible = false;
		binding.pkg_fpath = "";
	}
	else
	{
		binding.create_btn_enable = false;
		binding.next_btn_enable = true;
	}

	if(binding.cur_step != 0)
	{
		binding.back_btn_enable = true;
	}
	else
	{
		binding.back_btn_enable = false;
	}

	OnActivateStep(binding.cur_step);
}

function OnBack()
{
	binding.cur_step = parseInt(binding.cur_step) - 1;

	if(binding.cur_step == 0)
	{
		binding.back_btn_enable = false;
	}
	else
	{
		binding.back_btn_enable = true;
	}

	if(binding.cur_step != MAX_STEP)
	{
		binding.next_btn_enable = true;
		binding.create_btn_enable = false;
	}
	else
	{
		binding.next_btn_enable = false;
		binding.create_btn_enable = true;
	}

	OnActivateStep(binding.cur_step);
}

function OnGeneratePackage()
{
	if(binding.pkg_fpath == "")
	{
		comx.ui.InformationBox("Package file path can't be empty!");
		return;
	}

	binding.progbar_visible = true;

	comx.package.InitPackage();

	binding.progbar_val = 5;
	
	comx.package.PickupKulFilesFromUnit(binding.unit_name, binding.selected_kul_list);
	comx.package.PickupClsidItemsFromUnit(binding.unit_name, binding.selected_clsid_list);
	binding.progbar_val = 20;
	comx.package.PickupIidItemsFromUnit(binding.unit_name, binding.selected_iid_list);
	comx.package.PickupDataFilesFromUnit(binding.unit_name, binding.selected_data_list);
	binding.progbar_val = 40;
	comx.package.PickupIncludeFilesFromUnit(binding.unit_name, binding.selected_include_list);
	comx.package.PickupJsExtFilesFromUnit(binding.unit_name, binding.selected_js_ext_list);
	binding.progbar_val = 60;
	comx.package.PickupModuleFilesFromUnit(binding.unit_name, binding.selected_module_list);
	comx.package.PickupPictureFilesFromUnit(binding.unit_name, binding.selected_picture_list);
	binding.progbar_val = 80;

	comx.package.GeneratePackage(binding.pkg_fpath);

	binding.progbar_val = 100;

	binding.progbar_visible = false;
	
	//comx.package.InstallPackage("test1", "y:\\test.pkg");
}

function OnSelectPackageName()
{
	var filter = new Array();
    filter[0] = "Package Files(*.pkg)";
    filter[1] = "All Files(*.*)";
        
    var filename = comx.ui.CallSaveFileDialog(filter);

	if(filename != "")
		binding.pkg_fpath = filename;
}