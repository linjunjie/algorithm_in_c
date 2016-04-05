#include "algorithm.h"

// 数组打印函数
void printIntArray(int* arrData, int len){
	for(int i=0;i<len;i++) printf("%d,",arrData[i]);
	printf("\n");
}

// 交换两个变量
// 调用方式: swapInt(&a, &b)
void swapInt (int *a, int *b) {
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}