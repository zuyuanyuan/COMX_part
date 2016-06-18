// js_parser_shell.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <iostream>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
    CHAR szMoudlePath[MAX_PATH];
    DWORD dwCurDirPathLen = GetCurrentDirectory(MAX_PATH, szMoudlePath);

    cout << szMoudlePath << endl;

    string JS_HOME_DIR = getenv("COMX_SDK"); 
    JS_HOME_DIR += "alone\\";

	Initialize(JS_HOME_DIR.c_str());

    ExecuteScriptFile("sample/sample.js");

	//getchar();
	//Dispose();
	ConsoleRunLoop();
	return 0;
}

