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

function mkdirsSync(dirname, mode)
{
    console.log(dirname);
    if(fs.existsSync(dirname))
    {
        return true;
    }
    else
    {
        if(mkdirsSync(path.dirname(dirname), mode))
        {
            fs.mkdirSync(dirname, mode);
            return true;
        }
    }
}
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
    var tar_kproduct = root_dir.split('\\')[0].split('/')[0] + '/' + data.parameters.unit_name + '/' + data.parameters.unit_name + '.kproduct';
    console.log(tar_kproduct);
    copyFile(src_kproduct_template, tar_kproduct);
    
    var src_package_template = root_dir + 'data/template/simple.package.template';
    var tar_package = root_dir.split('\\')[0].split('/')[0] + '/' + data.parameters.unit_name + '/' + 'script/package.xml';
    console.log(tar_package);
    copyFile(src_package_template, tar_package);
});

function sleep(milliSeconds) { 
    var startTime = new Date().getTime(); 
    while (new Date().getTime() < startTime + milliSeconds);
 };

var rmdirSync = (function(){
    function iterator(url,dirs){
        var stat
	    = fs.statSync(url);
        if(stat.isDirectory()){
	    dirs.unshift(url);
	    inner(url,dirs);
            }else if(stat.isFile()){
		fs.unlinkSync(url);
            }
    }
    function inner(path,dirs){
        var arr
	    = fs.readdirSync(path);
        for(var i
	    = 0, el ; el = arr[i++];){
		iterator(path+"/"+el,dirs);
        }
    }
    return function(dir,cb){
        cb
	    = cb || function(){};
        var dirs
	    = [];

        try{
	    iterator(dir,dirs);
	    for(var i
		= 0, el ; el = dirs[i++];){
                fs.rmdirSync(el);
	    }
	    cb()
        }catch(e){
	    e.code
		=== "ENOENT" ?
		cb() : cb(e);
        }
    }
})();

var g_distribute_additional_items_cnt = 0;
var g_distribute_additional_items_num = 0;

function PackageFolder(location, folder, name, fn)
{
    g_distribute_additional_items_cnt = 0;
    var unit_name = name;
    var unit_sdk_folder = location + '/' + folder + '/' + name + '/';
    var cfg_fname = process.env[unit_name.toUpperCase()] + 'data/addition.cfg';
    
    console.log(cfg_fname);
    if(fs.existsSync(cfg_fname))
    {
        var cfg_obj = JSON.parse('' + fs.readFileSync(cfg_fname));
        if(cfg_obj.add_items)
        {
            var cfg_arr = cfg_obj.add_items.split(',');
            g_distribute_additional_items_num = cfg_arr.length/3;
            //console.log(g_distribute_additional_items_num);
            for(var loop = 0; loop < cfg_arr.length/3; ++loop)
            {
                var type = parseInt(cfg_arr[loop * 3 + 0]);
                var item_name = cfg_arr[loop * 3 + 2];
                
                if(type == 0)
                {
                    CopyFiles(process.env[unit_name.toUpperCase()] + '/', unit_sdk_folder + '/', 
                        item_name, 
                        function(){
                        g_distribute_additional_items_cnt++;
                        if(g_distribute_additional_items_cnt == g_distribute_additional_items_num)
                        {
                            console.log('packing...');
                            var cmd_7z = process.env['COMX_SDK'] + '/bin/7za.exe';
                            var parameters = [];
                            parameters.push('-r');
                            parameters.push('-tzip');
                            parameters.push('a');

                            parameters.push(location + '/' + name + '.zip');
                            parameters.push(location + '/' + folder + '/*');

                            var ls = cp.spawn(cmd_7z, parameters, {stdio:['ignore','ignore','ignore']});

                            /*ls.stderr.on('data', function (data) {
                                console.log('stderr: ' + data);
                            });*/

                            ls.on('exit', function (code) {
                            //console.log('child process exited with code ' + code);
                            if(fn)
                            {
                                fn();
                            }
                            });
                        }
                    });
                }
                else
                {
                    CopyFolder(process.env[unit_name.toUpperCase()] + '/' + item_name, unit_sdk_folder + '/' + item_name, 
                        [], 
                        [], 
                        function(){
                        g_distribute_additional_items_cnt++;
                        if(g_distribute_additional_items_cnt == g_distribute_additional_items_num)
                        {
                            console.log('packing...');
                            var cmd_7z = process.env['COMX_SDK'] + '/bin/7za.exe';
                            var parameters = [];
                            parameters.push('-r');
                            parameters.push('-tzip');
                            parameters.push('a');

                            parameters.push(location + '/' + name + '.zip');
                            parameters.push(location + '/' + folder + '/*');

                            var ls = cp.spawn(cmd_7z, parameters, {});

                            ls.stderr.on('data', function (data) {
                                console.log('stderr: ' + data);
                            });

                            ls.on('exit', function (code) {
                            //console.log('child process exited with code ' + code);
                            if(fn)
                            {
                                fn();
                            }
                            });
                        }
                    });
                }
            }   
        }
        else
        {
            console.log('packing...');
            var cmd_7z = process.env['COMX_SDK'] + '/bin/7za.exe';
            var parameters = [];
            parameters.push('-r');
            parameters.push('-tzip');
            parameters.push('a');

            parameters.push(location + '/' + name + '.zip');
            parameters.push(location + '/' + folder + '/*');

            var ls = cp.spawn(cmd_7z, parameters, {});

            ls.stderr.on('data', function (data) {
                console.log('stderr: ' + data);
            });

            ls.on('exit', function (code) {
            //console.log('child process exited with code ' + code);
                if(fn)
                {
                    fn();
                }
            });     
        }
    }     
    else
    {
        console.log('packing...');
        var cmd_7z = process.env['COMX_SDK'] + '/bin/7za.exe';
        var parameters = [];
        parameters.push('-r');
        parameters.push('-tzip');
        parameters.push('a');

        parameters.push(location + '/' + name + '.zip');
        parameters.push(location + '/' + folder + '/*');

        var ls = cp.spawn(cmd_7z, parameters, {});

        ls.stderr.on('data', function (data) {
            console.log('stderr: ' + data);
        });

        ls.on('exit', function (code) {
        //console.log('child process exited with code ' + code);
            if(fn)
            {
                fn();
            }
        });       
    }
}

