#!/bin/bash
filename="mpi/labs/bin/$(cat mpi/run_mpe.tmp | head -n 1 | tr -d '[[:space:]]')_mpe"
pc="$(cat mpi/run_mpe.tmp | tail -n 1 | tr -d '[[:space:]]')"

if [ ! -e "$filename" ]
then
	echo "Compiled program not found!"
	exit
fi

echo "!!Running $filename [$*] on $pc processes..."
export LD_LIBRARY_PATH=/home/gcc/objdir/tempinstall/home/gcc/gcc-4.9.2/lib64
mpiexec -n "$pc" "$filename" $*
echo "!!End running $filename"

if [ -e "$filename.clog2" ]; then
	java -jar "mpi/clog2TOslog2.jar" "$filename.clog2" > /dev/null
	echo "clog2TOslog2.jar exit code: $?"
fi