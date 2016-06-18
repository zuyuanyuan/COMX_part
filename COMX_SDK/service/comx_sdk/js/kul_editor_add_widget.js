/////////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');

/////////////////////////////////////////////////////////////////////////////////////////////////
// Input your javascript codes here.

var g_insert_mode = "document";

function OnWidgetTypeChange()
{
	binding.preview_index = binding.widget_type_index;
	
	if(binding.widget_type == "package" || binding.widget_type == "form" || binding.widget_type == "toolbar")
	{
		binding.widget_name_label_visible = true;
		binding.widget_name_visible = true;
	}
	else
	{
		binding.widget_name_label_visible = false;
		binding.widget_name_visible = false;
	}
}

function OnTabctrlSwith()
{
	binding.widget_name_label_visible = false;
	binding.widget_name_visible = false;
}

function OnClose()
{
	comx.gf.add_widget_okay = false;
	pane.Close();
}

function OnHotKey(id)
{
	if(id == 1001)
	{
		OnClose();
	}
}

function OnInsertToDocument()
{
	g_insert_mode = "document";
}

function OnCopyToClipboard()
{
	g_insert_mode = "clipboard";
}

function GenerateBlockWidgetInClipboard()
{
	var str_block_widget = '<widget type="block" refname="' 
                           + binding.ref_name
                           + '" refid="'
                           + binding.refid
                           + '">\n';

	var block_map_table_arr = binding.block_map_table.split(',');
 
    for(var loop = 0; loop < (block_map_table_arr.length / 3); ++loop)
	{
		if(block_map_table_arr[loop * 3 + 0] == "0")
        {
			str_block_widget += '<binding name="';
			str_block_widget += block_map_table_arr[loop * 3 + 1];
            str_block_widget += '">';
            if(block_map_table_arr[loop * 3 + 2] == 'null')
            {
				str_block_widget += ' ';
            }
            else
            {
                str_block_widget += block_map_table_arr[loop * 3 + 2];
            }
            str_block_widget += '</binding>\n';
        }

		if(block_map_table_arr[loop * 3 + 0] == "1")
        {
			str_block_widget += '<event name="';
			str_block_widget += block_map_table_arr[loop * 3 + 1];
            str_block_widget += '">';
            if(block_map_table_arr[loop * 3 + 2] == 'null')
            {
				str_block_widget += ' ';
            }
            else
            {
                str_block_widget += block_map_table_arr[loop * 3 + 2];
            }
            str_block_widget += '</event>\n';
        }
	}

	str_block_widget += '</widget>\n';

	comx.util.CopyToClipboard(str_block_widget);
}

function OnInsert()
{
	if(binding.widget_type == "package")
	{
		if(binding.widget_name == "")
		{
			comx.ui.InformationBox("Widget Name can't be NULL!");
			return;
		}
		
		comx.shell.kulpkg(binding.widget_name, binding.is_block_package);
	}
	else if(binding.widget_type == "form")
	{
		if(binding.widget_name == "")
		{
			comx.ui.InformationBox("Widget Name can't be NULL!");
			return;
		}
		
		if(IsBlockPackage(comx.gf.kul_doc_file))
		{
			comx.ui.InformationBox("Form Widget Can't been Support in Block Package!");
			return;
		}

		comx.shell.kulwidget("form", binding.widget_name);
	}
	else if(binding.widget_type == "toolbar")
	{
		if(binding.widget_name == "")
		{
			comx.ui.InformationBox("Widget Name can't be NULL!");
			return;
		}
		
		comx.shell.kulwidget("toolbar", binding.widget_name);
	}
	else if(binding.widget_type == "block")
	{
		if(IsBlockPackage(comx.gf.kul_doc_file))
		{
			comx.ui.InformationBox("Block Widget Can't been Support in Block Package!");
			return;
		}

		GenerateBlockWidgetInClipboard();
	}
	else
	{
		comx.shell.kulwidget(binding.widget_type, binding.widget_type);
	}

	if(g_insert_mode == "document")
	{
		comx.gf.insert_to_document = true;
	}

	comx.gf.add_widget_okay = true;

	pane.Close();
}

var block_json;
var block_fname = "";

function OnRefNameChanged()
{
	if(block_json != "")
	{
		binding.refid = block_json[binding.ref_name_index].id;
		block_fname = block_json[binding.ref_name_index].location;

		var events = block_json[binding.ref_name_index].mapping.events;
		var bindings = block_json[binding.ref_name_index].mapping.bindings;

		var block_map_table = "";
		for(var loop = 0; loop < events.length; ++loop)
        {
            block_map_table += "1,";
			block_map_table += events[loop];
            block_map_table += ",null,";
        }

		for(var loop = 0; loop < bindings.length; ++loop)
        {
            block_map_table += "0,";
			block_map_table += bindings[loop];
            block_map_table += ",null,";
        }

		binding.block_map_table = block_map_table;
	}
}

function IsBlockPackage(kul_fname)
{
	var ret = false;
	if(comx.gf.kul_doc_file != "")
	{
		ret = (comx.shell.IsBlockPackage(kul_fname) == "true");
	}

	return ret;
}

function OnInitializeData()
{
	websocket.Connect();

	comx.gf.add_widget_okay = false;
	var unit_name = comx.shell.GetUnitNameOfKUL(comx.gf.kul_doc_file);

	binding.ref_name_list = "1,nil";
	binding.refid = "";
	
	block_json = "";
	
	binding.block_map_table = "";
	
	if(unit_name != "")
	{
		websocket.Invoke('GetBlockInfo', {"unit_name" : unit_name}, function(data){
			block_json = data.parameters;
			//comx.ui.InformationBox(JSON.stringify(block_json));

			var ref_name_list = "" + block_json.length;
			for(var loop = 0; loop < block_json.length; ++loop)
			{
				ref_name_list += ",";
				ref_name_list += block_json[loop].name;
			}

			binding.ref_name_list = ref_name_list;

			OnRefNameChanged();
		});
	}

	binding.preview_index = 3;
	binding.widget_type_index = 3;
	binding.widget_name_label_visible = false;
	binding.widget_name_visible = false;
}

function OnCloseForm()
{
	websocket.Dispose();
}

function OnBlockMapping()
{
	binding.block_map_index2.index2 = 1;
	//comx.ui.InformationBox(binding.block_map_value);
    if(binding.block_map_value == 'null')
    {
		comx.gf.block_map_value = "";
	}
	else
	{
		comx.gf.block_map_value = binding.block_map_value;
	}

	comx.gf.block_map_value_okay = false;

	comx.ui.PopupSystemModalForm(comx.entry.FORMID_IBlockMapPane);

	if(comx.gf.block_map_value_okay == "true")
    {
		binding.block_map_value = comx.gf.block_map_value;
    }
}

function OnPreviewBlock()
{
	if(binding.refid == "")
	{
		return;
	}

	comx.shell.RefreshBlockPreviewBy(block_fname);
 	comx.ui.PopupSystemModalForm(comx.entry.FORMID_IKulBlockPreviewPane);
}
