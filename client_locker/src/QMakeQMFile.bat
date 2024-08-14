@echo off
setlocal

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

echo Generating .ts to .qm files forclientLocker...


%QTDir%\bin\lrelease.exe -silent resource/tr/clientLocker/en_US.ts -qm resource/tr/clientLocker/en_US.qm
%QTDir%\bin\lrelease.exe -silent resource/tr/clientLocker/vi_VN.ts -qm resource/tr/clientLocker/vi_VN.qm
%QTDir%\bin\lrelease.exe -silent resource/tr/clientLocker/id_ID.ts -qm resource/tr/clientLocker/id_ID.qm
%QTDir%\bin\lrelease.exe -silent resource/tr/clientLocker/th_TH.ts -qm resource/tr/clientLocker/th_TH.qm


echo .ts to .qm done
popd
endlocal

rem pause