@echo off
setlocal

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

echo Generating .ts to .qm files for Client App...


%QTDir%\bin\lrelease.exe -silent resource/tr/clientApp/en_US.ts -qm resource/tr/clientApp/en_US.qm
%QTDir%\bin\lrelease.exe -silent resource/tr/clientApp/vi_VN.ts -qm resource/tr/clientApp/vi_VN.qm
%QTDir%\bin\lrelease.exe -silent resource/tr/clientApp/id_ID.ts -qm resource/tr/clientApp/id_ID.qm
%QTDir%\bin\lrelease.exe -silent resource/tr/clientApp/th_TH.ts -qm resource/tr/clientApp/th_TH.qm



echo .ts to .qm done
popd
endlocal

rem pause