function _GenerateGUID()
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

function _Invoke(method_name, parameters, callback){
    if(this.socket)
    {
	var json_content = {"method" : method_name, "methodid" : this.UUID(), "parameters" : parameters};
	if(callback != undefined)
            this.callback_map_from_guid_to_fn[json_content.methodid] = callback;
	else
	    this.callback_map_from_guid_to_fn[json_content.methodid] = do_nothing_cb;
	
	//console.log('invoke emit msg');
	this.socket.emit("message", JSON.stringify(json_content));
    }

}

function _on(method_name, cb)
{
    if(cb == undefined)
    {
        console.log("method: " + method_name + "'s callback can't be null!");
    }
    this.message_callback_map[method_name] = cb;   
}

function _dispatch(data){
    var json_content = data;//JSON.parse(data);
    if(json_content.methodid != undefined)
    {
        var callback = this.callback_map_from_guid_to_fn[json_content.methodid];
        if(typeof(callback) == 'function')
        {
            callback(json_content);
        }
    }
    if(json_content.method != undefined)
    {
        var msg_cb = this.message_callback_map[json_content.method];
        if(typeof(msg_cb) == 'function')
        {
            msg_cb(json_content);
        }
    }
}

function _send(data, is_broadcast){
    //data = JSON.stringify(data);
    if(this.socket)
    {
	this.socket.emit('message', data);
	if(is_broadcast)
	{
	    this.socket.broadcast.emit('message', data);
	}
    }
}

function _dispose(){
    if(this.socket)
    {
	this.socket.disconnect();
	this.socket = false;
    }
}


module.exports = function(){
    var pThis = this;
    this.socket =                       false;
    this.callback_map_from_guid_to_fn = new Object();
    this.message_callback_map =         new Object();

    this.UUID     = _GenerateGUID;
    this.Invoke   = _Invoke;
    this.on       = _on;
    this.dispatch = _dispatch;
    this.send     = _send;
    this.dispose  = _dispose;
    this.ip_address = false;

    this.is_connect = false;

    this.IsConnect = function(){
	return this.is_connect;
    };

    this.open = function(obj, ip, port, debug, cb){
	this.ip_address = ip;
	this.socket = require('socket.io-client').
	    connect("ws://" + ip + ":" + port, {'force new connection' : true, 'reconnection' : true}).
	    on('connect', function(){
		obj.is_connect = true;
		cb();
	    });

	this.socket.on('message', function(msg){
	    if(debug)
	    {
		console.log('Remote Message Received: ', msg);
	    }
	    
	    //_dispatch.call(obj, msg);
	    obj.dispatch(msg);
	});
	
	this.socket.on('disconnect', function(){
	    obj.is_connect = false;
	    cb();
	});
	
	this.socket.on('connect_error', function(num){
	    obj.is_connect = false;
	    cb();
	});
    };

    var STREAM_MESSAGE = '482F28E3406941798F0308EC95473CC0';

    this.stream_cb_obj = new Object();

    this.stream_obj = false;
    this.stream = function()
    {
	if(!this.stream_obj)
	{
	    this.stream_obj = new TStream();
	}

	return this.stream_obj;
    };

    var ss = require('socket.io-stream');
    
    function TStream()
    {
	var ss_socket = ss(pThis.socket);
	this.Call = function(method_name, parameters, start_cb, end_cb)
	{
	    var istream = ss.createStream();
	    var data = {"method" : method_name,
			"methodid" : pThis.UUID(),
			"parameters" : parameters};
	    pThis.stream_cb_obj[data.methodid] = end_cb;
	    ss_socket.emit(STREAM_MESSAGE, istream, data);
	    start_cb(istream);
	};

	ss_socket.on(STREAM_MESSAGE, function(stream, data){
	    if(data.methodid && typeof(data.methodid) == 'string')
	    {
		var cb = pThis.stream_cb_obj[data.methodid];
		if(cb && typeof(cb) == 'function')
		{
		    cb(stream, data);
		}
	    }
	});
    }
};
