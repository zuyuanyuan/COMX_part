// js_parser.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "js_parser.h"
#include "comx_js_ext.hxx"

#include <base/type_cast.hxx>
using KMAS::type::type_cast;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

static bool exit_flag = false;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

Persistent<ObjectTemplate> global;
Persistent<Context> context;

string g_js_ext_dir;

extern void LoadJSExtDll();

extern void COMX_KERNEL_GetExtTopNamespaceList(vector<string> &top_ns);
extern void COMX_KERNEL_GetExtSubNamespaceList(string top_ns, vector<string> &sub_ns);
extern void COMX_KERNEL_GetExtFunctionNameList(string top_ns, string sub_ns, vector<string> &func_list);
extern void COMX_KERNEL_GetExtEntryPtr(string top_ns, string sub_ns, string func_name, comx_js_ext_entry*&p_js_ext_entry);
extern void COMX_KERNEL_GetExtEntryPath(string top_ns, string sub_ns, string func_name, string &path);



Handle<Value> Exit(const Arguments &args)
{
	exit_flag = true;
	//cout << "dispose" << endl;
	
	return Undefined();
}

Handle<Function> g_callback_func;

Handle<Value> Callback(const Arguments &args)
{
    if (!g_callback_func->IsNull())
    {
        Handle<Object> globalObj = Context::GetCurrent()->Global();
        Handle<Value> largs[1];
        largs[0] = args[0];
        g_callback_func->Call(globalObj, 1, largs);
        //delete[] largs;
    }

    return Undefined();
}

Handle<Value> TestFun(const Arguments &args)
{
    Handle<String> v8_func_name = args.Callee()->GetName()->ToString();
    string str_func_name = *(String::Utf8Value(v8_func_name));

    string str_entry_address_key_of_func = str_func_name + "0xba590c050x08530x45c10x940x2b0xbe0xcc0x6c0x8d0xeb0x1b";

    Handle<Object> self = args.Holder();
    Handle<Value> v8_var_name = self->Get(String::New(str_entry_address_key_of_func.c_str()));

    string str_entry_func_address = *(String::Utf8Value(v8_var_name));
    int entry_func_address = type_cast<int>(str_entry_func_address);

    comx_js_ext_entry_fn_t  fn = reinterpret_cast<comx_js_ext_entry_fn_t>(entry_func_address);
    const char *res = NULL;

    Handle<Value> _pane = Context::GetCurrent()->Global()->Get(String::New("pane"));
    assert(!_pane.IsEmpty());

    /*Handle<Value> formid_v8 = Handle<Object>::Cast(_pane)->Get(String::New("formid"));
    string formid = *(String::AsciiValue(formid_v8));*/
	string formid = "0x1141069c-0x926c-0x4cd5-0xa0-0x5e-0x93-0x0b-0x63-0x2b-0x4f-0xe0";

    bool ret = false;

    int paras_num = args.Length();

    Handle<Object> globalObj = Context::GetCurrent()->Global();

    bool is_callback_lock = false;

    if (paras_num > 0)
    {
        int last_para_index = paras_num - 1;

        if (args[last_para_index]->IsFunction())
        {
            paras_num -= 1;

            Handle<Function> func = Handle<Function>::Cast(args[last_para_index]);

            is_callback_lock = true;
            globalObj->Set(String::New("__callback_c0b08f9ee5844f46bdbac03fefbecf82"), func);
        }
    }
    

    if (paras_num == 0)
    {
        ret = fn((char*)str_func_name.c_str(), 0, NULL, res, formid.c_str());
    }
    else
    {
        vector<string> vec_args;
        vector<char*> args_para;

        vec_args.resize(paras_num);

        args_para.resize(paras_num);
        for (int loop = 0; loop < paras_num; ++loop)
        {
            Handle<String> para = args[loop]->ToString();
            string str_para = *(String::Utf8Value(para));

            str_para = JS_FROM_UTF8(str_para);

            vec_args[loop] = (str_para);
            args_para[loop] = (char*)vec_args[loop].c_str();
        }

        ret = fn((char*)str_func_name.c_str(), paras_num, &args_para[0], res, formid.c_str());

        string strRes = JS_TO_UTF8(res);
        strcpy((char*)res, strRes.c_str());
    }

    if (is_callback_lock)
    {
        is_callback_lock = false;
        globalObj->Set(String::New("__callback_c0b08f9ee5844f46bdbac03fefbecf82"), Undefined());
    }

    if (ret)
    {
		if (res == "true")
		{
			return Boolean::New(true);
		}

		if (res == "false")
		{
			return Boolean::New(false);
		}

		if (res == "Undefined" || res == "undefined")
		{
			return Undefined();
		}

        return String::New(res);
    }
    else
    {
        string msg = res;
        info_message(msg);

        return Undefined();
    }
}

