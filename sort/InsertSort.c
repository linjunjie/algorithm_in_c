/**
 * 插入排序 - 直接插入排序
 * 
 * 插入排序又被我称为扑克牌排序法，就好像我们平时摸牌时的排序是一样的
 * 每次摸到新牌之后，都插入到前面已经排序好的牌中
 */

#include <stdio.h>

// 计算数组长度的宏
#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

// 数组打印函数
void printIntArray(int* arrData, int len){
	for(int i=0;i<len;i++) printf("%d,",arrData[i]);
	printf("\n");
}

// Insert Sort 插入排序 - 直接插入排序
void InsertSort(int* pData, int len){
	int i, j;
	int target;	//target 用来保存被插入的元素

	//第一个元素默认已经排序过了，所以只对1-n的元素进行排序
	for(i=1;i<len;i++){

		//先拿到需要排序的元素，默认是第二个元素，也就是索引为1的元素
		target = pData[i];
		j = i;

		//将此元素插入到前面已经排序好的元素中去
		//假设此元素比进行比较的元素更小，则需将被比较的元素后移
		while(j>0 && pData[j-1] > target){
			pData[j] = pData[j-1];
			j--;
		}

		//最终有两种情况:
		//1.找到了比target更小的元素，则target插入到此元素之后;
		//2.没有找到比target更小的元素，也就是说target本身就是最小的元素，则此时j=0，target放置到索引0的位置
		pData[j] = target;

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

	// 插入排序
	InsertSort(data, len);

	// 打印最终结果
	printf("result : ");
	printIntArray(data, len);
}
