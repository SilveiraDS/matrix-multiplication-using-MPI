all:
	mpicc desafio2.c -o main

lab:
	mpicc.mpich2 desafio2.c -o main

clean:
	rm *~
