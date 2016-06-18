var fs = require('fs');
var xml2js = require('xml2js');
var websocket = require(process.env['COMX_SDK'] + 'js/socket.io/websocket.common.js');

/////////////////////////////////////////////////////////////////////////////////
// implement GetKulEntryInfo function.

var g_kul_cnt = 0;
var g_kul_cnt_block = false;

exports.GetKulEntryInfo = function(data){
	var unit_name = data.parameters.unit_name;
	if(!unit_name)
	{
		return;
	}
	if(g_kul_cnt_block)
	{
		return;
	}
	
	g_kul_cnt_block = true;

	g_kul_cnt = 0;
    
    var unit_entry_filename = "y:\\" + unit_name + "\\kul\\kul.entry.xml";
    //var json_fname = "y:\\" + unit_name + "\\kul\\kul.entry.json";

    function json_wrap(jsontree)
    {
	var ret = {events : [], bindings : [], events_hdr : [], bindings_hdr : [], events_type : [], bindings_type : []};
	function json(jsontree) 
	{  
	    if ((typeof jsontree == 'object') && (jsontree.constructor== Object.prototype.constructor)) 
	    {  
		var arrey = [];  
		arrey.push(jsontree);  
	    }  
	    else arrey = jsontree;  
	    for (var i = 0; i < arrey.length; i++) 
	    {  
		var jn = arrey[i];  
		if(jn.property != undefined)
		{
		    var bIsEvent = false;
		    var bIsBinding = false;
		    
		    var bind_hdr = "";
		    for(var loop = 0; loop < jn.property.length; ++loop)
		    {
			var prop_name = jn.property[loop].$.name;
			var prop_value = jn.property[loop]._;
			
			if(prop_name == "handle_bind")
			{
			    bind_hdr = prop_value;
			}
		    }
		    
		    for(var loop = 0; loop < jn.property.length; ++loop)
		    {
			var prop_name = jn.property[loop].$.name;
			var prop_value = jn.property[loop]._;
			
			if(is_binding(prop_name) && prop_value != undefined)
			{
			    ret.bindings.push(prop_value);
			    ret.bindings_type.push(prop_name);
			    ret.bindings_hdr.push(bind_hdr);
			    bIsBinding = true;
			}
			
			if(is_event(prop_name) && prop_value != undefined)
			{
			    ret.events.push(prop_value);
			    ret.events_type.push(prop_name);
			    ret.events_hdr.push(bind_hdr);
			    bIsEvent = true;
			}
		    }
		    
		    if (jn.widget && jn.widget.length > 0) 
		    {  
			json(jn.widget);  
		    }  
		}  
	    }  
	}
	
	json(jsontree);
	return ret;
    }
    
    var parser = new xml2js.Parser();
    parser.addListener('end', function(result) {
	//var json_content = [];
	
	var json_content = [];
	
	//var ret = json_wrap(result.kul_pkg.widget);
	//fs.writeFileSync(json_fname, JSON.stringify(ret));
	
	if(result.kul_pkg.blockid != undefined)
	{
	    for(var loop = 0; loop < result.kul_pkg.blockid.length; ++loop)
	    {
		var pkgid_obj = result.kul_pkg.blockid[loop];
		var pkg_name = pkgid_obj.$.name;
		var pkg_kulfile = pkgid_obj.location[0];
		
		pkg_kulfile = "y:/" + unit_name + "/kul/" + pkg_kulfile;
		
		if(!fs.existsSync(pkg_kulfile))
		{
			continue;
		}
		
		json_content.push({"name" : pkg_name, "kul_filepath" : pkg_kulfile, "is_block" : true, "id" : ""});
	    }
	}
	
	var all_kulfiles = [];
	
	for(var loop = 0; loop < result.kul_pkg.pkgid.length; ++loop)
	{
		var pkg_kulfile_ = result.kul_pkg.pkgid[loop].location[0];
		var pkg_name = result.kul_pkg.pkgid[loop].$.name;
		pkg_kulfile_ = "y:/" + unit_name + "/kul/" + pkg_kulfile_;
		all_kulfiles.push({'pkg_name' : pkg_name, 'pkg_kulfile' : pkg_kulfile_});
		
		//console.log(pkg_kulfile_);
	    if(fs.existsSync(pkg_kulfile_))
	    {
			var xsd_cmd = process.env["COMX_SDK"] + '/bin/xsd.exe';
			var cp = require('child_process');
			var xsd_parameters = [];
			xsd_parameters.push('-i');
			xsd_parameters.push(pkg_kulfile_);
			xsd_parameters.push('-p');
			xsd_parameters.push(loop + 1);

			var ls = cp.spawn(xsd_cmd, xsd_parameters, {});
			/*ls.stdout.on('data', function(data){

				console.log('stdout: ' + data);

			});

			ls.stderr.on('data', function(data){

				console.log('stderr: ' + data);

			});*/

			ls.on('exit', function (code) {
				if(code != 0)
				{
					var pkgid_obj = result.kul_pkg.pkgid[code - 1];
					var pkg_name = pkgid_obj.$.name;
					var pkg_kulfile = pkgid_obj.location[0];
					
					pkg_kulfile = "y:/" + unit_name + "/kul/" + pkg_kulfile;

					var parser_sub = new xml2js.Parser();
					
					parser_sub.addListener('end', function(result_sub_){
						var result_sub = result_sub_.all.content[0];
						var pkg_name = result_sub_.all.pkg_name[0];
						var pkg_kulfile = result_sub_.all.pkg_kulfile[0];
					
						//fs.writeFileSync("y:\\abc.json", JSON.stringify(result_sub));
						if(result_sub.kul_pkg[0].widget != undefined)
						{
							var js_filepath = result_sub.kul_pkg[0].widget[0].$.name + ".js";
							var id = "";
							//console.log(js_filepath);
							for(var loop_sub = 0; loop_sub < result_sub.kul_pkg[0].widget[0].property.length; ++loop_sub)
							{
							if(result_sub.kul_pkg[0].widget[0].property[loop_sub].$.name == "javascript")
							{
								js_filepath = result_sub.kul_pkg[0].widget[0].property[loop_sub]._;
								//console.log(js_filepath);
							}
							if(result_sub.kul_pkg[0].widget[0].property[loop_sub].$.name == "id")
							{
								id = result_sub.kul_pkg[0].widget[0].property[loop_sub]._;
								//console.log(js_filepath);
							}
							}
							
							js_filepath = "y:/" + unit_name + "/js/"  + js_filepath;
							
							//console.log(pkg_kulfile);
						
							json_content.push({"name" : pkg_name, "kul_filepath" : pkg_kulfile, "js_filepath" : js_filepath, "is_block" : false, "id" : id});
							g_kul_cnt++;
							
						}
						if(g_kul_cnt == result.kul_pkg.pkgid.length)
						{
							for(var loop = 0; loop < all_kulfiles.length; ++loop)
							{
								var pkg_kulfile = all_kulfiles[loop].pkg_kulfile;
								var is_find = false;
								for(var loop_s = 0; loop_s < json_content.length; ++loop_s)
								{
									if(pkg_kulfile == json_content[loop_s].kul_filepath)
									{
										is_find = true;
									}
								}
								if(!is_find)
								{
									//console.log(pkg_kulfile + ' not found');
									json_content.push({"name" : all_kulfiles[loop].pkg_name, "kul_filepath" : pkg_kulfile, "js_filepath" : false, "is_block" : false, "id" : false});
								}
							}
							data.parameters = {'unit_name' : unit_name, 'json_content' : json_content};
							websocket.send(data);
							g_kul_cnt_block = false;
						}
					});
					var parse_content = fs.readFileSync(pkg_kulfile);
					var parse_all = "<all><pkg_name>" + pkg_name + "</pkg_name>" + 
					                "<pkg_kulfile>" + pkg_kulfile + "</pkg_kulfile>" +
									"<content>" + parse_content + "</content></all>";
					//console.log(parse_all);
					parser_sub.parseString(parse_all);
				}
				else
				{
					g_kul_cnt++;
					if(g_kul_cnt == result.kul_pkg.pkgid.length)
					{
						json_content.push({"name" : all_kulfiles[0].pkg_name, "kul_filepath" : all_kulfiles[0].pkg_kulfile, "js_filepath" : false, "is_block" : false, "id" : false});
						data.parameters = {'unit_name' : unit_name, 'json_content' : json_content};
						websocket.send(data);
						g_kul_cnt_block = false;
					}
				}
			});
	    }
		else
		{
			g_kul_cnt++;
			if(g_kul_cnt == result.kul_pkg.pkgid.length)
			{
				json_content.push({"name" : all_kulfiles[0].pkg_name, "kul_filepath" : all_kulfiles[0].pkg_kulfile, "js_filepath" : false, "is_block" : false, "id" : false});
				data.parameters = {'unit_name' : unit_name, 'json_content' : json_content};
				websocket.send(data);
				g_kul_cnt_block = false;
			}
		}
	}
    });
    
    if(fs.existsSync(unit_entry_filename))
    {
		fs.readFile(unit_entry_filename, function(err, data) {
			parser.parseString(data);
		});
    }
    else
    {
		data.parameters = {'unit_name' : unit_name, 'json_content' : []};
		websocket.send(data);
		g_kul_cnt_block = false;
    }
};

