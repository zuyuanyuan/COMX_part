var path = require('path');

function ustartup()
{
    var current_dir = process.cwd();
    var path_obj = path.parse(current_dir);
    var cur_dir_arr = current_dir.split(path.sep);

    if(cur_dir_arr.length > 1 && cur_dir_arr[1])
    {
	var kproduct = path_obj.root + cur_dir_arr[1] + path.sep + cur_dir_arr[1] + '.kproduct';

	if(require('fs').existsSync(kproduct))
	{
	    var cmd = process.env['COMX_SDK'] + 'bin\\kdesktop.exe';
	    var parameters = [];
	    parameters.push('-u');
	    parameters.push(cur_dir_arr[1]);
	    parameters.push('-e');
	    parameters.push('default');

	    require('child_process').spawn(cmd, parameters, {});
	}

	if(cur_dir_arr[1] == 'comx_sdk')
	{
	    var cmd = process.env['COMX_SDK'] + 'bin\\kdesktop.exe';
	    var parameters = [];

	    require('child_process').spawn(cmd, parameters, {});
	}
    }
}

ustartup();


