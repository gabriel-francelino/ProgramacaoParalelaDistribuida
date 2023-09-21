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

/**
 * Gera valores aleatórios para uma matriz
 * 
 * @param matrix Matriz a receber os valores
 * @param size tamanho da matriz
*/
void generateMatrix(int *matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        *matrix = rand() % 10;
        matrix++;
    }
}

/**
 * Imprime os valores da matriz no terminal
 *
 * @param matrix Matriz que deseja imprimir
 * @param size Tamanho da matriz
 */
void printMatrix(int *matrix, int size)
{
    int count = 0;
    for (int i = 0; i < (size * size); i++)
    {
        printf("[");
        printf("\t%d\t", *matrix);
        printf("]");
        count++;
        matrix++;
        if (count >= size)
        {
            printf("\n");
            count = 0;
        }
    }
    printf("\n");
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

    if (world_size != size)
    {
        fprintf(stderr, "O número de processos deve ser igual ao tamanho da matriz!\n");
        exit(1);
    }

    // Cria as matrizers de números aleatórios no processo raiz (rank 0)
    int matrix1[size][size];
    int matrix2[size][size];
    if (world_rank == MASTER_RANK)
    {
        generateMatrix(&matrix1[0][0], size_matrix);
        generateMatrix(&matrix2[0][0], size_matrix);

        puts("Matriz 1:");
        printMatrix(&matrix1[0][0], size);
        puts("Matriz 2:");
        printMatrix(&matrix2[0][0], size);
    }

    // Cada processo cria um buffer para armazenar os subconjuntos dos números aleatórios
    int vet_matrix1_line[size];

    // Distribui cada linha da matriz1 do processo raiz para todos os processos
    MPI_Scatter(matrix1, size, MPI_INT, &vet_matrix1_line, size, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    // // Distribui cada coluna da matriz2 do processo raiz para todos os processos
    MPI_Bcast(matrix2, size_matrix, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    // Realiza o calculo da multiplicação de cada linha da matriz
    int result[size];
    for (int i = 0; i < size; i++)
    {
        result[i] = 0;
        for (int j = 0; j < size; j++)
        {
            result[i] += vet_matrix1_line[j] * matrix2[j][i];
        }    
    }

    // Obtem os resultados dos outros processos e une em uma matriz
    int matrix_result[size][size];
    MPI_Gather(&result, size, MPI_INT, matrix_result, size, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    // Processo master imprime a matriz resultante
    if (world_rank == MASTER_RANK)
    {
        puts("Matriz Resultante:");
        printMatrix(&matrix_result[0][0], size);
    }

    // Sincronização para garantir que todos os processos cheguem a este ponto
    MPI_Barrier(MPI_COMM_WORLD);

    // Finaliza MPI
    MPI_Finalize();

    return 0;
}
