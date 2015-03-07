@echo off

if "%~1"=="" exit

echo mpi/compile.sh "%~n1%~x1" > "%temp%\compile.txt"
plink -load "local-debian-vm" -pw 123 -m "%temp%\compile.txt"
pause