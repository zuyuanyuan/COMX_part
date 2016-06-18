var port = 35978;
var root_dir = process.env['COMX_SDK'];

var fs = require('fs');

process.chdir(root_dir + 'js/');

var websocket = require(root_dir + 'js/socket.io/websocket.common.js');

websocket.start(port, false);

websocket.on('Register', function(data){
    
});
