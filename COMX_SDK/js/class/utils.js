function GetExtFilename(fpath)
{
    var fpath_arr = fpath.split('.');
    var ext = fpath_arr[fpath_arr.length - 1];
    
    return ext.toLowerCase();
}

exports.IsExtFileName = function(fpath, ext_name)
{
    return (GetExtFilename(fpath) == ext_name);
};
