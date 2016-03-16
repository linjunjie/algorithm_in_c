
#include <stdio.h>
#include <stdlib.h>		/* for malloc */
#include <string.h>		/* for memcpy */

// 计算数组长度的宏
#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

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
