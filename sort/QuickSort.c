/**
 * 快速查找
 *
 * 一种通过一次排序将整个序列分为两个部分的排序算法，其中以中值为基准，一边序列的值一定比另一边序列的值大或小
 */

#include "algorithm.h"

// 我自己实现的快速排序算法
void MyQuickSort (int* pData, int start, int end){
	int i,j,k;
	int mid;	// 中指所在的元素索引
	int tmp;	// 被比较的符合特征的元素
	int len;

	// 得到此次序列的长度
	len = end - start + 1;
	for(i = start; i < len; i++){
		//中值起始位开始元素
		mid = i;	
		for(j = i; j < len; j++){

			//如果有比中值更小的元素
			if(pData[mid] > pData[j]){

				//则将此更小的元素取出
				tmp = pData[j];

				//将前面的元素依次后移
				for(k = j; k > i; k--){
					pData[k] = pData[k-1];
				}

				//将此元素安插在之前中值元素所在位置
				pData[i] = tmp;

				//中值元素本身的索引也后移一位
				mid++;	
			}
		}
	}

	if(mid - start > 1){
		MyQuickSort(pData, start, mid - 1);
	}else if(end - mid > 1){
		MyQuickSort(pData, mid + 1, end);
	}
}


int main(){
	int data[] = {8,5,3,1,10,2,7,9,4,6};
	int len;
	GET_ARRAY_LEN(data, len);

	// 打印原始需排序成员
	printf("original : ");
	printIntArray(data, len);

	// 快速排序
	MyQuickSort(data, 0, len - 1);

	// 打印最终结果
	printf("result : ");
	printIntArray(data, len);
}

