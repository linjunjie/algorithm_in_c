/**
 *	
 *	Balanced Binary Tree - AVL树 - 平衡二叉树 (Adelson-Velskii and Landis @ 1962)
 *
 *	其实平衡二叉树很好理解，其实在最开始实现二叉查找树的时候，你会发现二叉查找树如果变成一个链表的话，也是有可能的 - -! 因为没有规则来限定它不能变为一个链表
 *	所以为了避免这种情况，相信你也想过，那就是通过旋转节点来实现树平衡，也就是将一个太长的链表变成一个长得更像是树的结构，
 *	并且这里还有一个规则：左右子树的高度只差不能超过1
 *
 *	所以你可以简单的将平衡二叉树理解为：对执行过操作之后的二叉查找树进行平衡操作之后的树。
 *
 *	所以平衡二叉树最主要的是增加了节点的旋转操作，这个旋转的操作主要是在插入或者删除之后再进行的
 *	
 *  @Description: 	Description
 *  @Datetime: 		2016-04-16 14:28:14
 *  @Author: 		linjunjie
 *
 */

#include "algorithm.h"

typedef struct tree_node{
	int data;
	struct tree_node * parent;
	struct tree_node * left;
	struct tree_node * right;
	int height;					//节点高度，可以根据节点高度计算平衡因子，平衡二叉树中平衡度因子的定义为左子树深度减去右子树的深度, 应该只有-1，0，1这三个值，也即是绝对值应不大于1
} node;

node * root;

/*
 *	函数声明
 */
node * find_node_in_balanced_binary_tree(node * parent, node * n);
int insert_node_to_balanced_binary_tree(node * * relative_root_node, node * insert_node);
node * find_max_node(node * parent);
int get_node_height(node * n);	//得到节点的平衡度
node * rotate_on_left(node * * n);	//左边重了，要消除左边重的部分

//基本上是比较两个整数的大小，这里其实是传入左右两个子树的高度，看谁的高度更高而已
int get_max_height(int a, int b){
	return a > b ? a : b;
}

//取得节点的高度，这里是直接拿到节点的高度值即可
int get_node_height(node * n)
{
	/**
	 *	注意，这里的意思，不是说遍历到的节点如果是NULL，则返回失败-1。
	 *	这里的意思是，如果被遍历到的节点是空，也就是NULL的话，这个节点的元素平衡度，就是-1，这样方便后面计算上一级节点的平衡度
	 */
	if(n == NULL)
	{
		return -1;
	}

	return n -> height;
}

void init_tree()
{
	root = NULL;
}

// LL旋转
node * rotate_single_left(node * n)
{
	node * tmp = n -> left;
	n -> left = tmp -> right;
	tmp -> right = n;

	//维护变化节点的parent指针
	if(n -> parent != NULL)
	{
		if(n -> parent -> left == n)
		{
			n -> parent -> left = tmp;
		}
		else
		{
			n -> parent -> right = tmp;
		}
	}
	tmp -> parent = n -> parent;
	n -> parent = tmp;

	n -> height = get_max_height(get_node_height(n -> left), get_node_height(n -> right)) + 1;
	tmp -> height = get_max_height(get_node_height(tmp -> left), get_node_height(tmp -> right)) + 1;

	return tmp;
}

// RR旋转
node * rotate_single_right(node * n)
{
	node * tmp = n -> right;
	n -> right = tmp -> left;
	tmp -> left = n;

	//维护变化节点的parent指针
	if(n -> parent != NULL)
	{
		if(n -> parent -> left == n)
		{
			n -> parent -> left = tmp;
		}
		else
		{
			n -> parent -> right = tmp;
		}
	}
	tmp -> parent = n -> parent;
	n -> parent = tmp;

	n -> height = get_max_height(get_node_height(n -> left), get_node_height(n -> right)) + 1;
	tmp -> height = get_max_height(get_node_height(tmp -> left), get_node_height(tmp -> right)) + 1;

	return tmp;
}

// LR旋转
node * rotate_double_left(node * n)
{
	//先将左边变为LL型
	n -> left = rotate_single_right(n -> left);

	//进行LL型处理
	return rotate_single_left(n);
}

