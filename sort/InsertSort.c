/**
 * 插入排序 - 直接插入排序
 * 
 * 插入排序又被我称为扑克牌排序法，就好像我们平时摸牌时的排序是一样的
 * 每次摸到新牌之后，都插入到前面已经排序好的牌中
 *
 * 在网上看到一种比较清晰的算法描述:
 *
 * 设数组为a[0...n-1]。
 * 
 * 1. 初始时，a[0]自成1个有序区，无序区为a[1..n-1]。令i=1
 * 2. 将a[i]并入当前的有序区a[0…i-1]中形成a[0…i]的有序区间。
 * 3. i++并重复第二步直到i==n-1。排序完成。
 * 
 */

#include "algorithm.h"

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

// 按照直接插入排序的算法描述标准自己编写的直接插入排序算法
void MyInsertSort(int* pData, int len){
	int i,j;
	int tmp;

	for(i = 1; i < len; i++){	//遍历无序区
		tmp = pData[i];
		for(j = i; j > 0; j--){		//遍历有序区
			if(tmp < pData[j-1]){	//如果需排序元素 < 被比较元素
				pData[j] = pData[j-1];	//则被比较元素依次后移
			}else{
				break;	//如果 待排序元素 > 被比较元素，则此时的j就是正确的位置，应立即退出对有序区的遍历
			}
		}
		pData[j] = tmp;	//将元素放置到自己的位置j
		
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
	// MyInsertSort(data, len);

	// 打印最终结果
	printf("result : ");
	printIntArray(data, len);
}
