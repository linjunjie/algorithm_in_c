/**
 * 二分查找插入排序 - 一种添加了二分查找的直接插入排序
 * 
 * 算法描述为:
 * 
 * 设数组为a[0...n]。
 * 
 * 1.将原序列分成有序区和无序区。a[0...i-1]为有序区，a[i...n] 为无序区。（i从1开始）
 * 2.从无序区中取出第一个元素，即a[i]，使用二分查找算法在有序区中查找要插入的位置索引j
 * 3.将a[j]到a[i-1]的元素后移，并将a[i]赋值给a[j]
 * 4.重复步骤2~3，直到无序区元素为0
 */

#include "algorithm.h"

// Binary Insert Sort 二分查找插入排序
void BinInsertSort(int* pData, int len){
	int i,j,k;
	int tmp;
	int index;

	for(i = 1; i < len; i++){
		//获取到待排序元素
		tmp = pData[i];

		//通过对有序序列的二分搜索找到需要插入的位置
		index = BinSearch(pData, tmp, 0, i-1);

		//此位置开始的元素全部后移
		for(k = i; k > index; k--){
			pData[k] = pData[k-1];
		}

		//插入此位置
		pData[index] = tmp;
		
		// 打印排序过程
		printf("process %d: ",i);
		printIntArray(pData, len);
	}
}

// 二分搜索查找到需要插入的位置
int BinSearch (int* pData, int needle, int start, int end) {
	int len = end - start + 1;
	int half = len / 2;
	int current = start + half; //当前索引
	if(needle < pData[current]){
		if(current == start) return current;
		return BinSearch(pData, needle, start, current - 1);
	}else{
		if(current == end) return current + 1;
		return BinSearch(pData, needle, current + 1, end);
	}
}

int main(){
	int data[] = {8,5,3,1,10,9,2,7,4,6};
	int len;
	GET_ARRAY_LEN(data, len);

	// 打印原始需排序成员
	printf("original : ");
	printIntArray(data, len);

	// 插入排序
	BinInsertSort(data, len);

	// 打印最终结果
	printf("result : ");
	printIntArray(data, len);
}
