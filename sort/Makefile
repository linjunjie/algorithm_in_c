all : BubbleSort InsertSort ShellSort

BubbleSort : BubbleSort.o
InsertSort : InsertSort.o
ShellSort : ShellSort.o

BubbleSort.o : BubbleSort.c 
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc -c BubbleSort.c 

InsertSort.o : InsertSort.c
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc -c InsertSort.c

ShellSort.o : ShellSort.c
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc -c ShellSort.c

clean : 
	rm -rf *o BubbleSort InsertSort ShellSort
