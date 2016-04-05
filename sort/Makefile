#需要包含的头文件
INCLUS = -I../include

#源文件
SRC_DIR = ../src

#所有编译选项
COMMON_FLAGS = -Wall $(INCLUS) -L../lib -lalgorithm

#需要的静态文件
LIB_OBJ = libalgorithm.a

all : $(LIB_OBJ) \
		BubbleSort InsertSort ShellSort BinInsertSort SelectSort QuickSort MergeSort

#编译静态链接库
libalgorithm.a : algorithm.o
	#生成静态库libalgorithm.a
	ar rs libalgorithm.a algorithm.o
	#将生成的静态库文件转移到../lib目录下
	mv libalgorithm.a ../lib

algorithm.o : $(SRC_DIR)/algorithm.c
	gcc $(COMMON_FLAGS) -c $(SRC_DIR)/algorithm.c 


BubbleSort : BubbleSort.c 
	gcc -o BubbleSort $(COMMON_FLAGS) BubbleSort.c 

InsertSort : InsertSort.c
	gcc -o InsertSort $(COMMON_FLAGS) InsertSort.c

ShellSort : ShellSort.c
	gcc -o ShellSort $(COMMON_FLAGS) ShellSort.c

BinInsertSort : BinInsertSort.c
	gcc -o BinInsertSort $(COMMON_FLAGS) BinInsertSort.c

SelectSort : SelectSort.c
	gcc -o SelectSort $(COMMON_FLAGS) SelectSort.c

QuickSort : QuickSort.c
	gcc -o QuickSort $(COMMON_FLAGS) QuickSort.c

MergeSort : MergeSort.c
	gcc -o MergeSort $(COMMON_FLAGS) MergeSort.c

clean : 
	rm -rf *o BubbleSort InsertSort ShellSort BinInsertSort SelectSort QuickSort MergeSort
