@echo off
@echo SyncCmd Service is Running...
for /f %%i in ('"%~dp0fsnewdriver.exe"') do set DRIVER=%%i
set str=%~dp0
set TARGET_DIR=%str:~0,-11%
subst %DRIVER% "%TARGET_DIR%"
set COMX_SDK=%DRIVER%\comx_sdk\
set NODE_PATH=%DRIVER%\comx_sdk\nodejs\node_modules\
%COMX_SDK%\bin\start_cmd_srv.bat 1976
subst /D %DRIVER%
exit