
//System Pre-define functions Start

function OnInitializeData()
{
    g_unit_name = comx.gf.unit_name;
    
    if(g_unit_name)
    {
        binding.create_shortcut = true;
        var additional_json_file = comx.sys.GetEnvironmentVariables(g_unit_name.toUpperCase()) + 'data/addition.cfg';
        if(comx.sys.IsFileExist(additional_json_file))
        {
            comx.sys.CreateFileHandler('add_json_file', additional_json_file);
            var json_obj = JSON.parse(comx.file.add_json_file);
            comx.sys.CloseFileHandler('add_json_file');
            
            binding.list = json_obj.add_items;
            binding.create_shortcut = json_obj.create_shortcut;
            
            if(json_obj.disable_comx_support)
            {
                binding.disable_comx_support = json_obj.disable_comx_support;
            }
        }
    }
}

function OnInitializePost()
{
}

function OnCloseForm()
{
    var additional_json_file = comx.sys.GetEnvironmentVariables(g_unit_name.toUpperCase()) + 'data/addition.cfg';
    comx.sys.CreateFileHandler('add_json_file', additional_json_file);
    comx.file.add_json_file = JSON.stringify({'add_items' : binding.list, 'create_shortcut' : binding.create_shortcut, 'disable_comx_support': binding.disable_comx_support});
    comx.sys.CloseFileHandler('add_json_file');
}

function OnAnimate()
{
}

//System Pre-define functions End

var g_unit_name = false;

function OnAddFile()
{
    var filename = comx.ui.CallOpenFileDialog(['All Files(*.*)']);
    if(filename)
    {
        var item_name = filename.split('\\')[filename.split('\\').length -1];
        var unit_name = filename.split('\\')[filename.split('\\').length -2];
        
        if(g_unit_name != unit_name)
        {
            alert('Please Select File in Target Unit!');
            return;
        }
        
        var list_str = binding.list;
        if(list_str != "")
        {
            list_str += ',';
        }
        
        binding.list = list_str + ('0,File,' + item_name);
    }
    
}

function AddFolder()
{
    var folder_name = comx.ui.CallFolderDialog('Select Additional Folder');
    if(folder_name)
    {
        var item_name = folder_name.split('\\')[folder_name.split('\\').length -1];
        var unit_name = folder_name.split('\\')[folder_name.split('\\').length -2];
        
       
        if(g_unit_name != unit_name)
        {
            alert('Please Select Folder in Target Unit!');
            return;
        }
        
        var list_str = binding.list;
        if(list_str != "")
        {
            list_str += ',';
        }
        
        binding.list = list_str + ('1,Folder,' + item_name);
    }
}

function RemoveItem()
{
    var list_content = '';
    var list_arr = binding.list.split(',');
    for(var loop = 0; loop < list_arr.length/3; ++loop)
    {
        if(loop == binding.list_index.index1)
        {
            continue;
        }
        
        list_content += list_arr[loop * 3 + 0];
        list_content += ',';
        list_content += list_arr[loop * 3 + 1];
        list_content += ',';
        list_content += list_arr[loop * 3 + 2];
        list_content += ',';
    }
    binding.list = list_content;
}