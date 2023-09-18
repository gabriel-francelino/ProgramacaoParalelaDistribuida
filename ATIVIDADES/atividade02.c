/**
 * cada linha deve ser processada por um processo diferente
 *
 * gerar matriz e imprimir no processo raiz
 *
 * o numero de linhas(m) da primeira matriz deve ser igual ao numero de colunas (n) da segunda matriz
 *
 * m = n = número de processos
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <assert.h>

#define MASTER_RANK 0

int parallelMatrixMultiplication(int *vet1, int *vet2, int size1, int size2)
{
    int result[size2];

    for (int i = 0; i < size1; i++)
    {
        for (int i = 0; i < size2; i++)
        {
        }
    }

    return 0;
}

/**
 * Cria uma matriz de números aleatórios. Cada número está no intervalo de 0 a 9.
 *
 * @param lines número de linhas
 * @param columns número de colunas
 *
 * @return Uma matrix alocada dinamicamente
 */
int **create_matrix(int size)
{
    // Alocando linhas da matriz
    int **matrix = (int **)malloc(sizeof(int *) * size);
    assert(matrix != NULL);

    // Alocando colunas da matriz
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (int *)malloc(sizeof(int) * size);
        assert(matrix[i] != NULL);
    }

    // Criando ponteiro para percorrer a matriz
    int *ptr = *matrix;
    // Inicializando a matriz com valores de 0 a 9
    for (int i = 0; i < (size * size); i++)
    {
        *ptr = rand() % 10;
        ptr++;
    }

    return matrix;
}

// Função para calcular a matriz transposta
void transposedMatrix(int **t_matrix, int **matriz, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            t_matrix[i][j] = matriz[j][i];
        }
    }
}

/**
 * Imprime os valores da matriz no terminal
 *
 * @param matrix Matriz que deseja imprimir
 * @param size Tamanho da matriz
 */
void printMatrix(int **matrix, int size)
{
    int *ptr = *matrix;

    for (int i = 0; i < size; i++)
    {
        printf("p[%d]= %d\n", i, *ptr);
        ptr++;
    }
    printf("\n");
}

/**
 * Desaloca memória da matriz
 *
 * @param matrix Matriz a ser liberada da memória
 * @param n_lines Número de linhas da matriz
 */
void freeMatrix(int **matrix, int n_lines)
{
    for (int i = 0; i < n_lines; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, char const *argv[])
{
    // Verifica se o programa foi chamado com o número correto de argumentos
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./atividade02 <dimensao_matriz>\n");
        exit(1);
    }

    // Lê o número de elementos por processo da linha de comando
    int size = atoi(argv[1]);
    int size_matrix = size * size;

    // Inicializa o gerador de números aleatórios com uma semente baseada no tempo
    srand(time(NULL));

    // Inicializa MPI
    MPI_Init(NULL, NULL);
    // Obtém o número do processo atual e o número total de processos
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Cria as matrizers de números aleatórios no processo raiz (rank 0)
    int **matrix1 = NULL;
    int **matrix2 = NULL;
    int **t_matrix2 = NULL;
    if (world_rank == MASTER_RANK)
    {
        matrix1 = create_matrix(size);
        matrix2 = create_matrix(size);
        t_matrix2 = create_matrix(size);
        transposedMatrix(t_matrix2, matrix2, size);

        printMatrix(matrix1, (size_matrix));
        printMatrix(matrix2, (size_matrix));
        printMatrix(t_matrix2, (size_matrix));
    }

    // Cada processo cria um buffer para armazenar os subconjuntos dos números aleatórios
    int *vet_matrix1_line = (int *)malloc(sizeof(int) * size);
    int *vet_matrix2_column = (int *)malloc(sizeof(int) * size);

    // Distribui cada linha da matriz1 do processo raiz para todos os processos
    MPI_Scatter(matrix1, size, MPI_INT, vet_matrix1_line, size,
                MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    // Distribui cada coluna da matriz2 do processo raiz para todos os processos
    MPI_Scatter(matrix2, size, MPI_INT, vet_matrix2_column, size,
                MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    // Desalocando memória
    if (world_rank == MASTER_RANK)
    {
        freeMatrix(matrix1, size);
        freeMatrix(matrix2, size);
        freeMatrix(t_matrix2, size);
    }
    free(vet_matrix1_line);
    free(vet_matrix2_column);

    // Sincronização para garantir que todos os processos cheguem a este ponto
    MPI_Barrier(MPI_COMM_WORLD);
    // Finaliza MPI
    MPI_Finalize();

    return 0;
}
