#需要包含的头文件
INCLUS = -I..

all : LinkedList

LinkedList : LinkedList.o

LinkedList.o : LinkedList.c 
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc $(INCLUS) -c LinkedList.c 

clean : 
	rm -rf *o LinkedList
