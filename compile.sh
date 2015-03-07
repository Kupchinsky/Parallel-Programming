#!/bin/bash
export LANG=ru_RU.cp1251
echo Compiling $1...
mpic++ "mpi/labs/src/$1" -o "mpi/labs/bin/${1%.*cpp}"
echo Exit code: $?