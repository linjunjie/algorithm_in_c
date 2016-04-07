CC = cc

#定义本地目录
#DIR = /usr/local/github/algorithm_in_c

SRC_DIR = ../src
LIB_DIR = ../lib

#需要包含的头文件
INCLUDES = -I../include

#链接库目录
LIBS = -L../lib

#所有编译选项
COMMON_FLAGS = -Wall $(INCLUDES)

#所有要进行编译的文件，在这里也就是所有.c后缀的c文件
SRC = $(wildcard *.c)

#根据规则运算取得所有要生成的.o文件
#OBJ = $(subst .c, .o, $(SRC))

#根据规则运算取得所有要编译生成的目标文件
#TARGETS = $(subst .c, , $(SRC))

#需要的静态文件
LIB_OBJ = libdll.a

#所有需要生成的编译目标及其依赖
all : $(LIB_OBJ) \
	SingleLinkedList \
	DoubleLinkedList \
	SingleCircularLinkedList \
	DoubleCircularLinkedList \
	StackByArr \
	StackByDoubleLinkedList \
	Queue

#编译静态链接库
libdll.a : dll.o
	#生成静态库libdll.a
	ar rs libdll.a dll.o
	#将生成的静态库文件转移到../lib目录下
	mv libdll.a ../lib

dll.o : $(SRC_DIR)/dll.c
	$(CC) $(COMMON_FLAGS) -c $(SRC_DIR)/dll.c 

#单向链表
SingleLinkedList.o : SingleLinkedList.c
	$(CC) $(COMMON_FLAGS) -c SingleLinkedList.c 

#双向链表
DoubleLinkedList.o : DoubleLinkedList.c
	$(CC) $(COMMON_FLAGS) -c DoubleLinkedList.c 

#单向循环链表
SingleCircularLinkedList.o : SingleCircularLinkedList.c
	$(CC) $(COMMON_FLAGS) -c SingleCircularLinkedList.c

#双向循环链表
DoubleCircularLinkedList.o : DoubleCircularLinkedList.c
	$(CC) $(COMMON_FLAGS) -c DoubleCircularLinkedList.c

#栈 - 通过数组实现
StackByArr.o : StackByArr.c
	$(CC) $(COMMON_FLAGS) -c StackByArr.c

#栈 - 通过双向链表实现
#这里用到了../lib/中的静态链接库libdll.a
StackByDoubleLinkedList : StackByDoubleLinkedList.c
	$(CC) $(COMMON_FLAGS) $(LIBS) -ldll StackByDoubleLinkedList.c -o StackByDoubleLinkedList

#队列 - 通过双向链表库libdll实现
Queue : Queue.c
	$(CC) $(COMMON_FLAGS) $(LIBS) -ldll Queue.c -o Queue

# .PHONY是为了指定后面的元素，例如这里的clean并不是一个真正的文件，只是一个假想工作目标
.PHONY : clean

clean : 
	rm -rf $(LIB_DIR)/*.a *.o \
			SingleLinkedList \
			DoubleLinkedList \
			SingleCircularLinkedList \
			DoubleCircularLinkedList \
			StackByArr \
			StackByDoubleLinkedList \
			Queue