function CopyFolder(src, tar, ignore_files, ignore_dirs, fn)
{
    var cmd = process.env["COMX_SDK"] + "\\distribute\\robocopy.exe";
        
    var parameters = [];
    parameters.push(src);
    parameters.push(tar);
    parameters.push("/XF");
    for(var loop = 0; loop < ignore_files.length; ++loop)
    {
        parameters.push(ignore_files[loop]);
    }
    parameters.push("/XD");
    for(var loop = 0; loop < ignore_dirs.length; ++loop)
    {
        parameters.push(ignore_dirs[loop]);
    }
    parameters.push("/E"); 
    parameters.push("/r:3");       
    parameters.push("/w:5"); 
    //parameters.push("/V");
        
    
    //console.log(JSON.stringify(parameters));
        
    var cp = require('child_process').spawn(cmd, parameters, {stdio:['ignore','ignore','pipe']});
    /*cp.stdout.on('data', function(data){
        console.log('stdout: ' + data);
    });*/
    cp.stderr.on('data', function(data){
        console.log('stderr: ' + data);
    });
    cp.on('close', function(code){
        //console.log('exit code ' + code);
        if(fn)
        {
            fn();
        }
    }); 
}

function CopyFiles(src, tar, file_patten, fn)
{
    var cmd = process.env["COMX_SDK"] + "\\distribute\\robocopy.exe";
        
    var parameters = [];
    parameters.push(src);
    parameters.push(tar);
    parameters.push(file_patten);
    
    parameters.push("/r:3");       
    parameters.push("/w:5"); 
   
    //console.log(JSON.stringify(parameters));
        
    var cp = require('child_process').spawn(cmd, parameters, {});
    /*cp.stdout.on('data', function(data){
        console.log('stdout: ' + data);
    });*/
    cp.stderr.on('data', function(data){
        console.log('stderr: ' + data);
    });
    cp.on('close', function(code){
        if(fn)
        {
            fn();
        }
    }); 
}

var xml2js = require('xml2js');

function CheckLibItemsOfUnit(unit_name, fn)
{
    var unit_path = process.env[unit_name.toUpperCase()];
    var product_cfg_fname = unit_path + unit_name + '.kproduct';
    var parser = new xml2js.Parser();
    parser.addListener('end', function(result) {
        //fs.writeFileSync("y:\\a.json", JSON.stringify(result));
        var script_item = result.config.path[0].script_paths[0].item;
        var module_item = result.config.path[0].module_path[0].item;
        
        var list = [];
        
        for(var loop = 0; loop < script_item.length; ++loop)
        {
            var item = eval(script_item[loop]);
            //console.log(item.split('/'));
            if(item.split('/')[1] == 'lib')
            {
                list.push(item.split('/')[2]);
            }
        }
        
        for(var loop = 0; loop < module_item.length; ++loop)
        {
            var item = eval(module_item[loop]);
            //console.log(item.split('/'));
            if(item.split('/')[1] == 'lib')
            {
                list.push(item.split('/')[2]);
            }
        }
        list = list.reverse().join(",").match( /([^,]+)(?!.*\1)/ig).reverse();
        if(fn)
        {
            fn(list);
        }
    });
    
    if(fs.existsSync(product_cfg_fname))
    {
        fs.readFile(product_cfg_fname, function(err, data) {
            parser.parseString(data);
        });
    }
}

