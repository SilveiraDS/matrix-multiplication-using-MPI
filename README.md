Trabalho 2 da Disciplina de IPPD - CMP134

Dieison Silveira

Produto de matrizes NxN com MPI

A matriz A é dividida pela quantidade de nodos passados
cada uma das partes de A é enviada para os processos filhos e nestes processos
o cálculo do produto é realizado.
As matrizes são quadradas NxN e devem atender a restrição: N % nodos == 0

O tempo de execucao pode ser medido pela função "MPI_Wtime()", esse tempo eh medido com inicio antes das alocações dos vetores e fim antes da função "MPI_Finalize()"

Compilar:
$ cd dieison
$ make

Executar com a função "MPI_Wtime()" habilitada:
$ mpirun -np <#nodos> ./main <#size> -time

Executar sem a função "MPI_Wtime()":
$ mpirun -np <#nodos> ./main <#size>
