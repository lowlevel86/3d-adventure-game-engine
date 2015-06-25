@echo off
set PATH=%PATH%;C:\Dev-Cpp\bin;C:\Dev-Cpp
echo "Compiling main.c..."
rm main.o
make.exe  -C "%CD%" -f Makefile.win
