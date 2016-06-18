var port = parseInt(process.argv[2]);
if(isNaN(port))
{
    console.log('Warning: port is not given!!!');
    process.exit(1);
}

require('socket.io').listen(port).sockets.on('connection', function(socket){
    socket.on('sync_cmd', function(data){
	socket.broadcast.emit('sync_cmd', data);
    });
});

