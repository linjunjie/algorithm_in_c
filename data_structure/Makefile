#需要包含的头文件
INCLUS = -I..

all : SingleLinkedList \
	DoubleLinkedList \
	SingleCircularLinkedList \
	DoubleCircularLinkedList \
	StackByArr

SingleLinkedList : SingleLinkedList.o
DoubleLinkedList : DoubleLinkedList.o
SingleCircularLinkedList : SingleCircularLinkedList.o
DoubleCircularLinkedList : DoubleCircularLinkedList.o
StackByArr : StackByArr.o

#单向链表
SingleLinkedList.o : SingleLinkedList.c
	gcc $(INCLUS) -c SingleLinkedList.c 

#双向链表
DoubleLinkedList.o : DoubleLinkedList.c
	gcc $(INCLUS) -c DoubleLinkedList.c 

#单向循环链表
SingleCircularLinkedList.o : SingleCircularLinkedList.c
	gcc $(INCLUS) -c SingleCircularLinkedList.c

#双向循环链表
DoubleCircularLinkedList.o : DoubleCircularLinkedList.c
	gcc $(INCLUS) -c DoubleCircularLinkedList.c

#栈 - 通过数组实现
StackByArr.o : StackByArr.c
	gcc $(INCLUS) -c StackByArr.c

clean : 
	rm -rf *o SingleLinkedList \
			DoubleLinkedList \
			SingleCircularLinkedList \
			DoubleCircularLinkedList \
			StackByArr
