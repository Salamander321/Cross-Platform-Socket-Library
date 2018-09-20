## Cross Platform Socket Library (Linux and Windows)

# Directory Structure
- `src` contains all source files
- `include` contains all files for include
- `src/linux` contains all source files for linux
- `src/windows` contains all sourxe files for windows

## Building -- won't work currently
All build scripts are available in `build_scripts` directory

# Linux with GCC
- For debug build run [build_linux.sh](./build_scripts/build_linux.sh)
- For release build run [build_linux.sh](./build_scripts/build_linux.sh) with `-release` flag

# Windows with Visual Studio
- Open [VS Solution File](./build_scripts/vs_sln/vs_sln.sln) and compile


# Developing
Add the files in `src` directory and for compilation you may need to add files to the build script / VS, then compile as mentioned above

*Note: (for Visual Studio Users) Running the solution file, you can edit both windows and linux source files, then compile/run both, for linux project to build you'll need to setup remote connection to a linux machine*

## TODO
- Using the header files [SZ_Socket.h](./src/SZ_Socket.h) building source files for windows and linux