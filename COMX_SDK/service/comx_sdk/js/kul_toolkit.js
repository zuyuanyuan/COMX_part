function OnOpen()
{
   if(binding.tabctrl_index == 0)
      OnOpenKUL();

   if(binding.tabctrl_index == 1)
      OnOpenJS();
}

var g_js_filename = "";
var g_kul_filename = "";

function OnOpenJS()
{
   var filter = new Array();
   filter[0] = "Javascript Files(*.js)";
   filter[1] = "All Files(*.*)";

   g_js_filename = comx.ui.CallOpenFileDialog(filter);

   if(g_js_filename == "")
      return;

   comx.sys.CreateFileHandler("js_hdr", g_js_filename);
   binding.js_doc = comx.file.js_hdr;
   comx.sys.CloseFileHandler("js_hdr");
}

function OnOpenKUL()
{
   var filter = new Array();
   filter[0] = "KUL Files(*.kul)";
   filter[1] = "All Files(*.*)";

   g_kul_filename = comx.ui.CallOpenFileDialog(filter);

   if(g_kul_filename == "")
      return;

   comx.sys.CreateFileHandler("kul_hdr", g_kul_filename);
   binding.kul_doc = comx.file.kul_hdr;
   comx.sys.CloseFileHandler("kul_hdr");
}