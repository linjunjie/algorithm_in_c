#需要包含的头文件
INCLUS = -I..

all : SingleLinkedList DoubleLinkedList SingleCircularLinkedList DoubleCircularLinkedList

SingleLinkedList : SingleLinkedList.o
DoubleLinkedList : DoubleLinkedList.o
SingleCircularLinkedList : SingleCircularLinkedList.o
DoubleCircularLinkedList : DoubleCircularLinkedList.o

SingleLinkedList.o : SingleLinkedList.c
	gcc $(INCLUS) -c SingleLinkedList.c 

DoubleLinkedList.o : DoubleLinkedList.c
	gcc $(INCLUS) -c DoubleLinkedList.c 

#单向循环链表
SingleCircularLinkedList.o : SingleCircularLinkedList.c
	gcc $(INCLUS) -c SingleCircularLinkedList.c

#双向循环链表
DoubleCircularLinkedList.o : DoubleCircularLinkedList.c
	gcc $(INCLUS) -c DoubleCircularLinkedList.c

clean : 
	rm -rf *o SingleLinkedList DoubleLinkedList SingleCircularLinkedList DoubleCircularLinkedList
