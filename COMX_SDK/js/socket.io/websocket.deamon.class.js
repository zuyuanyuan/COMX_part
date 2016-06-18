module.exports = function(){
    this.connects = new Object();
    this.deamon = false;
    var pThis = this;

    this.stream = function(){
	return this.deamon.stream();
    }

    this.Open = function(ip, id, debug,  cb){
	if(this.connects[ip])
	{
	    this.deamon = this.connects[ip];
	    if(typeof(cb) == 'function')
	    {
		cb(this.deamon.IsConnect());
	    }
	}

	Init(ip, id, debug, function(wsc){
	    if(!wsc)
	    {
		if(typeof(cb) == 'function' && !pThis.IsConnect())
		{
		    cb(false);
		}
		return;
	    }

	    pThis.connects[wsc.ip_address] = wsc;

	    if(pThis.IsConnect() && pThis.GetCurrentAddress() != wsc.ip_address)
	    {
		if(typeof(cb) == 'function')
		{
		    cb(true);
		}
		return;
	    }

	    if(pThis.GetCurrentAddress() != wsc.ip_address && wsc.IsConnect())
	    {
		pThis.deamon = wsc;

		if(typeof(cb) == 'function')
		{
		    cb(pThis.deamon.IsConnect());
		}
		return;
	    }

	    if(typeof(cb) == 'function')
	    {
		if(!pThis.IsConnect())
		{
		    pThis.AutoSearch();
		}

		cb(pThis.deamon.IsConnect());
	    }
	});
    };

    this.AutoSearch = function(){
	if(!this.IsConnect())
	{
	    for(var ip in this.connects)
	    {
		if(this.connects[ip].IsConnect())
		{
		    this.deamon = this.connects[ip];
		}
	    }
	}
    };

    this.IsConnect = function(){
	return this.deamon && this.deamon.IsConnect();
    };

    this.GetCurrentAddress = function(){
	if(this.IsConnect())
	{
	    return this.deamon.ip_address;
	}
    };

    this.Invoke = function(name, parameters, cb){
	if(this.IsConnect())
	{
	    this.deamon.Invoke(name, parameters, cb);
	}
    };

    this.on = function(name, cb){
	if(this.IsConnect())
	{
	    this.deamon.on(name, cb);
	}
    };

    this.send = function(data){
	if(this.IsConnect())
	{
	    this.deamon.send(data);
	}
    };

    var RemoteInvoke = function(ip_address, port, debug, cb){
	return require('./websocket.client.js').start(
	    ip_address,
	    port,
	    debug,
	    cb
	);	
    };

    var Init = function(address, id, debug,  cb){
	var isCallbackFunction = (typeof(cb) == 'function');
	if(!address && isCallbackFunction)
	{
	    cb(false);
	    return;
	}
	//return require('./websocket.deamon.js').Init(exports, address, id, cb);
	RemoteInvoke(address, '20000', debug, function(ws_mgr){
	    if(ws_mgr.IsConnect())
	    {
		ws_mgr.Invoke('SearchDeamon', {'id' : id}, function(data){
		    var port = data.parameters.port;
		    
		    if(port)
		    {
			ws_mgr.dispose();
			
			RemoteInvoke(address, port, debug, function(ws_deamon){
			    cb(ws_deamon);
			});
		    }
		    else
		    {
			setTimeout(function(){
			    ws_mgr.Invoke('SearchDeamon', {'id' : id}, function(data){
				var port = data.parameters.port;
				if(port)
				{
				    RemoteInvoke(address, port, debug, function(ws_deamon){
					cb(ws_deamon);
				    });
				}	    

			    ws_mgr.dispose();
			    });
			},3000);
		    }
		});
	    }
	    else
	    {
		if(isCallbackFunction)
		{
		    cb(false);
		}
	    }
	});
    };
};
