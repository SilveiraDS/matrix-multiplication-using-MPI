/**Trabalho 2 da Disciplina de IPPD - CMP134
 * 
 * Dieison Silveira
 * 
 * Produto de matrizes NxN com MPI
 * 
 * A matriz A é dividida pela quantidade de nodos passados
 * cada uma das partes de A é enviada para os processos filhos e nestes processos
 * o cálculo do produto é realizado.
 * 
 * As matrizes são quadradas NxN e devem atender a restrição: N % nodos == 0
 * */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

#define RAIZ 0
#define CMP134_DECLARE_TIMER double cmp134_t1, cmp134_t2;
#define CMP134_START_TIMER  cmp134_t1 = cmp134_gettime();
#define CMP134_END_TIMER    cmp134_t2 = cmp134_gettime();
#define CMP134_REPORT_TIMER {printf("CMP134:%f\n", cmp134_t2-cmp134_t1);}

double cmp134_gettime (void);
float *malloc_matriz(int, int);
void fill_matriz(float *, int, int);

int main(int argc, char *argv[]) {
	float *A, *B, *C, *auxC, *auxA;
	double startTime, endTime;
	int stripSize, rank, nodos, N, i, j, k, numElements;
	CMP134_DECLARE_TIMER;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nodos);
	
	N = atoi(argv[1]);

/**quantidade de linhas que cada processo filho terá da matriz A*/
	stripSize = N / nodos;
	numElements = N * stripSize;
	
/**start timer*/ 
	if (rank == RAIZ && argc == 3) {
		startTime = MPI_Wtime();
	}

/**  alocação das matrizes A, B e C, as matrizes dos processos filhos são menores
 * O buffer de envio tem que ser diferente do buffer de recebimento
 O raiz tem as matrizes A,B e C com tamanho NxN
 E todos os processos possuem matrizes auxiliares para A e C, as quais tem tamanho somente para o necessario
*/	
	if (rank == RAIZ) {
		A = malloc_matriz(N, N);
		C = malloc_matriz(N, N);
	}
	auxA = malloc_matriz(stripSize, N);
	B = malloc_matriz(N, N);
	auxC = malloc_matriz(stripSize, N);
	

/**inicializa as matrizes A e B com um valor aleatorio*/
	if (rank == RAIZ) {
		srand(time(NULL));
		fill_matriz(A, N, N);
		fill_matriz(B, N, N);
	}
	CMP134_START_TIMER;

/**todos processos obtem parte da matriz A*/
	//       (send,  tamanho,    tipo,   receive, tamanho,     tipo,   mestre, comunicador)
	MPI_Scatter(A, numElements, MPI_FLOAT, auxA, numElements, MPI_FLOAT, RAIZ, MPI_COMM_WORLD);
	
/**todos processos obtem a matriz B*/
	MPI_Bcast(B, N*N, MPI_FLOAT, RAIZ, MPI_COMM_WORLD);

	/**calculo do produto de matriz*/
	for (i=0; i<stripSize; i++) { 
		for (j=0; j<N; j++) {
			auxC[i*N+j] = 0;
			for (k=0; k<N; k++) {
				auxC[i*N+j] = auxC[i*N+j] + (auxA[i*N+k] * B[k*N+j]);
			}
		}
	}

	/** cada processo envia para a raiz (matriz C) o calculo realizado*/
	//        (send,    tamanho,     tipo,  receive,   tamanho,      tipo,   mestre, comunicador)
	MPI_Gather(auxC, numElements, MPI_FLOAT, C, numElements, MPI_FLOAT, RAIZ, MPI_COMM_WORLD);

/**stop timer*/
	if (rank == RAIZ && argc == 3) {
		endTime = MPI_Wtime();
		printf("Time is %f\n", endTime-startTime);
	}

//**stop timer
	CMP134_END_TIMER;
	CMP134_REPORT_TIMER;

	MPI_Finalize();
	return 0;
}

float *malloc_matriz(int l, int c) {
 	float *matriz;
    if ((matriz = malloc(l*c * sizeof (float*))) == NULL) {
        fprintf(stderr, "Nao foi possivel alocar memoria!\n");
        exit(1);
    }
    return matriz;
}


void fill_matriz(float *matriz, int l, int c){
	int i,j;
	for (i=0; i<l*c; i++) {
		matriz[i] = ((float)rand() / (float) RAND_MAX) * INT_MAX;
	}
}

double cmp134_gettime (void){
  struct timeval tr;
  gettimeofday(&tr, NULL);
  return (double)tr.tv_sec+(double)tr.tv_usec/1000000;
}
