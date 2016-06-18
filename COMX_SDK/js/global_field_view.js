function OnInitializeData()
{
    binding.var_table = comx.sys.GetGlobalFieldTable();
    
    comx.ui.ClosePaneByFormID(comx.entry.FORMID_IBindingBrowserPane);
    comx.ui.ClosePaneByFormID(comx.entry.FORMID_IEventBrowserPane);
    //comx.ClosePaneByFormID(comx.FORMID_IGlobalFieldViewPane);
    comx.ui.ClosePaneByFormID(comx.entry.FORMID_INamespaceBrowserPane);
}

function OnClose()
{
    //comx.InformationBox("OnClose();");
    pane.Close();
}

function OnCopyToClipboard()
{
    comx.util.CopyToClipboard("comx.gf." + binding.var_value);
}

function OnHotKey(hot_key_id)
{
    if(hot_key_id == "1001")
    {
        OnCopyToClipboard();
    }

    if(hot_key_id == "1002")
    {
        OnClose();
    }
}

function OnListHeaderClick(item)
{
    binding.var_table = comx.util.SortTableBindContent(binding.var_table, 3, item);
}
