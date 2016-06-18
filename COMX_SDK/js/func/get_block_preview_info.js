var fs = require('fs');
var xml2js = require('xml2js');
var websocket = require(process.env['COMX_SDK'] + 'js/socket.io/websocket.common.js');

/////////////////////////////////////////////////////////////////////////////////
// implement GetBlockPreviewInfo function.

exports.GetBlockPreviewInfo = (function(data){
    var kul_filename = "y:\\comx_sdk\\kul\\kul_editor_block_preview.kul";

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
	   prop_name == 'visible_bind')
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
	   prop_name == 'onHeaderClick' ||
	   prop_name == 'onSwitch')
	{
	    return true;
	}
	
	return false;
    }
    
    
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
	var json_content = [];
	
	var ret = json_wrap(result.kul_pkg.widget);

	data.parameters = ret;
	websocket.send(data);
	//fs.writeFileSync(json_fname, JSON.stringify(ret));
    });
    
    fs.readFile(kul_filename, function(err, data) {
	parser.parseString(data);
    });
});
