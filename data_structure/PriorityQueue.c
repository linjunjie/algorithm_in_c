/**
 *	优先级队列 - 通过双向链表实现
 *
 *	这里实现的是最大优先级队列
 *	和普通队列的主要区别在于查找和出队列部分
 *	基本上，这里的实现是，在插入的时候将优先级越大的元素插入越靠近出队列的位置，这样最后优先级最大的就可以先出队列
 */

#include "dll.h"

//队列元素
typedef struct queue_element_struct {
	int num;
	int priority;	//优先级
} element;

//队列
typedef struct queue_struct {
	element * base;
	int tail;
	int size;
	int max;
	int min;
} queue;

int myPrintPriorityQueueData(void * data);
int myComparePriorityQueueData(void * left, void * right);

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

	Snode node;
	node.data = (element *)malloc(sizeof(element));
	memcpy(node.data, e, sizeof(element));

	//优先级队列的插入是参照优先级大小的
	addNodeWithPriority(&node, myComparePriorityQueueData);

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
int myPrintPriorityQueueData(void * data){
	element * e;
	e = data;
	printf("num : %d, priority : %d\n", e -> num, e -> priority);
	return 1;
}

//先判断优先级，在判断数值
int myComparePriorityQueueData(void * left, void * right){
	element * eleft, * eright;
	eleft = left;
	eright = right;
	if(eleft -> priority < eright -> priority){
		return -1;
	}else if(eleft -> priority > eright -> priority){
		return 1;
	}else if(eleft -> priority == eright -> priority){
		if(eleft -> num < eright -> num){
			return -1;
		}else if(eleft -> num == eright -> num){
			return 1;
		}else if(eleft -> num == eright -> num){
			return 0;
		}
	}

	return -2;
}

//打印整个队列
int print_queue(){
	printf("%s\n", "print the whole priority queue based on the linkedlist:");
	printdll(head, myPrintPriorityQueueData);
	return 1;
}

//自定义比较函数
int myCompare(void * data1, void * data2){
	element * e1, * e2;
	e1 = data1;
	e2 = data2;
	if(e1 -> num < e2 -> num){
		return 1;
	}else{
		return 0;
	}
}

int main(int argc, char * argv[]){
	int data[5][2] = {{8,5},{3,1},{10,2},{7,9},{4,6}};
	int len;
	GET_ARRAY_LEN(data, len);

	initList();

	queue * q;

	q = create_queue(len);
	if(q == NULL){
		return 0;
	}

	//声明一个队列元素指针
	element e;

	/* 元素入队列 */
	for(int i=0; i<len; i++){
		e.num = data[i][0];
		e.priority = data[i][1];
		push_queue(q, &e);
	}

	//打印整个队列
	print_queue();
	// return 1;

	printf("%s\n", "pop the priority queue elements:");
	while(pop_queue(q, &e) != 0){
		printf("num:%d, priority:%d\n", e.num, e.priority);
	}

}