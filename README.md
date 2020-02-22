# ABI Safety experiments

See what's safe to pass between binaries compiled with Clang+libc++ and msvc+stdlib.

From http://releases.llvm.org/download.html#9.0.0

Download and unpack
* Sources: LLVM source code => d:\React\llvm-9.0.0.src\
* Sources: libc++ source code => d:\React\libc++\
* Pre-Built Binaries: Windows (64-bit) => d:\React\LLVM\bin\

in the folder where you unpacked the libc++ source code

SET PATH=%PATH%;d:\React\LLVM\bin\

cmake -G Ninja  -DCMAKE_MAKE_PROGRAM=ninja  -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=clang-cl -DCMAKE_C_FLAGS="-fms-compatibility-version=19.00 --target=i686--windows" -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_CXX_FLAGS="-fms-compatibility-version=19.00 --target=i686--windows" -DLLVM_PATH=d:\React\llvm-9.0.0.src\ -DLIBCXX_ENABLE_SHARED=YES -DLIBCXX_ENABLE_STATIC=NO -DLIBCXX_ENABLE_EXPERIMENTAL_LIBRARY=NO ..

cmake --build .
