function OnInitializeData()
{
    SetTimeout(100, function(nID){
        KillTimeout(nID);
        OnInitializePost();
    });
    
	//binding.search_history_list = comx.gf.search_history_list;
	if(comx.gf.search_history_list == undefined)
	{
		binding.search_key_word = "";
		return;
	}

	binding.search_history_list = comx.gf.search_history_list;

	history_list = comx.gf.history_list_array.split(',');

	if(history_list.length != 0)
	{
		binding.search_key_word = history_list[history_list.length -1];
	}
}

function OnInitializePost()
{
	//comx.ui.InformationBox("21");
	comx.shell.SetFocusCtrl(binding.search_hdr);
}

var is_search_up = false;

function OnDownRadioClick()
{
	is_search_up = false;
}

function OnUpRadioClick()
{
	is_search_up = true;
}

function unique(data){  
    data = data || [];  
        var a = {};  
    for (var i=0; i<data.length; i++) {  
        var v = data[i];  
        if (typeof(a[v]) == 'undefined'){  
            a[v] = 1;  
        }  
    };  
    data.length=0;  
    for (var i in a){  
        data[data.length] = i;  
    }  
    return data;  
}  

function RefreshHistoryList()
{
	var search_key_word = binding.search_key_word;

	if(history_list.length != 0)
	{
		var list = "";
		list += history_list.length;
		list += ",";
		list += history_list.join(',');

		binding.search_history_list = list;
		binding.search_key_word = search_key_word;
	}
}

var history_list = [];

function OnFind()
{
	widget.scintilla.EnableWholeWord(comx.gf.kul_search_target_hdr, binding.is_whole_word);
	widget.scintilla.EnableMatchCase(comx.gf.kul_search_target_hdr, binding.is_match_case);
	widget.scintilla.EnableRegex(comx.gf.kul_search_target_hdr, binding.is_regex);

	if(binding.search_key_word != "")
	{
		history_list.push(binding.search_key_word);
		history_list = unique(history_list);

		if(is_search_up)
		{
			if(widget.scintilla.FindPrevious(comx.gf.kul_search_target_hdr, binding.search_key_word) == -1)
			{
				widget.scintilla.SetPosition(comx.gf.kul_search_target_hdr, 9999999);
				if(widget.scintilla.FindPrevious(comx.gf.kul_search_target_hdr, binding.search_key_word) == -1)
				{
					comx.ui.InformationBox("Find nothing!");
				}
			}
		}
		else
		{
			if(widget.scintilla.FindNext(comx.gf.kul_search_target_hdr, binding.search_key_word) == -1)
			{
				widget.scintilla.SetPosition(comx.gf.kul_search_target_hdr, 0);
				if(widget.scintilla.FindNext(comx.gf.kul_search_target_hdr, binding.search_key_word) == -1)
				{
					comx.ui.InformationBox("Find nothing!");
				}
			}
		}
		
		RefreshHistoryList();
	}
}

var is_search_up_origin = false;
function OnCheckRegex()
{
	binding.up_radio_enable = (binding.is_regex == "true" ? false : true);
	binding.down_radio_enable = (binding.is_regex == "true" ? false : true);

	is_search_up_origin = is_search_up;
	if(binding.is_regex == "true")
	{
		is_search_up = false;
	}
	else
	{
		is_search_up = is_search_up_origin;
	}
}

function OnHotKey(id)
{
	if(id == 1001 || id == 1002)
	{
		pane.Close();
	}
}

function OnCloseForm()
{
	if(history_list.length != 0)
	{
		comx.gf.search_history_list = binding.search_history_list;
		comx.gf.history_list_array = history_list.join();
	}
    
    comx.sys.RunJavaScriptCodes("G_is_search_window_active = false;", comx.gf.mainwindow_formid);
}
