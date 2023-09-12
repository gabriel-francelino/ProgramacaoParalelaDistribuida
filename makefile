np ?= 2
compile:
	mpicc $(file).c -o $(file)

run:
	mpicc $(file).c -o $(file)
	mpirun -np $(np) --hostfile hostfile  ./$(file)

clean:
	rm -rf 03_ping_pong 04_check_status 05_sum 06_compare_bcast 06_me_exercise 07_avg