function OnClose()
{
    //pane.Close();
    comx.sys.AbortProgram();
}

function OnRun()
{
    //comx.InformationBox(binding.entry_name_list);
    //comx.InformationBox("current entry name is " + binding.current_entry_name);
    comx.sys.PushValueToGlobalField("gfCurrentEntryName", binding.current_entry_name, "the entry name selected in entry_select pane.");
    comx.sys.PushValueToGlobalField("gfIsCurrentEntryDefault", binding.is_default);
    pane.Close();
}

function OnInitializeData()
{
    //comx.PushValueToGlobalField("test", "12345");
    binding.is_default = "no";
    binding.entry_name_list = unit.entry_list;
    comx.sys.PushValueToGlobalField("gfCurrentEntryName", binding.current_entry_name, "the entry name selected in entry_select pane.");
    comx.sys.PushValueToGlobalField("gfIsCurrentEntryDefault", binding.is_default);
}

function OnHotKey(hot_key_id)
{
    if(hot_key_id == "1001")
    {
        OnRun();
    }

    if(hot_key_id == "1002")
    {
        OnClose();
    }
}
