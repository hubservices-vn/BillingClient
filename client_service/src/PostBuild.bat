@echo off
SETLOCAL ENABLEEXTENSIONS

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

rem /I means case insensitive
if /I "%1"=="" goto ParamFail

IF /I "%1"=="Debug" (
    set Configuratoin=Debug
)
IF /I "%1"=="Release" (
    set Configuratoin=Release
)

echo ====Executing post-build events====

set toolDir=..\..\..\..\..\GBill-Tools
set trdPartyDir=..\..\..\3rdParty
set commonDir=..\..\..\common
IF /I "%1"=="Debug" (
    set outputDir=..\bin\Debug
)
IF /I "%1"=="Release" (
    set outputDir=..\bin\Release
)
IF /I "%1"=="Release_Internal" (
    set outputDir=..\bin\Release_Internal
)
IF /I "%1"=="Release_Public" (
    set outputDir=..\bin\Release_Public
)
IF NOT EXIST %outputDir% (
    mkdir %outputDir%
)

IF /I %Configuratoin%==Debug (
    set dllPrefix=d
)

rem copy QT dlls
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtCore%dllPrefix%4.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtNetwork%dllPrefix%4.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtSql%dllPrefix%4.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtXml%dllPrefix%4.dll %outputDir%

rem copy the mysql plugin files
xcopy /Y /D /C /Q /R %trdPartyDir%\dll\libmysql.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\plugins\sqldrivers\qsqlmysql%dllPrefix%4.dll %outputDir%

rem copy the sqlite cipher plugin files
IF NOT EXIST %outputDir%\sqldrivers (
    mkdir %outputDir%\sqldrivers
)
xcopy /Y /D /C /Q /R %QTDIR%\plugins\sqldrivers\sqlitecipher%dllPrefix%4.dll %outputDir%\sqldrivers

rem copy mini dump dll
xcopy /Y /D /C /Q /R %trdPartyDir%\dll\dbghelp.dll %outputDir%


echo Copying done
goto end

:ParamFail
echo -----------------------------WARNING -----------------------------
echo  Don't directly run this, run Make-Debug or Make-Release instead
echo -----------------------------WARNING -----------------------------
pause
goto end

:end
popd
endlocal

rem pause