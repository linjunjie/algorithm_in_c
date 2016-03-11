#需要包含的头文件
INCLUS = -I..

all : BubbleSort InsertSort ShellSort BinInsertSort

BubbleSort : BubbleSort.o
InsertSort : InsertSort.o
ShellSort : ShellSort.o
BinInsertSort : BinInsertSort.o

BubbleSort.o : BubbleSort.c 
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc $(INCLUS) -c BubbleSort.c 

InsertSort.o : InsertSort.c
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc $(INCLUS) -c InsertSort.c

ShellSort.o : ShellSort.c
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc $(INCLUS) -c ShellSort.c

BinInsertSort.o : BinInsertSort.c
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc $(INCLUS) -c BinInsertSort.c

clean : 
	rm -rf *o BubbleSort InsertSort ShellSort BinInsertSort
