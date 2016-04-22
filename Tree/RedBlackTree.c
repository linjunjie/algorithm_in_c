/**
 * 红黑树
 *
 * 因为红黑树不涉及到高度平衡度的旋转，所以插入效率肯定比AVL高，因为不涉及到复杂的旋转
 * 但是也就是因为如此，所以平衡度肯定不如AVL树
 *
 * 红黑树的平衡规则是根据红黑树自己的节点颜色定义来调整的，而不是AVL那样根据左右子树深度差不能超过1来调整的
 * 这就是红黑树和AVL树的平衡调整的不同
 *
 * @Description: 	Description
 * @Datetime: 		2016-04-19 16:03:56
 * @Author: 		linjunjie
 */

#include "algorithm.h"

typedef struct red_black_node{
	int data;
	struct red_black_node * parent;
	struct red_black_node * left;
	struct red_black_node * right;
	int color;			// 颜色标识
} rbnode;

#define BLACK 0
#define RED 1

rbnode * rbt_root;

//打印类函数
int print_tree_inorder(rbnode * n);
int print_tree_preorder(rbnode * n);
int print_tree_postorder(rbnode * n);
int print_tree(rbnode * n, char * order);

//树相关操作函数
rbnode * get_grand_parent(rbnode * node);
rbnode * get_uncle(rbnode * node);
void rotate_left(rbnode * node);
void rotate_right(rbnode * node);
void modify_color(rbnode * * root, rbnode * node);
rbnode * insert_node_to_red_black_tree(rbnode * * root, rbnode * node);


void init_red_black_tree()
{
	rbt_root = NULL;
}

//得到爷节点（祖父节点）
rbnode * get_grand_parent(rbnode * node)
{
	return node -> parent -> parent;
}

//得到叔节点（也就是父节点的平行节点）
rbnode * get_uncle(rbnode * node)
{
	if(node -> parent == get_grand_parent(node) -> left)
	{
		return get_grand_parent(node) -> right;
	}
	else
	{
		return get_grand_parent(node) -> left;
	}
}

/**
 *	左旋转，这种情况就是右边高度过高了
 *
 *	算法描述
 *	rotate_left(t)
 *	1.	k <- right(t)
 *	2.	right(t) <- left(k)
 *	3.	left(k) <- t
 *	4.	parent(k) <- parent(t)
 */
void rotate_left(rbnode * node)
{
	rbnode * tmp = node -> right;
	node -> right = tmp -> left;
	tmp -> left = node;
	tmp -> parent = node -> parent;

	//如果插入节点有父节点
	if(node -> parent != NULL)
	{
		//如果插入节点是父节点的左子节点
		if(node -> parent -> left == node)
		{
			node -> parent -> left = tmp;
		}
		else
		{
			node -> parent -> right = tmp;
		}
	}
	else
	{
		rbt_root = tmp;
	}

	node -> parent = tmp;
}

/**
 *	右旋转，这种情况就是左边过高了
 *
 *	看到有对右旋的算法描述很好，拿来这里看一下：
 *
 *	rotate_right(t)
 *	1.	k <- left(t)
 *	2.	left(t) <- right(k)
 *	3.	right(k) <- t
 *	4.	parent(k) <- parent(t)
 */
void rotate_right(rbnode * node)
{
	rbnode * tmp = node -> left;
	node -> left = tmp -> right;
	tmp -> right = node;
	tmp -> parent = node -> parent;

	//如果插入节点有父节点
	if(node -> parent != NULL)
	{
		//如果插入节点是父节点的左子节点
		if(node -> parent -> left == node)
		{
			node -> parent -> left = tmp;
		}
		else
		{
			node -> parent -> right = tmp;
		}
	}
	//如果父节点为空，那就是根节点啊兄弟！！！怎么这都能忘！！！
	else
	{
		rbt_root = tmp;
	}

	//上面的操作结束之后，终于可以把插入节点的父节点设置为左子节点
	node -> parent = tmp;
}