exports.GetKulBindingInfo = function(data){
    var kul_fname = data.parameters.kul_fname;

    function is_binding(prop_name)
    {
	if(prop_name == 'simple_bind' ||
	   prop_name == 'valid_bind' ||
	   prop_name == 'list_bind' ||
	   prop_name == 'index1_bind' ||
	   prop_name == 'range_bind' ||
	   prop_name == 'table_bind' ||
	   prop_name == 'index2_bind' ||
	   prop_name == 'enable_bind' ||
	   prop_name == 'visible_bind' ||
	   prop_name == 'handle_bind')
	{
	    return true;
	}
	
	return false;
    }
    
    function is_event(prop_name)
    {
	if(prop_name == 'onClick' ||
	   prop_name == 'onDoubleClick' ||
	   prop_name == 'onChange' ||
	   prop_name == 'onHotKey' ||
	   prop_name == 'onHeaderClick')
	{
	    return true;
	}
	
	return false;
    }

    function json_wrap(jsontree)
    {
	var ret = [];
	function json(jsontree) 
	{  
	    if ((typeof jsontree == 'object') && (jsontree.constructor== Object.prototype.constructor)) 
	    {  
		var arrey = [];  
		arrey.push(jsontree);  
	    }  
	    else arrey = jsontree;  
	    for (var i = 0; i < arrey.length; i++) 
	    {  
		var jn = arrey[i];  
		if(jn.property != undefined)
		{
		    for(var loop = 0; loop < jn.property.length; ++loop)
		    {
			var prop_name = jn.property[loop].$.name;
			var prop_value = jn.property[loop]._;
			
			if(is_binding(prop_name) && prop_value != undefined)
			{
			    ret.push(prop_value);
			}
		    }
		    
		    if (jn.widget && jn.widget.length > 0) 
		    {  
			json(jn.widget);  
		    }  
		}  
	    }  
	}

	function json_binding_block(jsontree) 
	{  
	    //console.log(JSON.stringify(jsontree));
	    //fs.writeFileSync("y:\\abc.json", JSON.stringify(jsontree));
	    if ((typeof jsontree == 'object') && (jsontree.constructor== Object.prototype.constructor)) 
	    {  
		var arrey = [];  
		arrey.push(jsontree);  
	    }  
	    else arrey = jsontree;  
	    for (var i = 0; i < arrey.length; i++) 
	    {  
		var jn = arrey[i];  
		//console.log(JSON.stringify(jn));
		if(jn.binding != undefined)
		{
		    for(var loop = 0; loop < jn.binding.length; ++loop)
		    {
			var prop_name = jn.binding[loop].$.name;
			var prop_value = jn.binding[loop]._;
			
			if(prop_value != undefined)
			{
			    ret.push(prop_value);
			}
		    }
		    
		    
		}  
		if (jn.widget && jn.widget.length > 0) 
		{  
		    json_binding_block(jn.widget);  
		}  
	    }  
	}
	
	json(jsontree);
	json_binding_block(jsontree);

	return ret;
    }
    
    var parser = new xml2js.Parser();
    parser.addListener('end', function(result) {
	var ret = json_wrap(result.kul_pkg.widget);
	if(ret.length != 0)
	{
	    data.parameters = ret;
	    websocket.send(data);
	}
	//console.log(json_content);
    });
    
	var xsd_cmd = process.env["COMX_SDK"] + '/bin/xsd.exe';
	var cp = require('child_process');
	var xsd_parameters = [];
	xsd_parameters.push('-i');
	xsd_parameters.push(kul_fname);

	var ls = cp.spawn(xsd_cmd, xsd_parameters, {});

	ls.on('exit', function (code) {
		if(code != 0)
		{
			fs.readFile(kul_fname, function(err, data) {
				parser.parseString(data);
			});
		}
		else
		{
			data.parameters = false;
			websocket.send(data);
		}
	});
};
