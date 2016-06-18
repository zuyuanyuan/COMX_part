////////////////////////////////////////////////////////////////////////
// implement PlayGif function

function PlayGif_(gif_fpath, uElapse, draw_cb)
{
    var nFrameNum = parseInt(widget.bitmap.ExtractGif(gif_fpath));

    var index1 = gif_fpath.lastIndexOf('/');
    var index2 = gif_fpath.lastIndexOf('\\');
    var index = (index1 > index2) ? index1 : index2;
    var gif_fname = gif_fpath.substr(index + 1);
    var iFrame = 1;

    return function()
    {
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\picture\\cache\\" + gif_fname + "." + iFrame + ".bmp";

	if(typeof(draw_cb) == 'function')
	    draw_cb(cache_dir);

	iFrame++;
	if(iFrame > nFrameNum)
	    iFrame = 1;
    };
}

function PlayGif(gif_fpath, uElapse, cb)
{
	var gif_fullpath = GetGifFullPath(gif_fpath);
	if(gif_fullpath)
	{
		return SetTimeout(uElapse, PlayGif_(gif_fullpath, uElapse, cb));
	}
	else
	{
		alert('' + gif_fpath + ' isn\'t exist!');
		return false;
	}
}

function PlayGifEx(gif_fpath, uElapse, cb)
{
	var gif_fullpath = (gif_fpath);
	if(gif_fullpath)
	{
		return SetTimeout(uElapse, PlayGif_(gif_fullpath, uElapse, cb));
	}
	else
	{
		alert('' + gif_fpath + ' isn\'t exist!');
		return false;
	}
}

function PlayGifRollBack_(gif_fpath, uElapse, draw_cb)
{
    var nFrameNum = parseInt(widget.bitmap.ExtractGif(gif_fpath));

    var index1 = gif_fpath.lastIndexOf('/');
    var index2 = gif_fpath.lastIndexOf('\\');
    var index = (index1 > index2) ? index1 : index2;
    var gif_fname = gif_fpath.substr(index + 1);
    var iFrame = 1;
    var bPlus = true;

    return function()
    {
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\picture\\cache\\" + gif_fname + "." + iFrame + ".bmp";

	if(typeof(draw_cb) == 'function')
	    draw_cb(cache_dir);

	if(bPlus)
	    iFrame++;
	else
	    iFrame--;

	if(iFrame > nFrameNum)
	{
	    iFrame = nFrameNum;
	    bPlus = false;
	}
	
	if(iFrame == 0)
	{
	    bPlus = true;
	    iFrame = 1;
	}
    };
}

function PlayGifRollBack(gif_fpath, uElapse, cb)
{
	var gif_fullpath = GetGifFullPath(gif_fpath);
	if(gif_fullpath)
	{
		return SetTimeout(uElapse, PlayGifRollBack_(gif_fullpath, uElapse, cb));
	}
	else
	{
		alert('' + gif_fpath + ' isn\'t exist!');
		return false;
	}
    
}

function StopGif(nID)
{
    KillTimeout(nID);
}

function GetNilBitmap()
{
    return comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\picture\\" + 'nil.bmp';
}

function GetGifFullPath(gif_fname)
{
    var fpath = comx.sys.GetEnvironmentVariables(unit.name.toUpperCase()) + "\\picture\\" + gif_fname;
    if(comx.sys.IsFileExist(fpath))
    {
	return fpath;
    }
    
    fpath = comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\picture\\" + gif_fname;
    if(comx.sys.IsFileExist(fpath))
    {
	return fpath;
    }

    return false;
}

exports.Play = PlayGif;
exports.PlayEx = PlayGifEx;
exports.PlayRollBack = PlayGifRollBack;
exports.NilBitmap = GetNilBitmap;
exports.GetFullPath = GetGifFullPath;
exports.Stop = StopGif;
