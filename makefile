run03:
	mpicc 03_ping_pong.c -o 03_ping_pong
	mpirun -np 2 --hostfile hostfile  ./03_ping_pong

run04:
	mpicc 04_check_status.c -o 04_check_status
	mpirun -np 2 --hostfile hostfile  ./04_check_status

clean:
	rm -rf 03_ping_pong 04_check_status