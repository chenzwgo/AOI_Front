echo Setting up a Qt environment...

set QTDIR=D:\Qt\4.8.6
echo -- QTDIR set to D:\Qt\4.8.6
set PATH=%QTDIR%\bin;%PATH%
echo -- Added D:\Qt\4.8.6\bin to PATH
set QMAKESPEC=win32-msvc2010
echo -- QMAKESPEC set to "win32-msvc2010"

%QTDIR%\bin\qmake.exe -spec win32-msvc2010 -tp vc -r

cd .\bin\win\debug32
