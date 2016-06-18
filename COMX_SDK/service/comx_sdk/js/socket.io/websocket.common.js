var socketio = require("socket.io");

var SYNV_CMD_METHOD_ID = '642AE2E9-E1FE-455F-B470-F2CBE5B870FB';

function GenerateGUID()
{
    var guid = "";
    for (var i = 1; i <= 32; i++){
        var n = Math.floor(Math.random()*16.0).toString(16);
        guid +=   n;
        if((i==8)||(i==12)||(i==16)||(i==20))
            guid += "-";
    }
    return guid;    
}

function do_nothing_cb(data)
{
    //console.log('nothing: ' + JSON.stringify(data));
}

var socket = false;

var callback_map_from_guid_to_fn = new Object();

exports.Invoke = function(method_name, parameters, callback)
{
    var json_content = {"method" : method_name, "methodid" : GenerateGUID(), "parameters" : parameters};
    if(callback != undefined)
        callback_map_from_guid_to_fn[json_content.methodid] = callback;
    else
	callback_map_from_guid_to_fn[json_content.methodid] = do_nothing_cb;
    socket.emit("message", json_content);
};

exports.InvokeBroadcast = function(method_name, parameters, callback)
{
    var json_content = {"method" : method_name, "methodid" : GenerateGUID(), "parameters" : parameters};
    if(callback != undefined)
        callback_map_from_guid_to_fn[json_content.methodid] = callback;
    else
	callback_map_from_guid_to_fn[json_content.methodid] = do_nothing_cb;
    socket.broadcast.emit("message", json_content);
};

var message_callback_map = new Object();

exports.on = function(method_name, cb)
{
    if(cb == undefined)
    {
        console.log("method: " + method_name + "'s callback can't be null!");
    }
    message_callback_map[method_name] = cb;   
};

exports.dispatch = function(data){
    var json_content = JSON.parse(data);
    if(json_content.methodid == SYNV_CMD_METHOD_ID)
    {
	if(socket_remote)
	{
	    socket_remote.emit('sync_cmd', data);
	}
	return;
    }
    if(json_content.methodid != undefined)
    {
        var callback = callback_map_from_guid_to_fn[json_content.methodid];
        if(typeof(callback) == 'function')
        {
            callback(json_content);
        }
    }
    if(json_content.method != undefined)
    {
        var msg_cb = message_callback_map[json_content.method];
        if(typeof(msg_cb) == 'function')
        {
            msg_cb(json_content);
        }
    }
};

exports.send = function(data){
    socket.emit('message', data);
};

exports.init = function(data){
    socket = data;
};

exports.start = function(port, debug_mode, init_cb){
    var io = socketio.listen(port);
    io.on('connection', function (socket){
	exports.init(socket);
	socket.on('message', function (msg){
	    if(debug_mode)
	    {
		console.log('Message Received: ', msg);
	    }
	    
	    exports.dispatch(msg);
	});
	if(typeof(init_cb) == 'function')
	{
	    init_cb();
	}
    });
    
    io.configure('development', function () {
	if(!debug_mode)
	{
	    io.disable('log');
	}
    });
};

var socket_remote = false;

var InitializeRemoteCmdSrv = function(ip_address, port){
    if(!socket_remote)
    {
	socket_remote = require('socket.io-client').connect('ws://' + ip_address + ':' + port);

	socket_remote.on('sync_cmd', function(data){
	    if(socket)
	    {
		socket.emit('message', JSON.parse(data));
	    }
	});

	return socket_remote;
    }

    return false;
};

var TerminateRemoteCmdSrv = function(){
    if(socket_remote == false)
    {
	return false;
    }
    else
    {
	socket_remote.disconnect();
	socket_remote = false;

	return true;
    }
};

exports.on('F331FB3422DC4D7BBC4603593E9A8493', function(data){
    InitializeRemoteCmdSrv(data.parameters.ip_address, data.parameters.port);
});

exports.on('821681791CBC4C0A9B64F10E3E73E5A3', function(data){
    TerminateRemoteCmdSrv();
});
