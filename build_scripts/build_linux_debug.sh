mkdir "../bin"
mkdir "../bin/Release/"
mkdir "../bin/Release/x86"
mkdir "../bin/intermediates"
mkdir "../bin/intermediates/proj_linux"
mkdir "../bin/intermediates/proj_linux/Release/"
mkdir "../bin/intermediates/proj_linux/Release/x86"
g++ -W"switch" -W"no-deprecated-declarations" -W"empty-body" -W"conversion" -W"return-type" -W"parentheses" -W"no-pointer-sign" -W"no-format" -W"uninitialized" -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -std=c++11 -Wall -fno-strict-aliasing -g2 -gdwarf-2 -o "./../bin/intermediates/proj_linux/Release/x86/client.o" "./../src/linux/client.cpp" -o "./../bin/intermediates/proj_linux/Release/x86/server.o" "./../src/linux/server.cpp"