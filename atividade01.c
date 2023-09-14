#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>

#define MASTER_RANK 0
#define TAG_TASK 0
#define TASK_FINISH 10
#define TAG_RESULT 1

// Função para calcular a soma de um vetor de inteiros
int calculateSum(int *arr, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return sum;
}

// Função para calcular a média de um vetor de inteiros
double calculateAverage(int *arr, int size)
{
    int sum = calculateSum(arr, size);
    return (double)sum / size;
}

// Função para encontrar o maior valor em um vetor de inteiros
int findMax(int *arr, int size)
{
    int max = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

// Função para calcular a mediana de um vetor de inteiros
int calculateMedian(int *arr, int size)
{
    int *sortedArr = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        sortedArr[i] = arr[i];
    }
    // Ordena o vetor
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (sortedArr[i] > sortedArr[j])
            {
                int temp = sortedArr[i];
                sortedArr[i] = sortedArr[j];
                sortedArr[j] = temp;
            }
        }
    }
    int median;
    if (size % 2 == 0)
    {
        median = (sortedArr[size / 2 - 1] + sortedArr[size / 2]) / 2;
    }
    else
    {
        median = sortedArr[size / 2];
    }
    free(sortedArr);
    return median;
}

int main(int argc, char **argv)
{
    int world_size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER_RANK)
    {
        srand(time(NULL));
        int tag_task = 0;                    // TAG que vai definir a tarefa a ser feita
        int total_task = (rand() % 51) + 50; // Gera uma quantidade de tarefas de 0 à 100

        // Enviando primeira rodada de tarefas
        for (int dest = 1; dest < world_size; dest++)
        {
            int numbers_amount = (rand() % 1001) + 1000; // Gerando números aleatórios entre 1000 e 2000
            tag_task = (rand() % 4);                     // Gerando task aleatória de 0 a 3
            int numbers[numbers_amount];                 // Vetor de números aleatórios

            for (int i = 0; i < numbers_amount; i++)
            {
                numbers[i] = rand() % 100; // Gerando números de 0 a 99
            }

            MPI_Send(&numbers_amount, 1, MPI_INT, dest, TAG_TASK, MPI_COMM_WORLD);
            MPI_Send(numbers, numbers_amount, MPI_INT, dest, TAG_TASK, MPI_COMM_WORLD);
            MPI_Send(&tag_task, 1, MPI_INT, dest, TAG_TASK, MPI_COMM_WORLD);
            total_task--;
        }

        // Enviando o restante das tarefas
        while (total_task)
        {
            int result = 0;
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recebendo a quantidade de números
            printf("Resultado da operação: %d\n", result);
        }
    }
    else
    {
        // Recebendo tarefas
        int recv_tag_task = 0;

        while (recv_tag_task != 10)
        {

            int recv_numbers_amount = 0;                                                                                    // Quantidade de némeros recebidas
            MPI_Recv(&recv_numbers_amount, 1, MPI_INT, MASTER_RANK, TAG_TASK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);           // Recebendo a quantidade de números
            int recv_numbers[recv_numbers_amount];                                                                          // Vetor de números recebidos
            MPI_Recv(recv_numbers, recv_numbers_amount, MPI_INT, MASTER_RANK, TAG_TASK, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recebendo valores do vetor de números
            MPI_Recv(&recv_tag_task, 1, MPI_INT, MASTER_RANK, TAG_TASK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);                 // Recebendo valores do vetor de números

            int result = 0;
            switch (recv_tag_task)
            {
            case 0: // Soma
                result = calculateSum(recv_numbers, recv_numbers_amount);
                break;
            case 1: // Média
                result = calculateAverage(recv_numbers, recv_numbers_amount);
                break;
            case 2: // Maior valor
                result = findMax(recv_numbers, recv_numbers_amount);
                break;
            case 3: // Mediana
                result = calculateMedian(recv_numbers, recv_numbers_amount);
                break;
            default:
                break;
            }

            MPI_Send(&result, 1, MPI_INT, MASTER_RANK, TAG_RESULT, MPI_COMM_WORLD);
            
        }

        // Tag recebida associada a tarefa
    }

    MPI_Finalize();
    return 0;
}
