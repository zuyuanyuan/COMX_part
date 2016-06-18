var fs = require('fs');
var xml2js = require('xml2js');
var websocket = require(process.env['COMX_SDK'] + 'js/socket.io/websocket.common.js');

/////////////////////////////////////////////////////////////////////////////////
// implement GetBlockInfo function.

exports.GetBlockInfo = (function(){
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
	var ret = {events : [], bindings : []};
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
			    ret.bindings.push(prop_value);
			}
			
			if(is_event(prop_name) && prop_value != undefined)
			{
			    ret.events.push(prop_value);
			}
		    }
		}
		
		if (jn.widget && jn.widget.length > 0) 
		{  
		    json(jn.widget);  
		}  
	    }  
	}  
	
	json(jsontree);
	return ret;
    }
    
    return function(data/*unit_name, method_id*/)
    {
	var unit_name = data.parameters.unit_name;
	var kul_folder = 'y:/' + unit_name + '/kul/';
	var kul_entry_xml = kul_folder + '/kul.entry.xml';
	
	var parser = new xml2js.Parser();
	parser.addListener('end', function(result){
	    var json_content = [];
	    
	    if(result.kul_pkg.blockid == undefined)
	    {
		return;
	    }
	    
	    for(var loop = 0; loop < result.kul_pkg.blockid.length; ++loop)
	    {
		var name = result.kul_pkg.blockid[loop].$.name;
		var location = result.kul_pkg.blockid[loop].location[0];
		var value = result.kul_pkg.blockid[loop].value[0];
		var block_fpath = kul_folder + location;

		var block_fpath_arr = block_fpath.split("");
		for(var index = 0; index < block_fpath_arr.length; ++index)
		{
		    if(block_fpath_arr[index] == "\\")
		    {
			block_fpath_arr[index] = "/";
		    }
		}
		
		block_fpath = block_fpath_arr.join("");
		
		var parser_sub = new xml2js.Parser();
		parser_sub.addListener('end', function(result_sub){
		    var ret = {events : [], bindings : []};
		    if(result_sub.kul_pkg.widget != undefined)
		    {
			ret = json_wrap(result_sub.kul_pkg.widget);
		    }
		    
		    
		    json_content.push({'name': name, 'id': value, 'location':block_fpath, 'mapping' : ret});
		    
		    if(loop +1 == result.kul_pkg.blockid.length)
		    {
			//console.log(JSON.stringify(json_content));
			//fs.writeFileSync(json_fname, JSON.stringify(json_content));
			//var json_cmd = {"method" : "SetBlockInfo", "method_id" : method_id, "parameters" : JSON.stringify(json_content)};
			//socketio.send(JSON.stringify(json_cmd));
			data.parameters = json_content;
			websocket.send(data);
		    }
		});
		
		//fs.readFile(block_fpath, function(err, data){
		//    parser_sub.parseString(data);
		//});
		if(fs.existsSync(block_fpath))
		{
		    parser_sub.parseString(fs.readFileSync(block_fpath));
		}
	    }
	});
	
	fs.readFile(kul_entry_xml, function(err, data) {
	    parser.parseString(data);
	
	});
    };
})();
