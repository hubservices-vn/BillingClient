@echo off
SETLOCAL ENABLEEXTENSIONS

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

rem /I means case insensitive
if /I "%1"=="" goto ParamFail

set toolDir=..\..\..\..\..\GBill-Tools
set trdPartyDir=..\..\..\3rdParty
set commonDir=..\..\..\common
set gInitToolName=gInitTool2P.exe
IF /I "%1"=="Debug" (
    set dllPrefix=d
    set Configuratoin=Debug
    set outputDir=..\bin\Debug
)
IF /I "%1"=="Release" (
    set Configuratoin=Release
    set outputDir=..\bin\Release
)

IF NOT EXIST %outputDir% (
    mkdir %outputDir%
)

rem compile .qrc file to .rcc file
CALL QMakeRCFile.bat

echo ====Executing post-build events====



rem copy the process hook dlls
IF NOT EXIST %outputDir%\x86 (
    mkdir %outputDir%\x86
)
IF NOT EXIST %outputDir%\x86\stub (
    mkdir %outputDir%\x86\stub
)
IF NOT EXIST %outputDir%\x64 (
    mkdir %outputDir%\x64
)
IF NOT EXIST %outputDir%\x64\stub (
    mkdir %outputDir%\x64\stub
)
xcopy /Y /D /C /Q /R %toolDir%\processprotect\%Configuratoin%\prochost.dll %outputDir%
xcopy /Y /D /C /Q /R %toolDir%\processprotect\%Configuratoin%\x86\prochost.dll %outputDir%\x86
xcopy /Y /D /C /Q /R %toolDir%\processprotect\%Configuratoin%\x86\prochook.dll %outputDir%\x86
xcopy /Y /D /C /Q /R %toolDir%\processprotect\%Configuratoin%\x86\stub\prochook.dll %outputDir%\x86\stub
xcopy /Y /D /C /Q /R %toolDir%\processprotect\%Configuratoin%\x64\prochook.dll %outputDir%\x64
xcopy /Y /D /C /Q /R %toolDir%\processprotect\%Configuratoin%\x64\stub\prochook.dll %outputDir%\x64\stub

rem copy QT dlls
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtCore%dllPrefix%4.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtGui%dllPrefix%4.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtNetwork%dllPrefix%4.dll %outputDir%
xcopy /Y /D /C /Q /R %QTDIR%\bin\QtXml%dllPrefix%4.dll %outputDir%

rem copy the image plugins
IF NOT EXIST %outputDir%\imageformats (
    mkdir %outputDir%\imageformats
)
xcopy /Y /D /C /Q /R %QTDIR%\plugins\imageformats\qjpeg%dllPrefix%4.dll %outputDir%\imageformats
xcopy /Y /D /C /Q /R %QTDIR%\plugins\imageformats\qgif%dllPrefix%4.dll %outputDir%\imageformats
xcopy /Y /D /C /Q /R %QTDIR%\plugins\imageformats\qico%dllPrefix%4.dll %outputDir%\imageformats

rem copy mini dump dll
xcopy /Y /D /C /Q /R %trdPartyDir%\dll\dbghelp.dll %outputDir%

rem copy the resource file
xcopy /Y /D /C /Q /R .\resource\clientApp.rcc %outputDir%

rem copy the sound files
IF NOT EXIST %outputDir%\sound (
    mkdir %outputDir%\sound
)
xcopy /Y /D /C /Q /R resource\sound\*.wav %outputDir%\sound

rem copy the translation files
IF NOT EXIST %outputDir%\tr\clientApp (
    mkdir %outputDir%\tr\clientApp
)
xcopy /Y /D /C /Q /R resource\tr\clientApp\*.qm %outputDir%\tr\clientApp

rem copy crash-sender
xcopy /Y /D /C /Q /R %toolDir%\CrashReport\%Configuratoin%\CrashSender.exe %outputDir%

xcopy /Y /D /C /Q /R %toolDir%\updatev2\src\%Configuratoin%\clientupdatelib.dll %outputDir%
xcopy /Y /D /C /Q /R %toolDir%\updatev2\src\%Configuratoin%\clientupdater.exe %outputDir%

rem copy gInitTool helper tool
echo f | xcopy /Y /D /E /C /Q /R %toolDir%\gInitTool2\bin\%Configuratoin%\%gInitToolName% %outputDir%\gInitTool.exe

rem copy vncserver
xcopy /Y /D /C /Q /R %trdPartyDir%\exe\tightvnc\tvnserver.exe %outputDir%


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