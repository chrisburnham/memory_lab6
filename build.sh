gcc -std=c11 -c memorySim.c -g
gcc -std=c11 -c freelist.c -g
gcc -std=c11 -o memSim freelist.o memorySim.o -g