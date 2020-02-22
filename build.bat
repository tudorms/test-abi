@echo off
md out
pushd out

if "%1"=="clang" goto clang

:msvc
cl /nologo /D BUILDING_DLL /EHsc /Zi ..\src\library\library.cpp /LD /Fedll /link
goto buildexe

:clang
clang++ -nostdinc++ -stdlib=libc++ ..\src\library\library.cpp -Id:\React\libc++\include\ -Ld:\React\libc++\out\lib\ -l c++.lib -shared -o dll.dll
goto buildexe

:buildexe
cl /EHsc /nologo dll.lib ..\src\consumer\consumer.cpp /Zi

popd