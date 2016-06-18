@echo off
color 71
title COMX Development Environment Ver5.0 @2015
set is_first=1
if exist "%USERPROFILE%\Desktop\KEditor.lnk" (set is_first=0) else (start /min first.bat)
@echo Starting COMX SDK...
if exist y:\ (subst y: /D)
pushd "%~dp0"
cd ..
set parent_folder="%cd%"
popd
if %parent_folder%=="%~d0\" (subst y: %parent_folder%) else (subst y: %parent_folder:~0,-1%")
set COMX_SDK=y:\comx_sdk\
set NODE_PATH=Y:\comx_sdk\nodejs\node_modules\
set path=%path%;y:\comx_sdk\bin;y:\comx_sdk\;y:\comx_sdk\nodejs
y:
cd \comx_sdk
if %is_first%==0 (call __.bat) else (call unit_reg.bat)
ver>data\os.ver.txt
if exist additional.bat (call additional.bat)
cls
@echo Notify: Use 'kide[k]' Command to Startup COMX Development Environment.
@echo         Use 'distribute[d]' Command to Distribute COMX Unit.
@echo         Use 'unit_reg[ur]' Command to Update COMX Units.
@echo         Use 'switch[s]' Command to Switch COMX Units.
@echo         Use 'view[v]' Command to Explorer Current Directory.
@echo         Use 'urun[u]' Command to Execute an Unit.
@echo         Use 'create_unit[cu]' Command to Create an Unit.
@echo --------------------------------------------------------------------------
@echo. 


