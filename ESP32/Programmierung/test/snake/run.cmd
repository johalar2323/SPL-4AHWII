@echo off
setlocal
cd /d "%~dp0..\..\..\.."
if not exist .work\snake mkdir .work\snake
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cl /nologo /EHsc /std:c++14 /I ESP32\Programmierung\lib\SnakeSpiel\src ESP32\Programmierung\test\snake\test_snake.cpp ESP32\Programmierung\lib\SnakeSpiel\src\SnakeSpiel.cpp /Fe:.work\snake\test.exe /Fo.work\snake\
if errorlevel 1 exit /b 1
.work\snake\test.exe
if errorlevel 1 exit /b 1
cl /nologo /EHsc /std:c++14 /I ESP32\Programmierung\test\snake\fakes /I ESP32\Programmierung\include /I ESP32\Programmierung\lib\SnakeSpiel\src ESP32\Programmierung\test\snake\test_modus.cpp ESP32\Programmierung\lib\SnakeSpiel\src\SnakeSpiel.cpp /Fe:.work\snake\modus.exe /Fo.work\snake\
if errorlevel 1 exit /b 1
.work\snake\modus.exe
if errorlevel 1 exit /b 1
cl /nologo /EHsc /std:c++14 /I ESP32\Programmierung\test\snake\hardware_fakes /I ESP32\Programmierung\include /I ESP32\Programmierung\lib\LedAnzeige\src ESP32\Programmierung\test\snake\test_joystick.cpp ESP32\Programmierung\src\Joystick_SPL.cpp ESP32\Programmierung\src\EntprellterTaster_SPL.cpp /Fe:.work\snake\joystick.exe /Fo.work\snake\
if errorlevel 1 exit /b 1
.work\snake\joystick.exe
