#ifndef COMX_COMX_JS_EXT_5b3ab22b483b4c23922724cba9e4fb47_HXX
#define COMX_COMX_JS_EXT_5b3ab22b483b4c23922724cba9e4fb47_HXX

#ifndef COMX_KERNEL_COMX_EXPORT_81c958c386154af495a84ae63135623e_HXX
#define COMX_KERNEL_COMX_EXPORT_81c958c386154af495a84ae63135623e_HXX

#ifdef LIBCOMXKERNEL_EXPORTS
#define LIBCOMXKERNEL_API __declspec(dllexport)
#else
#define LIBCOMXKERNEL_API __declspec(dllimport)
#endif

#endif

#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

#define KUL_PANE_RUN_JS_MSG (WM_USER + 512)

typedef bool (*comx_js_ext_entry_fn_t)(char *func_name, int argc, char **argv, const char*& res, const char* formid);

struct comx_js_ext_entry
{
	char **func_list;
	int func_num;
	char *ns;
	comx_js_ext_entry_fn_t entry_func;
};

extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RegisterExtEntry(comx_js_ext_entry *&entry, char *dll_path);
extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RunJS(const char* js_codes, char *ret, const char* formid);
extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RunAsyncJS(const char* js_codes, const char *formid);
extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RunJS_Ex(const char *js_file,  const char* js_lib, const char* js_codes, char *ret, const char *formid);

#define JS_EXT_MAIN_BEGIN(sub_ns,num) \
	BOOL APIENTRY DllMain( HANDLE hModule, \
	DWORD  ul_reason_for_call, \
	LPVOID lpReserved\
	)\
{\
	if (DLL_PROCESS_ATTACH == ul_reason_for_call)\
{\
	S_entry.entry_func = comx_entry_ext_main;\
	S_namespace = sub_ns;\
	S_entry.ns = (char*)S_namespace.c_str();\
	S_func_list.resize(num);\
	S_entry.func_list = &S_func_list[0];\
	S_entry.func_num = num;\
	int cnt = 0;

#define JS_EXT_FUNC_REG(name) S_func_list[cnt++] = #name;

#define JS_EXT_MAIN_END() \
	static comx_js_ext_entry *entry = NULL;\
	entry = &S_entry;\
	\
	char szModuleFileName[1024] = "";\
	GetModuleFileName((HMODULE)hModule, szModuleFileName, 4096);\
	\
	COMX_KERNEL_RegisterExtEntry(entry, szModuleFileName);\
}\
	return TRUE;\
}

#define JS_EXT_ENTRY_BEGIN() bool comx_entry_ext_main(char *func_name, int argc, char **argv, const char *&res, const char *formid){S_formid = formid;

#define JS_EXT_ENTRY(func) \
	if (strcmp(func_name, #func) == 0)\
{\
	return func(argc, argv, res);\
}

#define JS_EXT_ENTRY_END() \
	S_ret = "function isn't exist";\
	res = S_ret.c_str();\
	\
	return false;\
}

typedef void (*ptr_fn_gl_cache_entry_t)(double *vertexs, int *vertexs_prop, int vertex_num, 
										double *triangles, double *triangle_normals, int *triangles_prop, int triangle_num, 
										double *lines, int *lines_prop, int line_num,
										const char *comment,
										int display_mode,
										int *color,
										int mid,
										int sid,
										bool is_show);

#define JS_EXT_DATA_DECLARE() \
string S_formid;\
string S_ret;\
comx_js_ext_entry S_entry;\
vector<char*> S_func_list;\
string S_namespace;\
inline void JS_EXT_GL_CACHE_ENTRY(double *vertexs, int *vertexs_prop, int vertex_num, \
    double *triangles, double *triangle_normals, int *triangles_prop, int triangle_num, \
    double *lines, int *lines_prop, int line_num,\
    const char *comment,\
    int display_mode,\
    int *color,\
    int mid,\
    int sid,\
    bool is_show)\
{\
	char szEntry[256] = "";\
	COMX_KERNEL_RunJS("gl.cache.Entry()", szEntry, JS_EXT_FORMID);\
\
	unsigned long ulEntry = type_cast<unsigned long>(szEntry);\
	ptr_fn_gl_cache_entry_t ptrFnEntry = (ptr_fn_gl_cache_entry_t)ulEntry;\
\
	ptrFnEntry(vertexs, vertexs_prop, vertex_num, \
		triangles, triangle_normals, triangles_prop, triangle_num, \
		lines, lines_prop, line_num,\
		comment,\
		display_mode,\
		color,\
		mid,\
		sid,\
		is_show);\
}

inline string JS_EXT_FixedFileExtName(string fname, string ext_fnames[], int len)
{
    string::size_type idx = fname.find_last_of('.');

    if(std::find(ext_fnames, ext_fnames + len, fname.substr(idx + 1)) == ext_fnames + len)
    {
        fname += ".";
        fname += ext_fnames[0];
    }

    return fname;
}

inline string JS_EXT_FixedFileExtName(string fname, string ext_fname)
{
    string::size_type idx = fname.find_last_of('.');

    if(fname.substr(idx + 1) != ext_fname)
    {
        fname += ".";
        fname += ext_fname;
    }

    return fname;
}

template<typename Reader, typename Value>
inline bool JS_EXT_LOAD_JSON(string json_fpath, Reader &reader, Value &root)
{
    bool ret = true;

    bool bExist = true;
    ifstream ifos(json_fpath.c_str());
    if (!ifos) 
    {
        bExist = false;
    }

    if (bExist)
    {
        ifstream ifs;

        ifs.open(json_fpath.c_str());

        if (!reader.parse(ifs, root, false))
        {
            ret = false;
        }
    }

    return ret;
}


#define JS_EXT_FUNC_BEGIN(func_name, para_num, usage) \
	bool func_name(int argc, char** argv, const char*&res)\
{\
	bool ret = false;\
	if (argc != para_num)\
{\
	S_ret = usage;\
}\
	else\
{

#define JS_EXT_FUNC_ASSIGN_RET(val) S_ret = (val);

#define JS_EXT_FUNC_END() \
	ret = true;\
}\
	\
	res = S_ret.c_str();\
	\
	return ret;\
}

#define  JS_EXT_PARA(type_inf, para_index) \
	type_cast<type_inf>(string(argv[para_index]))

#define JS_EXT_FORMID (S_formid.c_str())

inline bool COMX_KERNEL_IsCallbackValid(string formid)
{
    char szRet[4096] = "";
    string szTestCmd = "typeof(__callback_c0b08f9ee5844f46bdbac03fefbecf82)";
    COMX_KERNEL_RunJS(szTestCmd.c_str(), szRet, formid.c_str());

    return (string(szRet) == "function");
}

inline string COMX_KERNEL_RunCallback(string formid, int n, ...)
{  
    va_list arg_ptr;

    string parameters = "";

    va_start(arg_ptr, n); 

    for (int loop = 0; loop < n; ++loop)
    {
        string para = va_arg(arg_ptr, char*);

        if (loop != 0)
        {
            para += ",";
        }

        para = "'" + para + "'";

        parameters += para;
    }
    va_end(arg_ptr); 

    string cmd = "if (typeof(__callback_c0b08f9ee5844f46bdbac03fefbecf82) == 'function')\n{\n    __callback_c0b08f9ee5844f46bdbac03fefbecf82(" + parameters + ");\n}";
    char szRet[4096] = "";
    
    COMX_KERNEL_RunJS(cmd.c_str(), szRet, formid.c_str());

    return szRet;
}

#endif