void RegisterGlobalFunction(Handle<ObjectTemplate> &global)
{
    LoadJSExtDll();

    HandleScope handle_scope;
    Handle<ObjectTemplate> _obj = ObjectTemplate::New();
	Handle<ObjectTemplate> _obj_process = ObjectTemplate::New();

    //开始处理JS扩展
	vector<string> top_name_list;
	COMX_KERNEL_GetExtTopNamespaceList(top_name_list);
	for (unsigned int tn_loop = 0; tn_loop < top_name_list.size(); ++tn_loop)
	{
		string top_name = top_name_list[tn_loop];
		vector<string> sub_name_list;
		COMX_KERNEL_GetExtSubNamespaceList(top_name, sub_name_list);

		Handle<ObjectTemplate> _obj_ext_top_ns = ObjectTemplate::New();

		for (unsigned int sn_loop = 0; sn_loop < sub_name_list.size(); ++sn_loop)
		{
			string sub_name = sub_name_list[sn_loop];
			vector<string> func_list;
		   COMX_KERNEL_GetExtFunctionNameList(top_name, sub_name, func_list);

		   Handle<ObjectTemplate> _obj_ext_ns = ObjectTemplate::New();

		   for (unsigned int fn_loop = 0; fn_loop < func_list.size(); ++fn_loop)
		   {
			   string func_name = func_list[fn_loop];

			   comx_js_ext_entry *p_js_ext_entry = NULL;
			   COMX_KERNEL_GetExtEntryPtr(top_name, sub_name, func_name, p_js_ext_entry);

			   if (p_js_ext_entry != NULL)
			   {
						int entry_address = reinterpret_cast<int>(p_js_ext_entry->entry_func);
						string str_entry_address_of_func = type_cast<string>(entry_address);
						string str_entry_address_key_of_func = func_name + "0xba590c050x08530x45c10x940x2b0xbe0xcc0x6c0x8d0xeb0x1b";

						_obj_ext_ns->Set(String::New(str_entry_address_key_of_func.c_str()), String::New(str_entry_address_of_func.c_str()));
						_obj_ext_ns->Set(String::New(func_name.c_str()), FunctionTemplate::New(TestFun, Handle<Value>(), Handle<Signature>()));
						
						//string dll_path = "";
						//COMX_KERNEL_GetExtEntryPath(top_name, sub_name , func_name, dll_path);

						const char *szComments = NULL;
						(*p_js_ext_entry->entry_func)(const_cast<char*>(func_name.c_str()),-1, NULL, szComments, "");

						string strComments = szComments;
						strComments = strComments.substr(strComments.find_first_of('=') + 1);
						strComments = strComments.substr(strComments.find_first_not_of(' '));
						strComments = "usage: " + strComments;

						string ns_final = top_name + "." + sub_name;
						//RegisterFunctionCommentsInNamespace(ns_final, func_name, strComments/*dll_path*/);
				}
		   }

		   if (top_name == "comx")
		   {
			   _obj->Set(String::New(sub_name.c_str()), _obj_ext_ns);
		   }
		   else if (top_name == "__gf")
		   {
			   global->Set(String::New(sub_name.c_str()), _obj_ext_ns);
		   }
		   else
		   {
			   _obj_ext_top_ns->Set(String::New(sub_name.c_str()), _obj_ext_ns);
		   }
		}

		if (top_name != "comx" || top_name != "__gf")
		{
			global->Set(String::New(top_name.c_str()), _obj_ext_top_ns);
		}
	}

	_obj_process->Set(String::New("exit"), FunctionTemplate::New(Exit, Handle<Value>(), Handle<Signature>()));
    
    global->Set(String::New("comx"), _obj);
	global->Set(String::New("process"), _obj_process);
}

bool is_init = false;

string trans_path_fmt(string path)
{
	for (string::size_type loop = 0; loop < path.size(); ++loop)
	{
		if (path[loop] == '\\')
		{
			path[loop] = '/';
		}
	}

	return path;
}

// This is an example of an exported function.
JS_PARSER_API bool Initialize(const char *js_ext_dir)
{
	if (!is_init)
	{
		
		HandleScope handle_scope;
		global = Persistent<ObjectTemplate>::New(ObjectTemplate::New());

		g_js_ext_dir = trans_path_fmt(js_ext_dir);

        string deps_dir = g_js_ext_dir + "deps\\";
        
        string path_env = getenv("path");
        path_env = deps_dir + ";" + path_env;
        SetEnvironmentVariable("path", path_env.c_str());

		RegisterGlobalFunction(global);

		context = Context::New(NULL, global, Handle<Value>());

		is_init = true;

		string strCmd = "var JS_HOME_DIR = ";
		strCmd += "'";
		strCmd += g_js_ext_dir;
		strCmd += "'";
		RunScript(strCmd.c_str());

		IncludeScript("socket.io.js");

		return true;
	}
    
	return false;
}

