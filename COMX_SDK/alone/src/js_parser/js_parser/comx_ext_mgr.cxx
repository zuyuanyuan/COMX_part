#pragma warning(disable:4503)

#include "stdafx.h"

#include <base/type_cast.hxx>
using KMAS::type::type_cast;

#include "comx_js_ext.hxx"

#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

//static map<string, comx_js_ext_entry*> _entry_map;
//static map<string, string>                   _entry_path_map;

typedef map<string, map<string, comx_js_ext_entry*> > entry_map_value_t;
map<string, entry_map_value_t> _entry_map;
map<string, map<string, map<string, string> > > _entry_path_map;

//extern HWND get_hwnd_from_formid(const char *formid);
//extern void info_message(const string &info);

extern string g_template_js_file = "";
extern string g_template_js_lib = "";

extern bool G_IsNamespaceExist(string top, string sub)
{
	if (_entry_map.find(top) != _entry_map.end())
	{
		if (_entry_map[top].find(sub) != _entry_map[top].end())
		{
			return true;
		}
	}

	return false;
}

extern const char* RunScript(const char *szJsBuf);

extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RunJS(const char* js_codes, char *ret, const char *formid)
{
    //ret = (char*)RunScript(js_codes);
	strcpy(ret, RunScript(js_codes));
	//HWND hPaneWnd = GetParentWindow();
	//
 //   if (NULL != hPaneWnd
 //   {
 //       g_template_js_file = "";
 //       ::SendMessage(hPaneWnd, KUL_PANE_RUN_JS_MSG, (WPARAM)js_codes, (LPARAM)ret);
 //   }
	//
	////::PostMessage(hPaneWnd, KUL_PANE_RUN_JS_MSG, (WPARAM)js_codes, (LPARAM)ret);

	return true;
}

extern void PushSocketIOCmdToQueue(string js_codes, string formid);

extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RunAsyncJS(const char* js_codes, const char *formid)
{
    RunScript(js_codes);
	//HWND hPaneWnd = get_hwnd_from_formid(formid);

	//g_template_js_file = "";
	////::SendMessage(hPaneWnd, KUL_PANE_RUN_JS_MSG, (WPARAM)js_codes, (LPARAM)0);
	//PushSocketIOCmdToQueue(js_codes, formid);
	////::PostMessage(hPaneWnd, KUL_PANE_RUN_JS_MSG, (WPARAM)js_codes, (LPARAM)ret);

	return true;
}

extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RunJS_Ex(const char *js_file, const char* js_lib, const char* js_codes, char *ret, const char *formid)
{
	/*HWND hPaneWnd = get_hwnd_from_formid(formid);

	g_template_js_file = js_file;
	g_template_js_lib = js_lib;
	::SendMessage(hPaneWnd, KUL_PANE_RUN_JS_MSG, (WPARAM)js_codes, (LPARAM)ret);*/


    //ret = (char*)RunScript(js_codes);

	strcpy(ret, RunScript(js_codes));

	return true;
}

extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RegisterExtEntry(comx_js_ext_entry *&pEntry, char *szDllPath)
{
	char szNamespace[256] = "";
	strcpy(szNamespace, pEntry->ns);
	//string ns = string(pEntry->ns);

	string ns = szNamespace;

	if (false/*ns == "comx.gf" || ns == "comx.file" || ns == "comx.entry" || ns == "comx.ui" || ns == "comx.sys" || ns == "comx.util" || ns == "comx.dev"*/)
	{
		char szMsg[1024] = "";
		sprintf(szMsg, "the namespace - \"%s\" is reserved by kernel, \nplease change the namespace definition \nin \"%s\"", ns.c_str(), szDllPath);

		info_message(szMsg);

		return false;
	}

	//命名空间分解
	if (ns.find('.') != string::npos)
	{
		ns[ns.find('.')] = ' ';
	}
	
	stringstream sstr(ns);
	string top_ns, sub_ns;
	sstr >> top_ns >> sub_ns;
	//命名空间分解完成

	for (int loop = 0; loop < pEntry->func_num; ++loop)
	{
		string func_name = pEntry->func_list[loop];

		if (_entry_map[top_ns][sub_ns].find(func_name) != _entry_map[top_ns][sub_ns].end())
		{
			char szMsg[1024] = "";
			sprintf(szMsg, "\"%s.%s.%s\" is redefined in \"%s\" and \"%s\"\n\"%s\" will be ignored", top_ns.c_str(), sub_ns.c_str(), func_name.c_str(), 
				_entry_path_map[top_ns][sub_ns][func_name].c_str(), szDllPath, szDllPath);

			info_message(szMsg);
			continue;
		}

		_entry_map[top_ns][sub_ns][func_name] = pEntry;
		_entry_path_map[top_ns][sub_ns][func_name] = szDllPath;
	}

	return true;
}

