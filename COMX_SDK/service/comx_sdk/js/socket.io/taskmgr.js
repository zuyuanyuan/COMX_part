var socketio = require("socket.io");
var fs = require('fs');

function GenerateGUID()
{
    var guid = "";
    for (var i = 1; i <= 32; i++){
        var n = Math.floor(Math.random()*16.0).toString(16);
        guid +=   n;
    }
    return guid;
}

var rmdirSync = (function(){
    function iterator(url,dirs){
        var stat
	    = fs.statSync(url);
        if(stat.isDirectory()){
	    dirs.unshift(url);//�ռ�Ŀ¼
	    inner(url,dirs);
            }else if(stat.isFile()){
		fs.unlinkSync(url);//ֱ��ɾ���ļ�
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
                fs.rmdirSync(el);//һ����ɾ�������ռ�����Ŀ¼
	    }
	    cb()
        }catch(e){//�����ļ���Ŀ¼�����Ͳ����ڣ�fs.statSync�ᱨ���������ǻ��ǵ���û���쳣����
	    e.code
		=== "ENOENT" ?
		cb() : cb(e);
        }
    }
})();

var websocket = false;
var g_cache_dir = false;
exports.Init = function(ws, cache_dir)
{
    websocket = ws;
    g_cache_dir = cache_dir;

    exports.Load();
    websocket.on('GetTaskList', function(data){
	data.parameters = task_list;
	websocket.send(data);
    });
    websocket.on('GetTaskResult', function(data){
	var task_id = data.parameters.task_id;
	var json_fname = cache_dir + task_id + '.json';

	console.log(json_fname);

	if(fs.existsSync(json_fname))
	{
	    data.parameters = JSON.parse(fs.readFileSync(json_fname));
	    //websocket.send(data);
	    websocket.Invoke('OnLoadTaskResult', {'task_item' : task_list[task_id]/*, 'result' : data.parameters*/});
	}
    });

  websocket.on('ClearCache', function(data){
    task_list = {};
    rmdirSync(g_cache_dir, function(){
	fs.mkdirSync(g_cache_dir);
    });
  });
}

var task_list = {};
exports.Create = function(name, data, callback, repeated_task_cb)
{
    var task_id = 'id_' + GenerateGUID();
    var task_item = {'name' : name, 'parameters' : data, 'callback' : callback, 'status' : 0, 'id' : task_id, 'cueline' : 'Prepairng...'};

    if(typeof(repeated_task_cb) == 'function')
    {
	var old_task_item = false;
	if((old_task_item = repeated_task_cb(task_list, task_item)) != false)
	{
	    task_id = old_task_item.id;
      old_task_item.parameters.criteria = task_item.parameters.criteria;
	    task_item = old_task_item;

	    if(task_item.status == 0)
	    {
		return task_id;
	    }
	    else
	    {
		task_item.status = 0;
	    }
      delete task_list[task_id];
      task_list[task_id] = task_item;
	}
	else
	{
	    task_list[task_id] = task_item;
	}
    }
    else
    {
	task_list[task_id] = task_item;
    }

    var active_task_num = 0;
    for(var task_id in task_list)
    {
	if(task_list[task_id].status == 0)
	{
	    active_task_num++;
	}
    }

    task_list[task_id].cueline = 'Prepairing...';
    websocket.Invoke('OnTaskListModified', {'task_list' : task_list , 'active_task_num' : active_task_num});
    callback(task_item);

    return task_id;
}

exports.UpdateCueline = function(task_id, cueline)
{
    task_list[task_id].cueline = cueline;
    websocket.Invoke('OnUpdateTaskCueline', {'task_id' : task_id, 'cueline' : cueline});
}

exports.SetDoneFlag = function(task_id)
{
    task_list[task_id].status = 1;

    var active_task_num = 0;
    for(var task_id in task_list)
    {
	if(task_list[task_id].status == 0)
	{
	    active_task_num++;
	}
    }

    websocket.Invoke('OnTaskDone', {'task_id' : task_id, 'active_task_num' : active_task_num});

    if(active_task_num == 0 && typeof(task_end_cb) == 'function')
      {
        task_end_cb();
      }
}

function isEmpty(obj)
{
    for (var name in obj)
    {
        return false;
    }
    return true;
};

exports.Save = function(){
    var cache_json = g_cache_dir + 'task_list.json';
    if(!isEmpty(task_list))
    {
	    fs.writeFileSync(cache_json, JSON.stringify(task_list));
    }
};

exports.Load = function(){
    var cache_json = g_cache_dir + 'task_list.json';
    if(fs.existsSync(cache_json))
    {
	task_list = JSON.parse(fs.readFileSync(cache_json));
	websocket.Invoke('OnTaskLoadFromCache', {'task_list' : task_list});
    }
};

var task_end_cb = false;
exports.Exit = function(cb){
  task_end_cb = cb;
  var active_task_num = 0;
  for(var task_id in task_list)
    {
      if(task_list[task_id].status == 0)
      {
          active_task_num++;
      }
    }

  if(active_task_num == 0 && typeof(task_end_cb) == 'function')
    {
      task_end_cb();
    }
};
