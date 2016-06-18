function OnClose()
{
    pane.Close();
}

function OnInitializeData()
{
	PutTopNamespaceToList();

    PutComxSubNamespaceToList();
    PutComxNamespaceToList();

    comx.ui.ClosePaneByFormID(comx.entry.FORMID_IBindingBrowserPane);
    comx.ui.ClosePaneByFormID(comx.entry.FORMID_IEventBrowserPane);
    comx.ui.ClosePaneByFormID(comx.entry.FORMID_IGlobalFieldViewPane);
}

function PutTopNamespaceToList()
{
	var ret = "comx,unit,pane";

	var ext_ns_arr = comx.sys.GetExtNamespaceList();

	var cnt = 3;
	for(var item in ext_ns_arr)
	{
		if(ext_ns_arr[item] == "comx")
			continue;

		ret += ",";
		ret += ext_ns_arr[item];

		cnt++;
	}

	ret = cnt + "," + ret;

	//comx.ui.InformationBox(ret);
	
	binding.top_ns_list = ret;
}

function PutComxSubNamespaceToList()
{
    var table_var = "";

    var cnt = 0;
    for(var item in comx)
    {
        var type = typeof(eval("comx." + item));
        if (type == "function")
        {
            continue;
        }
        
        table_var += item;
        table_var += ",";
        
        ++cnt;
    }

    table_var = cnt + "," + table_var;

    
    binding.comx_sub_ns_list = table_var;

	binding.comx_sub_ns_index = 5;
}

function PutExtSubNamespaceToList()
{
    var table_var = "";

    var cnt = 0;
    for(var item in eval(binding.current_namespace))
    {
        var type = typeof(eval(binding.current_namespace + "." + item));
        if (type == "function")
        {
            continue;
        }
        
        table_var += item;
        table_var += ",";
        
        ++cnt;
    }

    table_var = cnt + "," + table_var;

    
    binding.comx_sub_ns_list = table_var;
}

function PutComxNamespaceToList()
{
    var table_var = "";

    for(var item in eval("comx." + binding.comx_sub_ns))
    {
		if(item.indexOf("0xba590c050x08530x45c10x940x2b0xbe0xcc0x6c0x8d0xeb0x1b") != -1)
			continue;

        var type = typeof(eval("comx." + binding.comx_sub_ns + "." + item));
        if (type == "function")
        {
            table_var += "2,";
        }
        else
        {
            table_var += "0,";
        }
        
        //table_var += "0,";
        table_var += item;
        table_var += ",";
        //var type = typeof(eval("comx." + item));
        table_var += type;
        table_var += ",";

        if(type == "function")
        {
            table_var += comx.util.EncodeEscapeString(comx.sys.GetFunctionCommentsBuiltIn("comx." + binding.comx_sub_ns, item));
        }
        else if(binding.comx_sub_ns == "file")
        {
            table_var += comx.sys.GetFileHandlerPostion(item);
        }
        else
        {
            table_var += comx.util.EncodeEscapeString(eval("comx." + binding.comx_sub_ns + "." + item));
        }
        table_var += ",";
    }
    //table_var += "0,-,-,-";
    //comx.InformationBox(table_var);
    binding.content_table = table_var;
}

function PutExtNamespaceToList()
{
    var table_var = "";

    for(var item in eval(binding.current_namespace + "." + binding.comx_sub_ns))
    {
		if(item.indexOf("0xba590c050x08530x45c10x940x2b0xbe0xcc0x6c0x8d0xeb0x1b") != -1)
			continue;

        var type = typeof(eval(binding.current_namespace + "." + binding.comx_sub_ns + "." + item));
        if (type == "function")
        {
            table_var += "2,";
        }
        else
        {
            table_var += "0,";
        }
        
        //table_var += "0,";
        table_var += item;
        table_var += ",";
        //var type = typeof(eval("comx." + item));
        table_var += type;
        table_var += ",";

        if(type == "function")
        {
            table_var += comx.util.EncodeEscapeString(comx.sys.GetFunctionCommentsBuiltIn(binding.current_namespace + "." + binding.comx_sub_ns, item));
        }
        else if(binding.comx_sub_ns == "file")
        {
            table_var += comx.sys.GetFileHandlerPostion(item);
        }
        else
        {
            table_var += comx.util.EncodeEscapeString(eval(binding.current_namespace + "." + binding.comx_sub_ns + "." + item));
        }
        table_var += ",";
    }
    //table_var += "0,-,-,-";
    //comx.InformationBox(table_var);
    binding.content_table = table_var;
}

function PutUnitNamespaceToList()
{
    var table_var = "";

    for(var item in unit)
    {
        var type = typeof(eval("unit." + item));
        if (type == "function")
        {
            table_var += "2,";
        }
        else
        {
            table_var += "0,";
        }
        //table_var += "0,";
        table_var += item;
        table_var += ",";
        //var type = typeof(eval("unit." + item));
        table_var += type;
        table_var += ",";

        if(type == "function")
        {
            table_var += comx.util.EncodeEscapeString(comx.sys.GetFunctionCommentsBuiltIn("unit", item));
        }
        else
        {
            table_var += comx.util.EncodeEscapeString(eval("unit." + item));
        }
        table_var += ",";
    }
    //table_var += "0,-,-,-";
    //comx.InformationBox(table_var);
    binding.content_table = table_var;
}

function PutPaneNamespaceToList()
{
    var table_var = "";

    for(var item in pane)
    {
        var type = typeof(eval("pane." + item));
        if (type == "function")
        {
            table_var += "2,";
        }
        else
        {
            table_var += "0,";
        }        
        
        table_var += item;
        table_var += ",";
        
        table_var += type;
        table_var += ",";

        if(type == "function")
        {
            table_var += comx.util.EncodeEscapeString(comx.sys.GetFunctionCommentsBuiltIn("pane", item));
        }
        else
        {
            table_var += comx.util.EncodeEscapeString(eval("pane." + item));
        }
        table_var += ",";
    }
    //table_var += "0,-,-,-";
    //comx.InformationBox(table_var);
    binding.content_table = table_var;
}

function OnChange()
{
    if(binding.current_namespace == "comx")
    {
		PutComxSubNamespaceToList();
        PutComxNamespaceToList();
        binding.comx_sub_ns_visible = true;
    }
    else
    {
        binding.comx_sub_ns_visible = false;

		if(binding.current_namespace == "pane")
		{
			PutPaneNamespaceToList();
		}
		else if(binding.current_namespace == "unit")
		{
			PutUnitNamespaceToList();
		} 
		else
		{
			binding.comx_sub_ns_visible = true;
			PutExtSubNamespaceToList();
			PutExtNamespaceToList();
		}
    }  
}

function OnCopyToClipboard()
{
	if(binding.current_namespace != "unit" && binding.current_namespace != "pane")
    {
		comx.util.CopyToClipboard(binding.current_namespace + "." + binding.comx_sub_ns + "." + binding.var_name);
	}
	else
	{
		comx.util.CopyToClipboard(binding.current_namespace + "." + binding.var_name);
	}
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
    binding.content_table = comx.util.SortTableBindContent(binding.content_table, 3, item);
}

function OnComxSubNSChanged()
{
	if(binding.current_namespace == "comx")
		PutComxNamespaceToList();
	else
		PutExtNamespaceToList();
}
