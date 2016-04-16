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

	return insert( &(* relative_root_node), NULL, insert_node );

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
	int data[] = {8,5,3,1};	//只有左子树，测试节点旋转

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
	// return 1;


	printf("%s\n", "打印二叉树 :");
	print_tree(root);

	return 1;
}