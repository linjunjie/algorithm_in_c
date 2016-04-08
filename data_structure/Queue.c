/**
 *	队列 - 通过双向链表实现
 *
 *	FIFO first in, first out
 */

#include "dll.h"

//队列元素
typedef struct queue_element_struct {
	int num;
} element;

//队列
typedef struct queue_struct {
	element * base;
	int tail;
	int size;
	int max;
	int min;
} queue;

int myPrintQueueData(void * data);

queue * create_queue(int size){
	queue * q;
	assert(size > 0);

	q = ( queue * ) malloc (sizeof( queue ));
	if(q == NULL){
		printf("%s\n", "初始化队列失败");
		return NULL;
	}

	q -> base = (element * ) malloc ( size * sizeof( element ));
	if(q -> base == NULL){
		printf("%s\n", "初始化队列元素失败");
		return NULL;
	}
	q -> size = size;
	q -> tail = -1;
	q -> max = size - 1;
	q -> min = 0;

	return q;
}

int push_queue(queue * q, element * e){
	if(q -> tail == q -> max){
		return 0;
	}

	struct node n;
	n.data = (element *)malloc(sizeof(element));
	memcpy(n.data, e, sizeof(element));

	addNodeToHead(&n, myPrintQueueData);

	q -> tail += 1;

	return 1;
}

int pop_queue(queue * q, element * e){
	if(q -> tail == -1){
		return 0;
	}

	element * tmp = deleteNodeFromTail();
	memcpy(e, tmp, sizeof(element));

	q -> tail -= 1;

	return 1;
}

//打印队列元素
int myPrintQueueData(void * data){
	element * e = data;
	if(e == NULL){
		printf("num : %s\n", "is NULL");
	}else{
		printf("num : %d\n", e -> num);
	}
	
	return 1;
}

//打印整个队列
int print_queue(){
	printf("%s\n", "print the whole queue based on the linkedlist:");
	printdll(head, myPrintQueueData);
	return 1;
}

int main(int argc, char * argv[]){
	int data[] = {8,5,3,1,10,2,7,9,4,6};
	int len;
	GET_ARRAY_LEN(data, len);

	initList();

	queue * q;

	q = create_queue(len);
	if(q == NULL){
		return 0;
	}

	//声明一个队列元素结构
	element e;

	/* 元素入队列 */
	for(int i=0; i<len; i++){
		e.num = data[i];
		push_queue(q, &e);
	}

	//打印整个队列
	// print_queue();
	// return 1;

	printf("%s\n", "print the whole queue elements:");
	while(pop_queue(q, &e) != 0){
		printf("%d\n", e.num);
	}

}