// RL旋转
node * rotate_double_right(node * n)
{
	//先将右边处理为RR型
	n -> right = rotate_single_left(n -> right);

	//然后进行RR型处理
	return rotate_single_right(n);
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
		n -> height = 0;
		*pnode = n;

		return 1;
	}
	else
	{
		//小于则往左走
		if(pinsert -> data < (* pnode) -> data)
		{
			//先进行插入动作
			insert(&(* pnode) -> left, * pnode, pinsert);

			//如果这时候左子树 - 右子树 的平衡度大于1，那么我们就要通过旋转来维持平衡了
			if(get_node_height((* pnode) -> left) - get_node_height((* pnode) -> right) > 1)
			{
				//如果是插入了左子树的左边，则说明左边重了，需要进行LL旋转
				if(pinsert -> data < (* pnode) -> left -> data)
				{
					* pnode = rotate_single_left((* pnode));
				}
				//如果是插入了左子树的右边，则左子树需要进行LR旋转
				else
				{
					* pnode = rotate_double_left((* pnode));
				}
			}
		}
		//大于则往右走
		else if(pinsert -> data > (* pnode) -> data)
		{
			//先进行插入动作
			insert(&(* pnode) -> right, * pnode, pinsert);

			//失去平衡了
			if(get_node_height((* pnode) -> right) - get_node_height((* pnode) -> left) > 1)
			{
				//插入右子树的右边，右子树单次右旋转
				if(pinsert -> data > (* pnode) -> right -> data)
				{
					* pnode = rotate_single_right((* pnode));
				}
				//插入了右子树的左边，右子树要进行RL旋转
				else
				{
					* pnode = rotate_double_right((* pnode));
				}
			}
		}
	}

	//当前节点的高度等于其子树的最大高度+1
	(* pnode) -> height = get_max_height(get_node_height((* pnode) -> left), get_node_height((* pnode) -> right)) + 1;

	return 1;
}

// 对单个节点进行AVL调整的功能，在节点插入之后，对单个节点执行此功能即可
// insert_with_rotate_node函数使用了此函数
node * rotate_node(node * n)
{
	if(get_node_height(n -> left) - get_node_height(n -> right) > 1)
	{
		if(get_node_height(n -> left -> left) >= get_node_height(n -> left -> right))
		{
			n = rotate_single_left(n);
		}
		else
		{
			n = rotate_double_left(n);
		}
	}
	else if(get_node_height(n -> right) - get_node_height(n -> left) > 1)
	{
		if(get_node_height(n -> right -> right) >= get_node_height(n -> right -> left))
		{
			n = rotate_single_right(n);
		}
		else
		{
			n = rotate_double_right(n);
		}
	}

	return n;
}

/**
 *	这个函数跟上面insert函数的区别只在于在插入节点之后进行的旋转，使用的是封装好的rotate_node函数
 *	这里保留了上面的insert函数只为了让读者可以看到更完整的删除节点逻辑
 *	insert_with_rotate_node函数是通过rotate_node来进行平衡的函数，但是不如insert函数逻辑完整和利于阅读
 *
 * 	@params pnode 被遍历到的节点位置
 *	@params pparent 节点位置的父节点
 *	@params insert_node 要插入的节点
 */
int insert_with_rotate_node(node * * pnode, node * pparent, node * pinsert)
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
		n -> height = 0;
		*pnode = n;

		return 1;
	}
	else
	{
		//小于则往左走
		if(pinsert -> data < (* pnode) -> data)
		{
			//先进行插入动作
			insert_with_rotate_node(&((* pnode) -> left), * pnode, pinsert);
		}
		//大于则往右走
		else if(pinsert -> data > (* pnode) -> data)
		{
			//先进行插入动作
			insert_with_rotate_node(&((* pnode) -> right), * pnode, pinsert);
		}

		//对节点进行AVL调整
		*pnode = rotate_node((* pnode));
	}

	//当前节点的高度等于其子树的最大高度+1
	(* pnode) -> height = get_max_height(get_node_height((* pnode) -> left), get_node_height((* pnode) -> right)) + 1;

	return 1;
}

/**
 * 	@params relative_root_node 相对根节点，所谓相对根节点也就是在某个局部树中的根节点
 *			不一定是真正的根节点，但是这个局部树可大可小，就算把整个树看做局部树也是可以的
 *			
 *	@params insert_node 需要插入的节点
 */
int insert_node_to_balanced_binary_tree(node * * relative_root_node, node * insert_node){

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
		n -> height = 0;

		*relative_root_node = n;

		return 1;
	}

	// return insert(relative_root_node, NULL, insert_node );				//第一种插入函数
	return insert_with_rotate_node(relative_root_node, NULL, insert_node );	//第二种插入方式

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
 *
 *	这里使用的是二叉查找树里的删除节点方法
 *	
 *	但是平衡二叉树和二叉查找树的删除节点规则不同，所以还需要修改
 *
 */
int delete_node_from_binary_search_tree_with_bst_tree(node * * parent, node * delete_node){
	if(parent == NULL || delete_node == NULL){
		return 0;
	}

	//函数内部临时代替parent
	node * tmp = *parent;

	//node_gonna_delete也就是将要被删除的节点
	node * node_gonna_delete = NULL;
	node_gonna_delete = find_node_in_balanced_binary_tree(* parent, delete_node);

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

		// free(node_gonna_delete);
		// return 1;
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
	}

	free(node_gonna_delete);

	//重新计算根节点的高度
	tmp -> height = get_max_height(get_node_height(tmp -> left), get_node_height(tmp -> right)) + 1;
	
	//左子树如果不空则调整左子树平衡
	if(tmp -> left != NULL)
	{
		//确保左子树平衡
		tmp -> left = rotate_node(tmp -> left);
	}

	//右子树若非空则调整右子树平衡
	if(tmp -> right != NULL)
	{
		//确保右子树平衡
		tmp-> right = rotate_node(tmp -> right);
	}

	//根节点自己调节平衡
	if(tmp != NULL)
	{
		//确保根节点平衡
		tmp = rotate_node(tmp);
	}

	return 1;

}

