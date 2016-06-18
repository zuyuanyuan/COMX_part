/////////////////////////////////////////////////////////////////////////////////////////////////
// Import external javascript module by reqire function.

var websocket = require('socket.io/comx.websocket.js');

/////////////////////////////////////////////////////////////////////////////////////////////////
//System Pre-define functions 

function OnInitializeData()
{
    websocket.Connect();
    
    binding.unit_list = comx.sys.GetUnitNameList(false);
}

function OnInitializePost()
{
}

function OnAnimate()
{
}

function OnCloseForm()
{
    websocket.Dispose();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Input your javascript codes here.

function OnSimple()
{
    if(binding.unit_name != "")
    {
        websocket.Invoke('SimpleUnit', {'unit_name' : binding.unit_name}, function(data){
        
        });
    }
}