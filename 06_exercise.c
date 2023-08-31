/**
 * Criar um modelo master/sleve onde o processo 0 é o master que "dispara" tarefas diversificadas
 * para os slaves. Os slaves saberão qual tarefa executar pelo valor do TAG recebido. Assim que um sleve terminar a tarefa,
 * o master deverá atribuir uma nova tarefa.
 * 
 * Dica: Usar um sleep para controlar o fluxo.
 * 
 * Tarefas:
 *      TAG0: Soma dos valores
 *      TAG1: Média dos valores
 *      TAG2: Maior valor
 *      TAG3: Mediana dos valores
 *      TAG10: Finaliza o processo
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int sum(int *numbers, int number_amount){
  int soma = 0;
  for (int i = 0; i < number_amount; i++)
    soma += numbers[i];
  return soma;
}

int mean(int *numbers, int number_amount){
  int soma = 0;
  for (int i = 0; i < number_amount; i++)
    soma += numbers[i];
  return soma/number_amount;
}

int median(int *numbers, int number_amount){
  int soma = 0;
  for (int i = 0; i < number_amount; i++)
    soma += numbers[i];
  return soma/number_amount;
}

int max(int *numbers, int number_amount){
  int max = numbers[0];
  for (int i = 1; i < number_amount; i++)
    if(numbers[i] > max)
      max = numbers[i];
  return max;
}

int main(int argc, char** argv) {
  MPI_Status status;
  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 2000;
  int numbers[MAX_NUMBERS];
  int number_amount;

  if (world_rank == 0) {
    int soma = 0, proc, somalocal = 0;

    // Processo Mestre (Rank 0)
    srand(time(NULL));

    // Envia números aleatórios para processos trabalhadores
    for(int p = 1; p < world_size; p++){
      number_amount = 1000 + (rand() / (float)RAND_MAX) * (MAX_NUMBERS - 1000);
      for(int i = 0; i < number_amount; i++)
        numbers[i] = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
      
      // Quantidade de tarefas
      //int total_tasks = 10;
      int tasks = 4;
      int current_task = (rand() % tasks);
          

      // Envio dos números para o processo trabalhador 'p'
      MPI_Send(numbers, number_amount, MPI_INT, p, current_task, MPI_COMM_WORLD);
      printf("0 sent %d numbers to %d\n", number_amount, p);
      somalocal += number_amount;
    }

    // Recebe as somas parciais dos processos trabalhadores
    for(int p = 1; p < world_size; p++){
      MPI_Recv(&proc, 1, MPI_INT, p, 1, MPI_COMM_WORLD, &status);
      soma += proc;
    }

    // Imprime as somas totais
    printf("A soma total foi %d e a soma local foi %d\n", soma, somalocal);

  } else {
    int result = 0, tag = 0;
    MPI_Status status;

    MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    tag = status.MPI_TAG;
    printf("Em %d, recebi a tag %d\n", world_rank, tag);

    // Processos Trabalhadores (Outros Ranks)
    MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

    // Obtém o número real de números recebidos
    MPI_Get_count(&status, MPI_INT, &number_amount);

    // Executando a tarefa de acordo com a tag
    if (tag == 0)
    {
        result = sum(numbers, number_amount);
    }
    if (tag == 1)
    {
        result = mean(numbers, number_amount);
    }
    if (tag == 2)
    {
        result = max(numbers, number_amount);
    }
    if (tag == 3)
    {
        result = median(numbers, number_amount);
    }   
    
    // Envia a soma para o processo mestre
    MPI_Send(&result, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
  }

  // Sincroniza os processos e finaliza o MPI
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}