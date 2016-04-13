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
	// struct tree_node * parent;	//一个指向父节点的指针
	struct tree_node * left;
	struct tree_node * right;
} node;

node * root;

/*
 *	函数声明
 */
node * find_node_in_binary_tree(node * parent, node * n);
int insert_node_to_binary_tree(node * * parent, node * insert_node);
node * find_max_node(node * parent);


void init_tree(){
	root = NULL;
}

int insert_node_to_binary_tree(node * * parent, node * insert_node){

	node * n;
	n = (node *)malloc(sizeof(node));
	n -> left = n -> right = NULL;
	n -> data = insert_node -> data;

	if((* parent) == NULL){
		* parent = n;
	}else{
		if(insert_node -> data < (* parent) -> data){
			insert_node_to_binary_tree( &(* parent) -> left, n);
		}else if(n -> data > (* parent) -> data){
			insert_node_to_binary_tree( &(* parent) -> right, n);
		}else{
			return 0;
		}
	}

	return 1;

}

/**
 *	从二叉树中根据值删除指定的节点
 *
 *	二叉树的删除比较复杂，我们要一种情况一种情况的去分析
 *
 */
int delete_node_from_binary_tree(node * * parent, node * delete_node){
	if(parent == NULL || delete_node == NULL){
		return 0;
	}

	//函数内部临时代替parent
	node * tmp = *parent;

	node * n = NULL;
	n = find_node_in_binary_tree(* parent, delete_node);

	if(n == NULL){
		return 0;
	}

	//如果删除的元素恰好是父节点
	if(tmp == n){

		//左右子树都没有
		if( tmp -> left == NULL && tmp -> right == NULL)
		{
			*parent = NULL;
		}
		//有左子树没有右子树
		else if(tmp -> left != NULL && tmp -> right == NULL)
		{
			root = tmp -> left;
		}
		//有右子树没有左子树
		else if(tmp -> left == NULL && tmp -> right != NULL)
		{
			root = tmp -> right;
		}
		/**
		 *	如果左右子树都有
		 *
		 *	1.左子树的根节点就是左子树里最大的节点的时候，那么很简单，把左子树提到根节点就好了
		 *
		 *	2.左子树的根节点不是左子树里最大的节点，那么就把左子树中最大的节点提为根节点
		 */
		else
		{
			//得到左子树的最大节点
			node * pLeftMax = find_max_node(tmp -> left);

			//如果最大节点就是左子树根节点
			if(pLeftMax == tmp -> left)
			{
				root = tmp -> left;
				root -> right = tmp -> right;
			}
			//如果左节点不是最大节点，那么就找到左子树中的最大节点提为根节点
			else
			{

			}
		}

		free(n);
		return 1;
	}

	return 1;
}

/**
 *	找到指定节点作为根节点开始的最大节点
 *	
 *	基本思路其实就是一直往右子树找就好了
 */
node * find_max_node(node * parent){
	if(parent == NULL){
		return NULL;
	}

	node * n = parent;

	while(n != NULL){
		if(n -> right == NULL){
			return n;
		}else{
			n = n -> right;
		}
	}

	return NULL;
}

/**
 *	二叉树中寻找指定值的节点
 *
 *	函数返回的是二叉树中的节点指针
 */

node * find_node_in_binary_tree(node * parent, node * n){
	if(parent == NULL){
		return NULL;
	}

	if(n -> data < parent -> data){
		return find_node_in_binary_tree(parent -> left, n);
	}else if(n -> data > parent -> data){
		return find_node_in_binary_tree(parent -> right, n);
	}else if(n -> data == parent -> data) {
		return parent;
	}

	return NULL;
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
	// int data[] = {8,5,3,1,10,2,7,9,4,6};

	//删除根节点的情况
	// int data[] = {8,5,3};	//有左子树没有右子树
	// int data[] = {3,5,8};	//有右子树没有左子树
	int data[] = {8,5,3,10};	//左右子树都有并且左节点就是左子树做大节点


	int len;
	GET_ARRAY_LEN(data, len);

	init_tree();

	node n;
	for (int i = 0; i < len; i++)
	{
		n.data = data[i];
		insert_node_to_binary_tree(&root, &n);
	}

	/**
	 *	寻找元素为5的节点，并得到节点指针
	 */
	// n.data = 8;
	// node * node_to_find = find_node_in_binary_tree(root, &n);
	// printf("%p\n", node_to_find);
	// printf("%d\n", node_to_find -> data);


	/**
	 *	删除数据值为8的节点元素
	 */
	n.data = 8;
	delete_node_from_binary_tree(&root, &n);


	printf("%s\n", "打印二叉树 :");
	print_tree(root);

	return 1;
}