// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the JS_PARSER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// JS_PARSER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef JS_PARSER_EXPORTS
#define JS_PARSER_API __declspec(dllexport)
#else
#define JS_PARSER_API __declspec(dllimport)
#endif

JS_PARSER_API bool Initialize(const char *js_ext_dir);
JS_PARSER_API bool Dispose(void);

JS_PARSER_API const char* RunScript(const char *szJsBuf);
JS_PARSER_API void IncludeScript(const char *szFilename);

#define ExecuteScriptFile IncludeScript

JS_PARSER_API bool ConsoleRunLoop(void);