function FormatUnitName(unit_name)
{
    var unit_arr = unit_name.split('');
    unit_arr[0] = unit_arr[0].toUpperCase();
    return unit_arr.join('');
}

function CreateStartupBat(unit_name, entry, is_deamon)
{
    var cfg_fname = process.env[unit_name.toUpperCase()] + 'data/addition.cfg';
    
    var is_create_shortcut = true;
    var is_disable_comx_support = false;
    if(fs.existsSync(cfg_fname))
    {
        var cfg_obj = JSON.parse('' + fs.readFileSync(cfg_fname));
        is_create_shortcut = eval(cfg_obj.create_shortcut);
        if(cfg_obj.disable_comx_support)
        {
            is_disable_comx_support = eval(cfg_obj.disable_comx_support);
        }
    }
    
    var ret = '';
    ret += '@echo off\r\n';
    ret += 'if exist "%~dp0..\\' + unit_name + '\\picture\\default.ico" (set icon_file="%~dp0..\\' + unit_name + '\\picture\\default.ico") else (set icon_file="%~dp0\\..\\comx_sdk\\picture\\default.ico")\r\n';
    
    if(is_create_shortcut)
    {
        ret += 'if exist "%USERPROFILE%\\Desktop\\' + 
           FormatUnitName(unit_name) + 
           '.lnk"' + 
           ' (set is_first=0) else ("%~dp0\\..\\comx_sdk\\shortcut.exe" "%~dp0..\\startup.exe" /s 2 /d ' + 
           FormatUnitName(unit_name) + 
           ' /i ' + '%icon_file%' + ',0 /ld ' + 
           FormatUnitName(unit_name) + 
           '.lnk)\r\n';
    }
    
    ret += 'for /f %%i in (\'"%~dp0fsnewdriver.exe"\') do set DRIVER=%%i\r\n';
    ret += 'set str=%~dp0\r\n';
    ret += 'set TARGET_DIR=%str:~0,-11%\r\n';
    ret += 'subst %DRIVER% "%TARGET_DIR%"\r\n';
    ret += 'set COMX_SDK=%DRIVER%\\comx_sdk\\\r\n';
    ret += 'set ' + unit_name.toUpperCase() + '=%DRIVER%\\' + unit_name + '\\\r\n';
    if(is_deamon)
    {
        ret += 'set ' + 'DEAMONMGR=%DRIVER%\\' + 'deamonmgr' + '\\\r\n';
    }
    ret += 'set NODE_PATH=%DRIVER%\\comx_sdk\\nodejs\\node_modules\\\r\n';
    ret += 'set PATH=%PATH%;%DRIVER%\\comx_sdk\\deps\r\n';
    
    if(is_disable_comx_support)
    {
        ret += '%DRIVER%\\comx_sdk\\bin\\kdesktop.exe -u ' + unit_name + ' -e ' + entry + ' -s\r\n';
    }
    else
    {
        ret += '%DRIVER%\\comx_sdk\\bin\\kdesktop.exe -u ' + unit_name + ' -e ' + entry + '\r\n';
    }
    
    ret += 'subst /D %DRIVER%\r\n';
    ret += 'exit\r\n';
    
    return ret;
}

function GenerateGUID()
{
    var guid = "";
    for (var i = 1; i <= 32; i++){
        var n = Math.floor(Math.random()*16.0).toString(16);
        guid +=   n;
    }
    return guid;
}

var g_unit_distribute_cnt = 0;
var g_is_lib_distribute_done = false;
var g_lib_distribute_cnt = 0;

websocket.on('CreateUnit', function(data){
    var ls = cp.spawn(process.env['COMX_SDK'] + '/distribute/kunit.exe', ['-c', data.parameters.unit_name], {});

    ls.on('exit', function (code) {
		console.log('child process exited with code ' + code);
		websocket.send(data);
    });
    /*ls.stdout.on('data', function(data){
        console.log('' + data);
    });*/
});

websocket.on('UpdateUnit', function(data){
    var unit_name = data.parameters.unit_name;
    if(unit_name != 'comx_sdk')
    {
        var ls = cp.spawn(process.env['COMX_SDK'] + '/distribute/kunit.exe', ['-u', unit_name], {});

        ls.on('exit', function (code) {
            console.log('child process exited with code ' + code);
            websocket.send(data);
        });
        
        /*ls.stdout.on('data', function(data){
            console.log('' + data);
        });*/
    }
});

