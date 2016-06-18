// solver_srv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>

#include <iostream>
using namespace std;

#include <base/type_cast.hxx>
using namespace KMAS::type;

inline void start_srv(int port);


int _tmain(int argc, _TCHAR* argv[])
{
    if (argc == 2)
    {
        int port = type_cast<int>(argv[1]);
        start_srv(port);

        return 0;
    }
    else
    {
        cout << "Usage: solver_srv.exe port\r\n---------------------------------------------------------\r\nNotify: js/shadow_cli.js will be searched and loaded" << endl;

        return 1;
    }
}

inline string get_module_file_path()
{
    CHAR szMoudlePath[MAX_PATH];
    DWORD dwCurDirPathLen = GetModuleFileName(GetModuleHandle(NULL), szMoudlePath, MAX_PATH);

    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    _splitpath_s( szMoudlePath, drive, dir, fname, ext );

    return string(drive) + dir;
}

inline void start_srv(int port)
{
    Initialize(get_module_file_path().c_str());

    char szPortCmd[256];
    sprintf_s(szPortCmd, 256, "var g_mainfrm_port = %d", port);
    RunScript(szPortCmd);

    ExecuteScriptFile("js/shadow_cli.js");

    ConsoleRunLoop();
}