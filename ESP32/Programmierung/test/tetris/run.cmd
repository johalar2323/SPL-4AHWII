@echo off
setlocal
cd /d "%~dp0..\..\..\.."
if not exist .work\tetris mkdir .work\tetris
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cl /nologo /EHsc /std:c++14 /I ESP32\Programmierung\lib\TetrisSpiel\src ESP32\Programmierung\test\tetris\test_tetris.cpp ESP32\Programmierung\lib\TetrisSpiel\src\TetrisSpiel.cpp /Fe:.work\tetris\test.exe /Fo.work\tetris\
if errorlevel 1 exit /b 1
.work\tetris\test.exe
if errorlevel 1 exit /b 1
cl /nologo /EHsc /std:c++14 /I ESP32\Programmierung\test\snake\fakes /I ESP32\Programmierung\include /I ESP32\Programmierung\lib\SnakeSpiel\src /I ESP32\Programmierung\lib\TetrisSpiel\src ESP32\Programmierung\test\tetris\test_bedienung.cpp ESP32\Programmierung\lib\SnakeSpiel\src\SnakeSpiel.cpp ESP32\Programmierung\lib\TetrisSpiel\src\TetrisSpiel.cpp /Fe:.work\tetris\bedienung.exe /Fo.work\tetris\
if errorlevel 1 exit /b 1
.work\tetris\bedienung.exe
