/**
 * 带头尾指针的双向链表
 * 目前是用来实现栈和队列
 * dll.h
 * double linked list
 */

#ifndef _DLL_H_
#define _DLL_H_ 1

#include "algorithm.h"

struct node{
	// int num;
	void * data;	/* 这个数据位应该是一个可以指向任意数据结构的指针 */
	struct node * prev;		/* 指向上一个节点 */
	struct node * next;		/* 指向下一个节点 */
};

typedef struct node * link;

/**
 * 打印链表的回调函数接口, 调用链表的一方来自定义如何打印
 */
typedef int ( * dll_print_function )(void * data);

/**
 * 比较函数
 * left < right -1
 * left = right 0
 * left > right 1
 */
typedef int ( * dll_compare_function )(void * left, void * right);

//头尾指针
link head, tail;

//节点个数
int node_count;

void initList();
void printdll(link head, dll_print_function);
int addNodeToHead(link add_node, dll_print_function);
int addNodeToTail(link add_node);
int addNodeAscend(link add_node, dll_compare_function);
void freeNode(link free_node);
int deleteNode(link del_node, dll_compare_function);
void * deleteNodeFromHead();
void * deleteNodeFromTail();
void * getNode(int node_index);

#endif