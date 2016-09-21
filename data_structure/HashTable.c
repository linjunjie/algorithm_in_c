/**
 *	hashtable,哈希表也称为散列表
 * 
 *	这里使用拉链法来展示一个简单的哈希表
 *	所谓拉链法，也就是使用链表来处理哈希冲突
 *
 *  在目前php中（ php5 ）的哈希方法是：DJBX33A，其名称来源于此算法作者和方法：
 * 	DJBX33A (Daniel J. Bernstein, Times 33 with Addition)
 * 	文件位于/php-src/Zend/zend_hash.h中，其中也有详细的介绍
 * 	php中对它的介绍是这样的：
 * 	This is one of the best known hash functions for strings. 
 * 	Because it is both computed very fast and distributes very well.
 * 	
 */

#include "algorithm.h"

#define HASH_TABLE_SIZE 7		//哈希表大小（哈希表的大小一般是一个质数）

//哈希表元素
typedef struct hashtable_element_struct {
	char * str;		//一个用来保存字符串的字段
	struct hashtable_element_struct * next;
} element;

element * HashTable[HASH_TABLE_SIZE];

//unix中处理哈希的标准函数，这里直接拿来使用
//当然你可以编写自己的哈希函数
unsigned long hashELF(const char * name){
	unsigned long hash = 0, i;

	while(*name){
		hash = (hash << 4) + *name++;
		i = hash & 0xf0000000;
		if(i){
			hash ^= i >> 24;
		}
		hash &= ~i;
	}
	return hash;
}

unsigned int myHash(const char * name){
	unsigned long i = 0;
	i = hashELF(name);
	return i % HASH_TABLE_SIZE;
}

//初始化一个哈希表
void initHashTable(){
	for (int i = 0; i < HASH_TABLE_SIZE; i++){
		HashTable[i] = NULL;
	}
}

void printHashTable(){
	element * e;
	for (int i = 0; i < HASH_TABLE_SIZE; i++){
		e = HashTable[i];

		//按照槽打印
		printf("slot %d : ", i);	
		while(e != NULL){
			printf("%s,", e -> str);
			e = e -> next;
		}

		printf("\n");
	}
}

int insertHashTable(char * str){
	unsigned int hi;
	element * e;

	//计算出要插入的元素的哈希值
	hi = myHash(str);

	//为元素开辟内存
	e = (element *)malloc(sizeof(element));
	if(e == NULL){
		return 0;
	}

	//初始化
	// e -> str = (char *)malloc(sizeof(char) * (strlen(str) + 1));
	// strcpy(e -> str, str);
	e -> str = str;
	e -> next = NULL;

	element * tmp = HashTable[hi];
	if(tmp == NULL){
		HashTable[hi] = e;
	}else{
		while(tmp != NULL){
			if(tmp -> next == NULL){
				tmp -> next = e;
				break;
			}else{
				tmp = tmp -> next;
			}
		}
	}

	return 1;
	
}

int main(int argc, char * argv[]){
	char * data[] = {"my","name","is","hashtable","this","is","an","example","program","let","us","play"};
	int len;
	GET_ARRAY_LEN(data, len);

	initHashTable();

	for (int i = 0; i < len; i++)
	{
		insertHashTable(data[i]);
	}

	printHashTable();

	return 1;
}

