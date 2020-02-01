reduce:
	mpicc reduce.c -lm -o reduce && mpirun -np 16 reduce

scan:
	mpicc scan.c -lm -o scan && mpirun -np 16 scan

sort:
	mpicc sort.c -lm -o sort && mpirun -np 16 sort

polynomialFunction:
	mpicc polynomialFunction.c -lm -o polynomialFunction && mpirun -np 16 polynomialFunction

clean:
	clear && rm -rf reduce scan sort polynomialFunction