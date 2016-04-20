CC = cc

LIB_DIR = ../lib

#需要包含的头文件
INCLUDES = -I../include

#链接库目录
LIBS = -L../lib

#所有编译选项
COMMON_FLAGS = -Wall $(INCLUDES)

#所有需要生成的编译目标及其依赖
all : BinarySearchTree \
		BalancedBinaryTree \
		RedBlackTree

#二叉查找树
BinarySearchTree.o : BinarySearchTree.c
	$(CC) $(COMMON_FLAGS) -c BinarySearchTree.c

#平衡二叉树
BalancedBinaryTree.o : BalancedBinaryTree.c
	$(CC) $(COMMON_FLAGS) -c BalancedBinaryTree.c

#红黑树
RedBlackTree.o : RedBlackTree.c
	$(CC) $(COMMON_FLAGS) -c RedBlackTree.c

# .PHONY是为了指定后面的元素，例如这里的clean并不是一个真正的文件，只是一个假想工作目标
.PHONY : clean

clean : 
	rm -rf $(LIB_DIR)/*.a *.o \
			BinarySearchTree \
			BalancedBinaryTree \
			RedBlackTree