websocket.on('GetUnitEntryList', function(data){
    var unit_name = data.parameters.unit_name;
    var entry_xml = process.env[unit_name.toUpperCase()] + '/kul/kul.entry.xml';
    
    var parser = new xml2js.Parser();
    parser.addListener('end', function(result) {
        var formid_arr = result.kul_pkg.formid;
        var default_pane_id = formid_arr[0].value[0];
        if(result.kul_pkg.default)
        {
            default_pane_id = result.kul_pkg.default[0].$.value;
        }
        
        var name_arr = [];
        var val_arr = [];
        for(var loop = 0; loop < formid_arr.length; ++loop)
        {
            var name = formid_arr[loop].$.name;
            name_arr.push(name);
            var val = formid_arr[loop].value[0];
            val_arr.push(val);
        }
        
        data.parameters.names = name_arr;
        data.parameters.values = val_arr;
        data.parameters.defid = default_pane_id;
        websocket.send(data);
    });
    
    if(fs.existsSync(entry_xml))
    {
        fs.readFile(entry_xml, function(err, data) {
            parser.parseString(data);
        });
    }
});



var path = require('path');  



var g_create_js_ext_cnt = 0;

websocket.on('CreateJSExt', function(data){
    var sub_folder = 'js.ext';
    if(data.parameters.unit_name == 'comx_sdk')
    {
        sub_folder = 'js_ext';
    }
    
    var tmpl_type = data.parameters.tmpl_type;
    var tmpl_dir = root_dir + 'data/js.ext/template/' + tmpl_type + '/';
    var proj_dir = process.env[data.parameters.unit_name.toUpperCase()] + 'src/' + sub_folder + '/' + data.parameters.proj_name + '/';
    
    if(!fs.existsSync(proj_dir))
    {
        mkdirsSync(proj_dir);
    }
            
    var rule_fpath = root_dir + 'data/js.ext/template/' + data.parameters.rule_file;

    g_create_js_ext_cnt = 0;
    
    var cmd = root_dir + 'bin/replace_file.exe';

    var trans_files = [
        {'from' : 'Readme.txt', 'to' : 'Readme.txt'},
        {'from' : 'StdAfx.cpp', 'to' : 'StdAfx.cpp'},
        {'from' : 'StdAfx.h', 'to' : 'StdAfx.h'},
        {'from' : 'template.cpp', 'to' : data.parameters.proj_name + '.cpp'},
        {'from' : 'template.sln', 'to' : data.parameters.proj_name + '.sln'},
        {'from' : 'template.vcproj', 'to' : data.parameters.proj_name + '.vcproj'}
        ];
    
    for(var loop = 0; loop < trans_files.length; ++loop)
    {
        var parameters = [];
        parameters.push('-i');
        parameters.push(tmpl_dir + trans_files[loop].from);
        parameters.push('-o');
        parameters.push(proj_dir + trans_files[loop].to);
        parameters.push('-r');
        parameters.push(rule_fpath);
        
        var cp = require('child_process').spawn(cmd, parameters, {});
        cp.on('close', function(code){
            g_create_js_ext_cnt++;
            if(g_create_js_ext_cnt == trans_files.length)
            {
                websocket.send(data);
                fs.unlinkSync(rule_fpath);
            }
        });
    }
});


//GetJSExtProjList
websocket.on('GetJSExtProjList', function(data){
    var unit_name = data.parameters.unit_name;
    var src_path = process.env[unit_name.toUpperCase()] + 'src/js.ext/';
    if(unit_name == 'comx_sdk')
    {
        src_path = process.env[unit_name.toUpperCase()] + 'src/js_ext/';
    }
     
    var ret = [];
     
    if(fs.existsSync(src_path))
    {
        var files = fs.readdirSync(src_path);
    
        files.forEach(function(item){
            var item_path = src_path + item;
            if(fs.statSync(item_path).isDirectory())
            {
                ret.push(item);
            }
        });
    }
    
    
    data.parameters = ret;
    websocket.send(data);
});

//AppendJSExtMethod

websocket.on('AppendJSExtMethod', function(data){
    var parameters = [];
    parameters.push('-f');
    parameters.push(data.parameters.file);
    parameters.push('-m');
    parameters.push(data.parameters.method);
    parameters.push('-n');
    parameters.push(data.parameters.num);
   
    var cmd = root_dir + 'bin/js_ext_func.exe';
    
    var cp = require('child_process').spawn(cmd, parameters, {});
        cp.on('close', function(code){
            websocket.send(data);
        });
    
});

websocket.on('GetOperatorSystemRelease', function(data){
    data.parameters.release = require('os').release();
    websocket.send(data);
});

