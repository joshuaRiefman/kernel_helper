# Kernel Helper
_Kernel Helper_ is a lightweight C application that can convert an OpenCL kernel file 
into a format that is paste-able into a C++ program.

## Install
To install _Kernel Helper_, it is recommended that you clone this repository to your platform.

To ensure you have git installed,
```bash
git --version
```
To clone this repository,
```bash
git clone https://github.com/joshuaRiefman/kernel_helper.git
```

## Build
To build and use _Kernel Helper_, it is recommended to create a `build` folder and use CMake to build the executable. Here's an example of the commands you'd use on MacOS.

To create a build folder,
```bash
mkdir build
cd build
```
Now, use CMake to create the executable.
```bash
cmake ..
make
```

The executable will be placed into your build directory.

## Usage
Currently, _Kernel Helper_ must be located in the directory that your OpenCL kernel is located.

To convert `kernel.cl` using _Kernel Helper_ to `out.txt`,

```bash
./kernel_helper -f kernel.cl -o out.txt
```

The `-f` flag will tell _Kernel Helper_ what file to parse, and the `-o` flag where to put the result (it will create the file if it doesn't exist).
Use the `-v` flag to enable verbose mode which will output the result to the console, as well. Use the `-b` flag to enable the inclusion of blank lines (which would otherwise be skipped).


## Known Issues
