
//System Pre-define functions Start

function OnInitializeData()
{
    binding.unit_list = comx.sys.GetUnitNameList(false);
    
    var unit_arr = binding.unit_list.split(',');
    var bat_content = "";
    for(var loop = 1; loop < unit_arr.length; ++loop)
    {
        var unit_name = unit_arr[loop];
        bat_content += "set " + unit_name.toUpperCase() + "=y:\\" + unit_name + "\\\r\n";
    }
    
    comx.sys.CreateFileHandler('unit_reg_hdr', 'y:\\comx_sdk\\__.bat');
    comx.file.unit_reg_hdr = bat_content;
    comx.sys.CloseFileHandler('unit_reg_hdr');
    
    pane.Close();
}

function OnInitializePost()
{
}

function OnCloseForm()
{
}

function OnAnimate()
{
}

//System Pre-define functions End

