function OnHotKey(hot_key_id)
{
    if(hot_key_id == "1001")
        OnBind();
    if(hot_key_id == "1002")
        OnEvent();
    if(hot_key_id == "1003")
        OnGlobal();
    if(hot_key_id == "1004")
        OnNamespace();
    if(hot_key_id == "1005")
        pane.Close();
}

function OnBind()
{
    pane.Close();
    comx.dev.CallBindingBrowser();
    //pane.Close();
}

function OnEvent()
{
    pane.Close();
    comx.dev.CallEventBrowser();
    //pane.Close();
}

function OnGlobal()
{
    pane.Close();
    comx.dev.CallGlobalFieldBrowser();
    //pane.Close();
}

function OnNamespace()
{
    pane.Close();
    comx.dev.CallNamespaceBrowser();
    //pane.Close();
}