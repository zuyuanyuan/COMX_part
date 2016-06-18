var fs = require('fs');

console.log("Unpacking COMX 3rd-Dependence Lib...");

var rmdirSync = (function(){
    function iterator(url,dirs){
        var stat
	    = fs.statSync(url);
        if(stat.isDirectory()){
	    dirs.unshift(url);
	    inner(url,dirs);
            }else if(stat.isFile()){
		fs.unlinkSync(url);
            }
    }
    function inner(path,dirs){
        var arr
	    = fs.readdirSync(path);
        for(var i
	    = 0, el ; el = arr[i++];){
		iterator(path+"/"+el,dirs);
        }
    }
    return function(dir,cb){
        cb
	    = cb || function(){};
        var dirs
	    = [];

        try{
	    iterator(dir,dirs);
	    for(var i
		= 0, el ; el = dirs[i++];){
                fs.rmdirSync(el);
	    }
	    cb()
        }catch(e){
	    e.code
		=== "ENOENT" ?
		cb() : cb(e);
        }
    }
})();

if(!fs.existsSync('./deps.7z'))
{
    console.log('Warning: deps.7z isn\'t found, please use \'git pull command\' to get it.');
    return;
}

if(fs.existsSync('./deps'))
{
    rmdirSync('./deps');
}

var cmd_7z = './bin/7za.exe';

var parameters = [];
parameters.push('x');
parameters.push('./deps.7z');
parameters.push('-o' + './');
parameters.push('*');
parameters.push('-r');

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
        console.log('Unpacking is Successful.');
    }
    else
    {
        console.log('Failed, exited code: ' + code);
    }
    process.exit();
});



