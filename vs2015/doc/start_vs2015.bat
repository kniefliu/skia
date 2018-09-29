@echo off
rem set buffer width and height
mode con: cols=160 lines=20000
rem
set depot_tools_path=C:\google\depot_tools
set python_path=C:\Python27
set PATH=%PATH%;%depot_tools_path%;%python_path%;
rem "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe"
cmd /k