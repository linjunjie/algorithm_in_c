#include <stdio.h>

// Bubble Sort 冒泡排序
// 2015年11月23日 星期一 11时38分59秒 CST
void BubbleSort(int* pData, int Count){
	int iTemp;
	for(int i=1;i<Count;i++){
		for(int j=0;j<Count-i;j++){
			if(pData[j] > pData[j+1]){
				iTemp = pData[j+1];
				pData[j+1] = pData[j];
				pData[j] = iTemp;
			}
		}
	}
}

int main(){
	int data[] = {8,5,3,1,9};
	BubbleSort(data,5);
	for(int i=0;i<5;i++) printf("%d",data[i]);
	printf("\n");
}
