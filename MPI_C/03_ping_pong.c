
// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// An intro MPI hello world program that uses MPI_Init, MPI_Comm_size,
// MPI_Comm_rank, MPI_Finalize, and MPI_Get_processor_name.
//
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
  const int PING_PONG_LIMIT = 10;

  // Initialize the MPI  environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // we are assuming 2 processes for this task
  if(world_size != 2) {
    fprintf(stderr, "World size nust be two for %s\n",  argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int ping_pong_count = 0;
  int partner_rank = (world_rank + 1) % 2;

  while(ping_pong_count < PING_PONG_LIMIT){
    // Increment the ping pong count before you send it
    ping_pong_count++;

    if(world_rank == ping_pong_count % 2){
      MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
      printf("%d sent and increment ping_pong_count %d to %d\n", world_rank, ping_pong_count, partner_rank);
    }else{
      MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%d receive and increment ping_pong_count %d to %d\n", world_rank, ping_pong_count, partner_rank);
    }

  // Finalize the MPI environment. No more MPI calls can be made after this
  }
  MPI_Finalize();
}