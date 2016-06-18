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
        binding.bind_table = comx.ui.GetSysFormBindingListByEntryName(binding.entry_name);
    }

    if(global_mode == "unit")
    {
        binding.bind_table = comx.ui.GetFormBindingListByEntryName(binding.entry_name);
    }
}

function OnInitializeData()
{
    binding.formid_list = comx.entry.entry_list;
    //comx.InformationBox(comx.GetSysFormBindingListByEntryName(binding.entry_name));
    binding.bind_table = comx.ui.GetSysFormBindingListByEntryName(binding.entry_name);

    //comx.ClosePaneByFormID(comx.FORMID_IBindingBrowserPane);
    comx.ui.ClosePaneByFormID(comx.FORMID_IEventBrowserPane);
    comx.ui.ClosePaneByFormID(comx.FORMID_IGlobalFieldViewPane);
    comx.ui.ClosePaneByFormID(comx.FORMID_INamespaceBrowserPane);
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
    //comx.TestFun();
    comx.util.CopyToClipboard("binding." + binding.bind_var);
    //comx.PopupSystemModalessForm("0x63a39b86-0xbe34-0x4885-0x85-0x49-0x43-0x55-0xb7-0x94-0xc1-0x6d");
}

function IncreaseData(data)
{
    data = parseInt(data) + 5;
}

function OnListHeaderClick(item)
{
    binding.bind_table = comx.util.SortTableBindContent(binding.bind_table, 3, item);
}
