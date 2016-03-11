/**
 * 选择排序
 *
 * 仍然是将整个序列分为有序序列和无序序列
 * 但是是从无序序列中找到最小或最大的元素直接放置到有序序列中去
 */

#include <algorithm.h>

// 选择排序
void SelectSort(int* pData, int len){
	int i,j;
	int tmp;
	int min_index;	//无序序列中最小的元素索引

	for(i = 0; i < len; i++){
		tmp = pData[i];
		for(min_index = j = i; j < len; j++){
			if(pData[min_index] > pData[j]){
				min_index = j;
			}
		}

		//将最小的值和第i个比较元素位置互换
		swapInt(&pData[i], &pData[min_index]);

		// 打印排序过程
		printf("process %d: ",i);
		printIntArray(pData, len);
	}
}

int main(){
	int data[] = {8,5,3,1,9,2,7,4,6};
	int len;
	GET_ARRAY_LEN(data, len);

	// 打印原始需排序成员
	printf("original : ");
	printIntArray(data, len);

	// 冒泡排序
	SelectSort(data, len);

	// 打印最终结果
	printf("result : ");
	printIntArray(data, len);
}