rbnode * find_node_in_red_black_tree(rbnode * root, rbnode * node){
	if(root == NULL){
		return NULL;
	}

	if(node -> data < root -> data){
		return find_node_in_red_black_tree(root -> left, node);
	}else if(node -> data > root -> data){
		return find_node_in_red_black_tree(root -> right, node);
	}else if(node -> data == root -> data) {
		return root;
	}

	return NULL;
}

//这里的插入就是BST的插入
rbnode * insert_node_with_bst(rbnode * * current, rbnode * parent, rbnode * insert)
{
	if((* current) == NULL)
	{
		rbnode * node;
		node = (rbnode *)malloc(sizeof(rbnode));
		node -> color = RED;		//默认插入红色
		node -> data = insert -> data;
		node -> parent = parent;
		node -> left = NULL;
		node -> right = NULL;
		*current = node;

		return *current;
	}
	else
	{
		if(insert -> data < (* current) -> data)
		{
			return insert_node_with_bst(&((* current) -> left), *current, insert);
		}
		else
		{
			return insert_node_with_bst(&((* current) -> right), *current, insert);
		}
	}

	return NULL;
}

/**
 *	红黑树中的节点插入
 *	
 *	很多地方在介绍插入的时候，为了简化，默认是这么定义的：
 *
 *	G节点：爷爷节点，祖父节点
 *	P节点：父节点
 *	U节点：叔父节点
 *	N节点：插入节点
 *
 *	另外经过种种权衡，我们选择将插入的节点的默认颜色设置为红色（你可以去了解下为什么需要默认设置为红色而不是黑色）
 *
 *	在对红黑树颜色做调整的时候，我们需要注意比较常遇到的两点：
 *	1.父子节点不能同时为红色
 *	2.一个路径上的黑色高度是不允许改变的
 *
 *
 *	很多地方把红黑树的颜色调整分解为5种情况，这样挺便于理解的：
 *	1.	空树的情况，则直接设置为黑色
 *	2.	父节点黑色，则颜色不变更
 *	3.	父节点红色，叔节点红色
 *	4.	父节点红色，叔节点黑色
 *	5.1 插入节点是父节点的右孩子
 *	5.2 插入节点是父节点的左孩子
 */
rbnode * insert_node_to_red_black_tree(rbnode * * root, rbnode * insert)
{
	//如果根节点为空
	if(root == NULL)
	{
		printf("%s\n", "case 1");
		rbnode * n;
		n = (rbnode *)malloc(sizeof(rbnode));
		n -> color = BLACK;		//根节点为黑色
		n -> data = insert -> data;
		n -> parent = NULL;
		n -> left = NULL;
		n -> right = NULL;
		*root = n;

		return *root;
	}

	//节点插入
	rbnode * node = insert_node_with_bst(root, NULL, insert);

	modify_color(root, node);

	return node;
}

