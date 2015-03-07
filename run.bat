@echo off

if "%~1"=="" exit

if not exist "%~dp0\labs\bin\%~n1" (
	echo First: compile program to "%~n1"!
	pause
	exit /b
)

set pc=2
set args=""
set /p "pc=Enter process count (default 2): "
set /p "args=Enter custom arguments: "

echo mpi/run.sh %args% > "%temp%\run.txt"
echo %~n1 > "%~dp0\run.tmp"
echo %pc% >> "%~dp0\run.tmp"
plink -load "local-debian-vm" -pw 123 -m "%temp%\run.txt"
rm "%~dp0\run.tmp"
pause