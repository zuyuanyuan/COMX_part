var websocket = require('comx.websocket.js');

var port = 30000;

var runningLog = "Y:\\nxcocadapp\\data\\Running.log";

websocket.ConnectTo(port, function(){
    sleep(500);
    websocket.Invoke('TestConnection', {}, function(data){
	//console.Clear();
	// console.log(data.parameters.ret);
    // var deamonmgr_json_fpath = comx.sys.GetEnvironmentVariables('NXCOCADAPP') + 'data/NXCOCADAPP.log';
    // comx.ui.InformationBox("1\r\n2\r\n3");
    if(fs.existsSync(runningLog))
    {
        fs.writeFileSync(runningLog, data.parameters.ret + "\n");
    }
    else
    {
        fs.writeFileSync(runningLog, data.parameters.ret + "\n");
    }
    });
});

websocket.on('InvokeTestConnection', function(data){
    // console.log('Websocket服务器返回的结果是' + data.parameters.res);
    // sleep(500)
    // comx.ui.InformationBox(data.parameters.res);
    // logString += "\n"+'Websocket服务器返回的结果是' + data.parameters.res;
    // sleep(100);
    
    appendFileSync(runningLog, data.parameters.res);
    //websocket.Dispose();
    //process.exit();
});

function OnProcessExit(){
    websocket.Dispose();
}

// websocket.on('SolveOpt', function(data){
	// opt.SetCadParameters(data.parameters.id, data.parameters.cad_paras);
    // data.parameters.res = opt.Solve(data.parameters.id);
    // websocket.send(data);
// });

// function SaveResultAs(id, fpath)
// {
	// websocket.Invoke('SaveOptResultAs', {'id' : id, 'fpath' : fpath}, function(data){
	// });
// }

// websocket.on('SetOptTaskDoneFlag', function(data){
	// opt.SetDoneFlag(data.parameters.id);
// });

function sleep(n)
{
    var start=new Date().getTime();
    while(true) if(new Date().getTime()-start>n) break;
}

function appendFileSync(fileName, data)
{
    var logString = fs.readFileSync(fileName);
    fs.writeFileSync(fileName, logString + data + "\n");
}