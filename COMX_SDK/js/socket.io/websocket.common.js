var portscanner = require('portscanner');
var ss = require('socket.io-stream');

var SYNV_CMD_METHOD_ID = '642AE2E9-E1FE-455F-B470-F2CBE5B870FB';
var SANDBOX_METHOD_ID = '658DC6DB-6237-4B12-A8BA-6232E94D5E2A';

var socket = false;
var callback_map_from_guid_to_fn = new Object();
var message_callback_map = new Object();
var g_stream_callback = new Object();

var g_sandbox_socket_map = {};
var g_sandbox_map = {};
var g_debug_mode = false;
var socket_remote = false;
var g_socket_local = false;

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

////////////////////////////////////////////////////////////////////////////////////////////
// Websocket Main

exports.Invoke = function(method_name, parameters, callback)
{
    var json_content = {"method" : method_name, 
			"methodid" : GenerateGUID(), 
			"parameters" : parameters};
    if(callback != undefined)
        callback_map_from_guid_to_fn[json_content.methodid] = callback;
    else
	callback_map_from_guid_to_fn[json_content.methodid] = do_nothing_cb;
	
	//console.log('invoke emit msg');
    socket.emit("message", json_content);
};

////////////////////////////////////////////////////////////////////////////////////////////////
// The following tow  functions are abolished, are reserved only for old code compatibility,  //
// should not be used again in the future.                                                    //
exports.InvokeEx = function(method_name, parameters, callback)                                //
{                                                                                             //
    var json_content = {"method" : method_name,                                               //
                        "methodid" : GenerateGUID(),                                          //
                        "parameters" : parameters};                                           //
    if(callback != undefined)                                                                 //
        callback_map_from_guid_to_fn[json_content.methodid] = callback;                       //
    else                                                                                      //
        callback_map_from_guid_to_fn[json_content.methodid] = do_nothing_cb;                  //
                                                                                              //
    //console.log('invoke emit msg');                                                         //
    socket.emit("message", json_content);                                                     //
    //socket.broadcast.emit("message", json_content);                                         //
};                                                                                            //
                                                                                              //
exports.InvokeBroadcast = function(method_name, parameters, callback)                         //
{                                                                                             //
    var json_content = {"method" : method_name,                                               //
                        "methodid" : GenerateGUID(),                                          //
                        "parameters" : parameters};                                           //
    if(callback != undefined)                                                                 //
        callback_map_from_guid_to_fn[json_content.methodid] = callback;                       //
    else                                                                                      //
        callback_map_from_guid_to_fn[json_content.methodid] = do_nothing_cb;                  //
    //socket.broadcast.emit("message", json_content);                                         //
    socket.emit("message", json_content);                                                     //
};                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////

exports.InvokeLocal = function(method_name, parameters, callback)
{
    var json_content = {"method" : method_name, 
			"methodid" : GenerateGUID(), 
			"parameters" : parameters};
    if(callback != undefined)
        callback_map_from_guid_to_fn[json_content.methodid] = callback;
    else
	callback_map_from_guid_to_fn[json_content.methodid] = do_nothing_cb;
    //socket.broadcast.emit("message", json_content);

    if(!g_socket_local)
	g_socket_local.emit("message", json_content);
    else
	socket.emit('message', json_content);
};

exports.InvokeRemote = function(method_name, parameters, callback)
{
    var json_content = {"method" : method_name, 
			"methodid" : GenerateGUID(), 
			"parameters" : parameters};
    if(callback != undefined)
        callback_map_from_guid_to_fn[json_content.methodid] = callback;
    else
	callback_map_from_guid_to_fn[json_content.methodid] = do_nothing_cb;
    //socket.broadcast.emit("message", json_content);
    g_socket_local.broadcast.emit("message", json_content);
	comx.ui.InformationBox('zuyuanzhang');
};

exports.on = function(method_name, cb)
{
    if(cb == undefined)
    {
        console.log("method: " + method_name + "'s callback can't be null!");
    }
    message_callback_map[method_name] = cb;   
};

exports.onStream = function(method_name, cb)
{
    if(!cb)
    {
	console.log("stream method: " + method_name + "'s callback can't be null!");
    }

    g_stream_callback[method_name] = cb;
};

exports.dispatch = function(data, socket_local){
    var json_content = data;
    if(typeof(data) == 'string')
    {
	json_content = JSON.parse(data);
    }

    //var json_content = JSON.parse(data);
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
	    json_content.socket = socket_local;
            msg_cb(json_content);
        }
    }
};

