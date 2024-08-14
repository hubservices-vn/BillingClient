@echo off
setlocal

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

echo Generating Visual Studio project for gbClientService...

set QMAKESPEC=win32-msvc2008
qmake -tp vc gbClientService.pro

echo done
popd
endlocal

rem pause
