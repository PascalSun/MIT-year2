# Protype for Zika Virus Modelling Project
## Basic Information
- Translate original python code to c++
- Add openMP
## Libraries needed
* CMake
* OpenMP
* Boost


It recommended that you use msys2 on Windows, brew on OSX, pacman or apt-get on linux and g++ compiler in order to utilize the package manager.

Note: as part of turning Epidemica into a single cmake project, support for visual studio has been added
To setup your environment, please remember to add BOOST_ROOT, GTEST_ROOT, etc. to your environment variables and restart the IDE.

These can be added to the CMakeLists.txt, but they will need to be removed when pushed via git.

(If you are having trouble with any libraries, add guides here)

## Running CMake
This project has been setup with CMake, so it should support cross-platform builds, and be should be fixed otherwise.

CMake is a tool to generate project files for other build tools to run, it does not actually compile the code.

At the moment, we should be able to have this working for:

- Linux
- Windows MSVC
- Windows MinGW
- OSX

Linux based systems should be relatively straight forward to get libraries for since apt-get and pacman do a good job of finding procompiled binaries.

Guides below should help when this is not so straightforward.

To build cmake files out of source (seperates generated files from source):

- `mkdir build`
- `cd build`
- `cmake ../ -G <Generator Name>`

If you use the "Unix Makefiles" generator, simply run run `make` in the build directory.

If you use an IDE generator like "Visual Studio 15 2017 x64", open the solution file in the build directory.

## Build Toolchains
Toolchains is essentially cmake code that gets run first, and can be specified when running the cmake command

###Magnus
Compiling over Magnus is different than compile it over local machine. This apply to every supercomputers having multiple compile environment, module management and SLURM workload management.

To enable compile it, make sure you have the following module loaded:
- CMake/3.4.1
- Boost/1.57.0
- GNU Program Environment (PrgEnv-gnu)
- gcc/4.9.2

This can be achieve by module load / swap, by running the following command:
- `module swap PrgEnv-cray PrgEnv-gnu`
- `module swap gcc gcc/4.9.2`
- `module load cmake/3.4.1`
- `module load boost/1.57.0`
When finished executing these commands from your logon terminal, you are ready to run `cmake`. after `cmake` finished, you can run `make` to compile the code.

## Installing Libraries for Windows via MinGW
How to setup MinGW and libs on windows (the linux toolchain way :D)

follow this trusty site: http://www.math.ucla.edu/~wotaoyin/windows_coding.html
### Steps
- install msys2.
  - honest to god, this is the only version of ming32 and ming64 you want installed
  - the number of libraries available via pacman is much better than the mingw-get alternative
  - projects written for linux will compile in the msys2 bash, and into windows binaries

- add msys64/mingw64/bin to your PATH
  - put it towards the end, you will want all your normal processes to find dlls compiled in msvc first
  - msys2 bash on the contrary will prioritize binaries in its install location
  - this method lets you do mingw builds in msys2 bash, and msvc builds in cmd shells. No more cygwin :)

## Boost (Required)
### MinGW Windows Install

install boost from msys2

- run `pacman -Ss boost` boost
- `pacman -S <name>`

### MSVC Windows Install
I recommend building everything in x64, but Win32 is possible, check link below

- Download the latest boost source to some location
- Run Visual Studio command prompt
  - for VS2017, run `"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64`
- run: `./tools/build/bootstrap msvc`
- run: `./tools/build/b2 toolset=msvc warnings=off threading=multi link=static variant=release stage`
- Add BOOST_ROOT to your environment variables pointing to the location of your boost folder