extern void COMX_KERNEL_GetExtTopNamespaceList(vector<string> &top_ns)
{
	top_ns.clear();

	for (map<string, map<string, map<string, comx_js_ext_entry*> > >::iterator iter = _entry_map.begin(); iter != _entry_map.end(); ++iter)
	{
		string ns = iter->first;
		top_ns.push_back(ns);
	}
}

extern void COMX_KERNEL_GetExtSubNamespaceList(string top_ns, vector<string> &sub_ns)
{
	sub_ns.clear();
	if (_entry_map.find(top_ns) != _entry_map.end())
	{
		for (map<string, map<string, comx_js_ext_entry*> >::iterator iter = _entry_map[top_ns].begin(); iter != _entry_map[top_ns].end(); ++iter)
		{
			string ns = iter->first;
			sub_ns.push_back(ns);
		}
	}
}

extern void COMX_KERNEL_GetExtFunctionNameList(string top_ns, string sub_ns, vector<string> &func_list)
{
	func_list.clear();

	if (_entry_map.find(top_ns) != _entry_map.end())
	{
		if (_entry_map[top_ns].find(sub_ns) != _entry_map[top_ns].end())
		{
			for(map<string, comx_js_ext_entry*>::iterator iter = _entry_map[top_ns][sub_ns].begin(); 
				iter != _entry_map[top_ns][sub_ns].end(); ++iter)
			{
				string func_name = iter->first;
				func_list.push_back(func_name);
			}
		}
	}
}

extern void COMX_KERNEL_GetExtEntryPtr(string top_ns, string sub_ns, string func_name, comx_js_ext_entry*&p_js_ext_entry)
{
	p_js_ext_entry = NULL;

	if (_entry_map.find(top_ns) != _entry_map.end())
	{
		if (_entry_map[top_ns].find(sub_ns) != _entry_map[top_ns].end())
		{
			if (_entry_map[top_ns][sub_ns].find(func_name) != _entry_map[top_ns][sub_ns].end())
			{
				p_js_ext_entry = _entry_map[top_ns][sub_ns][func_name];
			}
		}
	}
}

extern void COMX_KERNEL_GetExtEntryPath(string top_ns, string sub_ns, string func_name, string &path)
{
	path = "";
	if (_entry_map.find(top_ns) != _entry_map.end())
	{
		if (_entry_map[top_ns].find(sub_ns) != _entry_map[top_ns].end())
		{
			if (_entry_map[top_ns][sub_ns].find(func_name) != _entry_map[top_ns][sub_ns].end())
			{
				path = _entry_path_map[top_ns][sub_ns][func_name];
			}
		}
	}
}

extern string GetUnitJSExtDirectory();

void LoadJSExtDllByUnit(string unit_name)
{
#ifdef _M_X64
    #ifdef _DEBUG
        string path = GetUnitJSExtDirectory() + "*_d.64bit.js.ext";
    #else
        string path = GetUnitJSExtDirectory() + "*.64bit.js.ext";
    #endif
#else
    #ifdef _DEBUG
        string path = GetUnitJSExtDirectory() + "*_d.js.ext";
    #else
        string path = GetUnitJSExtDirectory() + "*.js.ext";
    #endif
#endif
	
	//MessageBox(NULL, path.c_str(), "", MB_OK);

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind=::FindFirstFile(path.c_str(),&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)    return;

	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			;
		}
		else
		{
			string filename = GetUnitJSExtDirectory() + FindFileData.cFileName;

			//MessageBox(NULL, filename.c_str(), "", MB_OK);

#ifndef _DEBUG
			if (filename.find("_d.64bit.js.ext") != string::npos)
			{
				if(!FindNextFile(hFind,&FindFileData))    break;
				continue;
			}
#endif
			LoadLibrary(filename.c_str());
		}
		if(!FindNextFile(hFind,&FindFileData))    break;
	}
	FindClose(hFind);
}

void LoadJSExtDll()
{
    LoadJSExtDllByUnit("");
}
