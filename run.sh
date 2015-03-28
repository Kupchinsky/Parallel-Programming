#!/bin/bash
filename="mpi/labs/bin/$(cat mpi/run.tmp | head -n 1 | tr -d '[[:space:]]')"
pc="$(cat mpi/run.tmp | tail -n 1 | tr -d '[[:space:]]')"

if [ ! -e "$filename" ]
then
	echo "Compiled program not found!"
	exit
fi

echo "!!Running $filename [$*] on $pc processes..."
export LD_LIBRARY_PATH=/home/gcc/objdir/tempinstall/home/gcc/gcc-4.9.2/lib64
mpiexec -n "$pc" "$filename" $*
echo "!!End running $filename"