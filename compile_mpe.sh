#!/bin/bash
export LANG=ru_RU.cp1251

cd mpi/labs/bin

target_source="../src/$1"
target_binary="../bin/${1%.*cpp}_mpe"

check_exit_code()
{
	if [ $1 -gt 0 ]; then
		echo Failed! Exit code: $1
		exit
	fi
}

echo Compiling $1...

echo \$ mpic++ -profile=mpicxx -c "$target_source"
mpic++ -profile=mpicxx --version | head -n1
mpic++ -profile=mpicxx -c "$target_source"
check_exit_code $?

echo Linking $1 with MPE...
echo \$ mpecc -profile=mpicxx -lstdc++ -lm -mpilog "${1%.*cpp}.o" -o "$target_binary"
mpecc -profile=mpicxx -mpicc=mpic++ -mpilog --version | head -n1
mpecc -profile=mpicxx -mpicc=mpic++ -lm -mpilog "${1%.*cpp}.o" -o "$target_binary"
check_exit_code $?

rm "${1%.*cpp}.o"
check_exit_code $?