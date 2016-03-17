#需要包含的头文件
INCLUS = -I..

all : SingleLinkedList DoubleLinkedList

SingleLinkedList : SingleLinkedList.o
DoubleLinkedList : DoubleLinkedList.o

SingleLinkedList.o : SingleLinkedList.c
	#In the Makefile syntax, the command must start with the [tab]
	#otherwise you will get an error.
	gcc $(INCLUS) -c SingleLinkedList.c 

DoubleLinkedList.o : DoubleLinkedList.c
	gcc $(INCLUS) -c DoubleLinkedList.c 

clean : 
	rm -rf *o SingleLinkedList DoubleLinkedList
