#!/bin/bash
export LANG=ru_RU.cp1251

target_source="mpi/labs/src/$1"
target_binary="mpi/labs/bin/${1%.*cpp}"

echo Compiling $1...
echo \$ mpic++ "$target_source" -o "$target_binary"
mpic++ "$target_source" -o "$target_binary"
echo Exit code: $?