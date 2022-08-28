all: unixshell
unixshell: main
	gcc -o unixshell main.o
main: main.c
	gcc -c main.c