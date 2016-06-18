var http    = require('http');
var url     = require('url');
var path    = require('path');
var fs      = require('fs');
var express = require('express');

var easyui_dir = process.env['COMX_SDK'] + 'nodejs/node_modules/easyui/';
var app = express();

app.listen(80);

app.use(express.static(easyui_dir));
app.use(express.static(__dirname));

app.use('/', function(req, res){
    //res.writeHead(200, {'Content-Type':'text/html'});
    //fs.readFile(__dirname + '/index.html', function(err, data){
//	res.end(data);
  //  });
});

console.log('COMX WEB UI is listening on port-80');