maybe try using these settings: [link](https://studiofreya.com/2015/05/20/building-boost-1-58-for-32-bit-and-64-bit-architectures-with-visual-studio-2013/)


## Boost MPI (Optional)
### MinGW Windows Install
Make sure you installed MPI first as described in this readme

download the latest boost source to compile boost_mpi

- run: `./tools/build/bootstrap gcc`
- run: `./tools/build/b2 toolset=gcc warnings=off threading=multi link=static variant=release --prefix=./build/ mpi`
- copy the libboost_mpi-mt.a and .dll.a to ming64/lib/, .dll to ming64/bin/


## MPI (optional)
### Windows MinGW Install
[install the latest msmpi sdk and runtime (link to v8)](https://www.microsoft.com/en-us/download/details.aspx?id=54607)

  - do the dll linkage thing
    - run: gendef msmpi.dll
    - run: dlltool -d msmpi.def -D msmpi.dll -l libmpi.a
    - add libmpi.a file to mingw64/lib/
      - no need to copy msmpi.dll to bin as shell and msys2 check windows directories now after adding to PATH
  - do the header tweak from the website [link](http://www.math.ucla.edu/~wotaoyin/windows_coding.html)
  - add mpi.h to mingw64/include


## OpenCL (Optional)
### MINGW install
OpenCL dll should already be installed with your graphics driver

 - check by running `where opencl.dll` in command prompt

Install header files and gcc linked binary

 - run: `pacman -Ss opencl`, install the most relevent one
 - run: `gendef opencl.dll`
 - run: dlltool -d opencl.def -D opencl.dll -l libopencl.a
 - add libopencl.a to mingw64/lib
 - - no need to copy opencl.dll to bin as shell and msms2 check windows directories

## GoogleTest (Optional)
### Windows MSVC Install

- Clone the googletest repository
- Run it via cmake
- Open the solution in Visual Studio
  - switch to Release build
  - run the Install project
    - Should install to Program Files somewhere
  - Add environment variable GTEST_ROOT with the install path

### Windows MinGW Install

- should be available via pacman

## Build note 1 (irrelevent/ignore)
as of boost 1_63_0: boost has deprecated function calls to auto_ptr, and can't compile boost_mpi correctly

this is becoming really hard to make work on mingw, but on normal distros it should be cake

i would suggest

- downloading the same version source code
- modify the build configuration to point to msmpi, or find mpich2 for windows
- build the mpi module
- copy it back into the msys2 x64 lib directory

## Build note 2
to fix current boost mpi build issues on mingw

- in boost_1_63_0\libs\serialization\build\Jamfile.v2, line 109, add: <define>BOOST_SERIALIZATION_SOURCE

Let me know if you want my compiled versions of boost binaries

## Building

CMake generates project files for other build systems and IDEs, however, if you use an IDE, you will need to update the cmakelists.txt when
appropriate. (Wildcards are currently being used for filenames, so just make sure you add new source files into the correct location)

Make sure to run cmake whenever you pull code.

# Virus Models

Models to add include:

* Arvo virus
* Dengi virus
* Yellow fever virus
* Zika virus

# Projects
Projects within this solution include:

* VirusFramework
* VirusSimulator
* VirusControlPanel

(naming to be updated if necessary)

## Architecture Considerations
- Configuration File
- Exception Handling
- Command Line Arguments
  - ModeFlags: OpenMP/OpenACC/OpenCL/MPI
  - Duration
  - Scenario/Map
  - Verbose Execution (TRACE)
  - Rendered Execution (DWARFFORTRESS style)
- Cross Platform CMake
- Plugin System
- Python Plugin System
- Execution Logging
- Configuration Generator GUI
- Remote Deployment
- Progress/Notifications
- Output Data Preparation
  - XML
  - Generated HTML

## Testing Framework
- CTest
  - CTest is only a plugin for cmake, it works with a variety of unit test frameworks :)
- CppUnit
- GoogleTest (GTest)
  - Currently being used, it seems alright and has a VS IDE integration
- UnitTest++

 ## Other Third-Party Libraries
 * TinyXML/LibXML++
 * Qt/QtOpenGL/QtXml
 * OPENMP
 * OpenACC
 * OpenCL
 * MPI
