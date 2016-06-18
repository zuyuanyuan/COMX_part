function TClass(val)
{
    this.val = val;
    var pThis = this;

    var Test = function(cb){
	cb();
    }

    this.Open = function(){
	Test(function(){
	    console.log(pThis.val);
	});
    }
}

var obj1 = new TClass(1);
var obj2 = new TClass(2);

obj1.Open();
obj2.Open();
obj1.Open();
obj2.Open();
