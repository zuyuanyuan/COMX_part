function OnInitializeData()
{
	binding.back_btn_enable = false;
	binding.install_btn_enable = false;

	binding.unit_list = comx.sys.GetUnitNameList(false);
}

function OnNext()
{
	binding.stack_index += 1;
	
	binding.back_btn_enable = true;
	binding.install_btn_enable = true;

	binding.next_btn_enable = false;

	binding.conflict_table = "";

	if(binding.pkg_fpath != "")
		CheckConflict();
}

function OnBack()
{
	binding.stack_index -= 1;

	binding.back_btn_enable = false;
	binding.install_btn_enable = false;

	binding.next_btn_enable = true;
}

function OnOpenPkg()
{
	var filter = new Array();
    filter[0] = "Package Files(*.pkg)";
    filter[1] = "All Files(*.*)";
        
    var filename = comx.ui.CallOpenFileDialog(filter);

	if(filename != "")
		binding.pkg_fpath = filename;

	CheckConflict();
}

function CheckConflict()
{
	var check_list = comx.package.ConfilictCheck(binding.unit_name, binding.pkg_fpath);

	binding.conflict_table = check_list;
}

function IsConflictBetweenPkgAndUnit()
{
	var arr = binding.conflict_table.split(',');
	
	for(loop = 0; loop < arr.length; ++loop)
	{
		if(loop%3 == 0 && arr[loop] == "1")
			return true;
	}

	return false;
}

function OnInstall()
{
	if(binding.pkg_fpath == "")
	{
		comx.ui.InformationBox("Package file path can't be empty!");
		return;
	}

	var ret = true;

	var is_conflict = IsConflictBetweenPkgAndUnit();

	if(is_conflict)
		ret = comx.ui.CallYesNoDialog("current package conflict with unit - \"" + binding.unit_name + "\",\ndo you want to continue?", "COMX|Notify");

	if(ret)
	{
		comx.package.InstallPackage(binding.unit_name, binding.pkg_fpath);
		CheckConflict();
	}
}