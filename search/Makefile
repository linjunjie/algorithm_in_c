#需要包含的头文件
INCLUS = -I..

all : BinarySearch

BinarySearch : BinarySearch.o

BinarySearch.o : BinarySearch.c 
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc $(INCLUS) -c BinarySearch.c 

clean : 
	rm -rf *o BinarySearch
