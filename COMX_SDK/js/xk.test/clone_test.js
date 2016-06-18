var data = {'method' : 'Method', 'methodid' : 'Methodid'};
data.socket = {'ip' : 'localhost'};

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

var ret = simpleClone(data, 'socket');

console.log(JSON.stringify(data));
console.log('------------------------------------------');
console.log(JSON.stringify(ret));

console.log('------------------------------------------');
console.log(JSON.stringify(simpleClone(data, ['socket', 'method'])));
