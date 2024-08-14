@echo off
setlocal

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

echo Generating Visual Studio project for gbClientApp...

set QMAKESPEC=win32-msvc2008
qmake -tp vc gbClientApp.pro

echo done
popd
endlocal

rem pause
