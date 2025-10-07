all:
	gcc -c c8vm.c
	gcc -c main.c
	gcc -o chip8 c8vm.o main.o
