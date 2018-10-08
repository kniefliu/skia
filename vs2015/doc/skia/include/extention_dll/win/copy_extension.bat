@echo off
set current_path=%~dp0
echo %current_path%
copy "%current_path%..\..\..\src\extention_dll\win\GLContext_angle.h" "%current_path%" /y
copy "%current_path%..\..\..\src\utils\win\SkWGL.h" "%current_path%" /y
