sqrt: sqrt.c
	gcc $? -o $@ -lpthread -lm

memset: memset.c
	gcc $? -o $@ -lpthread -lm

readFiles: readFiles.c
	gcc $? -o $@ -lpthread -lm

sase:
	mpicc sase.c -o sase && time mpirun -np 10 sase && rm sase

clean:
	rm -f sqrt memset readFiles sase

