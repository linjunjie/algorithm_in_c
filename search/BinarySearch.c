/**
 * 二分查找
 */

#include "algorithm.h"

/** 
 * 二分查找算法
 * 
 * pData 数组
 * len 数组长度
 * needle 待查询元素
 * start 查询数组起始位置
 * end 查询数组结束位置
 */
int BinarySearch (int* pData, int len, int needle, int start, int end) {
	int half = len / 2;
	int current = start + half; //当前索引
	if( needle < pData[current] ){
		return BinarySearch(pData, half, needle, start, current);
	}else if( needle > pData[current] ){
		return BinarySearch(pData, len - half - 1, needle, current + 1, end);
	}else{
		return current;
	}
}

int main(){
	int data[] = {1,2,4,7,10,13,15,20,22,25,30};
	int len;
	GET_ARRAY_LEN(data, len);

	// 插入排序
	int index = BinarySearch(data, len, 10, 0, len - 1);
	printf("%d\n", index);
}