function simpleClone(data, except)
{
    function isArray(obj) {  
	return Object.prototype.toString.call(obj) === '[object Array]';   
    }

    function isInExcept(p, except)
    {
	if(typeof(except) == 'string')
	{
	    return (p == except);
	}
	else if(isArray(except))
	{
	    for(var loop = 0; loop < except.length; ++loop)
	    {
		if(p == except[loop])
		{
		    return true;
		}
	    }
	}

	return false;
    }
    var ret = new Object();
    for(var p in data)
    {
	if(!isInExcept(p, except))
	{
	    ret[p] = data[p];
	}
    }

    return ret;
}

exports.send = function(data){
    if(data.socket)
    {
	var socket_local = data.socket;
	socket_local.emit('message', simpleClone(data, 'socket'));
    }
    else if(data.ss_socket)
    {
	var ss_socket = data.ss_socket;
	var new_data = simpleClone(data, ['ss_socket', 'ostream']);
	ss_socket.emit(STREAM_MESSAGE, data.ostream, new_data);		
    }
    else
    {
	socket.emit('message', data);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////
// This function is abolished, are reserved only for old code compatibility,                  //
// should not be used again in the future.                                                    //
exports.sendBroadcast = exports.send;                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////

exports.init = function(data){
    socket = data;
};

exports.GetConnectionsNum = function(){
    return g_connects_num - 1;
};

var g_connects_num = 0;
var STREAM_MESSAGE = '482F28E3406941798F0308EC95473CC0';

exports.start = function(port, debug_mode, init_cb, connect_cb){
    if(process.env['COMX_WSD_LEVEL'] == '2')
    {
	process.env['DEBUG'] = 'socket.io*';
    }
    g_debug_mode = debug_mode;
    var io = require('socket.io')();

    io.on('connection', function (socket_local){
	g_connects_num++;
	console.log('g_connections_num = ' + g_connects_num);
	if(typeof(connect_cb) == 'function')
	{
	    connect_cb(g_connects_num - 1);
	}

	/////////////////////////////////////////////////////////////////////////////
	// stream invoke callback setup
	var ss_socket = ss(socket_local);
	ss_socket.on(STREAM_MESSAGE,function(istream, data){
	    if(data.method && typeof(data.method) == 'string')
	    {
		var cb = g_stream_callback[data.method];
		if(cb)
		{
		    var ostream = ss.createStream();

		    data.ss_socket = ss_socket;
		    data.ostream = ostream;

		    cb(istream, ostream, data);
		}
	    }
	});

	/////////////////////////////////////////////////////////////////////////////
	//socket_local.emit('message', '5C640BD7-3B19-4AEF-8583-98878CFEFEA4');
	//local socket test begin

	var json_content = {"method" : '5C640BD73B194AEF858398878CFEFEA4', 
			    "methodid" : GenerateGUID(), 
			    "parameters" : {}};

	socket_local.emit('message', json_content);
	//local socket test end

	socket_local.on('message', function (msg){
	    //local socket test callback start
	    if(msg == '8155B128FEB843578D40A499E7C9DB33')
	    {
		if(g_socket_local)
		{
		    g_connects_num--;
		    console.log('g_connections_num = ' + g_connects_num);
		    if(typeof(connect_cb) == 'function')
		    {
			connect_cb(g_connects_num - 1);
		    }
		}

		g_socket_local = socket_local;
	    
		if(typeof(init_cb) == 'function')
		{
		    init_cb();
		}
		
		return;
	    }
	    //local socket test callback end

	    if(debug_mode)
	    {
		console.log('Message Received: ', msg);
	    }
		
	    if(sandbox_dispatch(socket_local, msg))
	    {
		return;
	    }
	    
	    exports.dispatch(msg, socket_local);
	});
	socket_local.on('disconnect', function(){
	    g_connects_num--;
	    console.log('g_connections_num = ' + g_connects_num);
	    if(typeof(connect_cb) == 'function')
	    {
		connect_cb(g_connects_num - 1);
	    }

	    var sandbox_id = g_sandbox_socket_map[socket_local.id];
	    if(g_sandbox_map[sandbox_id])
	    {
		var data = g_sandbox_map[sandbox_id];
		data.parameters = {'code' : -1};
		exports.send(data, true);
	    }
	});
    });

    exports.init(io);
    io.listen(port);    
};

////////////////////////////////////////////////////////////////////////////////////////////
// Remote Sync Command Service

function InitializeRemoteCmdSrv(ip_address, port){
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
}

function TerminateRemoteCmdSrv(){
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
}

exports.on('F331FB3422DC4D7BBC4603593E9A8493', function(data){
    InitializeRemoteCmdSrv(data.parameters.ip_address, data.parameters.port);
});

exports.on('821681791CBC4C0A9B64F10E3E73E5A3', function(data){
    TerminateRemoteCmdSrv();
});

exports.CmdSync = function(method_name, parameters){
    if(socket_remote)
    {
	socket_remote.emit('sync_cmd', JSON.stringify({"method" : method_name, "methodid" : SYNV_CMD_METHOD_ID, "parameters" : parameters}));
    }
};

////////////////////////////////////////////////////////////////////////////////////////////
// Sandbox Implementation.

function sandbox_dispatch(socket, msg)
{
    var json_content = msg;
    if(typeof(msg) == 'string')
    {
	json_content = JSON.parse(msg);
    }

    if(json_content.methodid == SANDBOX_METHOD_ID)
    {
	if(json_content.method == "BeginSandbox")
	{
	    var sandbox_id = json_content.parameters.id;
	    g_sandbox_socket_map[socket.id] = sandbox_id;
	}
	
	if(json_content.method == "EndSandbox")
	{
	    var sandbox_id = json_content.parameters.id;
	    var data = g_sandbox_map[sandbox_id];
	    g_sandbox_map[sandbox_id] = false;
	    data.parameters = {'code' : 0};
	    exports.send(data, true);
	    exports.send(json_content, true);
	}
	
	return true;
    }
    
    return false;
}

exports.on('SANDBOX_AFAA50AF19704C0BA55E4D74735068B9', function(data){
    var unit_name = data.parameters.unit_name;
	var formid = data.parameters.formid;
	var cache = data.parameters.cache;
	var port = data.parameters.port;
	var id = data.parameters.id;
	
	if(!cache)
	{
		cache = 'undefined';
	}
	
	var root_dir = process.env['COMX_SDK'];
	
	var cmd = root_dir + 'bin/kdesktop.exe';
	
	var parameters = [];
	parameters.push('-u');
	parameters.push(unit_name);
	parameters.push('-e');
	parameters.push(formid);
	parameters.push('-c');
	parameters.push(cache);
	parameters.push('-p');
	parameters.push(port);
	parameters.push('-i');
	parameters.push(id);
	
	g_sandbox_map[id] = data;
    
    var ls = require('child_process').spawn(cmd, parameters, {});
    
    ls.on('exit', function (code) {
	    data.parameters = {'code' : code};
        exports.send(data);
	});
});

////////////////////////////////////////////////////////////////////////////////////////////
//websocket client and deamon service interface

function GetCurrentNetworkSeg(){  
    var interfaces = require('os').networkInterfaces();
    var ret = [];
    for(var devName in interfaces){  
        var iface = interfaces[devName];  
        for(var i=0;i<iface.length;i++){  
            var alias = iface[i];  
            if(alias.family === 'IPv4' && 
	       alias.address !== '127.0.0.1' && 
	       !alias.internal && 
	       alias.address.indexOf('169.254.') === -1){  
                ret.push(GetNetSegFromIP(alias.address));  
            }  
        }  
    }
    return ret;
}

function GetNetSegFromIP(ip)
{
    var arr = ip.split('.');
    arr.pop();
    return arr.join('.');
}

exports.RemoteInvoke = function(ip_address, port, cb){
    return require('./websocket.client.js').start(
	ip_address,
	port,
	g_debug_mode,
	cb
    );
};

exports.InitDeamon = function(){
    return require('./websocket.deamon.js').Init();
};

exports.SearchDeamonManager = function(cb)
{
    var net_segs = GetCurrentNetworkSeg();

    var ip_arr = [];

    for(var loop = 0; loop < net_segs.length; ++loop)
    {
	var net_seg = net_segs[loop];
	for(var address_loop = 0; address_loop < 255; ++address_loop)
	{
	    ip_arr.push(net_seg + '.' +  address_loop);
	}
    }

    var ret = [];

    var cnt = 0;

    for(var loop = 0; loop < ip_arr.length; ++loop)
    {
	exports.CheckValidOfIP(ip_arr[loop], function(ip){
	    if(ip)ret.push(ip);
	    cnt++;
	    if(cnt == ip_arr.length && typeof(cb) == 'function')
	    {
		cb(ret);
	    }
	});
    }
};

exports.CheckValidOfIP = function(ip_address, cb)
{
    portscanner.checkPortStatus(20000, ip_address, function(error, status) {
	// Status is 'open' if currently in use or 'closed' if available 
	if(status == 'open')
	{
	    cb(ip_address);
	}
	else
	{
	    cb(false);
	}
    })
};

exports.UUID = function(){
    var guid = "";
    for (var i = 1; i <= 32; i++){
        var n = Math.floor(Math.random()*16.0).toString(16);
        guid +=   n;
    }
    return guid;    
};
