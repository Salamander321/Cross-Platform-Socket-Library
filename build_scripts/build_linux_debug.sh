#!/bin/bash
echo "Creating directories..."
$(mkdir -p "./../bin" "./../bin/Debug" "./../bin/intermediates" "./../bin/intermediates/proj_linux" "./../bin/intermediates/proj_linux/Debug")
echo "Compiling...(Debug)"
$(g++ -W"switch" -W"no-deprecated-declarations" -W"empty-body" -W"conversion" -W"return-type" -W"parentheses" -W"no-format" -W"uninitialized" -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -std=c++11 -Wall -fno-strict-aliasing -g2 -gdwarf-2 -o ./../bin/intermediates/proj_linux/Debug/SZ_Socket_linux.o ./../src/linux/SZ_Socket_linux.cpp)
echo "Linking..."
$(ar rvs ./..bin/Debug/SZ_Socket.a ./../bin/intermediates/proj_linux/Debug/SZ_Socket_linux.o)
read -p "Press enter to continue"