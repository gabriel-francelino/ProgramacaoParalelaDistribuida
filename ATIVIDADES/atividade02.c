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

// int *parallelMatrixMultiplication(int *vet1, int *matrix, int size)
// {
//     int result[size];

//     for (int i = 0; i < size; i++)
//     {
//         int aux = 0;
//         for (int j = 0; j < size; j++)
//         {
//             aux += vet1[j] * matrix[j];
//         }
//         result[i] = aux;
//     }

//     return 0;
// }

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

void generateMatrix(int *matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        *matrix = rand() % 10;
        matrix++;
    }
}

// Função para calcular a matriz transposta
void transposedMatrix(int *matrix, int size, int *result)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // Acesso aos elementos da matriz usando ponteiros
            result[j * size + i] = matrix[i * size + j];
        }
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
        printf(" %d ", *matrix);
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
    int matrix1[size][size];
    int matrix2[size][size];
    int matrix2_t[size][size];
    int line_id[size];
    if (world_rank == MASTER_RANK)
    {
        generateMatrix(&matrix1[0][0], size_matrix);
        generateMatrix(&matrix2[0][0], size_matrix);
        transposedMatrix(&matrix2[0][0], size, &matrix2_t[0][0]);

        

        for (int i = 0; i < size; i++)
        {
            line_id[i] = i;
        }

        printMatrix(&matrix1[0][0], size);
        printMatrix(&matrix2[0][0], size);
        // printMatrix(&matrix2_t[0][0], size);
    }

    // Cada processo cria um buffer para armazenar os subconjuntos dos números aleatórios
    int vet_matrix1_line[size];

    // int vet_matrix2_column[size];

    // Distribui cada linha da matriz1 do processo raiz para todos os processos
    // MPI_Scatter(matrix1, size, MPI_INT, vet_matrix1_line, size,
    //             MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    int line = 0;
    MPI_Scatter(line_id, 1, MPI_INT, &line, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    // printf("rank: %d, line: %d\n", world_rank, line);

    // // Distribui cada coluna da matriz2 do processo raiz para todos os processos
    MPI_Scatter(matrix1, size, MPI_INT, &vet_matrix1_line, size, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    // MPI_Bcast(matrix1, size_matrix, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    MPI_Bcast(matrix2, size_matrix, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    int result[size];
    for (int i = 0; i < size; i++)
    {
        result[i] = 0;
        for (int j = 0; j < size; j++)
        {
            result[i] += vet_matrix1_line[j] * matrix2[j][i];
            printf("%d resultado[%d] = %d * %d = %d\n", world_rank, i, vet_matrix1_line[j], matrix2[j][line], result[i]);
            // printf(" resultado[%d] = vetM1[%d] * M2[%d][%d] = %d\n", i, j, j, line, result[i]);
        }    
        //calculo do segundo valor do vetor com erro          
        printf("\n");
    }

    // for (int i = 0; i < size; i++)
    // {
    //     printf("vet_resul[%d] = %d\n", i, result[i]);
    // }
    

    // Desalocando memória
    // if (world_rank == MASTER_RANK)
    // {
    //     freeMatrix(matrix1, size);
    //     freeMatrix(matrix2, size);
    //     freeMatrix(t_matrix2, size);
    // }
    // free(vet_matrix1_line);
    // free(vet_matrix2_column);

    // Sincronização para garantir que todos os processos cheguem a este ponto
    MPI_Barrier(MPI_COMM_WORLD);
    // Finaliza MPI
    MPI_Finalize();

    return 0;
}
