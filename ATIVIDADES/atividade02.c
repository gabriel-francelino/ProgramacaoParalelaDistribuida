/**
 * cada linha deve ser processada por um processo diferente
 *
 * gerar matriz e imprimir no processo raiz
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <assert.h>

int matrixMultiplication() { return 0; }

/**
 * Cria uma matriz de números aleatórios. Cada número está no intervalo de 0 a 9.
 * 
 * @param lines número de linhas
 * @param columns número de colunas
 * 
 * @return Uma matrix alocada dinamicamente
*/
int **create_matrix(int lines, int columns)
{
    // Alocando linhas da matriz
    int **matrix = (int **)malloc(sizeof(int *) * lines);
    assert(matrix != NULL);

    // Alocando colunas da matriz
    for (int i = 0; i < lines; i++)
    {
        matrix[i] = (int *)malloc(sizeof(int) * columns);
        assert(matrix[i] != NULL);
    }

    // Criando ponteiro para percorrer a matriz
    int *ptr = *matrix;
    // Inicializando a matriz com valores de 0 a 9
    for (int i = 0; i < (lines * columns); i++)
    {
        *ptr = rand() % 10;
        ptr++;
    }

    

    return matrix;
}

/**
 * Imprime os valores da matriz no terminal
 * 
 * @param matrix Matriz que deseja imprimir
 * @param size Tamanho da matriz
*/
void printMatrix(int **matrix, int size){
    int *ptr = *matrix;

    for (int i = 0; i < size; i++)
    {
        printf("p[%d]= %d\n", i, *ptr);
        ptr++;
    }
}

int main(int argc, char const *argv[])
{
    // Verifica se o programa foi chamado com o número correto de argumentos
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./avg num_elements_per_proc\n");
        exit(1);
    }

    // Lê o número de elementos por processo da linha de comando
    int n_lines = atoi(argv[1]);
    int n_columns = atoi(argv[2]);

    // Inicializa o gerador de números aleatórios com uma semente baseada no tempo
    srand(time(NULL));

    // Inicializa MPI
    MPI_Init(NULL, NULL);
    // Obtém o número do processo atual e o número total de processos
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Cria um array de números aleatórios no processo raiz (rank 0)
    int **matrix = NULL;
    if (world_rank == 0)
    {
        matrix = create_matrix(n_lines, n_columns);
        printMatrix(matrix, (n_lines*n_columns));
    }

    // Sincronização para garantir que todos os processos cheguem a este ponto
    MPI_Barrier(MPI_COMM_WORLD);
    // Finaliza MPI
    MPI_Finalize();

    return 0;
}
