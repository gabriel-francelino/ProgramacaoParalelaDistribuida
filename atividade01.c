#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>

#define MASTER_RANK 0
#define TAG_TASK 0
#define TAG_RESULT 1
#define TAG_FINISH 10

// Função para gerar números aleatórios entre 0 e 99
int generateRandomNumber() {
    return rand() % 100;
}

// Função para imprimir um vetor de inteiros
void printIntArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Função para calcular a soma de um vetor de inteiros
int calculateSum(int* arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

// Função para calcular a média de um vetor de inteiros
double calculateAverage(int* arr, int size) {
    int sum = calculateSum(arr, size);
    return (double)sum / size;
}

// Função para encontrar o maior valor em um vetor de inteiros
int findMax(int* arr, int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Função para calcular a mediana de um vetor de inteiros
int calculateMedian(int* arr, int size) {
    int* sortedArr = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        sortedArr[i] = arr[i];
    }
    // Ordena o vetor
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (sortedArr[i] > sortedArr[j]) {
                int temp = sortedArr[i];
                sortedArr[i] = sortedArr[j];
                sortedArr[j] = temp;
            }
        }
    }
    int median;
    if (size % 2 == 0) {
        median = (sortedArr[size / 2 - 1] + sortedArr[size / 2]) / 2;
    } else {
        median = sortedArr[size / 2];
    }
    free(sortedArr);
    return median;
}

int main(int argc, char** argv) {
    int num_processes, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + rank);

    if (rank == MASTER_RANK) {
        int num_tasks = 0;
        int total_tasks = 5; // Número total de tarefas a serem executadas

        while (num_tasks < total_tasks) {
            // Gera valores aleatórios e envia para os escravos com uma tag aleatória
            int num_values = rand() % 1001 + 1000; // Entre 1000 e 2000
            int operation = rand() % total_tasks; // Operação aleatória (0 a 4)
            operation = (operation == 4) ? 10 : operation;
            int values[num_values];
            for (int i = 0; i < num_values; i++) {
                values[i] = generateRandomNumber();
            }
            int task_tag = rand() % num_processes; // Tag aleatória para a tarefa
            
            // Envie a tarefa para o escravo com a tag aleatória
            MPI_Send(&num_values, 1, MPI_INT, task_tag, TAG_TASK, MPI_COMM_WORLD);
            MPI_Send(&operation, 1, MPI_INT, task_tag, TAG_TASK, MPI_COMM_WORLD);
            MPI_Send(values, num_values, MPI_INT, task_tag, TAG_TASK, MPI_COMM_WORLD);

            printf("Tarefa #%d - Enviando para Escravo %d:\n", num_tasks, task_tag);
            // printf("Valores: ");
            // printIntArray(values, num_values);
            printf("Operação: %d\n\n", operation);

            num_tasks++;

            // Recebe o resultado do escravo
            int result;
            MPI_Recv(&result, 1, MPI_INT, task_tag, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Resultado do Escravo %d: %d\n", task_tag, result);
        }

        // Envia mensagem de finalização para os escravos
        for (int i = 1; i < num_processes; i++) {
            MPI_Send(NULL, 0, MPI_INT, i, TAG_FINISH, MPI_COMM_WORLD);
        }
    } else {
        while (1) {
            int num_values, operation;
            MPI_Recv(&num_values, 1, MPI_INT, MASTER_RANK, TAG_TASK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&operation, 1, MPI_INT, MASTER_RANK, TAG_TASK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            if (operation == TAG_FINISH) {
                break;
            }

            int values[num_values];
            MPI_Recv(values, num_values, MPI_INT, MASTER_RANK, TAG_TASK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Realiza a operação solicitada
            int result = 0;
            switch (operation) {
                case 0: // Soma
                    result = calculateSum(values, num_values);
                    break;
                case 1: // Média
                    result = calculateAverage(values, num_values);
                    break;
                case 2: // Maior valor
                    result = findMax(values, num_values);
                    break;
                case 3: // Mediana
                    result = calculateMedian(values, num_values);
                    break;
                default:
                    break;
            }

            // Aguarde um tempo para simular a execução
            sleep(1);

            // Envie o resultado de volta para o mestre
            MPI_Send(&result, 1, MPI_INT, MASTER_RANK, TAG_RESULT, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
