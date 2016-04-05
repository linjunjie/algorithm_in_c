CC = cc

#需要包含的头文件
INCLUS = -I..

all : SingleLinkedList \
	DoubleLinkedList \
	SingleCircularLinkedList \
	DoubleCircularLinkedList \
	StackByArr \
	StackByDoubleLinkedList

SingleLinkedList : SingleLinkedList.o
DoubleLinkedList : DoubleLinkedList.o
SingleCircularLinkedList : SingleCircularLinkedList.o
DoubleCircularLinkedList : DoubleCircularLinkedList.o
StackByArr : StackByArr.o
StackByDoubleLinkedList : StackByDoubleLinkedList.o

#单向链表
SingleLinkedList.o : SingleLinkedList.c
	$(CC) $(INCLUS) -c SingleLinkedList.c 

#双向链表
DoubleLinkedList.o : DoubleLinkedList.c
	$(CC) $(INCLUS) -c DoubleLinkedList.c 

#单向循环链表
SingleCircularLinkedList.o : SingleCircularLinkedList.c
	$(CC) $(INCLUS) -c SingleCircularLinkedList.c

#双向循环链表
DoubleCircularLinkedList.o : DoubleCircularLinkedList.c
	$(CC) $(INCLUS) -c DoubleCircularLinkedList.c

#栈 - 通过数组实现
StackByArr.o : StackByArr.c
	$(CC) $(INCLUS) -c StackByArr.c

#栈 - 通过双向链表实现
StackByDoubleLinkedList.o : StackByDoubleLinkedList.c
	$(CC) $(INCLUS) -c StackByDoubleLinkedList.c

clean : 
	rm -rf *o SingleLinkedList \
			DoubleLinkedList \
			SingleCircularLinkedList \
			DoubleCircularLinkedList \
			StackByArr \
			StackByDoubleLinkedList
