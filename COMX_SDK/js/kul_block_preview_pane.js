/////////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');

var map_to_hdr = [];

function OnInitializeData()
{
    websocket.Connect();
    websocket.Invoke('GetBlockPreviewInfo', {}, function(data){
	var json_obj = data.parameters;
	var events = json_obj.events;
	var bindings = json_obj.bindings;
	var events_type = json_obj.events_type;
	var bindings_type = json_obj.bindings_type;
	var events_hdr = json_obj.events_hdr;
	var bindings_hdr = json_obj.bindings_hdr;

	var mapping_table = "";

	for(var loop = 0; loop < events.length - 2; ++loop)
	{
		mapping_table += "1,";
		mapping_table += events[loop];
		mapping_table += ",";
		mapping_table += events_type[loop];
		mapping_table += ",";

		map_to_hdr.push(events_hdr[loop]);
	}

	for(var loop = 0; loop < bindings.length - 2; ++loop)
	{
		mapping_table += "0,";
		mapping_table += bindings[loop];
		mapping_table += ",";
		mapping_table += bindings_type[loop];
		mapping_table += ",";
		
		map_to_hdr.push(bindings_hdr[loop]);
	}
	
	binding.mapping_table = mapping_table;

    });
}

var form_hdr = "";
function OnClickMap()
{
	//comx.ui.InformationBox(map_to_hdr[binding.mapping_index.index1]);
	var cmd = "form_hdr = comx.shell.HighlightWidget(binding." + map_to_hdr[binding.mapping_index.index1] + ");";
	//comx.ui.InformationBox(cmd);
	eval(cmd);
}

function OnClose()
{
	//if(form_hdr != "")
	//{
	//	comx.shell.DisableHighlightWidget(form_hdr);
	//	form_hdr = "";
	//	return;
	//}
	pane.Close();
}

function OnCloseForm()
{
    websocket.Dispose();
}
