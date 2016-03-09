/**
 * 希尔排序: 一种增量序列插入排序(shell sort) <Donald.Shell@1959>
 * 
 * 是一种将整个序列根据增量进行分割，并在各子序列中分别进行插入排序的算法
 * 
 * 之所以比直接插入排序的效率高的原因在于两点:
 * 		1.一开始增量长度step_length较大时数据项每一趟排序需要的个数较少, 但数据项的间隔距离很长;
 * 		2.当step_length减小时每一趟需要变动的数据项增多, 但此时已经基本接近于它们排序后的最终位置;
 *
 * 希尔排序的性能关键在于增量序列的选择
 * 
 * 个人理解：
 * 因为直接插入排序在序列元素基本有序时是非常快的，所以希尔排序就是利用了这一点。
 * 希尔排序前期就是对整个序列中间距由大到小进行排序，直到最后序列已基本有序时再对全体元素进行一次插入排序。
 * 所以这样效率是很高的，如果直接对序列进行排序则效率会比较低。
 */

#include <stdio.h>

// 计算数组长度的宏
#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

// 数组打印函数
void printIntArray(int* arrData, int len){
	for(int i=0;i<len;i++) printf("%d,",arrData[i]);
	printf("\n");
}

// Shell Sort 希尔排序
void ShellSort(int* pData, int len){
	int i,j;
	int tmp;
	int step_index = 0;
	int step_length = len / 2; //得到初次增量序列长度
	while(step_length >= 1){
		for(i = step_length; i < len; i++){
			//在各个增量子序列中进行插入排序
			tmp = pData[i];
			for(j = i - step_length; j >= 0 && tmp < pData[j]; j = j - step_length){
				pData[j+step_length] = pData[j];
			}
			pData[j+step_length] = tmp;
		}

		// 打印排序过程
		printf("process %d: ", ++step_index);
		printIntArray(pData, len);

		step_length = step_length / 2;	// 得到下一个增量序列长度
	}
}

int main(){
	int data[] = {8,5,3,1,9,2,7,4,6};
	int len;
	GET_ARRAY_LEN(data, len);

	// 打印原始需排序成员
	printf("original : ");
	printIntArray(data, len);

	// 希尔排序
	ShellSort(data, len);

	// 打印最终结果
	printf("result : ");
	printIntArray(data, len);
}
