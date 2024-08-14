@echo off
setlocal

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

echo ====Building .rc to .rcc====
rcc -compress 1 -threshold 9 -binary resource/clientApp.qrc -o resource/clientApp.rcc
echo .rc to .rcc done

rem pause