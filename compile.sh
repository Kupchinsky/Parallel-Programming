#!/bin/bash
export LANG=ru_RU.cp1251
cd ~/mpi/labs/src

target_source="$1"
target_binary="../bin/${1%.*cpp}"

echo Compiling $1...
echo \$ mpic++ -profile=mpicxx "$target_source" -o "$target_binary"
mpic++ -profile=mpicxx --version | head -n1
mpic++ -profile=mpicxx "$target_source" -o "$target_binary"
echo Exit code: $?