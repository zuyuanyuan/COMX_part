var q = require('q');

function Div(left, right, cb)
{
    if(typeof(cb) != 'function')
    {
	return false;
    }

    if(right == 0)
    {
	cb(true, 'right isn\'t zero!');
    }
    else
    {
	cb(false, left/right);
    }

    return true;
}

var Q_Step1 = function(left, right){
    var deferred = q.defer();
    if(!Div(left, right, function(err, data){
	if(!err)
	{
	    deferred.resolve(data);
	}
	else
	{
	    deferred.reject(data);
	}
    }))
    {
	deferred.reject('cb parameters undefine');
    }

    return deferred.promise;
};

Q_Step1(8, 2).then(function(data){
    console.log(data);
}, function(err){
    console.log(err);
});
