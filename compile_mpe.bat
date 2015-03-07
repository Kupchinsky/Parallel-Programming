@echo off

if "%~1"=="" exit

echo mpi/compile_mpe.sh "%~n1%~x1" > "%temp%\compile_mpe.txt"
plink -load "local-debian-vm" -pw 123 -m "%temp%\compile_mpe.txt"
pause