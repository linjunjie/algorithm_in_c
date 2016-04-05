#需要包含的头文件
INCLUS = -I../include

#源文件
SRC_DIR = ../src

#链接库目录
LIBS = -L../lib

#所有编译选项
COMMON_FLAGS = -Wall $(INCLUS) 

#需要的静态文件
LIB_OBJ = libalgorithm.a

#需要生成的文件
all : $(LIB_OBJ) \
		BinarySearch

#编译静态链接库
libalgorithm.a : algorithm.o
	#生成静态库libalgorithm.a
	ar rs libalgorithm.a algorithm.o
	#将生成的静态库文件转移到../lib目录下
	mv libalgorithm.a ../lib

algorithm.o : $(SRC_DIR)/algorithm.c
	gcc $(COMMON_FLAGS) -c $(SRC_DIR)/algorithm.c 

BinarySearch : BinarySearch.o
	gcc -o BinarySearch -L../lib -lalgorithm BinarySearch.o

BinarySearch.o : BinarySearch.c 
	gcc $(COMMON_FLAGS) -c BinarySearch.c 

clean : 
	rm -rf *o BinarySearch
