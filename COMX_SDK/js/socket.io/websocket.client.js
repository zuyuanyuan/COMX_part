var class_wrap_t = require(__dirname + '/websocket.client.class.js')

exports.start = function(ip, port, debug, cb)
{
    var obj =  new class_wrap_t();

    obj.open(obj, ip, port, debug, function(){
	if(typeof(cb) == 'function')
	{
	    cb(obj);
	}
    });
    return obj;
};
