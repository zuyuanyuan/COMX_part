var websocket = require('comx.websocket.js');

var port = 30000;

console.log(comx.shell.GetCurrentProcessPID());


websocket.ConnectTo(port, function(){
    
    websocket.Invoke('Test', {}, function(data){
	//console.Clear();
	console.log(data.parameters.ret);
    });
});

websocket.on('InvokeTest', function(data){
    console.log('Websocket服务器返回的结果是' + data.parameters.res);
    //websocket.Dispose();
    //process.exit();
});

function OnProcessExit(){
    websocket.Dispose();
}


