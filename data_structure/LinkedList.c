/**
 * 单链表
 *
 * 每一个节点都包含一个或多个存储数据的data域和一个指向子链节点的指针域
 * 多个前后相连的链节点便组成了链表 
 *
 */

#include "algorithm.h"

/* 一个存放正整数的链节点 */
struct node
{
	int num;
	struct node * next;
};

//定义一个链表节点指针类型link
typedef struct node * link;

//定义一个头类型
link head;

//节点个数
int node_count;

/* 初始化一个空链表 */
void initList(){
	head = NULL;
	node_count = 0;
}

/* 打印以head作为头开始的链表 */
void print(link head){
	link node;
	node = head;
	printf("print the whole linkedlist:\n");
	while(node != NULL){
		printf("%d,", node->num);
		node = node -> next;
	}
	printf("\n\n");
}

/* 在链表尾部添加一个节点 */
int addNodeToTheEnd(link add_node){
	link tmp;		//临时链，用于存储申请到的链
	link current;	//当前链，用于遍历链表时存储当前遍历元素

	//申请一个链空间，若申请失败返回0，然后拷贝一份儿add_node，并且将尾部指向NULL
	tmp = (link)malloc(sizeof(struct node));
	if(tmp == NULL) return 0;
	memcpy(tmp, add_node, sizeof(struct node));
	tmp -> next = NULL;

	if(head == NULL){	/* 如果还是空链表，则将申请的链直接赋予head */
		head = tmp;
	}else{
		current = head;
		for(;;current = current -> next){
			if(current -> next == NULL){	/* 循环到的node指向是NULL的话，则已经到达尾部 */
				current -> next = tmp;		/* 将申请的链安装在尾部 */
				break;
			}
		}	
	}
	
	return 1;
}

int deleteNode(link del_node){
	link current;	/* 保存当前所在的链表 */
	link prev;		/* 保存上一个链 */
	link tmp;

	if(head == NULL){
		return 0;
	}

	// 开辟一块儿新内存存放prev链节点，也就是所谓的“上一个链节点”，默认设置为指向head头结点
	prev = (link) malloc (sizeof(struct node));

	prev -> next = current = head;
	for(;;prev = current, current = current -> next){	/* 将当前的链保存到prev，并遍历到下一个链 */
		if(current == NULL){
			break;
		}else if(current -> num == del_node -> num){	/* 如果找到了要删除的节点 */
			if(current == head){			/* 如果要删除的节点是头节点 */
				head = current -> next;		/* 则将被删除的节点的子节点赋予头结点 */
			}else{
				prev -> next = current -> next;		/* 如果要删除的节点不是头结点，则将父节点的子节点指向被删除节点的子节点 */
			}
			free(current);		/* 经过上面处理了链节点的指向之后，现在可以释放掉要删除链节点的内存了 */
			printf("delete node: %d success!\n\n", current -> num);

			return 1;
		}
	}

	printf("delete node: %d fail!\n\n", current -> num);
	return -1;
}

int main(int argc, char *argv[]){
	int data[] = {8,5,3,1,10,2,7,9,4,6};
	int len;
	GET_ARRAY_LEN(data, len);

	initList();
	struct node add_node;
	for(int i = 0; i < len; i++){
		add_node.num = data[i];
		addNodeToTheEnd(&add_node);
	}
	print(head);

	//删除一个链节点
	add_node.num = 5;
	deleteNode(&add_node);

	print(head);
}




