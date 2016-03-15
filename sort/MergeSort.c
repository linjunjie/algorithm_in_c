/**
 * 归并排序 - 又称为合并排序，二路排序
 *
 * 算法描述:
 * 1.将一个待排序序列分成两个序列
 * 2.将两个序列中的项依次进行比较，将较小的项放入有序序列中
 * 3.递归遍历左右两个序列，直至排序完成
 *
 * 属于稳定排序
 * 
 */

#include "algorithm.h"

// 将两个有序序列数组合并成一个有序序列
void MyMergeArr(int *pA, int *pB, int start, int mid, int end) {
	int i,j,k;
	int leftIndex = start;
	int rightIndex = mid + 1;
	int m;

	// 将左右两边的数组对应项进行比较，将较小的值放入pB中
	for(i = start; leftIndex <= mid && rightIndex <= end; i++){
		if(pA[leftIndex] < pA[rightIndex]){
			pB[i] = pA[leftIndex];
			leftIndex++;
		}else{
			pB[i] = pA[rightIndex];
			rightIndex++;
		}
	}

	// 将left序列中剩余的成员接到最终序列后面
	for(j = leftIndex; j <= mid; i++, j++){
		pB[i] = pA[j];
	}

	// 将right序列中剩余的成员接到最终序列后面
	for(k = rightIndex; k <= end; i++, k++){
		pB[i] = pA[k];
	}

	/* 这一步至关重要，是将此次排序结果赋予pA，这样才能在下次排序时得到已排好序的序列 */
	for(m = start; m <= end; m++){
		pA[m] = pB[m];
	}

	// 打印排序过程
	printf("mid is %d: ", mid);
	printIntArray(pB, end - start + 1);
}

void MyMergeSort(int *pData, int *pTmpData, int start, int end){
	// 取得中间数，目的是将带排序序列分为左右两部分
	int mid = (start + end) / 2;

	// 只有序列中成员大于2的时候，才需要进行两个序列的合并，否则不需要再进行处理
	if(end > start){
		MyMergeSort(pData, pTmpData, start, mid);
		MyMergeSort(pData, pTmpData, mid + 1, end);
		MyMergeArr(pData, pTmpData, start, mid, end);
	}
}

int main(){
	int data[] = {8,5,3,1,10,2,7,9,4,6};
	int len;
	GET_ARRAY_LEN(data, len);
	int *tmpData = (int *)malloc(sizeof(int) * len);	/* 归并排序中用到的数组，用于保存排序好之后的成员，其长度应跟data一样大小 */

	// 打印原始需排序成员
	printf("original : ");
	printIntArray(tmpData, len);

	// 快速排序
	MyMergeSort(data, tmpData, 0, len - 1);

	// 打印最终结果
	printf("result : ");
	printIntArray(tmpData, len);
}