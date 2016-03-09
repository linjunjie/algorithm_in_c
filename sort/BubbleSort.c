/**
 * 冒泡排序
 * 
 * 此算法简单说就是总是将最大数往后移动到最后
 *
 * 编译之后运行, 将可以看到算法运行过程和排序结果
 *
 * 但是通过算法的步骤展示可以看出，此算法每次都会进行(len-1)次排序,
 * 此结果中在第四次排序之后已经排序成功，但是算法依然进行了四次没有必要的排序
 */

#include <stdio.h>

// 计算数组长度的宏
#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

// 数组打印函数
void printIntArray(int* arrData, int len){
	for(int i=0;i<len;i++) printf("%d,",arrData[i]);
	printf("\n");
}

// Bubble Sort 冒泡排序
// 2015年11月23日 星期一 11时38分59秒 CST
void BubbleSort(int* pData, int len){
	int iTemp;	// 临时存储成员函数的变量
	for(int i=1;i<len;i++){
		// 将最大的成员移动到最后，然后再对前面n-1个成员进行同样做法
		for(int j=0;j<len-i;j++){
			if(pData[j] > pData[j+1]){
				iTemp = pData[j+1];
				pData[j+1] = pData[j];
				pData[j] = iTemp;
			}
		}
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
	BubbleSort(data, len);

	// 打印最终结果
	printf("result : ");
	printIntArray(data, len);
}
