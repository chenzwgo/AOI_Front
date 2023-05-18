@echo off
set current_path="%cd%"
del /f/s/q *.log
del /f/s/q *.vcxproj
del /f/s/q *.vcxproj.filters
del /f/s/q *.vcxproj.user
del /f/s/q Makefile
del /f/s/q Makefile.*
del /f/s/q *.sdf
del /f/s/q *.sln
del /f/s/q *.exp
del /f/s/q *.obj
del /f/s/q *.tlog
del /f/s/q *.pdb
del /f/s/q *.lastbuildstate
del /f/s/q *.unsuccessfulbuild
del /f/s/q *.manifest
del /f/s/q *.user
del /f/s/q qrc_*.cpp
del /f/s/q moc_*.cpp
del /f/s/q moc_*.h
del /f/s/q ui_*.h
del /f/s/q *.ilk
del /f/s/q *.ipch
del /f/s/q *.suo
del /f/s/q *.moc
del /f/s/q *.o
del /f/s/q *.cache
del /f/s/q *.res
del /f/s/q *.VC.db
del /f/s/q *.stash
del /f/s/q .vs
del /f/s/q *.cbt

for /f "delims=" %%i in ('dir /ad /b /s') do call :rd_dir_empty "%%i"
if exist "%cd%/.svn" mkdir "%cd%/.svn/tmp"
goto :eof

rem ======= 删除空目录 =========
:rd_dir_empty
rd %1 2>nul||goto :eof

set dir_route=%1
for /f "delims=" %%i in (%dir_route%) do (
    set dir_route="%%~dpi"
    for /f "delims=" %%j in ('dir /ad /b "%%~dpi"')do rd "%%~dpi%%j" 2>nul||goto :eof
)
:: 把路径最后的\去掉，以便set route_deepest="%%~dpi"能取到上一层路径
if "%dir_route:~-2,1%"=="\" set dir_route="%dir_route:~1,-2%"
if /i not "%cd%"==%dir_route% call :rd_dir_empty %dir_route%
goto :eof
