#!/bin/bash
echo "Creating dreictories..."
$(mkdir -p "./../bin" "./../bin/Release" "./../bin/intermediates" "./../bin/intermediates/proj_linux" "./../bin/intermediates/proj_linux/Release")
echo "Compiling...(Release)"
$(g++ -W"switch" -W"no-deprecated-declarations" -W"empty-body" -W"conversion" -W"return-type" -W"parentheses" -W"no-format" -W"uninitialized" -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -std=c++11 -Wall -fno-strict-aliasing -g2 -gdwarf-2 -D NDEBUG -o ./../bin/intermediates/proj_linux/Release/SZ_Socket_linux.o ./../src/linux/SZ_Socket_linux.cpp)
echo "Linking..."
$(ar rvs ./..bin/Release/SZ_Socket.a ./../bin/intermediates/proj_linux/Release/SZ_Socket_linux.o)
read -p "Press enter to continue"