/**
 *	改造了之前二叉查找树的删除逻辑
 *	这里的参数 node * parent,其实就是局部的根节点
 *	在代码的注释里统一称为根节点（并不是树的真正根节点），不要混淆
 *
 */
node * delete_node_from_balanced_binary_tree(node * parent, node * delete_node){
	if(parent == NULL || delete_node == NULL){
		return NULL;
	}

	//node_gonna_delete也就是将要被删除的节点
	node * node_gonna_delete = NULL;
	node_gonna_delete = find_node_in_balanced_binary_tree(parent, delete_node);

	//如果没有找到被删除的节点，则返回
	if(node_gonna_delete == NULL){
		return NULL;
	}

	//如果被删除的是根节点
	if(parent == node_gonna_delete)
	{
		//如果右子树为空，则直接删除此节点
		if(parent -> right == NULL)
		{
			node * tmp = parent;
			parent = parent -> left;
			free(tmp);
		}
		//如果右子树不为空，则用右树种的最小元素代替根节点
		else
		{
			//找到右节点的最小元素
			node * tmp = parent -> right;
			while(tmp -> left != NULL)
				tmp = tmp -> left;

			//将此元素赋值给局部树的根节点
			parent -> data = tmp -> data;

			//从右树中删除节点
			parent -> right = delete_node_from_balanced_binary_tree(parent -> right, tmp);

			//重新计算现在根节点的高度值
			parent -> height = get_max_height(get_node_height(parent -> left), get_node_height(parent -> right)) + 1;
		}

		return parent;
	}
	//否则
	else
	{
		//被删除的节点小于根节点
		if(delete_node -> data < parent -> data)
		{
			//从左子树中查找删除此元素
			parent -> left = delete_node_from_balanced_binary_tree(parent -> left, delete_node);
		}
		//被删除的节点大于根节点
		else
		{
			//从右子树中查找删除此元素
			parent -> right = delete_node_from_balanced_binary_tree(parent -> right, delete_node);
		}
	}

	/*** 以上为删除节点操作，上面删除节点的操作结束之后，下面就要进行节点的高度计算和平衡操作 ***/

	//重新计算根节点的高度
	parent -> height = get_max_height(get_node_height(parent -> left), get_node_height(parent -> right)) + 1;
	
	//左子树如果不空则调整左子树平衡
	if(parent -> left != NULL)
	{
		//确保左子树平衡
		parent -> left = rotate_node(parent -> left);
	}

	//右子树若非空则调整右子树平衡
	if(parent -> right != NULL)
	{
		//确保右子树平衡
		parent-> right = rotate_node(parent -> right);
	}

	//根节点自己调节平衡
	if(parent != NULL)
	{
		//确保根节点平衡
		parent = rotate_node(parent);
	}

	//返回一切操作尘埃落定之后的根节点
	return parent;
}

/**
 *	二叉树中寻找指定值的节点
 *
 *	函数返回的是二叉树中的节点指针
 */

node * find_node_in_balanced_binary_tree(node * parent, node * n){
	if(parent == NULL){
		return NULL;
	}

	if(n -> data < parent -> data){
		return find_node_in_balanced_binary_tree(parent -> left, n);
	}else if(n -> data > parent -> data){
		return find_node_in_balanced_binary_tree(parent -> right, n);
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
	 *		平衡二叉树：
	 *
	 *				    5
	 *			     /     \
	 *			   2         8
	 *			 /  \     	/ \
	 *			1    3     7   10
	 *		          \   /   /
	 *		           4 6   9
	 */
	int data[] = {8,5,3,1,10,2,7,9,4,6};

	//删除根节点的情况
	// int data[] = {8,5,3,1};	//只有左子树，测试节点旋转

	int len;
	GET_ARRAY_LEN(data, len);

	init_tree();

	node n;
	for (int i = 0; i < len; i++)
	{
		n.data = data[i];
		insert_node_to_balanced_binary_tree(&root, &n);
	}

	/**
	 *	寻找元素为8的节点，并得到节点指针
	 */
	// n.data = 8;
	// node * node_to_find = find_node_in_balanced_binary_tree(root, &n);
	// printf("%p\n", node_to_find);
	// printf("%d\n", node_to_find -> data);
	// printf("%d\n", node_to_find -> parent -> data);
	// return 1;

	/**
	 *	删除数据值为8的节点元素
	 */
	// n.data = 8;
	// root = delete_node_from_balanced_binary_tree(root, &n);
	// n.data = 5;
	// root = delete_node_from_balanced_binary_tree(root, &n);
	// n.data = 1;
	// root = delete_node_from_balanced_binary_tree(root, &n);
	// n.data = 10;
	// root = delete_node_from_balanced_binary_tree(root, &n);

	//用从BST删除节点改造过来的方法删除节点
	n.data = 8;
	delete_node_from_binary_search_tree_with_bst_tree(&root, &n);
	n.data = 5;
	delete_node_from_binary_search_tree_with_bst_tree(&root, &n);

	print_tree(root, "inorder");

	return 1;
}