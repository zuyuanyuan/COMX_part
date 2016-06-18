var fs = require('fs');
var cp = require('child_process');
var xml2js = require('xml2js');

var root_dir = process.env['COMX_SDK'];

function GenerateGUID()
{
    var guid = "";
    for (var i = 1; i <= 32; i++){
        var n = Math.floor(Math.random()*16.0).toString(16);
        guid +=   n;
    }
    return guid;
}

function deal_with_filepath_format(file_path)
{
    var file_path_arr = file_path.split("");
    for(var loop = 0; loop < file_path_arr.length; ++loop)
    {
	if(file_path_arr[loop] == '\\')
	{
	    file_path_arr[loop] = '/';
	}
    }

    return file_path_arr.join("");
}

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

var excel_json_cache = {};

function GenerateExcelFileHandle()
{
    var excel_cache_dir = process.env['EXCEL_NODEJS_CACHE_DIR'];
    if(excel_cache_dir == undefined)
    {
	excel_cache_dir = process.env['COMX_SDK'];
	excel_cache_dir += 'data\\cache\\';
    }

    return excel_cache_dir + GenerateGUID() + '\\';
}

exports.Open = function(filepath, fn){
    var excel_file = filepath;
    var handler = GenerateExcelFileHandle();

    var temp_dir = handler;

    var cmd_7z = root_dir + 'bin/7za.exe';

    if(fs.existsSync(temp_dir))
    {
        rmdirSync(temp_dir);
    }

    var parameters = [];
    parameters.push('x');
    parameters.push(excel_file);
    parameters.push('-o' + temp_dir);
    parameters.push('*');
    parameters.push('-r');

    var ls = cp.spawn(cmd_7z, parameters, {});

    ls.stderr.on('data', function (data) {
        console.log('stderr: ' + data);
    });

    ls.on('exit', function (code) {
        console.log('child process exited with code ' + code);
	if(code == 0)
	{
            var xlsx_sheet1_xml = deal_with_filepath_format(temp_dir + '/xl/worksheets/sheet1.xml');
            var xlsx_sharedString_xml = deal_with_filepath_format(temp_dir + '/xl/sharedStrings.xml');
	    
            var parser = new xml2js.Parser();
            parser.addListener('end', function(sheet1){
		var parser_str = new xml2js.Parser();
		parser_str.addListener('end', function(sharedString){
		    var cache_item = {'sheet1' : sheet1, 'sharedStrings' : sharedString, 'excel_filepath' : excel_file};
		    excel_json_cache[handler] = cache_item;
		    
		    fn(handler);
		});
		
		fs.readFile(xlsx_sharedString_xml, function(err, data){
                    parser_str.parseString(data);
		});
            });
	    
            fs.readFile(xlsx_sheet1_xml, function(err, data) {
		parser.parseString(data);
            });
	}
	else
	{
	    fn(undefined);
	}
    });

    return handler;
};

exports.Close = function(handler, fn)
{
    var cmd_7z = root_dir + 'bin/7za.exe';
    var parameters = [];
    parameters.push('-r');
    parameters.push('a');

    var filepath = excel_json_cache[handler].excel_filepath;

    if(fs.existsSync(filepath))
    {
        rmdirSync(filepath);
    }
    
    parameters.push(filepath);
    parameters.push(handler + '*');

    var ls = cp.spawn(cmd_7z, parameters, {});

    ls.stderr.on('data', function (data) {
        console.log('stderr: ' + data);
    });

    ls.on('exit', function (code) {
	console.log('child process exited with code ' + code);
	if(fs.existsSync(handler))
	{
            rmdirSync(handler);
	}

	excel_json_cache[handler] = false;
	if(fn)
	{
	    fn();
	}
    });
}

