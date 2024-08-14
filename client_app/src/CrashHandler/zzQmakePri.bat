@echo off
setlocal

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

qmake -project

set curDir=%CD%
%MyProjBin%\QTPriGenerator\QTPriGenerator.exe %curDir%

popd
endlocal

rem pause