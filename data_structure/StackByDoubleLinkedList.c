#include "dll.h"
struct stack_struct{

	/* 栈内容纳的元素 */
	struct stack_element * base;

	/* 栈可以容纳元素的最大数 */
	int max;

	/* 栈可以容纳元素的最小数 */
	int min;

	/* 栈目前的大小 */
	int size;

	/* 栈顶指针 */
	int top;
};

typedef struct stack_struct stack;

/* 一个最简单的栈元素，只保存一个整形变量 */
struct stack_element
{
	int num;
};

typedef struct stack_element element;

int myPrintStackData(void * data);

stack * create_stack(int size){
	stack * s;
	assert(size > 0);

	s = (stack *) malloc (sizeof(stack));
	if(s == NULL){
		return NULL;
	}

	s -> size = size;
	s -> base = (element *) malloc ( size * sizeof( element ));
	if(s -> base == NULL){
		return NULL;
	}

	s -> min = 0;
	s -> max = size - 1;
	s -> top = -1;

	return s;
}

/* 销毁栈 */
void destroy_stack(stack * s){
	if(s == NULL){
		return;
	}

	free(s -> base);
	free(s);

	return;
}

int push_stack(stack * s, element * e){

	/* 栈满 */
	if(s -> top == s -> max){
		return 0;
	}

	/* 将压入栈中的元素添加到链表的开头 */
	Snode n;
	n.data = (element *)malloc(sizeof(element));
	memcpy(n.data, e, sizeof(element));
	addNodeToHead(&n, myPrintStackData);

	/* 栈顶位置 +1 */
	s -> top += 1;

	return 1;
}

int pop_stack(stack * s, element * e){

	/* 空栈 */
	if(s -> top == -1){
		return 0;
	}

	//开辟一块儿内存空间
	element * tmp = (element *)malloc(sizeof(element));

	/* 从链表的头部位置开始删除元素 */
	tmp = deleteNodeFromHead();

	//拷贝内存区到我们的参数指针element *中
	memcpy(e, tmp, sizeof(element));

	/* 栈顶位置 -1 */
	s -> top -= 1;

	return 1;
}

//打印队列元素
int myPrintStackData(void * data){
	element  * e;
	e = data;
	printf("%d,", e -> num);
	return 1;
}

/* 打印整个栈: head -> tail */
int print_stack(){
	printf("%s\n", "print the whole stack based on the linkedlist:");
	printdll(head, myPrintStackData);
	return 1;
}

/* 查看指定栈位置的栈元素 */
void * view_element(stack * s, int element_num){
	if(s -> top == -1){
		return NULL;
	}

	if(s -> top - element_num < 0){
		return NULL;
	}

	return getNode(element_num);
}

int main(int argc, char * argv[]){
	int data[] = {8,5,3,1,10,2,7,9,4,6};
	// int data[] = {8,5};
	int len;
	GET_ARRAY_LEN(data, len);

	initList();

	/* 声明一个栈指针 */
	stack * s;

	/* 构建一个栈,长度为上面的数组长度 */
	s = create_stack (len);
	if(s == NULL){
		return 0;
	}

	/* 声明一个栈元素 */
	element * e;

	/* 为栈元素开辟内存空间 */
	e = (element *) malloc (sizeof( element ));
	if(e == NULL){
		return 0;
	}

	/* 将上面的数组元素全部压入栈 */
	for(int i=0; i<len; i++){
		e -> num = data[i];
		push_stack(s, e);
	}

	//打印整个栈内元素
	print_stack();
	// return 1;

	/* 打印栈内某一个元素 */
	// int num = view_element(stack, 4);
	// printf("%d\n", num);
	// return 1;

	// 弹出一个栈元素
	// print_stack();
	// pop_stack(stack, element);
	// printf("%d\n", element -> num);
	// print_stack();	//弹出一个之后再打印一下栈目前的情况
	// return 1;

	/* 执行弹出操作，并打印被弹出的栈元素 */
	printf("%s\n", "pop the stack elements:");
	while(pop_stack(s, e) != 0){
		printf("%d,", e -> num);
	}
	printf("\n");

	return 1;
}