//var io = require('socket.io-client');
//var iosocket = io.connect("http://202.118.69.179:8080/");
/*
var readline = require('readline');
var events = require('events');

//var addons = require('./hellonode');

var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

function Chat()
{
    events.EventEmitter.call(this);
}

Chat.super_ = events.EventEmitter;

Chat.prototype = Object.create(events.EventEmitter.prototype, {
    constructor:{
	value: Chat,
	enumerable: false
    }
});

Chat.prototype.Run = function(){
    var self = this;

    rl.question("", function(answer) {
	// TODO: Log the answer in a database
	if(answer == "quit")
	{
	    process.exit(0);
	}

	iosocket.send("console:" + answer);

	self.emit('main', self);
	self.emit('window', self);
    });
}

var chat = new Chat();
chat.on('main', main);
chat.on('window', OnCalc);

function OnCalc()
{
    addons.hello();
}

function main()
{
    chat.Run();
}
 
iosocket.on('connect', function () {
    iosocket.send("console is connectd");
    iosocket.on('message', function(message) {
	//console.log(message);
	
    });
    iosocket.on('disconnect', function() {
	//console.log("disconnect");
    });
});

//addons.hello();

//main();
*/