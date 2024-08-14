@echo off
setlocal

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

rem compile .qrc file to .rcc file
CALL QMakeRCFile.bat

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


echo Copying done

rem copy the *.css files to debug env (in debug env, we don't compile the styles to rcc, so that we can edit & continue)
IF /I %Configuratoin%==Debug (
    goto DEBUGONLY
)
if /I %Configuratoin%==Release (
    goto RELEASEONLY
)

:DEBUGONLY
rem copy css files
IF NOT EXIST %outputDir%\css (
    mkdir %outputDir%\css
)
xcopy /Y /D /C /Q /R .\resource\css\* %outputDir%\css  

rem copy the background files
IF NOT EXIST %outputDir%\ccBackground (
    mkdir %outputDir%\ccBackground
)
xcopy /Y /D /E /C /Q /R resource\ccBackground\*.*  %outputDir%\ccBackground


goto end

:RELEASEONLY
goto end

:ParamFail
COLOR 0C
echo -----------------------------WARNING -----------------------------
echo  Don't directly run this, run Make-Debug or Make-Release instead
echo -----------------------------WARNING -----------------------------
pause
goto end

:end
popd
endlocal

rem pause