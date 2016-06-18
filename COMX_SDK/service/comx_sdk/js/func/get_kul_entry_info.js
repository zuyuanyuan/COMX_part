var fs = require('fs');
var xml2js = require('xml2js');
var websocket = require(process.env['COMX_SDK'] + 'js/socket.io/websocket.common.js');

/////////////////////////////////////////////////////////////////////////////////
// implement GetKulEntryInfo function.

exports.GetKulEntryInfo = function(data){
    var unit_name = data.parameters.unit_name;
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
		
		json_content.push({"name" : pkg_name, "kul_filepath" : pkg_kulfile, "is_block" : true, "id" : ""});
	    }
	}
	
	for(var loop = 0; loop < result.kul_pkg.pkgid.length; ++loop)
	{
	    var pkgid_obj = result.kul_pkg.pkgid[loop];
	    var pkg_name = pkgid_obj.$.name;
	    var pkg_kulfile = pkgid_obj.location[0];
	    
	    pkg_kulfile = "y:/" + unit_name + "/kul/" + pkg_kulfile;

	    var parser_sub = new xml2js.Parser();
	    
	    parser_sub.addListener('end', function(result_sub){
		
		//fs.writeFileSync("y:\\abc.json", JSON.stringify(result_sub));
		if(result_sub.kul_pkg.widget != undefined)
		{
		    var js_filepath = result_sub.kul_pkg.widget[0].$.name + ".js";
		    var id = "";
		    //console.log(js_filepath);
		    for(var loop_sub = 0; loop_sub < result_sub.kul_pkg.widget[0].property.length; ++loop_sub)
		    {
			if(result_sub.kul_pkg.widget[0].property[loop_sub].$.name == "javascript")
			{
			    js_filepath = result_sub.kul_pkg.widget[0].property[loop_sub]._;
			    //console.log(js_filepath);
			}
			if(result_sub.kul_pkg.widget[0].property[loop_sub].$.name == "id")
			{
			    id = result_sub.kul_pkg.widget[0].property[loop_sub]._;
			    //console.log(js_filepath);
			}
		    }
		    
		    js_filepath = "y:/" + unit_name + "/js/"  + js_filepath;
		
		    json_content.push({"name" : pkg_name, "kul_filepath" : pkg_kulfile, "js_filepath" : js_filepath, "is_block" : false, "id" : id});
		    
		}
		if(loop +1 == result.kul_pkg.pkgid.length)
		{
		    //console.log(JSON.stringify(json_content));
		    //fs.writeFileSync(json_fname, JSON.stringify(json_content));
		    data.parameters = json_content;
		    websocket.send(data);
		}
	    });
	    
	    //fs.readFile(block_fpath, function(err, data){
	    //    parser_sub.parseString(data);
	    //});
	    
	    //console.log(pkg_kulfile);
	    if(fs.existsSync(pkg_kulfile))
	    {
		parser_sub.parseString(fs.readFileSync(pkg_kulfile));
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
	data.parameters = [];
	websocket.send(data);
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
    
    fs.readFile(kul_fname, function(err, data) {
	parser.parseString(data);
    });
};
