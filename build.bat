@echo off
md out
pushd out

if "%1"=="clang" goto clang

:msvc
cl /std:c++17 /nologo /D BUILDING_DLL /EHsc /Zi ..\src\library\library.cpp /LD /Fedll /link
goto buildexe

:clang
REM the -g -gcodeview options add PDB support but significantly bloat the binary
REM -fvisibility-inlines-hidden for exported inlines? (JSError)
clang++ -nostdinc++ -D BUILDING_DLL -stdlib=libc++ ..\src\library\library.cpp -Id:\React\libc++\include\ -Ld:\React\libc++\out\lib\ -l c++.lib -shared -o dll.dll -g -gcodeview
goto buildexe

:buildexe
cl /std:c++17 /EHsc /nologo dll.lib ..\src\consumer\consumer.cpp /Zi

popd