//UpdateDeamonUnit
websocket.on('UpdateDeamonUnit', function(data){
    var rule = [{'from' : '0xe4e0c838-0x5460-0x4368-0x89-0x97-0x98-0x19-0x17-0x73-0x2f-0xec', 'to' : data.parameters.pkgid},
                {'from' : '0x5871bc8b-0x64d6-0x45c1-0x98-0x65-0x30-0xc3-0xfc-0x95-0x24-0x3d', 'to' : data.parameters.formid},
                {'from' : '0x11607d9f-0xaf58-0x4cae-0xa6-0x7b-0x4f-0xf7-0xf7-0x1a-0xe0-0xbd', 'to' : data.parameters.refid},
                {'from' : '0x7f3c409a-0x7093-0x40c5-0xa4-0xbd-0x0e-0x1f-0x06-0x7a-0x06-0x76', 'to' : data.parameters.id},
                {'from' : '0x5015c1de-0x6a65-0x43d1-0xa2-0xb2-0xed-0x29-0x0f-0xde-0x29-0xbb', 'to' : data.parameters.name}];
                
    var rule_fpath = root_dir + 'js/tmpl/rule.json';
    
    fs.writeFileSync(rule_fpath, JSON.stringify(rule));

    g_create_js_ext_cnt = 0;
    
    var cmd = root_dir + 'bin/replace_file.exe';

    var deamon_home_dir = "y:/" + data.parameters.unit_name + "/";
    
    //main.kul
    {
        var parameters = [];
        parameters.push('-i');
        parameters.push(deamon_home_dir + 'kul/main.kul');
        parameters.push('-o');
        parameters.push(deamon_home_dir + 'kul/main.kul');
        parameters.push('-r');
        parameters.push(rule_fpath);
            
        var cp = require('child_process').spawn(cmd, parameters, {});
        cp.on('close', function(code){
            g_create_js_ext_cnt++;
            if(g_create_js_ext_cnt == 4)
            {
                websocket.send(data);
                fs.unlinkSync(rule_fpath);
            }
        });
    }
    
    //ui_block.kul
    {
        var parameters = [];
        parameters.push('-i');
        parameters.push(deamon_home_dir + 'kul/ui_block.kul');
        parameters.push('-o');
        parameters.push(deamon_home_dir + 'kul/ui_block.kul');
        parameters.push('-r');
        parameters.push(rule_fpath);
            
        var cp = require('child_process').spawn(cmd, parameters, {});
        cp.on('close', function(code){
            g_create_js_ext_cnt++;
            if(g_create_js_ext_cnt == 4)
            {
                websocket.send(data);
                fs.unlinkSync(rule_fpath);
            }
        });
    }
    
    //main_pane.js
    {
        var parameters = [];
        parameters.push('-i');
        parameters.push(deamon_home_dir + 'js/main_pane.js');
        parameters.push('-o');
        parameters.push(deamon_home_dir + 'js/main_pane.js');
        parameters.push('-r');
        parameters.push(rule_fpath);
            
        var cp = require('child_process').spawn(cmd, parameters, {});
        cp.on('close', function(code){
            g_create_js_ext_cnt++;
            if(g_create_js_ext_cnt == 4)
            {
                websocket.send(data);
                fs.unlinkSync(rule_fpath);
            }
        });
    }
    
    //websocket.js
    {
        var parameters = [];
        parameters.push('-i');
        parameters.push(deamon_home_dir + 'js/websocket.js');
        parameters.push('-o');
        parameters.push(deamon_home_dir + 'js/websocket.js');
        parameters.push('-r');
        parameters.push(rule_fpath);
            
        var cp = require('child_process').spawn(cmd, parameters, {});
        cp.on('close', function(code){
            g_create_js_ext_cnt++;
            if(g_create_js_ext_cnt == 4)
            {
                websocket.send(data);
                fs.unlinkSync(rule_fpath);
            }
        });
    }     
});

