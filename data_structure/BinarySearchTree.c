/**
 * 以下注释使用了这个Sublime插件生成(你可以根据自己的需要任意修改):
 * https://github.com/linjunjie/sublime_plugins/blob/master/addComments.py
 *			
 * @Description: 	Binary - 二叉树
 * @Datetime: 		2016-04-10 19:08:53
 * @Author: 		linjunjie
 *
 * 其实这是一个二叉查找树，所以应更名为 Binary Search Tree - 简称 BST
 */

#include "algorithm.h"

typedef struct tree_node{
	int data;
	struct tree_node * parent;	//一个指向父节点的指针，这是因为如果你删除的是一个子节点的时候，需要把相应父节点的左或右节点设置为NULL
	struct tree_node * left;
	struct tree_node * right;
} node;

node * root;

/*
 *	函数声明
 */
node * find_node_in_binary_search_tree(node * parent, node * n);
// int insert(node * * pnode, node * pparent, node * pinsert);
int insert_node_to_binary_search_tree(node * * relative_root_node, node * insert_node);
node * find_max_node(node * parent);


void init_tree(){
	root = NULL;
}


/**
 * 	@params pnode 被遍历到的节点位置
 *	@params pparent 节点位置的父节点
 *	@params insert_node 要插入的节点
 */
int insert(node * * pnode, node * pparent, node * pinsert)
{

	if((*pnode) == NULL)
	{
		/**
		 *	开辟一个节点
		 *	其实节点都是在这里开辟和添加的
		 */
		node * n;
		n = (node *)malloc(sizeof(node));
		n -> parent = n -> left = n -> right = NULL;
		n -> data = pinsert -> data;
		n -> parent = pparent;
		*pnode = n;

		return 1;
	}
	else
	{
		if(pinsert -> data < (* pnode) -> data){
			insert(&((* pnode) -> left), *pnode, pinsert);
		}else if(pinsert -> data > (* pnode) -> data){
			insert(&((* pnode) -> right), *pnode, pinsert);
		}else{
			return 0;
		}
	}

	return 1;
}

/**
 * 	@params relative_root_node 相对根节点，所谓相对根节点也就是在某个局部树中的根节点
 *			不一定是真正的根节点，但是这个局部树可大可小，就算把整个树看做局部树也是可以的
 *			
 *	@params insert_node 需要插入的节点
 */
int insert_node_to_binary_search_tree(node * * relative_root_node, node * insert_node){

	if((*relative_root_node) == NULL)
	{
		/**
		 *	开辟一个节点
		 *	其实节点都是在这里开辟和添加的
		 */
		node * n;
		n = (node *)malloc(sizeof(node));
		n -> parent = n -> left = n -> right = NULL;
		n -> data = insert_node -> data;

		*relative_root_node = n;

		return 1;
	}

	return insert(relative_root_node, NULL, insert_node );

}


/**
 *
 * 	@params relative_root_node 相对根节点
 *	@params insert_node 需要删除的节点
 *
 *	这个方法是不包含处理父节点的函数，这个方法再删除左节点不是左子树最大节点的时候是无法正确删除节点的
 *	这里暂不删除这个方法，留在这里让大家看看
 */
// int insert_node_to_binary_search_tree(node * * relative_root_node, node * insert_node){

// 	if((*relative_root_node) == NULL)
// 	{
// 		//开辟一个树节点
// 		node * n;
// 		n = (node *)malloc(sizeof(node));
// 		n -> parent = n -> left = n -> right = NULL;
// 		n -> data = insert_node -> data;

// 		*relative_root_node = n;
// 	}
// 	else
// 	{
// 		if(insert_node -> data < (* relative_root_node) -> data)
// 		{
// 			insert_node_to_binary_search_tree(&(*relative_root_node) -> left, insert_node);
// 		}
// 		else if(insert_node -> data > (* relative_root_node) -> data)
// 		{
// 			insert_node_to_binary_search_tree(&(*relative_root_node) -> right, insert_node);
// 		}else
// 		{
// 			return 0;
// 		}
// 	}

// 	return 1;

// }


/**
 *	从二叉树中根据值删除指定的节点
 *
 *	二叉树的删除比较复杂，但是我们可以一种情况一种情况的去分析
 *
 */
