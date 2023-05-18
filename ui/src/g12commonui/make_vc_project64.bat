echo Setting up a Qt environment...

set PLATFORM=x64
set QTDIR=C:\Qt\Qt5.9.3\5.9.3\msvc2015_64
echo -- QTDIR set to C:\Qt\Qt5.9.3\5.9.3\msvc2015_64
set PATH=%QTDIR%\bin;%PATH%
echo -- Added C:\Qt\Qt5.9.3\5.9.3\msvc2015_64 to PATH
set QMAKESPEC=win32-msvc2015
echo -- QMAKESPEC set to "win32-msvc2015"

%QTDIR%\bin\qmake.exe -spec win32-msvc -tp vc -r

pause