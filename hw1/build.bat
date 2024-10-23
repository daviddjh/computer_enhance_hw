@echo off

:: Env variables and "current directory" we change will reset after script is over
setlocal

:: Change directory to Batch file location. %0 = batch file location. "~dp" = Parameter extensions to extract drive and path
:: https://stackoverflow.com/questions/17063947/get-current-batchfile-directory
cd /d %~dp0

:: Change directory to the build folder
mkdir .\build
pushd .\build

:: If program is running in the debugger, then stop it so the compiler can write to the .exe
:: remedybg.exe stop-debugging

:: /std:c++20 automatically adds /permissive- ( note the '-' ). This causes some compiler errors that I don't care about right now.
:: added /permissive to work around this
set flags= /std:c++20 /permissive /Feout /FAs /EHsc
set code=..\main.cpp
set includes=/I"..\d_core"
set link_libs= Winmm.lib ole32.lib oleaut32.lib Advapi32.lib crypt32.lib

:: Compile our app, or DirectXTex
if "%1" == "-d" (

    :: Compile this
    cl /Zi /DDEBUG /D_DEBUG /MDd %flags% %code% %includes% %link_libs% 

    popd

) else if "%1" == "-ods" (

    :: Compile this
    cl /MD /Zi /O2 /DPCOUNTER /fp:fast %flags% %code% %includes% %link_libs%

    popd

) else if "%1" == "-o" (

    :: Compile this
    cl /MD /O2 /fp:fast %flags% %code% %includes% %link_libs%

    popd

) else (

    :: Compile this
    cl /Zi /DDEBUG /D_DEBUG /MDd %flags% %code% %includes% %link_libs%

    popd
)