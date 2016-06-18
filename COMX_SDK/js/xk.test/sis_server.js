var io = require('socket.io').listen(1976);
var ss = require('socket.io-stream');
var path = require('path');

 
io.of('/user').on('connection', function(socket) {
    var ss_socket = ss(socket);
    ss_socket.on('file', function(stream, data) {
	var filename = path.basename(data.name);
	var gzip = require('zlib').createGunzip();
	stream.
	    pipe(gzip).
	    pipe(require('fs').createWriteStream(filename)).on('finish', function(){
		console.log('complete service');

		var stream_b = ss.createStream();
		require('fs').createReadStream(filename).pipe(stream_b);
		ss_socket.emit('bfile', stream_b, {name : filename});
	  });
    });
});
