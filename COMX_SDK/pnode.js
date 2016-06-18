var fs = require('fs');

console.log("Re-Packing Node.JS Lib...");

if(!fs.existsSync('./nodejs'))
{
    console.log('Warning: nodejs folder isn\'t found, please use \'git pull command\' then \'unode.bat\' to get it.');
    return;
}

if(fs.existsSync('./nodejs.7z'))
{
    fs.unlinkSync('./nodejs.7z');    
}


var cmd_7z = './bin/7za.exe';

var parameters = [];
parameters.push('-r');
parameters.push('-t7z');
parameters.push('a');
parameters.push('./nodejs.7z');
parameters.push('./nodejs/*');

var cp = require('child_process').spawn(cmd_7z, parameters, {stdio:['ignore','ignore','pipe']});
/*cp.stdout.on('data', function(data){
    console.log('stdout: ' + data);
});*/
cp.stderr.on('data', function(data){
    console.log('stderr: ' + data);
});
cp.on('close', function(code){
    if(code == 0)
    {
        console.log('Re-Packing is Successful.');
    }
    else
    {
        console.log('Failed, exited code: ' + code);
    }
});



