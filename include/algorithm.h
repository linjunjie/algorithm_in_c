#ifndef _ALGO_H_
#define _ALGO_H_ 1

#include <stdio.h>
#include <stdlib.h>		/* for malloc */
#include <string.h>		/* for memcpy */
#include <assert.h>		/* for assert */

// 计算数组长度的宏
#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

void printIntArray(int* arrData, int len);
void swapInt (int *a, int *b);

#endif