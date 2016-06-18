
//System Pre-define functions Start

function IsInArray(str, arr)
{
    for(var loop = 0; loop < arr.length; ++loop)
    {
        if(arr[loop] == str)
        {
            return true;
        }
    }
    
    return false;
}

function OnInitializeData()
{
    var unit_arr = comx.sys.GetUnitNameList(true).split(',');
    var git_arr = comx.sys.GetGitReposList().split(',');
    var final_arr = [];
    
    for(var loop = 1; loop < unit_arr.length; ++loop)
    {
        if(!IsInArray(unit_arr[loop], final_arr) && !IsInArray(unit_arr[loop], git_arr))
        {
            final_arr.push(unit_arr[loop]);
        }
    }
    
    if(!IsInArray('comx_sdk', git_arr))
    {
        final_arr.push('comx_sdk');
    }
    
    var is_comx_sdk_git = false;
    if(git_arr.length)
    {
        final_arr.push('----------------------------------------------------------');
    
        for(var loop = 1; loop < git_arr.length; ++loop)
        {
            if(git_arr[loop] == 'comx_sdk')
            {
                is_comx_sdk_git = true;
                continue;
            }
            
            {
                final_arr.push(git_arr[loop]);
            }
        }
    }
    
    if(is_comx_sdk_git)
    {
        final_arr.push('comx_sdk');
    }
    
    binding.unit_list = '' + final_arr.length + ',' + final_arr.join(',');
    binding.unit_index = binding.unit_list.split(',').length - 2;
}

function OnInitializePost()
{
}

function OnCloseForm()
{
}

function OnAnimate()
{
}

//System Pre-define functions End

function OnGo()
{
    if(binding.unit_name.match('--------'))
    {
        return;
    }
    var bat_content = '@echo off\r\n';
    bat_content += 'y: \r\ncd y:\\';
    bat_content += binding.unit_name;
    bat_content += '\r\n';
    
    comx.sys.CreateFileHandler('go_bat', comx.sys.GetEnvironmentVariables('COMX_SDK') + '/go.bat');
    comx.file.go_bat = bat_content;
    comx.sys.CloseFileHandler('go_bat');
    
    pane.Close();
}

function OnHotKey(id)
{
    if(id == 1001)
    {
        pane.Close();
    }
}