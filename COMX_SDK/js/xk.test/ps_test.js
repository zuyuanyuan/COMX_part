function GetCurrentNetworkSeg(){  
    var interfaces = require('os').networkInterfaces();
    var ret = [];
    for(var devName in interfaces){  
          var iface = interfaces[devName];  
          for(var i=0;i<iface.length;i++){  
               var alias = iface[i];  
               if(alias.family === 'IPv4' && alias.address !== '127.0.0.1' && !alias.internal && alias.address.indexOf('169.254.') === -1){  
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

function SearchDeamonManager(cb)
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
	CheckValidOfIP(ip_arr[loop], function(ip){
	    if(ip)ret.push(ip);
	    cnt++;
	    if(cnt == ip_arr.length && typeof(cb) == 'function')
	    {
		cb(ret);
	    }
	});
    }
}

var portscanner = require('portscanner');

function CheckValidOfIP(ip_address, cb)
{
    /*var wsc = require('socket.io-client')("ws://" + ip_address + ":20000", {'force new connection' : true, 'reconnection' : false}).
	on('connect', function(){
	    if(typeof(cb) == 'function')
	    {
		cb(ip_address);
	    }
	    wsc.disconnect();
	}).
	on('error', function(){
	    if(typeof(cb) == 'function')
	    {
		cb(false);

	    }
	}).
	on('reconnect_failed', function(){
	    if(typeof(cb) == 'function')
	    {
		cb(false);

	    }
	});
    */
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
}

SearchDeamonManager(function(ret){
    console.log(JSON.stringify(ret));
});

