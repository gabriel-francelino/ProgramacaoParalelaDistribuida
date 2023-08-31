run:
	mpicc $(file).c -o $(file)
	mpirun -np 2 --hostfile hostfile  ./$(file)

clean:
	rm -rf 03_ping_pong 04_check_status 05_sum