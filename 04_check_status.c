
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
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
  // Initialize the MPI  environment
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  
  const int MAX_NUMBERS = 100;
  int numbers[MAX_NUMBERS];
  int numbers_amount;
  // we are assuming 2 processes for this task

  if (world_rank == 0)
  {
    // Pick a random amount of integers to send to process one 
    srand(time(NULL));
    numbers_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
    //Send the amount of integers to process one
    MPI_Send(numbers, numbers_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 sent %d numbers to 1\n", numbers_amount);

  }else if (world_rank == 1)
  {
    MPI_Status status;
    // Receive ar most MAX_NUMBERS from process zero
    MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    // After receiving the message, check the status to determine how many
    // numbers were actually received
    MPI_Get_count(&status, MPI_INT, &numbers_amount);
    // Print off the amout if numbers, and also addictional...incomplete
    printf("1 received %d numbers from 0. Message source = %d, tag = %d\n", numbers_amount, status.MPI_SOURCE, status.MPI_TAG);

  }

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}