#!/bin/bash
export LANG=ru_RU.cp1251
echo Compiling $1...
cd mpi/labs/bin
mpic++ -c "../src/$1"
EXITCODE=$?

if [ $EXITCODE -gt 0 ]; then
	echo Failed! $EXITCODE
	exit
fi

echo Linking $1 with MPE...
mpecc -lstdc++ -lm -mpilog "${1%.*cpp}.o" -o "../bin/${1%.*cpp}_mpe"
EXITCODE=$?

if [ $EXITCODE -gt 0 ]; then
	echo Failed! $EXITCODE
	exit
fi

rm "${1%.*cpp}.o"
echo Exit code: 0