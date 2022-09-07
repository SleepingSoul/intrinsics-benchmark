# intrinsics-benchmark

Project uses Google Benchmark to test few intrinsics implementations agains MSVC max optimizations with /arch:AVX2.
Instruction sets that are used in these examples are AVX and AVX2.

Example of running this benchmark on my PC (with Intel i7-10700k):
![Imgur](https://imgur.com/Wjggwe2)

In examples I use 3 simple operations:
1. Transform 32 chars to 32 normalized floats;
2. Transform 32 int16s to 32 normalized floats;
3. Transform 32 int32s to 32 normalized floats.

### Building and launching
0. You need Visual Studio 2022 (it might also work on older versions)
1. Recursivly clone this repository (it contains subrepo [vcpkg](https://github.com/microsoft/vcpkg));
2. Go to external/vcpkg and run 'bootstrap-vcpkg.bat';
3. Run 'vcpkg install benchmark:x64-windows benchmark:x86-windows';
4. Now you should be able to build and launch the program in any configuration;
5. Tweak '/arch' setting in Project Settings depending on your CPU instruction set and/or add '_DEBUGMODE' macro if you don't want to run benchmark but do your own things in the main function instead.

### Build status (Visual Studio 2022)
[![Release x64](https://ci.appveyor.com/api/projects/status/2xgnc4cguseu42gd/branch/main?svg=true)](https://ci.appveyor.com/project/SleepingSoul/intrinsics-benchmark/branch/main) - Release x64

[![Debug x64](https://ci.appveyor.com/api/projects/status/x82xih3ve3jg9789/branch/main?svg=true)](https://ci.appveyor.com/project/SleepingSoul/intrinsics-benchmark-shqem/branch/main) - Debug x64

[![Release x86](https://ci.appveyor.com/api/projects/status/novl5bhagvxo74b7/branch/main?svg=true)](https://ci.appveyor.com/project/SleepingSoul/intrinsics-benchmark-62qf6/branch/main) - Release x86

[![Debug x86](https://ci.appveyor.com/api/projects/status/ebay6cku1eiyk8rq/branch/main?svg=true)](https://ci.appveyor.com/project/SleepingSoul/intrinsics-benchmark-3rc5k/branch/main) - Debug x86
