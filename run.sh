#!/bin/bash
filename="mpi/labs/bin/$(cat mpi/run.tmp | head -n 1 | tr -d '[[:space:]]')"
pc="$(cat mpi/run.tmp | tail -n 1 | tr -d '[[:space:]]')"

if [ ! -e "$filename" ]
then
	echo "Compiled program not found!"
	exit
fi

echo "!!Running $filename [$*] on $pc processes..."
mpiexec -n "$pc" "$filename" $*
echo "!!End running $filename"