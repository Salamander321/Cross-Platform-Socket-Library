#!/bin/bash

# Variables
let debug;

# Detect arguments, if -release is present, use release compilation
if [ "$1" == "-release" ]; then
    debug="-D NDEBUG";
fi

# Make directories if not exists
if [ ! -d ./../bin ]; then
  mkdir ./../bin;
fi
if [ ! -d ./../bin/Release/ ]; then
  mkdir ./../bin/Release/;
fi
if [ ! -d ./../bin/intermediates ]; then
  mkdir ./../bin/intermediates;
fi
if [ ! -d ./../bin/intermediates/proj_linux ]; then
  mkdir ./../bin/intermediates/proj_linux;
fi
if [ ! -d ./../bin/intermediates/proj_linux/Release/ ]; then
  mkdir ./../bin/intermediates/proj_linux/Release/;
fi

# Compile files
g++ -W"switch" -W"no-deprecated-declarations" -W"empty-body" -W"conversion" -W"return-type" -W"parentheses" -W"no-format" -W"uninitialized" -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -std=c++11 -Wall -fno-strict-aliasing -g2 -gdwarf-2 $debug -o "./../bin/intermediates/proj_linux/Release/SZ_Socket_linux.o" "./../src/linux/SZ_Socket_linux.cpp"
ar rvs "./..bin/Release/SZ_Socket.a" "./../bin/intermediates/proj_linux/Release/SZ_Socket_linux.o"