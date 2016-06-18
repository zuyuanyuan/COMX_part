
var websocket = require('socket.io/comx.websocket.js');

//System Pre-define functions Start

var entry_name_arr = false;
var entry_val_arr = false;
function OnInitializeData()
{
    websocket.Connect();
    
    websocket.Invoke('GetUnitEntryList', {'unit_name' : comx.gf.unit_name}, function(data){
        entry_name_arr = data.parameters.names;
        entry_val_arr = data.parameters.values;
        
        binding.entry_list = '' + entry_name_arr.length + ',' + entry_name_arr.join(',');
    });    
}

function OnAdditional()
{
    comx.ui.PopupSystemModalForm("0x5354b753-0x6807-0x41a9-0x92-0x38-0xf6-0x2f-0x42-0xd8-0x22-0x50");
}

function OnInitializePost()
{
}

function OnCloseForm()
{
    websocket.Dispose();
}

function OnAnimate()
{
}

//System Pre-define functions End

function OnContinue()
{
    var index = parseInt(binding.entry_index);
    comx.gf.distribute_entry = entry_val_arr[index];
    
    pane.Close();
}