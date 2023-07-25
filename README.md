# Kernel Helper
_Kernel Helper_ is a lightweight C application that can convert an OpenCL kernel file 
into a text format that is paste-able into a C++ program, or loaded directly in a program at runtime. 
_Kernel Helper_ is two mostly independent parts: it can be built and used as an executable,
or it can be included in a project and used through public interfaces at runtime.
See [interfaces](docs/INTERFACE.md) to learn about the public interfaces available. See `examples/` for an example of what the reformatting looks like.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Install](#install)
3. [Build](#build)
4. [Use as Executable](#use-as-executable)
5. [Use at Runtime](#use-at-runtime)
6. [Known Issues](#known-issues)

## Prerequisites

To ensure you have git installed,
```bash
git --version
```
You're also going to want CMake as a build system.
```bash
cmake --version
```

## Install
To install _Kernel Helper_, it is recommended that you clone this repository to your platform or add it as a submodule.

To clone this repository,
```bash
git clone https://github.com/joshuaRiefman/kernel_helper.git
```
If you are going to use _Kernel Helper_ at runtime in your own code, it is preferable to use _Kernel Helper_ as a submodule. It's common practice to put an external tool such as _Kernel Helper_ in an `external/` directory.
```bash
mkdir external && cd external
git submodule add https://github.com/joshuaRiefman/kernel_helper.git
```

No matter whether you intend on using _Kernel Helper_ as an executable or at runtime, it's vital to build the project.

## Build
To build and use _Kernel Helper_, it is recommended to create a `build` folder and use CMake to build the executable. Here's an example of the commands you'd use on MacOS.

To create a build folder,
```bash
mkdir build && cd build
```
Now, use CMake to create the executable.
```bash
cmake ..
make
```

You'll now have an executable in `build/bin` called `convert_kernel` and a library in `build/lib` called `libkernel_helper.a`.

## Use as Executable
Currently, _Kernel Helper_ assumes that you'll be executing from the top-level directory of the project, so return to that directory from `build` if you haven't already.
```bash
cd ..
```

_Kernel Helper_ searches the `data/` directory for `.cl` kernels and it is also the location that output files get placed.
To convert `kernel.cl` using _Kernel Helper_ to `out.txt`,

```bash
./build/bin/convert_kernel -f kernel.cl -o out.txt
```
`./build/bin/convert_kernel` is the command to run the executable on MacOS (may be different for your platform).
The `-f` flag will tell _Kernel Helper_ what file to parse, and the `-o` flag where to put the result (it will create the file if it doesn't exist).
Use the `-v` flag to enable verbose mode which will output the result to the console, as well. Use the `-b` flag to enable the inclusion of blank lines (which would otherwise be skipped). Use the `-a` flag to tell the executable to search the directory it is in, instead of trying to find the `data/` directory; this is useful if you move the executable from the `build/bin` folder it generates in.

## Use at Runtime
To use _Kernel Helper_ in your own code to be executed at runtime, you'll need to include the `kernel_helper.h` header file and let your project know where the library is; the preferred tool for this is CMake. The path to the header file for you may be different, but here's an example that follows the steps of adding _Kernel Helper_ as an external submodule.
```c
#include "external/kernel_helper/include/kernel_helper.h"
```
You'll also need to link the libraries to your project (`your_project`). Add this to your `CMakeLists.txt`.
```cmake
target_link_libraries(your_project PRIVATE kernel_helper::kernel_helper)
find_package(KernelHelper REQUIRED)
```
You may need to explicitly let CMake know where the `KernelHelperConfig.cmake` file is. If you followed the submodule example, use this:
```cmake
set(KernelHelper_DIR external/kernel_helper/build)
```

Now, you should be able to use the public interfaces provided by _Kernel Helper_!

### [View Interfaces](docs/INTERFACE.md)

## Known Issues

1. _Kernel Helper_ requires an end-of-file blank line or else it has issues resolving the length of the last line.
