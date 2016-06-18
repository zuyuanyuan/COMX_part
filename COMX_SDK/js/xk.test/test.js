var port = parseInt(process.argv[2]);
if(isNaN(port))
{
    console.log('Warning: port is not given!!!');
    process.exit(1);
}


var io = require('socket.io-client');
var socket = io.connect('ws://localhost:1976');
socket.on('error', function(data){
    console.log(data);
});

socket.on('connect', function(){
    socket.on('sync_cmd', function(data){
	console.log(data);
	socket.emit('sync_cmd', 'hello, socket');
    });
    socket.emit('sync_cmd', {test : 'abc'});
});

socket.on('disconnect', function(){
    console.log('disconnect');
});

require('socket.io').listen(port);

