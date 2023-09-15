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
int calculateAverage(int *arr, int size)
{
    int sum = calculateSum(arr, size);
    int avg = sum / size;
    return avg;
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

// Função para comparar dois valores usando qsort
int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

// Função para calcular a mediana de um vetor de inteiros
int calculateMedian(int *arr, int size)
{
    // Ordena o vetor
    qsort(arr, size, sizeof(int), compare);

    // Se o tamanho do array for par, retorna a média dos dois elementos do meio
    if (size % 2 == 0)
    {
        int middle1 = arr[(size - 1) / 2];
        int middle2 = arr[size / 2];
        return (double)(middle1 + middle2) / 2.0;
    }

    // Se o tamanho for ímpar, retorne o elemento do meio
    else
    {
        return (double)arr[size / 2];
    }
}

int main(int argc, char **argv)
{
    int world_size;                             // Quantidade de processos
    int rank;                                   // rank dos processos
    MPI_Status status;                          // Variável para obter os status dos envios
    MPI_Init(NULL, NULL);                       // Inicializa o ambiente MPI.
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Obtém o número total de processos em execução.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);       // Obtém o rank (identificador) do processo atual.

    if (rank == MASTER_RANK)
    {
        srand(time(NULL)); // // Inicializa a semente do gerador de números pseudoaleatórios com o tempo atual do sistema.
        int tag_task = 0;  // TAG que vai definir a tarefa a ser feita
        int total_task = (rand() % 91) + 10; // Gera uma quantidade de tarefas de 10 à 100
        // int total_task = 20; // Quantidade de tarefas para teste
        int new_dest;        // Recebe o rank da variável que termina a tarefa

        // Enviando primeira rodada de tarefas para todos os processos
        for (int dest = 1; dest < world_size; dest++)
        {
            int numbers_amount = (rand() % 1001) + 1000; // Gerando números aleatórios entre 1000 e 2000
            tag_task = (rand() % 4);                     // Gerando task aleatória de 0 a 3
            int numbers[numbers_amount];                 // Vetor de números aleatórios

            for (int i = 0; i < numbers_amount; i++)
            {
                numbers[i] = rand() % 100; // Gerando números de 0 a 99
            }

            MPI_Send(&numbers_amount, 1, MPI_INT, dest, tag_task, MPI_COMM_WORLD);      // Envia a quantidade de números
            MPI_Send(numbers, numbers_amount, MPI_INT, dest, tag_task, MPI_COMM_WORLD); // Envia o vetor de números
            total_task--;
            // printf("Total tasks: %d\n", total_task);
        }

        // Enviando o restante das tarefas
        while (total_task)
        {
            // Recebendo  resultado do processo
            int result = 0; // Resultado da operação

            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // Recebendo a quantidade de números
            printf("Operação: %d, Processo: %d, Resultado: %d\n", status.MPI_TAG, status.MPI_SOURCE, result);

            // Enviando outra tarefa para o processo ocioso
            int numbers_amount = (rand() % 1001) + 1000; // Gerando números aleatórios entre 1000 e 2000
            tag_task = (rand() % 4);                     // Gerando task aleatória de 0 a 3
            int numbers[numbers_amount];                 // Vetor de números aleatórios

            // Preenchendo vetor com valores aleatórios entre 0 à 99
            for (int i = 0; i < numbers_amount; i++)
            {
                numbers[i] = rand() % 100;
            }

            new_dest = status.MPI_SOURCE; // Recebe o rank do processo que enviou a tarefa
            MPI_Send(&numbers_amount, 1, MPI_INT, new_dest, tag_task, MPI_COMM_WORLD);
            MPI_Send(numbers, numbers_amount, MPI_INT, new_dest, tag_task, MPI_COMM_WORLD);
            total_task--;
            // printf("Total tasks: %d\n", total_task);
        }

        // Enviando tag para processos finalizarem
        for (int i = 0; i < world_size; i++)
        {
            int finish = 0;
            MPI_Send(&finish, 1, MPI_INT, i, TASK_FINISH, MPI_COMM_WORLD);
        }
    }
    else
    {
        // Recebendo tarefas
        int recv_tag_task = 0;

        while (1)
        {

            int recv_numbers_amount = 0;
            MPI_Recv(&recv_numbers_amount, 1, MPI_INT, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // Recebendo a quantidade de números

            recv_tag_task = status.MPI_TAG;
            if (recv_tag_task == 10)
            {
                printf("Processo %d finalizado!\n", rank);
                break;
            }

            int recv_numbers[recv_numbers_amount];                                                                   // Vetor de números recebidos
            MPI_Recv(recv_numbers, recv_numbers_amount, MPI_INT, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // Recebendo valores do vetor de números

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

            sleep(1);                                                                  // Aguardando para enviar a resposta
            MPI_Send(&result, 1, MPI_INT, MASTER_RANK, recv_tag_task, MPI_COMM_WORLD); // Envia resultado da opaeração
            // sleep(1);
        }

        // MPI_Bcast(&TASK_FINISH, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