bool IsUTF8(const void* pBuffer, long size)
{
	bool IsUTF8 = true;
	unsigned char* start = (unsigned char*)pBuffer;
	unsigned char* end = (unsigned char*)pBuffer + size;
	while (start < end)
	{
		if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符
		{
			start++;
		}
		else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
		{
			IsUTF8 = false;
			break;
		}
		else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符
		{
			if (start >= end - 1) 
				break;
			if ((start[1] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 2;
		} 
		else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符
		{
			if (start >= end - 2) 
				break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 3;
		} 
		else
		{
			IsUTF8 = false;
			break;
		}
	}
	return IsUTF8;
}

bool HasBOM(const void *buf, const int buf_size)
{
	if (buf_size < 3)
	{
		return false;
	}

	unsigned char* start = (unsigned char*)buf;

	if (start[0] == 0xef && start[1] == 0xbb && start[2] == 0xbf)
	{
		return true;
	}

	return false;
}

JS_PARSER_API const char* RunScript(const char *szJsBuf)
{
	static string ret = "";

	ret = "nil";

	if (!is_init)
	{
		info_message("JS Environment don't been Initialize!");
		return ret.c_str();
	}

	string js_buf = szJsBuf;
	if (!IsUTF8(js_buf.c_str(), (long)js_buf.size()))
	{
		js_buf = JS_TO_UTF8(js_buf);
	}

	HandleScope handle_scope;

	Context::Scope context_scope(context);

	Handle<String> js_src = String::New(js_buf.c_str(), static_cast<int>(js_buf.length()));

	TryCatch catcher;
	Handle<Script> js_script = Script::Compile(js_src, NULL, NULL/*, Handle<String>()*/);
	if (catcher.HasCaught()) 
	{
		Handle<String> reason = catcher.Message()->Get();
		int line = catcher.Message()->GetLineNumber();
		Handle<String> line_content = catcher.Message()->GetSourceLine();

		string strReason = *(String::AsciiValue(reason));
		string strLineContent = *(String::AsciiValue(line_content));

		char szMsg[1024];
		sprintf(szMsg, "Line %d: %s <Error Reason: %s>.", line, strLineContent.c_str(), strReason.c_str());

		info_message(szMsg);

		return ret.c_str();
	}

	Handle<Value> res = js_script->Run();

	if (catcher.HasCaught()) 
	{
		Handle<String> reason = catcher.Message()->Get();
		int line = catcher.Message()->GetLineNumber();
		Handle<String> line_content = catcher.Message()->GetSourceLine();

		string strReason = *(String::AsciiValue(reason));
		string strLineContent = *(String::AsciiValue(line_content));

		char szMsg[1024];
		sprintf(szMsg, "Line %d: %s <Error Reason: %s>.", line, strLineContent.c_str(), strReason.c_str());

		info_message(szMsg);

		return ret.c_str();
	}

	ret = *(String::AsciiValue(res->ToString()));
    return ret.c_str();
}

#include <io.h>
bool is_file_exist(const string fpath)
{
	return _access(fpath.c_str(), 0) == 0;
}

JS_PARSER_API void IncludeScript(const char *szFilename)
{
	string js_file_codes = "";

	string strFilePath = g_js_ext_dir + "/" + szFilename;

	if (!is_file_exist(strFilePath))
	{
		strFilePath = g_js_ext_dir + "/js/" + szFilename;
	}

	if (!is_file_exist(strFilePath))
	{
		return;
	}

	ifstream ifs(strFilePath.c_str());
	if (ifs)
	{
		js_file_codes += string(istreambuf_iterator<char>(ifs.rdbuf()), istreambuf_iterator<char>());
		js_file_codes += string("\n");
		RunScript(js_file_codes.c_str());
	}
}

JS_PARSER_API bool Dispose(void)
{
	if (is_init)
	{
		RunScript("if(typeof(OnProcessExit) == 'function')OnProcessExit();");
		//Sleep(1000);
		global.Dispose();
		context.Dispose();

		return true;
	}

    return false;
}

BOOL CtrlHandler( DWORD fdwCtrlType) 
{ 
	switch( fdwCtrlType) 
	{ 
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT :
		//printf("Ctrl-C event\n\n" );
		Dispose();
		exit(0);
		return(TRUE );

		// CTRL-CLOSE: confirm that the user wants to exit.
	default:
		return FALSE;
	} 
} 

#define INVOKE_WS_MSG_CMD "if(typeof(comx.iosocket.InvokeWebsocketMessage) == 'function'){	comx.iosocket.InvokeWebsocketMessage();}"

JS_PARSER_API bool ConsoleRunLoop(void)
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler,TRUE);
	MSG   msg;   
	while(GetMessage(&msg,NULL,0,0))   
	{   
		if(msg.message==WM_TIMER)   
		{   
			DispatchMessage(&msg);   
		}  
		if (exit_flag)
		{
			Dispose();
			break;
		}
		RunScript(INVOKE_WS_MSG_CMD);
	}  
	return true;
}

string GetUnitJSExtDirectory()
{
    return g_js_ext_dir + "/js.ext/";
}