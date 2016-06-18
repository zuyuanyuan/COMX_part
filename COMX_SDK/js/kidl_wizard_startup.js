function OnInitializeData()
{
    //binding.is_sub_category_enable = "true";
    binding.default_proj_name = "";
    binding.unit_list = comx.sys.GetUnitNameList(true);
    RefreshCategoryComboboxStatus();
}

function RefreshCategoryComboboxStatus()
{
    if(binding.unit_name == "comx_sdk")
        binding.is_sub_category_enable = "true";
    else
        binding.is_sub_category_enable = "false";
}

function OnUnitChange()
{
    RefreshCategoryComboboxStatus();
}