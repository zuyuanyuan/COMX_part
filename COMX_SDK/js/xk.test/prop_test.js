var student = {
    _name : "zxk",
    get name(){
	return this._name;
    },
    set name(n){
	this._name = n;
    }
    
};


//console.log(student.name);
//student.name = "ZhangXK";
//console.log(student.name);

function TStudent(name)
{
    this._name = name;
}

TStudent.prototype.__defineGetter__('Name', function(){
    return this._name;
});

TStudent.prototype.__defineSetter__('Name', function(name){
    this_name = name;
});



var stu = new TStudent('zxk');
console.log(stu.Name);
