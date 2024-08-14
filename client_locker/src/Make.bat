@echo off
SETLOCAL ENABLEEXTENSIONS
COLOR 00

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0


rem parameters:
rem %1 build option [Debug/Release]
rem %2 version, format of "1.2.13.abef"

rem /I means case insensitive
if /I "%1"=="" goto ParamFail
if /I "%2"=="" goto ParamFail

set toolDir=..\..\..\..\..\GBill-Tools
set trdPartyDir=..\..\..\3rdParty
set commonDir=..\..\..\common
set outputRootDir=..\bin

set ProjFile=gbClientLocker.vcproj
rem parse 1st parameter
IF /I "%1"=="Debug" (
    set Configuratoin=Debug
    set binDir=..\bin\Debug
)
IF /I "%1"=="Release" (
    set Configuratoin=Release
    set binDir=..\bin\Release
)

rem parse 2nd parameter
set VersionStr=%2

rem clean the build folder
if /I "%1"=="Clean"  goto Clean


rem make sure the %binDir% is clean
if exist %binDir%  rd /s /q %binDir%

rem make sure the bin folder exists
IF NOT EXIST %binDir% (
    mkdir %binDir%
)


echo Building Client Locker...

rem compile .qrc file to .rcc file
CALL QMakeRCFile.bat
rem copile .ts file to .qm file
CALL QMakeQMFile.bat


IF EXIST "CZUniqueBuildLogs.logcz" (
    del /F /Q "CZUniqueBuildLogs.logcz"
)
rem compile the code
devenv %ProjFile% /build "%Configuratoin%|win32" /out "CZUniqueBuildLogs.logcz"
If %ERRORLEVEL% NEQ 0 (
    goto BuildFail
)
del /F /Q "CZUniqueBuildLogs.logcz"

echo Building Done...Begin post build event

rem #############################################
rem post build event
rem #############################################

rem after copying all files, also copy them into instlaler folder (in the end), so installer script could use them directly
set binInstallDir=%binDir%_Install

rem ensure install folder is clean
if exist %binInstallDir%  rd /s /q %binInstallDir%
IF NOT EXIST %binInstallDir% (
    mkdir %binInstallDir%
)

rem get the date time and append it to the folder name
rem format of "20121221-1300"
rem sample of cmd's "date" output "08/13/2012 Mon"
set mon=%date:~0,2%
set day=%date:~3,2%
set yr=%date:~6,4%

rem ensure leading zero (if it's " ", fill it with 0)
set now=%time: =0%
set hr=%now:~0,2%
set min=%now:~3,2%
set dt=%yr%%mon%%day%_%hr%%min%

rem create the folders
set outputDir=%outputRootDir%\clientLocker_%dt%_%VersionStr%
IF NOT EXIST %outputDir% (
    mkdir %outputDir%
)

IF /I %Configuratoin%==Debug (
    set dllPrefix=d
)

rem copy the executable
xcopy /Y /D /C /Q /R %binDir%\gbClientLocker.exe %outputDir%


rem copy the SystemHook dll
xcopy /Y /D /C /Q /R %toolDir%\SystemHook\%Configuratoin%\SystemHook.dll %outputDir%

rem copy QT dlls
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtCore%dllPrefix%4.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtGui%dllPrefix%4.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtNetwork%dllPrefix%4.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtXml%dllPrefix%4.dll %outputDir%

rem copy mini dump dll
xcopy /Y /D /C /Q /R %trdPartyDir%\dll\dbghelp.dll %outputDir%

rem copy the resource file
xcopy /Y /D /C /Q /R .\resource\clientLocker.rcc %outputDir%

rem copy the translation files
IF NOT EXIST %outputDir%\tr\clientLocker (
    mkdir %outputDir%\tr\clientLocker
)
xcopy /Y /D /C /Q /R resource\tr\clientLocker\*.qm %outputDir%\tr\clientLocker

rem copy the version tag file
echo f | xcopy /Y /D /C /Q /R %commonDir%\Version\version.tag %outputDir%\version_ClientLocker.tag

rem copy crash-sender
xcopy /Y /D /C /Q /R %toolDir%\CrashReport\%Configuratoin%\CrashSender.exe %outputDir%

rem copy all files in outputDir to installer folder (before backing up pdb file)
echo d | xcopy /E /Y /D /C /Q /R %outputDir% %binInstallDir%

rem add digital signature to the exe files
CALL %SignToolDir%\sign_hoabinh.bat %binInstallDir%\gbClientLocker.exe
CALL %SignToolDir%\sign_hoabinh.bat %binInstallDir%\SystemHook.dll

rem also backup the pdb 
xcopy /Y /D /C /Q /R %binDir%\gbClientLocker.pdb %outputDir%

rem prompt message
COLOR 0A
echo -----------------------------CONGRATULATIONS -----------------------------
echo -----------------------------CONGRATULATIONS -----------------------------
echo                   Client Locker BUILT SUCCESSFULLY!
echo -----------------------------CONGRATULATIONS -----------------------------
echo -----------------------------CONGRATULATIONS -----------------------------
goto end

:ParamFail
COLOR 0C
echo -----------------------------WARNING -----------------------------
echo  Don't directly run this, run Make-Debug or Make-Release instead
echo -----------------------------WARNING -----------------------------
rem goto end
popd
endlocal
EXIT /B 1

:BuildFail
COLOR 0C
echo -----------------------------ERROR -----------------------------
echo  Build client locker failed, read "CZUniqueBuildLogs.logcz" for details
echo -----------------------------ERROR -----------------------------
rem goto end
popd
endlocal
EXIT /B 1

:Clean
echo Cleaning Client Locker...
 if exist %binDir%  rd /s /q %binDir%
echo Cleaning done
goto end

:end
popd
endlocal

rem pause