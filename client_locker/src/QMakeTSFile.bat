@echo off
setlocal

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

echo Generating .ts files for Client Locker...

IF NOT EXIST resource\tr\clientLocker (
    mkdir resource\tr\clientLocker
)
%QTDir%\bin\lupdate.exe -silent -noobsolete -pro gbClientLocker.pro -ts resource/tr/clientLocker/en_US.ts
%QTDir%\bin\lupdate.exe -silent -noobsolete -pro gbClientLocker.pro -ts resource/tr/clientLocker/vi_VN.ts
%QTDir%\bin\lupdate.exe -silent -noobsolete -pro gbClientLocker.pro -ts resource/tr/clientLocker/id_ID.ts
%QTDir%\bin\lupdate.exe -silent -noobsolete -pro gbClientLocker.pro -ts resource/tr/clientLocker/th_TH.ts


echo making .ts file done
popd
endlocal

rem pause