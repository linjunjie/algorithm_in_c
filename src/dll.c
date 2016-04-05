/* dll.c 
 * double linked list
 */

#include "dll.h"

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
		printf("%d,", node -> num);
		node = node -> next;
	}
	printf("\n\n");
}

/* 将一个节点加入到双向链表的开始 */
int addNodeToHead(link add_node){
	link tmp;
	link current;

	tmp = (link) malloc (sizeof(struct node));
	if(tmp == NULL){
		return 0;
	}
	tmp -> num = add_node -> num;
	tmp -> prev = NULL;
	tmp -> next = NULL;

	if(head == NULL){
		head = tmp;
	}else{
		current = head;
		tmp -> next = current;
		current -> prev = tmp;
		head = tmp;
	}

	return 1;
}

/* 将一个节点加入到双向链表的末尾 */
int addNodeToTail(link add_node){
	link tmp;
	link current;

	tmp = (link) malloc (sizeof(struct node));
	if(tmp == NULL){
		return 0;
	}
	tmp -> num = add_node -> num;
	tmp -> prev = NULL;
	tmp -> next = NULL;

	if(head == NULL){
		head = tmp;
	}else{
		current = head;
		for(;;current = current -> next){
			if(current -> next == NULL){
				tmp -> prev = current;
				current -> next = tmp;
				break;
			}
		}
	}

	return 1;
}

/* 按照从小到大的顺序插入节点 */
int addNodeAscend(link add_node){
	link prev,current;
	link tmp;

	tmp = (link) malloc (sizeof(struct node));
	if(tmp == NULL){
		return 0;
	}
	memcpy(tmp, add_node, sizeof(struct node));
	tmp -> prev = tmp -> next = NULL;

	if(head == NULL){	/* 如果是空链接 */
		head = tmp;		/* 直接将节点加入头结点 */
	}else{
		current = head;	/* 从头结点出发寻找 */
		prev = current -> prev;
		for(;;prev = current, current = current -> next){
			if(current == NULL){			/* 如果是最后一个节点，也就说明到了最后一个节点扔未找到合适的位置，则插入节点应插在链表尾部 (注意此时的current节点绝对不可能是head头结点) */
				tmp -> prev = prev;			/* 插入节点的头结点应该是上一个节点 */
				prev -> next = tmp;			/* 上一个节点的尾节点应该是插入节点 */
				break;
			}else if(tmp -> num < current -> num){			/* 如果找到了自己应该插入的位置 */
				if(current == head){		/* 如果此时是头结点 */
					current -> prev = tmp;	/* 则此时当前节点不再作为头结点，并且当前节点的头结点变为了插入节点 */
					tmp -> next = current;	/* 插入节点的尾节点应该是当前节点 */
					head = tmp;				/* 将插入节点作为头结点 */
				}else{						/* 如果不是头结点 */
					tmp -> next = current;	/* 则插入节点的尾节点为当前节点 */
					tmp -> prev = prev;		/* 插入节点的头结点是当前节点的头结点 */
					current -> prev = tmp;	/* 则当前节点的头结点是插入节点 */
					prev -> next = tmp;		/* 头结点的尾节点应为插入节点 */
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

/* 双向链表删除节点 */
int deleteNode(link del_node){
	link current;
	link prev;
	link next;

	if(head == NULL){
		return 0;
	}

	current = head;
	for(;;current = current -> next){
		if(current -> num == del_node -> num){
			if(current == head){
				head = current -> next;
				head -> prev = NULL;
			}else{
				prev = current -> prev;
				next = current -> next;
				prev -> next = next;
				next -> prev = prev;
			}
			freeNode(current);
			printf("delete node: %d success!\n\n", current -> num);

			return 1;
		}
	}
}

/* 删除一个头结点, 并返回删除的头结点指针 */
int deleteNodeFromHead(){
	int del_node;
	link current;
	link next;

	if(head == NULL){
		return -1;
	}

	current = head;
	next = head -> next;
	del_node = current -> num;
	if(next != NULL){
		next -> prev = NULL;
		head = next;
	}
	freeNode(current);

	return del_node;
}

/* 根据位置获取链表中的特定元素(第一个元素的位置为1) */
int getNode(int node_index){
	link current;
	if(head == NULL){
		return -1;
	}

	int i = 1;
	current = head;
	for(;;current = current -> next, i++){
		if(node_index == i){
			return current -> num;
		}
	}
}