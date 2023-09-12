// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Comparison of MPI_Bcast with the my_bcast function
//
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

void my_bcast(void* data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
  int world_rank;
  MPI_Comm_rank(communicator, &world_rank); // Obtém o rank (identificador) do processo atual
  int world_size;
  MPI_Comm_size(communicator, &world_size); // Obtém o número total de processos

  if (world_rank == root) {
    // Se este processo é o processo raiz, envia os dados para todos os outros processos
    int i;
    for (i = 0; i < world_size; i++) {
      if (i != world_rank) {
        MPI_Send(data, count, datatype, i, 0, communicator); // Envia os dados para o processo 'i'
      }
    }
  } else {
    // Se este processo não é o processo raiz, recebe os dados do processo raiz
    MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
  }
}

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: compare_bcast num_elements num_trials\n");
    exit(1);
  }

  int num_elements = atoi(argv[1]); // Número de elementos de dados
  int num_trials = atoi(argv[2]);    // Número de tentativas

  MPI_Init(NULL, NULL); // Inicializa o ambiente MPI

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // Obtém o rank (identificador) do processo atual

  double total_my_bcast_time = 0.0; // Tempo total para my_bcast
  double total_mpi_bcast_time = 0.0; // Tempo total para MPI_Bcast
  int i;
  int* data = (int*)malloc(sizeof(int) * num_elements); // Aloca espaço para o array de dados
  assert(data != NULL);

  for (i = 0; i < num_trials; i++) {
    // Mede o tempo para my_bcast
    MPI_Barrier(MPI_COMM_WORLD); // Sincroniza todos os processos
    total_my_bcast_time -= MPI_Wtime(); // Inicia a medição de tempo
    my_bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD); // Chama a função de broadcast personalizada
    MPI_Barrier(MPI_COMM_WORLD); // Sincroniza novamente todos os processos
    total_my_bcast_time += MPI_Wtime(); // Conclui a medição de tempo

    // Mede o tempo para MPI_Bcast
    MPI_Barrier(MPI_COMM_WORLD); // Sincroniza todos os processos
    total_mpi_bcast_time -= MPI_Wtime(); // Inicia a medição de tempo
    MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD); // Chama a função de broadcast MPI
    MPI_Barrier(MPI_COMM_WORLD); // Sincroniza novamente todos os processos
    total_mpi_bcast_time += MPI_Wtime(); // Conclui a medição de tempo
  }

  // Imprime informações de tempo
  if (world_rank == 0) {
    printf("Data size = %d, Trials = %d\n", num_elements * (int)sizeof(int),
           num_trials);
    printf("Avg my_bcast time = %lf\n", total_my_bcast_time / num_trials);
    printf("Avg MPI_Bcast time = %lf\n", total_mpi_bcast_time / num_trials);
  }

  free(data); // Libera a memória alocada para o array de dados
  MPI_Finalize(); // Finaliza o ambiente MPI
}
