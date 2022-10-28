@echo off
echo Building Client...
g++ Client.cpp -std=c++17 -O2 -Wall -lwsock32 -lm -static -o Client.exe
echo Build Done.
echo Building Server...
g++ Server.cpp -std=c++17 -O2 -Wall -lwsock32 -lm -static -o Server.exe
echo Build Done.
pause