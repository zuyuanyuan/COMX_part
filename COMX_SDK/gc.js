var cmd=process.platform=='win32'?'tasklist':'ps aux';
var exec = require('child_process').exec;

var my_pid = process.pid;

function kill_process(process_name, cb, done)
{
    exec(cmd, function(err, stdout, stderr) {
	if(err){ return console.log(err); }
	stdout.split('\n').filter(function(line){
            var p=line.trim().split(/\s+/),pname=p[0],pid=p[1];
            if(pname.toLowerCase().indexOf(process_name)>=0 && parseInt(pid)){
		cb(pname,pid);
            }
	});
	done();
    });   
}

var cnt = 0;

kill_process('kdesktop',
function(pname, pid){
    //if(cnt == 0) console.log('\n');
    
    try
    {
	process.kill(pid);
    }
    catch(err)
    {
	console.log(pname + '[pid=' + pid + '] is failed!');
	return;
    }
    
    console.log(pname + '[pid=' + pid + '] is killed!');
    cnt++;
},
function(){
    kill_process('node', 
    function(pname, pid){
	if(pid != my_pid)
	{
	    //if(cnt == 0) console.log('\n');
	    
	    try
	    {
		process.kill(pid);
	    }
	    catch(err)
	    {
		console.log(pname + '[pid=' + pid + '] is failed!');
		return;
	    }
	    
	    console.log(pname + '[pid=' + pid + '] is killed!');
	    cnt++;
	}
    },
    function(){
	Quit();
    });
});

function Quit()
{
    console.log('\n----------------------------------------------\nComplete, ' + cnt + ' processes are cleaned.');
    process.abort();
}
