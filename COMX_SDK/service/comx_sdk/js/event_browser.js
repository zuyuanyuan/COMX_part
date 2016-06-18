var global_mode = "comx_sdk";

function SwitchModeTo(mode)
{
    //comx.InformationBox(mode);
    global_mode = mode;
    if(mode == "comx_sdk")
    {
        binding.formid_list = comx.entry.entry_list;
    }

    if(mode == "unit")
    {
        binding.formid_list = unit.entry_list;
    }
    
    RefreshBindingList();
}

function RefreshBindingList()
{
    if(global_mode == "comx_sdk")
    {
        binding.event_table = comx.ui.GetSysFormEventListByEntryName(binding.entry_name);
    }

    if(global_mode == "unit")
    {
        binding.event_table = comx.ui.GetFormEventListByEntryName(binding.entry_name);
    }
}

function OnInitializeData()
{
    binding.formid_list = comx.entry.entry_list;
    //comx.InformationBox(comx.GetSysFormBindingListByEntryName(binding.entry_name));
    binding.event_table = comx.ui.GetSysFormEventListByEntryName(binding.entry_name);
    
    comx.ui.ClosePaneByFormID(comx.entry.FORMID_IBindingBrowserPane);
    //comx.ClosePaneByFormID(comx.FORMID_IEventBrowserPane);
    comx.ui.ClosePaneByFormID(comx.entry.FORMID_IGlobalFieldViewPane);
    comx.ui.ClosePaneByFormID(comx.entry.FORMID_INamespaceBrowserPane);
}

function OnEntryNameChange()
{
    RefreshBindingList();
}

function OnHotKey(hot_key_id)
{
    if(hot_key_id == "1001")
    {
        OnCopyToClipboard();
    }
    
    if(hot_key_id == "1002")
    {
        pane.Close();
    }
}

function OnCopyToClipboard()
{
    comx.util.CopyToClipboard(binding.event_var);
    /*
    for(var item in comx)
    {
        comx.InformationBox(item + " is " + typeof(eval("comx." + item)));
    }
    */
}

function OnListHeaderClick(item)
{
    binding.event_table = comx.util.SortTableBindContent(binding.event_table, 3, item);
}

