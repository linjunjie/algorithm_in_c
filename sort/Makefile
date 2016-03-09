all : BubbleSort InsertSort

BubbleSort : BubbleSort.o
InsertSort : InsertSort.o

BubbleSort.o : BubbleSort.c 
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc -c BubbleSort.c 

InsertSort.o : InsertSort.c
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc -c InsertSort.c

clean : 
	rm -rf *o BubbleSort InsertSort
