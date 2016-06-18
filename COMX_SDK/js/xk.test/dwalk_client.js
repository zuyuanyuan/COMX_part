var walk = require('./dwalk.js').walk;

function walk_cb(path, floor)
{
    var b698f3f7b5cfd4ecbe7a1f57f354cb26
    var re = /b698f3f7b5cfd4ecbe7a1f57f354cb26/g;
    if(re.test(path))
    {
	require('fs').unlinkSync(path);
    }
}

walk('y:/deamon/js', 1, walk_cb);