exports.SaveAs = function(handler, filepath, fn)
{
    var cmd_7z = root_dir + 'bin/7za.exe';
    var parameters = [];
    parameters.push('-r');
    parameters.push('a');

    parameters.push(filepath);
    parameters.push(handler + '*');

    var ls = cp.spawn(cmd_7z, parameters, {});

    ls.stderr.on('data', function (data) {
        console.log('stderr: ' + data);
    });

    ls.on('exit', function (code) {
	console.log('child process exited with code ' + code);
	if(fn)
	{
	    fn();
	}
    });
};

function isDigital(ch)
{
    return (ch == '0' || 
	ch == '1' ||
	ch == '2' ||
	ch == '3' ||
	ch == '4' ||
	ch == '5' ||
	ch == '6' ||
	ch == '7' ||
	ch == '8' ||
	ch == '9');
}

function parseRowOfExcel(index_str)
{
    var index = 0;
    for(var loop = 0; loop < index_str.length; ++loop)
    {
	var ch = index_str[loop];
	if(isDigital(ch))
	{
	    index = loop;
	    break;
	}
    }

    return parseInt(index_str.substr(index));
}

exports.Get = function(handler, index)
{
    if(excel_json_cache[handler].sheet1.worksheet.sheetData[0].row == undefined)
    {
	return undefined;
    }

    var row = parseRowOfExcel(index);
    if(row > excel_json_cache[handler].sheet1.worksheet.sheetData[0].row.length)
    {
	return undefined;
    }

    var row_obj = excel_json_cache[handler].sheet1.worksheet.sheetData[0].row[row - 1];
    
    if(row_obj == undefined)
    {
	return undefined;
    }
    
    if(row_obj.c == undefined)
    {
	return undefined;
    }
    
    for(var loop = 0; loop < row_obj.c.length; ++loop)
    {
	var cell_obj = row_obj.c[loop];
	if(cell_obj.$.r.toLowerCase() == index.toLowerCase())
	{
	    if(cell_obj.$.t != undefined && cell_obj.$.t == 's')
	    {
		var indexSharedString = cell_obj.v[0];
		return excel_json_cache[handler].sharedStrings.sst.si[indexSharedString].t[0];
	    }
	    else
	    {
		if(cell_obj.v == undefined)
		{
		    return undefined;
		}
		else
		{
		    return cell_obj.v[0];
		}
	    }
	}
    }
};

exports.GetLineNumber = function(handler)
{
    if(excel_json_cache[handler].sheet1.worksheet.sheetData[0].row == undefined)
    {
	return undefined;
    }

    return excel_json_cache[handler].sheet1.worksheet.sheetData[0].row.length;
};

exports.ReloadCache = function(handler)
{
    if(!excel_json_cache[handler])
    {
	return;
    }

    var temp_dir = handler;
    var xlsx_sheet1_xml = deal_with_filepath_format(temp_dir + '/xl/worksheets/sheet1.xml');
    var xlsx_sharedString_xml = deal_with_filepath_format(temp_dir + '/xl/sharedStrings.xml');

    var parser = new xml2js.Parser();
    parser.addListener('end', function(sheet1){
        var parser_str = new xml2js.Parser();
        parser_str.addListener('end', function(sharedString){
	    excel_json_cache[handler].sheet1 = sheet1;
	    excel_json_cache[handler].sharedStrings = sharedString;
        });
        
        fs.readFile(xlsx_sharedString_xml, function(err, data){
            parser_str.parseString(data);
        });
    });
    
    fs.readFile(xlsx_sheet1_xml, function(err, data) {
        parser.parseString(data);
    });
};

exports.Modify = function(handler, index, value, fn)
{
    var cmd = root_dir + 'bin/excel_write.exe';
    var parameters = [];
    parameters.push(handler);
    parameters.push(index);
    parameters.push(value);

    var ls = cp.spawn(cmd, parameters, {});

    ls.stderr.on('data', function (data) {
        console.log('stderr: ' + data);
    });

    ls.on('exit', function (code) {
	console.log('excel_write.exe process exited with code ' + code);
	if(fn)
	{
	    fn();
	}
    });
}
