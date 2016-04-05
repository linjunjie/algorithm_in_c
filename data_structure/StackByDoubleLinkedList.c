#include "dll.c"

struct _stack{

	/* 栈内容纳的元素 */
	struct _stack_element * base;

	/* 栈可以容纳元素的最大数 */
	int max;

	/* 栈可以容纳元素的最小数 */
	int min;

	/* 栈目前的大小 */
	int size;

	/* 栈顶指针 */
	int top;
};

typedef struct _stack stack_struct;

/* 一个最简单的栈元素，只保存一个整形变量 */
struct _stack_element
{
	int num;
};

typedef struct _stack_element stack_element_struct;


stack_struct * create_stack(int size){
	stack_struct * stack;
	assert(size > 0);

	stack = (stack_struct *) malloc (sizeof(stack_struct));
	if(stack == NULL){
		return NULL;
	}

	stack -> size = size;
	stack -> base = (stack_element_struct *) malloc ( size * sizeof( stack_element_struct ));
	if(stack -> base == NULL){
		return NULL;
	}

	stack -> min = 0;
	stack -> max = size - 1;
	stack -> top = -1;

	return stack;
}

/* 销毁栈 */
void destroy_stack(stack_struct * stack){
	if(stack == NULL){
		return;
	}

	free(stack -> base);
	free(stack);

	return;
}

int push_stack(stack_struct * stack, stack_element_struct * element){

	/* 栈满 */
	if(stack -> top == stack -> max){
		return 0;
	}

	/* 降压入栈中的元素添加到链表的开头 */
	struct node op_node;
	op_node.num = element -> num;
	addNodeToHead(&op_node);

	/* 栈顶位置 +1 */
	stack -> top += 1;

	return 1;
}

int pop_stack(stack_struct * stack, stack_element_struct * element){

	/* 空栈 */
	if(stack -> top == -1){
		return 0;
	}

	/* 从链表的头部位置开始删除元素 */
	element -> num = deleteNodeFromHead();

	/* 栈顶位置 -1 */
	stack -> top -= 1;

	return 1;
}

/* 打印整个栈: head -> tail */
int print_stack(){
	printf("%s\n", "print the whole stack based on the linkedlist:");
	print(head);
	return 1;
}

int view_element(stack_struct * stack, int element_num){
	if(stack -> top == -1){
		return -1;
	}

	if(stack -> top - element_num < 0){
		return -1;
	}

	// return ( & ( (stack -> base)[stack -> top - element_num] ) );
	return getNode(element_num);
}

int main(int argc, char * argv[]){
	int data[] = {8,5,3,1,10,2,7,9,4,6};
	int len;
	GET_ARRAY_LEN(data, len);

	/* 声明一个栈指针 */
	stack_struct * stack;

	/* 构建一个栈,长度为上面的数组长度 */
	stack = create_stack (len);
	if(stack == NULL){
		return -1;
	}

	/* 声明一个栈元素 */
	stack_element_struct * element;

	/* 为栈元素开辟内存空间 */
	element = (stack_element_struct *) malloc (sizeof( stack_element_struct ));
	if(element == NULL){
		return -1;
	}

	/* 将上面的数组元素全部压入栈 */
	for(int i=0; i<len; i++){
		element -> num = data[i];
		push_stack(stack, element);
	}

	//打印整个栈内元素
	// print_stack();
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
	printf("%s\n", "print the whole stack element:");
	while(pop_stack(stack, element) != 0){
		printf("%d\n", element -> num);
	}

	return 1;
}