int delete_node_from_binary_search_tree(node * * parent, node * delete_node){
	if(parent == NULL || delete_node == NULL){
		return 0;
	}

	//函数内部临时代替parent
	node * tmp = *parent;

	//node_gonna_delete也就是将要被删除的节点
	node * node_gonna_delete = NULL;
	node_gonna_delete = find_node_in_binary_search_tree(* parent, delete_node);

	//如果没有找到被删除的节点，则返回
	if(node_gonna_delete == NULL){
		return 0;
	}

	//如果删除的元素恰好是局部根节点
	if(tmp == node_gonna_delete){

		//左右子树都没有
		if( tmp -> left == NULL && tmp -> right == NULL)
		{
			*parent = NULL;
		}
		//有左子树没有右子树
		else if(tmp -> left != NULL && tmp -> right == NULL)
		{
			tmp -> left -> parent = tmp -> parent;
			tmp = tmp -> left;
		}
		//有右子树没有左子树
		else if(tmp -> left == NULL && tmp -> right != NULL)
		{
			tmp -> right -> parent = tmp -> parent;
			tmp = tmp -> right;
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

			//如果最大节点就是左子树根节点，那么很简单，只需要把左子树整个提到根节点就好了
			if(pLeftMax == tmp -> left)
			{
				tmp -> left -> parent = tmp -> parent;
				tmp -> left -> right = tmp -> right;
				tmp = tmp -> left;
			}
			//如果左节点不是最大节点，那么就找到左子树中的最大节点提为根节点
			else
			{
				//在这里发现树节点是需要一个父节点指针的，否则删除的时候没法把父节点的右子节点设置为NULL
				//另外这里只是赋值的话，就不会涉及到删除父节点的左右子节点的情况，所以就避免了复杂性
				tmp -> data = pLeftMax -> data;

				//这里用到了被删除的左子树最大节点的父节点，需要把父节点指向的右子节点指向NULL
				pLeftMax -> parent -> right = pLeftMax -> left;

				//如果这个时候最大节点的右子节点不是NULL，那么还需要把右子节点的父节点改为指向最大节点的父节点
				if(pLeftMax -> left != NULL){
					pLeftMax -> left -> parent = pLeftMax -> parent;
				}

				//这个时候你删除的其实不是根节点，而是已经被找到的左子树中的最大节点
				node_gonna_delete = pLeftMax;
			}
		}

		free(node_gonna_delete);
		return 1;
	}
	//删除节点不是父节点时的情况
	else
	{
		//被删除的节点没有左子树也没有右子树
		if(node_gonna_delete -> left == NULL && node_gonna_delete -> right == NULL)
		{
			//如果是左节点，则将最后的左节点赋NULL
			if(node_gonna_delete -> parent -> left == node_gonna_delete)
			{
				node_gonna_delete -> parent -> left = NULL;
			}
			//如果是右节点，则将最后的左节点赋NULL
			else
			{
				node_gonna_delete -> parent -> right = NULL;
			}
		}
		//左子树 != NULL && 右子树 == NULL
		else if(node_gonna_delete -> left != NULL && node_gonna_delete -> right == NULL)
		{
			//如果在左侧，则直接将左子树的子节点上提
			if(node_gonna_delete -> parent -> left == node_gonna_delete)
			{
				node_gonna_delete -> parent -> left = node_gonna_delete -> left;
			}
			//如果在右侧，也是把右子树上的左子节点作为根节点的右子节点
			else
			{
				node_gonna_delete -> parent -> right = node_gonna_delete -> left;
			}
		}
		//左子树 == NULL && 右子树 != NULL
		else if(node_gonna_delete -> left == NULL && node_gonna_delete -> right != NULL)
		{
			//如果在左侧，则直接将左子树的右子节点上提
			if(node_gonna_delete -> parent -> left == node_gonna_delete)
			{
				node_gonna_delete -> parent -> left = node_gonna_delete -> right;
			}
			//如果在右侧，也是把右子树上的右子节点作为根节点的右子节点
			else
			{
				node_gonna_delete -> parent -> right = node_gonna_delete -> right;
			}
		}
		//左右子树均不为空
		else if(node_gonna_delete -> left != NULL && node_gonna_delete -> right != NULL)
		{
			//得到被删除节点左子树的最大节点
			node * pLeftMax = find_max_node(node_gonna_delete -> left);

			//如果左节点是被删除节点左侧做大的节点
			if(node_gonna_delete -> left == pLeftMax)
			{
				//如果被删除节点位于父节点的左侧，则修改父节点的左节点指向其左子树
				if(node_gonna_delete == node_gonna_delete -> parent -> left)
				{
					node_gonna_delete -> parent -> left = node_gonna_delete -> left;
				}
				//如果被删除节点位于父节点的右侧，则修改父节点的右节点指向其左子树
				else
				{
					node_gonna_delete -> parent -> right = node_gonna_delete -> left;
				}

				//将左节点父节点指向被删除节点的父节点
				node_gonna_delete -> left -> parent = node_gonna_delete -> parent;

				//将左节点的右节点指向被删除节点的右节点
				node_gonna_delete -> left -> right = node_gonna_delete -> right;

				//将被删除节点指向的整个左子树往上提到被删除节点的位置，所以这个时候被删除节点的左节点的父节点应该变为被提上来的被删除节点的左节点
				node_gonna_delete -> right -> parent = node_gonna_delete -> left;

			}
			//如果左节点不是被删除节点左侧的最大节点
			else
			{
				//将最大节点的数据赋值给被删除节点，因为后面被删除节点准备金蝉脱壳~~
				node_gonna_delete -> data = pLeftMax -> data;

				//将最大节点的父节点的右节点指向最大节点的左子树
				pLeftMax -> parent -> right = pLeftMax -> left;

				//将最大节点的左子树的父节点赋值为最大节点的父节点
				//当然前提是你要先检查下最大节点有没有左子节点
				if(pLeftMax -> left != NULL)
				{
					pLeftMax -> left -> parent = pLeftMax -> parent;
				}

				//被删除节点金蝉脱壳，被删除的其实是最大节点~~
				node_gonna_delete = pLeftMax;

			}
		}

		free(node_gonna_delete);
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

node * find_node_in_binary_search_tree(node * parent, node * n){
	if(parent == NULL){
		return NULL;
	}

	if(n -> data < parent -> data){
		return find_node_in_binary_search_tree(parent -> left, n);
	}else if(n -> data > parent -> data){
		return find_node_in_binary_search_tree(parent -> right, n);
	}else if(n -> data == parent -> data) {
		return parent;
	}

	return NULL;
}

int print_tree_inorder(node * n);
int print_tree_preorder(node * n);
int print_tree_postorder(node * n);

//打印二叉树
int print_tree(node * n, char * order){

	if(strcmp(order, "inorder") == 0)
	{
		printf("%s\n", "中序遍历打印二叉树 :");
		return print_tree_inorder(n);
	}
	else if(strcmp(order, "preorder") == 0)
	{
		printf("%s\n", "前序遍历打印二叉树 :");
		return print_tree_preorder(n);
	}
	else if(strcmp(order, "postorder") == 0)
	{
		printf("%s\n", "后序遍历打印二叉树 :");
		return print_tree_postorder(n);
	}

	printf("%s\n", "中序遍历打印二叉树 :");
	return print_tree_inorder(n);
}


// 中序遍历
int print_tree_inorder(node * n){
	if(n == NULL){
		return 0;
	}else{
		print_tree_inorder(n -> left);
		printf("%d\n", n -> data);
		print_tree_inorder(n -> right);
	}

	return 1;
}

//递归方式打印二叉树
//前序遍历，父节点位于最前
int print_tree_preorder(node * n){

	if(n == NULL){
		return 0;
	}else{
		printf("%d\n", n -> data);
		print_tree_preorder(n -> left);
		print_tree_preorder(n -> right);
	}

	return 1;
}

//后序遍历
int print_tree_postorder(node * n){

	if(n == NULL){
		return 0;
	}else{
		print_tree_postorder(n -> left);
		print_tree_postorder(n -> right);
		printf("%d\n", n -> data);
	}

	return 1;
}


int main(int argc, char * argv[]){
	/**
	 *
	 *				 8
	 *			   /   \
	 *			  5     10
	 *			 / \   /
	 *			3   7 9
	 *		   / \
	 *		  1   4
	 *		   \
	 *			2
	 */
	int data[] = {8,5,3,1,10,2,7,9,4,6};

	//删除根节点的情况
	// int data[] = {8,5,3};	//有左子树没有右子树
	// int data[] = {3,5,8};	//有右子树没有左子树
	// int data[] = {8,5,3,10};	//左右子树都有并且左节点就是左子树做大节点
	// int data[] = {8,5,7,10};	//左节点不是左子树最大节点

	int len;
	GET_ARRAY_LEN(data, len);

	init_tree();

	node n;
	for (int i = 0; i < len; i++)
	{
		n.data = data[i];
		insert_node_to_binary_search_tree(&root, &n);
	}

	/**
	 *	寻找元素为5的节点，并得到节点指针
	 */
	// n.data = 8;
	// node * node_to_find = find_node_in_binary_search_tree(root, &n);
	// printf("%p\n", node_to_find);
	// printf("%d\n", node_to_find -> data);


	/**
	 *	删除数据值为8的节点元素
	 */
	n.data = 3;
	delete_node_from_binary_search_tree(&root, &n);
	n.data = 8;
	delete_node_from_binary_search_tree(&root, &n);
	n.data = 10;
	delete_node_from_binary_search_tree(&root, &n);

	print_tree(root, "inorder");

	return 1;
}