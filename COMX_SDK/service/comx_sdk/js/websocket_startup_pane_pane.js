
var gif = require('class/gif_animate.js');

//System Pre-define functions Start

var flag = false;

function OnInitializeData()
{
	var progress_timer = gif.Play('wait3.gif', 80, function(bmp_fpath){
			binding.prog_val = bmp_fpath;
	});

	SetTimeout(1000, function(nid_main){
		if(comx.shell.IsCurrentWebSocketServiceValid() == "true" && flag == false)
		{
			KillTimeout(nid_main);
			flag = true;
			comx.sys.RunJavaScriptCodes("websocket.ConnectToNodeJS_();", comx.gf.mainwindow_formid);
            //pane.Close();
            SetTimeout(1000, function(nid){
				KillTimeout(nid);
                gif.Stop(progress_timer);
				pane.Close();
			});
        }
	});
}

function OnInitializePost()
{

}

function OnCloseForm()
{
}

function sleep(n)
{
    var start=new Date().getTime();
    while(true) if(new Date().getTime()-start>n) break;
}

//System Pre-define functions End

