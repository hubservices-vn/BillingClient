@echo off
setlocal

call "%VS90COMNTOOLS%vsvars32.bat" > NUL
pushd %~dp0

echo Generating .ts files for Client App...

IF NOT EXIST resource\tr\clientApp (
    mkdir resource\tr\clientApp
)
%QTDir%\bin\lupdate.exe -silent -noobsolete -pro gbClientApp.pro -ts resource/tr/clientApp/en_US.ts
%QTDir%\bin\lupdate.exe -silent -noobsolete -pro gbClientApp.pro -ts resource/tr/clientApp/vi_VN.ts
%QTDir%\bin\lupdate.exe -silent -noobsolete -pro gbClientApp.pro -ts resource/tr/clientApp/id_ID.ts
%QTDir%\bin\lupdate.exe -silent -noobsolete -pro gbClientApp.pro -ts resource/tr/clientApp/th_TH.ts


echo making .ts file done
popd
endlocal

rem pause