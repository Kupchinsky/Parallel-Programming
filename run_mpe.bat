@echo off

if "%~1"=="" exit

if not exist "%~dp0\labs\bin\%~n1_mpe" (
	echo First: compile program to "%~n1_mpe"!
	pause
	exit /b
)

set pc=2
set args=""
set /p "pc=(MPE) Enter process count (default 2): "
set /p "args=Enter custom arguments: "

echo mpi/run_mpe.sh %args% > "%temp%\run_mpe.txt"
echo %~n1 > "%~dp0\run_mpe.tmp"
echo %pc% >> "%~dp0\run_mpe.tmp"
plink -load "local-debian-vm" -pw 123 -m "%temp%\run_mpe.txt"
rm "%~dp0\run_mpe.tmp"
pause