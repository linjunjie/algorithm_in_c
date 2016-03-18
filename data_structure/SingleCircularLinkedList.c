/**
 * 单向循环链表 - Singly Circular Linked List
 *
 * 最后一个节点的尾节点指向头结点
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

//定义一个头类型，并增加了一个尾部链节点
link head,tail;

//节点个数
int node_count;

/* 初始化一个空链表 */
void initList(){
	head = NULL;
	// head -> next = head;
	node_count = 0;
}

/* 打印以head作为头开始的链表 */
void print(link head){
	link node;
	node = head;
	printf("print the whole linkedlist:\n");
	do{
		printf("%d,", node->num);
		node = node -> next;
	}while(node != head);
	printf("\n\n");
}

/* 在链表尾部添加一个节点 */
int addNodeToTheEnd(link add_node){
	link tmp;		//临时链，用于存储要添加的链，基本上就是要添加的节点add_node的拷贝
	link current;	//当前链，用于遍历链表时存储当前遍历元素

	//申请一个链空间，若申请失败返回0，然后拷贝一份儿add_node，并且将尾部指向NULL
	tmp = (link)malloc(sizeof(struct node));
	if(tmp == NULL){ return 0; }	/* 申请内存空间失败则退出 */
	memcpy(tmp, add_node, sizeof(struct node));
	tmp -> next = NULL;

	if(head == NULL){	/* 如果还是空链表，则将申请的链节点直接赋予head头结点 */
		head = tmp;
		head -> next = head;
	}else{
		current = head;
		for(;;current = current -> next){
			if(current -> next == head){	/* 循环到的node指向的下一个节点如果是head的话，则已经到达尾部 */
				tmp -> next = head;
				current -> next = tmp;		/* 将申请的链安装在尾部 */
				break;
			}
		}	
	}
	
	return 1;
}

/* 以降序添加节点 */
int addNodeAscend(link add_node){
	link tmp;		/* 用于保存被插入的临时节点 */
	link current;	/* 循环遍历时的当前节点 */
	link prev;		/* 前一个链节点 */

	tmp = (link)malloc(sizeof(struct node));
	if(tmp == NULL){ return 0; }	/* 申请内存空间失败则退出 */
	memcpy(tmp, add_node, sizeof(struct node));
	tmp -> next = NULL;

	if(head == NULL){		/* 如果还是空链表，则将申请的链节点直接赋予head头结点 */
		tail = head = tmp;
	}else{
		prev = current = head;
		for(;;prev = current, current = current -> next){	/* 循环遍历链表，每次都保留上一个节点，并遍历到下一个节点 */
			if(current == tail){		/* 如果遍历到链表最后一个节点都没有找到合适的位置 */
				if(tmp -> num < current -> num){	/* 这个时候需要判断最后一个节点和插入节点的大小关系，如果小于 */
					tmp -> next = current;			/* 插入节点必然指向当前节点 */
					prev -> next = tmp;
					if(current == head){			/* 如果当前节点是头结点(当前节点也有可能是头结点) */
						head = tmp;					/* 则插入节点变为头结点 */
					}
				}else{
					tmp -> next = head;		/* 此时插入节点必然指向头结点 */
					current -> next = tmp;	/* 当前节点指向插入节点 */
					tail = tmp;				/* 插入节点作为尾部节点 */
				}
				break;
			}else if(tmp -> num < current -> num){		/* 如果找到了自己的位置 */
				if(current == head){					/* 如果这个位置是头结点 */
					head = tmp;							/* 则插入节点此时作为头结点 */
					tmp -> next = current;				/* 插入节点的尾部指向当前节点 */
					tail -> next = head;				/* 链表的尾部节点的指向也应该变为插入节点 */
				}else{									/* 如果这个位置不是头结点 */
					prev -> next = tmp;					/* 则必然存在上一个节点，上一个节点必然指向插入节点 */
					tmp -> next = current;				/* 插入节点必然指向当前节点 */
				}
				break;
			}
		}
	}

	return 1;
}

/* 对节点的释放，如果这里包含有其他指针，需要统一在这里一起释放 */
void freeNode(link free_node){
	free(free_node);
}

int deleteNode(link del_node){
	link current;	/* 保存当前所在的链表 */
	link prev;		/* 保存上一个链 */

	if(head == NULL){
		return 0;
	}

	// 开辟一块儿新内存存放prev链节点，也就是所谓的“上一个链节点”，默认设置为指向head头结点
	prev = (link) malloc (sizeof(struct node));
	if(prev == NULL){ return 0; }	/* 申请内存空间失败则退出 */
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
			freeNode(current);		/* 经过上面处理了链节点的指向之后，current现在已经完全被孤立掉了，现在可以释放掉要删除链节点的内存了 */
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
		// addNodeToTheEnd(&add_node);		/* 插入到链表尾部 */
		addNodeAscend(&add_node);		/* 以降序插入链表 */
	}
	print(head);

	//删除一个链节点
	add_node.num = 5;
	deleteNode(&add_node);

	print(head);
}




