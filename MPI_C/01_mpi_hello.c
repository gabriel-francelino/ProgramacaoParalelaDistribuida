// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// An intro MPI hello world program that uses MPI_Init, MPI_Comm_size,
// MPI_Comm_rank, MPI_Finalize, and MPI_Get_processor_name.
//

// Incluindo as bibliotecas necessárias para MPI e entrada/saída padrão.
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
  // Inicializa o ambiente MPI. Os dois argumentos para MPI_Init não são
  // atualmente usados pelas implementações MPI, mas estão lá no caso de futuras
  // implementações precisarem dos argumentos.
  MPI_Init(NULL, NULL);

  // Obtém o número de processos em MPI_COMM_WORLD (o comunicador padrão).
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Obtém o número de classificação (rank) do processo em MPI_COMM_WORLD.
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Obtém o nome do processador atual e o comprimento do nome.
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  // Imprime uma mensagem de "Hello World" com informações sobre o processador,
  // rank e o número total de processos.
  printf("Hello world from processor %s, rank %d out of %d processors\n",
         processor_name, world_rank, world_size);

  // Finaliza o ambiente MPI. Nenhum chamado MPI adicional pode ser feito após isso.
  MPI_Finalize();
}
