/////////////////////////////////////////////////////////////////////////////////////////////////
// Please don't modified the following codes at any time.

var port = parseInt(process.argv[2]);
var debug = JSON.parse(process.argv[3]);
var home_dir = process.argv[4];
var root_dir = process.env['COMX_SDK'];

var fs = require('fs');

process.chdir(home_dir + 'js/');

var websocket = require(root_dir + 'js/socket.io/websocket.common.js');

websocket.start(port, debug);

websocket.on("exit", function(data){
    process.exit();
});

process.on('uncaughtException', function (err) {
    console.error(err.stack);
    //console.log("Node NOT Exiting...");
    websocket.Invoke('error', {"message" : err.message});
});

/////////////////////////////////////////////////////////////////////////////////////////////////
// Append your codes here please.

var block_info = require('./func/get_block_info.js');
var block_preview_info = require('./func/get_block_preview_info.js');
var kul_entry_info = require('./func/get_kul_entry_info.js');

websocket.on('GetBlockInfo', function(data){
    block_info.GetBlockInfo(data);
});

websocket.on('GetBlockPreviewInfo', function(data){
    block_preview_info.GetBlockPreviewInfo(data);
});

websocket.on('GetKulEntryInfo', function(data){
    kul_entry_info.GetKulEntryInfo(data);
});

websocket.on('GetKulBindingInfo', function(data){
    kul_entry_info.GetKulBindingInfo(data);
});

var cp = require('child_process');

var g_func_list_lock = false;
websocket.on('GetFunctionListOfJavaScript', function(data){
    if(g_func_list_lock)
    {
        return;
    }
    else
    {
        g_func_list_lock = true;
    }
    
    var cmd = root_dir + 'bin/regex_pcre.exe';
    //console.log(cmd);
    var ls = cp.spawn(cmd, [data.parameters.pattern,data.parameters.src,data.parameters.res], {});

    ls.on('exit', function (code) {
		console.log('child process exited with code ' + code);
		var result = fs.readFileSync(data.parameters.res);
		data.parameters = JSON.parse('' + result);
		websocket.send(data);
        g_func_list_lock = false;
    });
});

var g_var_list_lock = false;
websocket.on('GetVariableListOfJavaScript', function(data){
    if(g_var_list_lock)
    {
        return;
    }
    else
    {
        g_var_list_lock = true;
    }
    var cmd = root_dir + 'bin/regex_pcre.exe';
    //console.log(cmd);
    var ls = cp.spawn(cmd, [data.parameters.pattern,data.parameters.src,data.parameters.res], {});

    ls.on('exit', function (code) {
		console.log('child process exited with code ' + code);
		var result = fs.readFileSync(data.parameters.res);
		data.parameters = JSON.parse('' + result);
		websocket.send(data);	
        
        g_var_list_lock = false;
    });
});

var g_global_var_lock = false;
websocket.on('GetGlobalVariableListOfJavaScript', function(data){
    if(g_global_var_lock)
    {
        return;
    }
    else
    {
        g_global_var_lock = true;
    }
    var cmd = root_dir + 'bin/regex_pcre.exe';
    //console.log(cmd);
    var ls = cp.spawn(cmd, [data.parameters.pattern,data.parameters.src,data.parameters.res], {});

    ls.on('exit', function (code) {
		console.log('child process exited with code ' + code);
		var result = fs.readFileSync(data.parameters.res);
		data.parameters = JSON.parse('' + result);
		websocket.send(data);	
        
        g_global_var_lock = false;
    });
});

var util = require("util");
//copy a file
function copyFile(src, dst){
    var is = fs.createReadStream(src);
    var os = fs.createWriteStream(dst);
    util.pump(is, os, function(err){
        if(err){
            //copy error
        }
    });
}

websocket.on('SimpleUnit', function(data){
    var src_kproduct_template = root_dir + 'data/template/simple.kproduct.template';
    var tar_kproduct = process.env[data.parameters.unit_name.toUpperCase()] + data.parameters.unit_name + '.kproduct';
    copyFile(src_kproduct_template, tar_kproduct);
    
    var src_package_template = root_dir + 'data/template/simple.package.template';
    var tar_package = process.env[data.parameters.unit_name.toUpperCase()] + 'script/package.xml';
    copyFile(src_package_template, tar_package);
});
