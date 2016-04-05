/**
 * 双向链表 - Doubly-Linked List
 *
 * 每一个节点都包含一个或多个存储数据的data域和两个分别指向父节点（或称为头结点）和子节点（或称为尾节点）的指针域
 * 多个前后相连的双向链节点便组成了双向链表 
 *
 */


#ifndef _DLL_H_
#define _DLL_H_ 1


#include "algorithm.h"

struct node{
	int num;
	struct node * prev;		/* 指向上一个节点 */
	struct node * next;		/* 指向下一个节点 */
};

typedef struct node * link;

//定义一个头类型
link head;

//节点个数
int node_count;

void initList();
void print(link head);
int addNodeToHead(link add_node);
int addNodeToTail(link add_node);
int addNodeAscend(link add_node);
void freeNode(link free_node);
int deleteNode(link del_node);
int deleteNodeFromHead();
int getNode(int node_index);

#endif