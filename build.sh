gcc -std=c11 -c schedulingSim.c -g
gcc -std=c11 -c queue.c -g
gcc -std=c11 -o schedSim queue.o schedulingSim.o -g