/////////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');
var player = require('class/gif_animate.js');

/////////////////////////////////////////////////////////////////////////////////////////////////
//System Pre-define functions 


function OnInitializeData()
{
    websocket.Startup(false/*debug_mode*/);
    
    binding.unit_list = comx.sys.GetUnitNameList(true);
    binding.distribute_location = "y:\\comx_sdk\\setup\\";
    
    var unit_num = binding.unit_list.split(',').length;
    binding.unit_index = unit_num - 2;
    
    RefreshEntryList();   
    
    binding.create_shortcut = true;
    
    var cfg_file = comx.sys.GetEnvironmentVariables('COMX_SDK') + '/data/distribute.cfg';
    
    if(comx.sys.IsFileExist(cfg_file))
    {
        comx.sys.CreateFileHandler('cfg_file', cfg_file);
        binding.distribute_location = comx.file.cfg_file;
        comx.sys.CloseFileHandler('cfg_file');
    }
}

var entry_name_arr = false;
var entry_val_arr = false;

function RefreshEntryList()
{
    var unit_name = binding.unit_name;
    if(unit_name == 'comx_sdk')
    {
        binding.entry_list_enable = false;
        binding.entry_list = '';
        binding.add_item_btn_enable = false;
        return;
    }
    
    websocket.Invoke('GetUnitEntryList', {'unit_name' : unit_name}, function(data){
        entry_name_arr = data.parameters.names;
        entry_val_arr = data.parameters.values;
        
        var defid = data.parameters.defid;
        var index = 0;
        
        for(var loop = 0; loop < entry_val_arr.length; ++loop)
        {
            if(entry_val_arr[loop] == defid)
            {
                index = loop;
            }
        }

        binding.entry_list = '' + entry_name_arr.length + ',' + entry_name_arr.join(',');
        binding.entry_list_enable = true;
        binding.add_item_btn_enable = true;
        binding.entry_index = index;
    });
}

function OnInitializePost()
{
}

function OnAnimate()
{
}

function OnCloseForm()
{
    var cfg_file = comx.sys.GetEnvironmentVariables('COMX_SDK') + '/data/distribute.cfg';
    
    comx.sys.CreateFileHandler('cfg_file', cfg_file);
    comx.file.cfg_file = binding.distribute_location;
    comx.sys.CloseFileHandler('cfg_file');

    websocket.Stop();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Input your javascript codes here.

function OnDistribute()
{
    if(!comx.sys.IsFileExist(binding.distribute_location))
    {
        alert('"' + binding.distribute_location + '" isn\'t exist! create it and try again.');
        return;
    }
    
    binding.distribute_enable = false;
    binding.close_enable = false;

    /*var nID = player.Play('wait3.gif', 50, function(fname){
        binding.prog_bmp = fname;
    });*/
    binding.prog_bmp_stack_index = 1;
    binding.prog_bmp = "play";
    
    var index = parseInt(binding.entry_index);
    var distribute_entry = entry_val_arr[index];    
    
    websocket.Invoke('DistributeUnit', {'unit_name' : binding.unit_name, 'location' : binding.distribute_location, 'entry' : distribute_entry, 'is_deamon' : comx.shell.IsDeamonUnit(binding.unit_name)}, function(data){
        //player.Stop(nID);
        binding.distribute_enable = true;
        binding.close_enable = true;
        binding.prog_bmp = "stop";
        binding.prog_bmp_stack_index = 0;
    });
}

function OnOpen()
{
    var folder = comx.ui.CallFolderDialog("Distribute Folder Select");
    if(folder)
    {
        binding.distribute_location = folder;
    }
}

function OnUnitChanged()
{
    RefreshEntryList();
}

function OnAddition()
{
    comx.gf.unit_name = binding.unit_name;
    comx.ui.PopupSystemModalForm("0x5354b753-0x6807-0x41a9-0x92-0x38-0xf6-0x2f-0x42-0xd8-0x22-0x50");
}