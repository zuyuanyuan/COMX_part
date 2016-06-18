var io = require('socket.io-client');
var ss = require('socket.io-stream');
 
var socket = io.connect('ws://localhost:1976/user');
var stream = ss.createStream();
var filename = 'y:/dksclient/data/1.txt';

var gzip = require('zlib').createGzip();

var ss_socket = ss(socket);
ss_socket.emit('file', stream, {name: filename});
require('fs').createReadStream(filename).
    pipe(gzip).
    pipe(stream).on('end', function(){
	console.log('complete');
    }).on('finish', function(){
	console.log('finish');
    });;


ss_socket.on('bfile', function(stream, data){
    var fname = filename + '.output1';
    stream.pipe(require('fs').createWriteStream(fname));
});