websocket.on('DistributeUnit', function(data){
    var unit_name = data.parameters.unit_name;
    var location = data.parameters.location;
    var entry = data.parameters.entry;
    
    var is_deamon = (data.parameters.is_deamon == 'true');

    if(unit_name == "comx_sdk")
    {
        /*
        if(fs.existsSync(location + "\\comx_sdk\\"))
        {
            rmdirSync(location + "\\comx_sdk\\");
        }
        */
        
        var cmd = process.env["COMX_SDK"] + "\\distribute\\robocopy.exe";
        
        var tmp_dir = GenerateGUID();
        
        var parameters = [];
        parameters.push(process.env["COMX_SDK"]);
        parameters.push(location + "\\" + tmp_dir + "\\");
        parameters.push("/XF");
        parameters.push("deps.7z");
        parameters.push("deps.sh");
        parameters.push("deps.js");
        parameters.push(".gitignore");
        parameters.push("/XD");
        parameters.push(".git");
        parameters.push("src");
        parameters.push("setup");
        parameters.push("/E");       
  
        
        var cp = require('child_process').spawn(cmd, parameters, {});
        /*cp.stdout.on('data', function(data){
            console.log('stdout: ' + data);
        });*/
        cp.stderr.on('data', function(data){
            console.log('stderr: ' + data);
        });
        cp.on('close', function(code){
            var target_arch = location + '/' + 'comx_sdk' + '.7z';
            if(fs.existsSync(target_arch))
            {
                rmdirSync(target_arch);
            }
            
            PackageFolder(location, tmp_dir, 'comx_sdk', function(){
                if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                {
                    rmdirSync(location + "\\" + tmp_dir + "\\");
                }
                data.parameters = {};
                websocket.send(data);
            });
        }); 
    }
    else
    {
        var tmp_dir = GenerateGUID();
        //创建unit文件夹
        var unit_target_folder = location + '/' + tmp_dir;
        fs.mkdirSync(unit_target_folder);
        
        //发布COMX_SDK
        var comx_sdk_folder = unit_target_folder + '/comx_sdk';
        fs.mkdirSync(comx_sdk_folder); 
        
        g_unit_distribute_cnt = 0;
        g_is_lib_distribute_done = false;
        g_lib_distribute_cnt = 0;
        
        if(fs.existsSync(location + "\\" + unit_name + ".zip"))
        {
            rmdirSync(location + "\\" + unit_name + ".zip");
        }
        
        var COPY_CNT = 26;
        if(is_deamon)
        {
            COPY_CNT = 35;
        }
        
        //bin
        CopyFolder(process.env["COMX_SDK"] + '/bin', comx_sdk_folder + '/bin', 
            ['*_d.exe','*.lib', '*.tmp', '*.awx', '*_d.kpi', '*.log', 'kid*.exe', 'comx*.bat', 'keditor.bat', 'pdf.bat'], 
            [], 
            function(){
            console.log('bin is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //data
        CopyFolder(process.env["COMX_SDK"] + '/data', comx_sdk_folder + '/data', 
            [], 
            [], 
            function(){
            console.log('data is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //distribute
        CopyFolder(process.env["COMX_SDK"] + '/distribute', comx_sdk_folder + '/distribute', 
            [], 
            [], 
            function(){
            console.log('distribute is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //js
        CopyFolder(process.env["COMX_SDK"] + '/js', comx_sdk_folder + '/js', 
            ['*.db'], 
            [], 
            function(){
            console.log('js is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //kul
        CopyFolder(process.env["COMX_SDK"] + '/kul', comx_sdk_folder + '/kul', 
            ['*.tmp'], 
            [], 
            function(){
            console.log('kul is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //js.ext
        CopyFolder(process.env["COMX_SDK"] + '/js.ext', comx_sdk_folder + '/js.ext', 
            ['*_d.js.ext'], 
            [], 
            function(){
            console.log('js.ext is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //nodejs
        CopyFolder(process.env["COMX_SDK"] + '/nodejs', comx_sdk_folder + '/nodejs', 
            [], 
            [], 
            function(){
            console.log('nodejs is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //picture
        CopyFolder(process.env["COMX_SDK"] + '/picture', comx_sdk_folder + '/picture', 
            [], 
            [], 
            function(){
            console.log('picture is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //widget.addin
        CopyFolder(process.env["COMX_SDK"] + '/widget.addin', comx_sdk_folder + '/widget.addin', 
            ['*_d.widget.addin'], 
            [], 
            function(){
            console.log('widget.addin is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deps
        CopyFolder(process.env["COMX_SDK"] + '/deps', comx_sdk_folder + '/deps', 
            [], 
            [], 
            function(){
            console.log('deps is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //*.bat
        CopyFiles(process.env["COMX_SDK"] + '/', comx_sdk_folder + '/', 
            '*.bat', 
            function(){
            console.log('*.bat is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //shortcut.exe
        CopyFiles(process.env["COMX_SDK"] + '/', comx_sdk_folder + '/', 
            'shortcut.exe', 
            function(){
            console.log('shortcut.exe is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //Lib
        CheckLibItemsOfUnit(unit_name, function(list){
            for(var loop = 0; loop < list.length; ++loop)
            {
                var lib_item = list[loop];
                CopyFolder(process.env["COMX_SDK"] + '/lib/' + lib_item, comx_sdk_folder + '/lib/' + lib_item, 
                    ['*_d.kpi','*.lib', '*.log'], 
                    ['js_ext_alone'], 
                    function(){
                    g_lib_distribute_cnt++;
                    if(g_lib_distribute_cnt == list.length)
                    {
                        g_is_lib_distribute_done = true;
                    }
                    
                    if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                    {
                        PackageFolder(location, tmp_dir, unit_name, function(){
                            if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                            {
                                rmdirSync(location + "\\" + tmp_dir + "\\");
                            }
                            data.parameters = {};
                            websocket.send(data);
                        });
                    }
                });
            }
        });
        
        //Unit Copy
        var unit_sdk_folder = unit_target_folder + '/' + unit_name;
        fs.mkdirSync(unit_sdk_folder); 
        
        var deamonmgr_sdk_folder = unit_target_folder + '/deamonmgr';
        
        if(is_deamon)
        {
            fs.mkdirSync(unit_target_folder + '/deamonmgr');
        }
        
        //data
        CopyFolder(process.env[unit_name.toUpperCase()] + '/data', unit_sdk_folder + '/data', 
            [], 
            [], 
            function(){
            console.log('unit.data is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deamonmgr data
        if(is_deamon) 
        {
            CopyFolder(process.env['DEAMONMGR'] + '/data', deamonmgr_sdk_folder + '/data', 
                [], 
                [], 
                function(){
                console.log('deamonmgr.data is successful');
                g_unit_distribute_cnt++;
                if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                {
                    PackageFolder(location, tmp_dir, unit_name, function(){
                        if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                        {
                            rmdirSync(location + "\\" + tmp_dir + "\\");
                        }
                        data.parameters = {};
                        websocket.send(data);
                    });
                }
            });
        }
        
        //js
        CopyFolder(process.env[unit_name.toUpperCase()] + '/js', unit_sdk_folder + '/js', 
            ['*.db', '*.tmp'], 
            [], 
            function(){
            console.log('unit.js is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deamonmgr js
        if(is_deamon) 
        {
            CopyFolder(process.env['DEAMONMGR'] + '/js', deamonmgr_sdk_folder + '/js', 
                ['*.db', '*.tmp'], 
                [], 
                function(){
                console.log('deamonmgr.js is successful');
                g_unit_distribute_cnt++;
                if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                {
                    PackageFolder(location, tmp_dir, unit_name, function(){
                        if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                        {
                            rmdirSync(location + "\\" + tmp_dir + "\\");
                        }
                        data.parameters = {};
                        websocket.send(data);
                    });
                }
            });
        }
        
        //js.ext
        CopyFolder(process.env[unit_name.toUpperCase()] + '/js.ext', unit_sdk_folder + '/js.ext', 
            ['*_d.js.ext', '*.ilk'], 
            [], 
            function(){
            console.log('unit.js.ext is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deamonmgr js.ext
        if(is_deamon) 
        {
            CopyFolder(process.env['DEAMONMGR'] + '/js.ext', deamonmgr_sdk_folder + '/js.ext', 
                ['*_d.js.ext', '*.ilk'], 
                [], 
                function(){
                console.log('deamonmgr.js.ext is successful');
                g_unit_distribute_cnt++;
                if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                {
                    PackageFolder(location, tmp_dir, unit_name, function(){
                        if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                        {
                            rmdirSync(location + "\\" + tmp_dir + "\\");
                        }
                        data.parameters = {};
                        websocket.send(data);
                    });
                }
            });
        }
        
        //kul
        CopyFolder(process.env[unit_name.toUpperCase()] + '/kul', unit_sdk_folder + '/kul', 
            ['*.tmp'], 
            [], 
            function(){
            console.log('unit.kul is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deamonmgr kul
        if(is_deamon) 
        {
            CopyFolder(process.env['DEAMONMGR'] + '/kul', deamonmgr_sdk_folder + '/kul', 
                ['*.tmp'], 
                [], 
                function(){
                console.log('deamonmgr.kul is successful');
                g_unit_distribute_cnt++;
                if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                {
                    PackageFolder(location, tmp_dir, unit_name, function(){
                        if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                        {
                            rmdirSync(location + "\\" + tmp_dir + "\\");
                        }
                        data.parameters = {};
                        websocket.send(data);
                    });
                }
            });
        }
        
        //module
        CopyFolder(process.env[unit_name.toUpperCase()] + '/module', unit_sdk_folder + '/module', 
            ['*_d.kpi', '*.ilk'], 
            [], 
            function(){
            console.log('unit.module is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deamonmgr module
        if(is_deamon) 
        {
            CopyFolder(process.env['DEAMONMGR'] + '/module', deamonmgr_sdk_folder + '/module', 
                ['*_d.kpi', '*.ilk'], 
                [], 
                function(){
                console.log('deamonmgr.module is successful');
                g_unit_distribute_cnt++;
                if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                {
                    PackageFolder(location, tmp_dir, unit_name, function(){
                        if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                        {
                            rmdirSync(location + "\\" + tmp_dir + "\\");
                        }
                        data.parameters = {};
                        websocket.send(data);
                    });
                }
            });
        }
        
        //uiplugin
        CopyFolder(process.env[unit_name.toUpperCase()] + '/uiplugin', unit_sdk_folder + '/uiplugin', 
            ['*_d.uiplugin', '*.ilk'], 
            [], 
            function(){
            console.log('unit.uiplugin is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deamonmgr uiplugin
        if(is_deamon) 
        {
            CopyFolder(process.env['DEAMONMGR'] + '/uiplugin', deamonmgr_sdk_folder + '/uiplugin', 
                ['*_d.uiplugin', '*.ilk'], 
                [], 
                function(){
                console.log('deamonmgr.uiplugin is successful');
                g_unit_distribute_cnt++;
                if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                {
                    PackageFolder(location, tmp_dir, unit_name, function(){
                        if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                        {
                            rmdirSync(location + "\\" + tmp_dir + "\\");
                        }
                        data.parameters = {};
                        websocket.send(data);
                    });
                }
            });
        }
        
        //picture
        CopyFolder(process.env[unit_name.toUpperCase()] + '/picture', unit_sdk_folder + '/picture', 
            [], 
            [], 
            function(){
            console.log('unit.picture is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deamonmgr picture
        if(is_deamon) 
        {
            CopyFolder(process.env['DEAMONMGR'] + '/picture', deamonmgr_sdk_folder + '/picture', 
                [], 
                [], 
                function(){
                console.log('deamonmgr.picture is successful');
                g_unit_distribute_cnt++;
                if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                {
                    PackageFolder(location, tmp_dir, unit_name, function(){
                        if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                        {
                            rmdirSync(location + "\\" + tmp_dir + "\\");
                        }
                        data.parameters = {};
                        websocket.send(data);
                    });
                }
            });
        }
        
        //script
        CopyFolder(process.env[unit_name.toUpperCase()] + '/script', unit_sdk_folder + '/script', 
            [], 
            [], 
            function(){
            console.log('unit.script is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deamonmgr script
        if(is_deamon)
        {
            CopyFolder(process.env['DEAMONMGR'] + '/script', deamonmgr_sdk_folder + '/script', 
                [], 
                [], 
                function(){
                console.log('deamonmgr.script is successful');
                g_unit_distribute_cnt++;
                if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                {
                    PackageFolder(location, tmp_dir, unit_name, function(){
                        if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                        {
                            rmdirSync(location + "\\" + tmp_dir + "\\");
                        }
                        data.parameters = {};
                        websocket.send(data);
                    });
                }
            });
        }
        
        //kproduct
        CopyFiles(process.env[unit_name.toUpperCase()] + '/', unit_sdk_folder + '/', 
            unit_name + '.kproduct', 
            function(){
            console.log('kproduct is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //deamonmgr kproduct 35
        if(is_deamon)
        {
            CopyFiles(process.env['DEAMONMGR'] + '/', deamonmgr_sdk_folder + '/', 
                'deamonmgr' + '.kproduct', 
                function(){
                console.log('deamonmgr.kproduct is successful');
                g_unit_distribute_cnt++;
                if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
                {
                    PackageFolder(location, tmp_dir, unit_name, function(){
                        if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                        {
                            rmdirSync(location + "\\" + tmp_dir + "\\");
                        }
                        data.parameters = {};
                        websocket.send(data);
                    });
                }
            });
        }
    
        //Configure
        //startup.exe
        CopyFiles(process.env["COMX_SDK"] + '/distribute/', unit_target_folder + '/', 
            'startup.exe',
            function(){
            console.log('startup.exe is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //configure
        fs.mkdirSync(unit_target_folder + '/configure');
        //fsnewdriver.exe
        CopyFiles(process.env["COMX_SDK"] + '/distribute', unit_target_folder + '/configure', 
            'fsnewdriver.exe',
            function(){
            console.log('fsnewdriver.exe is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        //_.bat
        var bat_fpath = unit_target_folder + '/configure/_.bat';
        fs.writeFileSync(bat_fpath, CreateStartupBat(unit_name, entry, is_deamon));
        
        //supprot
        CopyFolder(process.env["COMX_SDK"] + '/deps/support', unit_target_folder + '/support', 
            [], 
            [], 
            function(){
            console.log('support is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        
        //service
        CopyFolder(process.env["COMX_SDK"] + '/service', unit_target_folder + '/service', 
            [], 
            [], 
            function(){
            console.log('service is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
        CopyFolder(process.env["COMX_SDK"] + '/nodejs', unit_target_folder + '/service/comx_sdk/nodejs', 
            [], 
            [], 
            function(){
            console.log('service.nodejs is successful');
            g_unit_distribute_cnt++;
            if(g_unit_distribute_cnt == COPY_CNT && g_is_lib_distribute_done)
            {
                PackageFolder(location, tmp_dir, unit_name, function(){
                    if(fs.existsSync(location + "\\" + tmp_dir + "\\"))
                    {
                        rmdirSync(location + "\\" + tmp_dir + "\\");
                    }
                    data.parameters = {};
                    websocket.send(data);
                });
            }
        });
    }
});




