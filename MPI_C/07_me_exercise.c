/**

    EXERCICIO

    1) Gerar um vetor de 1000 elemento, dividir em P processos e encontrar o menor valor do vetor.

    2) Gerar 2 matrizes e realizar a soma entre elas em uma 3ª matriz, onde cada linha será calculada em um processo.

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Tamanho do vetor e quantidade de elementos por processo
  int total_elements = 1000;
  int elements_per_process = total_elements / world_size;

  int* local_data = (int*)malloc(sizeof(int) * elements_per_process);

  if (world_rank == 0) {
    // Processo raiz gera o vetor de 0 a 1000
    int* data = (int*)malloc(sizeof(int) * total_elements);
    for (int i = 0; i < total_elements; i++) {
      data[i] = i; // Preenche o vetor com valores de 0 a 1000
    }

    // Distribuir os dados para todos os processos
    MPI_Scatter(data, elements_per_process, MPI_INT, local_data, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    free(data);
  } else {
    // Processos não raiz recebem os dados
    MPI_Scatter(NULL, 0, MPI_INT, local_data, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);
  }

  // Calcular a média local
  double local_sum = 0.0;
  for (int i = 0; i < elements_per_process; i++) {
    local_sum += local_data[i];
  }
  double local_avg = local_sum / elements_per_process;

  // Reduzir os valores locais para calcular a média global
  double global_avg;
  MPI_Reduce(&local_avg, &global_avg, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    global_avg /= total_elements; // Calcula a média final
    printf("A média final é: %lf\n", global_avg);
  }

  free(local_data);

  MPI_Finalize();
  return 0;
}


