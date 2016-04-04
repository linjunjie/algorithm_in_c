/**
 *	栈结构 - 通过数组结构实现
 *
 *	LIFO last in, first out
 */

#include "algorithm.h"

struct _stack{
	struct _stack_element * base;
	int max;
	int min;
	int size;
	int top;
};

typedef struct _stack stack_struct;

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

	/* 将压入栈的元素的内存拷贝到对应的栈数组中 */
	memmove( & (( stack -> base )[ stack -> top]), element, sizeof(stack_element_struct));

	/* 栈顶位置 +1 */
	stack -> top += 1;

	return 1;
}

int pop_stack(stack_struct * stack, stack_element_struct * element){

	/* 空栈 */
	if(stack -> top == -1){
		return 0;
	}

	/* 将栈顶元素的内存拷贝到弹出的栈元素中 */
	memmove(element, & ( (stack -> base)[stack -> top]), sizeof(stack_element_struct));

	/* 栈顶位置 -1 */
	stack -> top -= 1;

	return 1;
}

stack_element_struct * view_element(stack_struct * stack, int element_num){
	if(stack -> top == -1){
		return NULL;
	}

	if(stack -> top - element_num < 0){
		return NULL;
	}

	return ( & ( (stack -> base)[stack -> top - element_num] ) );
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


	/* 打印栈内某一个元素 */
	// element = view_element(stack, 2);
	// printf("%d\n", element -> num);


	/* 执行弹出操作，并打印被弹出的栈元素 */
	while(pop_stack(stack, element) != 0){
		printf("%d\n", element -> num);
	}

	return 1;
}