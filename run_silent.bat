@echo off

if "%~1"=="" exit

if not exist "%~dp0\labs\bin\%~n1" (
	echo First: compile program to "%~n1"!
	pause
	exit /b 255
)

set pc=%~2
set args=%~3

echo mpi/run.sh %args% > "%temp%\run.txt"
echo %~n1 > "%~dp0\run.tmp"
echo %pc% >> "%~dp0\run.tmp"
plink -load "local-debian-vm" -pw 123 -m "%temp%\run.txt"
set err=%ERRORLEVEL%
rm "%~dp0\run.tmp"
exit /b %err%