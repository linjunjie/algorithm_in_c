/**
 * 以下注释使用了这个Sublime插件生成(你可以根据自己的需要任意修改):
 * https://github.com/linjunjie/sublime_plugins/blob/master/addComments.py
 *			
 * @Description: 	Binary - 二叉树
 * @Datetime: 		2016-04-10 19:08:53
 * @Author: 		linjunjie
 */

#include "algorithm.h"

typedef struct tree_node{
	int data;
	struct tree_node * left;
	struct tree_node * right;
} node;

node * root;

void init_tree(){
	root = NULL;
}

int insert_tree(node * * parent, node * insert_node){

	node * n;
	n = (node *)malloc(sizeof(node));
	n -> data = insert_node -> data;

	if((* parent) == NULL){
		* parent = n;
	}else{
		if(insert_node -> data < (* parent) -> data){
			insert_tree( &(* parent) -> left, n);
		}else if(n -> data > (* parent) -> data){
			insert_tree( &(* parent) -> right, n);
		}else{
			return 0;
		}
	}

	return 1;

}

//递归方式打印二叉树
int print_tree(node * n){

	if(n == NULL){
		return 0;
	}else{
	
		print_tree(n -> left);

		printf("%d\n", n -> data);

		print_tree(n -> right);
	}

	return 1;
}

int main(int argc, char * argv[]){
	int data[] = {8,5,3,1,10,2,7,9,4,6};
	int len;
	GET_ARRAY_LEN(data, len);

	init_tree();

	node n;
	for (int i = 0; i < len; i++)
	{
		n.data = data[i];
		insert_tree(&root, &n);
	}

	printf("%s\n", "打印二叉树 :");
	print_tree(root);

	return 1;
}