// Inclui bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

// Cria um array de números aleatórios. Cada número está no intervalo de 0 a 1.
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  int i;
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

// Calcula a média de um array de números
float compute_avg(float *array, int num_elements) {
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elements; i++) {
    sum += array[i];
  }
  return sum / num_elements;
}

int main(int argc, char** argv) {
  // Verifica se o programa foi chamado com o número correto de argumentos
  if (argc != 2) {
    fprintf(stderr, "Usage: ./avg num_elements_per_proc\n");
    exit(1);
  }

  // Lê o número de elementos por processo da linha de comando
  int num_elements_per_proc = atoi(argv[1]);
  
  // Inicializa o gerador de números aleatórios com uma semente baseada no tempo
  srand(time(NULL));

  // Inicializa MPI
  MPI_Init(NULL, NULL);

  // Obtém o número do processo atual e o número total de processos
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Cria um array de números aleatórios no processo raiz (rank 0)
  float *rand_nums = NULL;
  if (world_rank == 0) {
    rand_nums = create_rand_nums(num_elements_per_proc * world_size);
  }

  // Cada processo cria um buffer para armazenar um subconjunto dos números aleatórios
  float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
  assert(sub_rand_nums != NULL);

  // Distribui os números aleatórios do processo raiz para todos os processos no mundo MPI
  MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums,
              num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // Calcula a média do subconjunto
  float sub_avg = compute_avg(sub_rand_nums, num_elements_per_proc);

  // Coleta todas as médias parciais no processo raiz
  float *sub_avgs = NULL;
  if (world_rank == 0) {
    sub_avgs = (float *)malloc(sizeof(float) * world_size);
    assert(sub_avgs != NULL);
  }
  MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // Agora que temos todas as médias parciais no processo raiz, calculamos a média total
  if (world_rank == 0) {
    float avg = compute_avg(sub_avgs, world_size);
    printf("Avg of all elements is %f\n", avg);
    // Calcula a média dos dados originais para comparação
    float original_data_avg =
      compute_avg(rand_nums, num_elements_per_proc * world_size);
    printf("Avg computed across original data is %f\n", original_data_avg);
  }

  // Limpeza de memória
  if (world_rank == 0) {
    free(rand_nums);
    free(sub_avgs);
  }
  free(sub_rand_nums);

  // Sincronização para garantir que todos os processos cheguem a este ponto
  MPI_Barrier(MPI_COMM_WORLD);
  // Finaliza MPI
  MPI_Finalize();
}