void modify_color(rbnode * * root, rbnode * node)
{
	//如果根节点为空，也就是说是一棵空树的话，很简单，直接调整根节点颜色为黑色即可
	if(node -> parent == NULL)
	{
		printf("%s\n", "case 1");
		node -> color = BLACK;
	}
	//如果插入节点的父节点是黑色，那么红黑树颜色是不需要调整的
	else if(node -> parent -> color == BLACK)
	{
		printf("%s\n", "case 2");
		// return NULL;
	}
	/**
	 *	否则父节点是红色，叔节点也存在并且为红色
	 *	因为红黑树不能允许父子节点同时为红色，所以这里我们需要调整
	 *	然后我们考虑一种特殊情况，那就是如果父节点和叔节点此时如果同时是红色，那么问题就变得简单了
	 *
	 *			黑                                              红
	 *		   /  \                                           /   \
	 *		 红    红                  ======>               黑     黑
	 *		/                                              /
	 *	  红 <-(insert node)                              红 <-- (inserted)
	 *
	 *	如上图，我们调整的方法就是把爷节点变成红色，然后父节点和叔节点都变成黑色就可以了
	 */
	else if(get_uncle(node) != NULL && get_uncle(node) -> color == RED)
	{
		printf("%s\n", "case 3");
		//叔父节点全部变为黑色
		node -> parent -> color = BLACK;
		get_uncle(node) -> color = BLACK;

		//爷节点调整为红色
		get_grand_parent(node) -> color = RED;

		/**
		 *	因为我们调整了爷爷节点（也就是G节点）的颜色，那么让我们想一想......这样搞肯定会涉及破坏了G节点及其P节点的红黑性质
		 *	所以此节点的G节点需要依次向上递归再次向上调整颜色
		 *	
		 *	很多地方在介绍这里的时候用了一个词：尾递归，其实不要被这个词吓住哈哈，都是一样的意思
		 */
		modify_color(root, get_grand_parent(node));
	}
	//那如果不是上面的情况，只有父节点是红色，叔节点不是红色（是黑色或者NULL）
	else
	{
		//插入节点是其父节点的右孩子，而父节点又是其父节点的左孩子
		if(node == node -> parent -> right && node -> parent == get_grand_parent(node) -> left)
		{
			printf("%s\n", "case 4.1");
			rotate_left(node -> parent);
			node = node -> left;
		}
		//插入节点是其父节点的左孩子，而父节点又是其父节点的右孩子
		else if(node == node -> parent -> left && node -> parent == get_grand_parent(node) -> right)
		{
			printf("%s\n", "case 4.2");
			rotate_right(node -> parent);
			node = node -> right;
		}

		node -> parent -> color = BLACK;
		get_grand_parent(node) -> color = RED;

		//插入节点是父节点的左节点，并且父节点是爷爷节点的左节点
		if(node == node -> parent -> left && node -> parent == get_grand_parent(node) -> left)
		{
			printf("%s\n", "case 5.1");
			rotate_right(get_grand_parent(node));
		}
		// (node == node -> parent -> left && node -> parent == get_grand_parent(node) -> right)
		else
		{
			printf("%s\n", "case 5.2");
			rotate_left(get_grand_parent(node));
		}
	}
}

int main(int argc, char * argv[]){
	int data[] = {8,5,3,1,10,2,7,9,4,6};
	// int data[] = {8,5,3,1,4};	//for test
	int len;
	GET_ARRAY_LEN(data, len);

	init_red_black_tree();

	rbnode n;
	for (int i = 0; i < len; i++)
	{
		n.data = data[i];
		insert_node_to_red_black_tree(&rbt_root, &n);
	}

	/**
	 *		最终形态为：
	 *			
	 *				  	     5[黑]
	 *			   		/    		\
	 *			 2[红]   				  8[红]
	 *			/	 \				   /		\
	 *		  1[黑]   3[黑]	  	    7[黑]		10[黑]
	 *					\		   /		    /
	 *					4[红]	6[红]		  9[红]
	 *			
	 */
	print_tree(rbt_root, "preorder");

	return 1;
}

//打印二叉树
int print_tree(rbnode * n, char * order){

	if(strcmp(order, "inorder") == 0)
	{
		printf("%s\n", "中序遍历打印红黑树 :");
		return print_tree_inorder(n);
	}
	else if(strcmp(order, "preorder") == 0)
	{
		printf("%s\n", "前序遍历打印红黑树 :");
		return print_tree_preorder(n);
	}
	else if(strcmp(order, "postorder") == 0)
	{
		printf("%s\n", "后序遍历打印红黑树 :");
		return print_tree_postorder(n);
	}

	printf("%s\n", "中序遍历打印红黑树 :");
	return print_tree_inorder(n);
}

// 中序遍历
int print_tree_inorder(rbnode * n){
	if(n == NULL){
		return 0;
	}else{
		print_tree_inorder(n -> left);
		printf("%d,%d\n", n -> data, n -> color);
		print_tree_inorder(n -> right);
	}

	return 1;
}

//递归方式打印二叉树
//前序遍历，父节点位于最前
int print_tree_preorder(rbnode * n){

	if(n == NULL){
		return 0;
	}else{
		printf("%d,%d\n", n -> data, n -> color);
		print_tree_preorder(n -> left);
		print_tree_preorder(n -> right);
	}

	return 1;
}

//后序遍历
int print_tree_postorder(rbnode * n){

	if(n == NULL){
		return 0;
	}else{
		print_tree_postorder(n -> left);
		print_tree_postorder(n -> right);
		printf("%d,%d\n", n -> data, n -> color);
	}

	return 1;
}