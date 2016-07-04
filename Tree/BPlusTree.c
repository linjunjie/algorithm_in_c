/**
 * B+树不但比B树简单，还比B树高效，并且使用更广泛
 * 
 * m阶B+树的定义：
 *
 * 1. 每个节点至少有m棵子树
 * 2. 所有非叶节点的关键字数目等于他的孩子数量
 * 3. 除根节点以外的其他节点，必须有m/2棵子树
 * 4. 所有关键字信息都保存在叶子节点
 * 5. 所有叶子节点都在同一层，并且关键字数目大于ceil(m/2)
 * 6. 所有非叶子节点中的关键字都是子节点中的最大或者最小关键字
 *
 *	B+树的特点简单点说，就是除了叶子节点，其他节点只是保存关键字的索引，而不是保存关键字，所有关键字都保存在叶子终端结点中
 * 
 * 	感觉最欠缺的还是调试方式和调试工具，靠printf打断点来调试真的效率低下
 *  三个星期的时间，磕磕绊绊的才写出B+树，而且还只有添加，节点删除还没写，另外这个B+树也不稳固
 *  那些写B+树觉得难的，应该心里会有些安慰吧，比我笨的应该很少了...
 *
 * 	
 * 
 *	@Description: 	Description
 *	@Datetime: 		2016-07-04 10:37:00
 *	@Author: 		linjunjie
 */

#include "algorithm.h"

#define BPLUSTREE_M 3		// B+树阶
#define T (int)ceil((float)BPLUSTREE_M/2)	//B+树的度

#define null NULL

typedef struct bptree_node{
	int keynum;		//关键字数量
	int isleaf;		//是否叶节点
	int pos;		//当前节点在父节点的位置
	int keys[BPLUSTREE_M];	//关键字域
	struct bptree_node * parent;	//父指针
	struct bptree_node * right;		//右兄弟指针
	struct bptree_node * children[BPLUSTREE_M];	//保存的孩子指针
} bpnode;

bpnode * bptree_root;

bpnode * bptree_create_node();
void bptree_init();
void bptree_print(bpnode * root);
void bptree_insert(bpnode * root, int key);
void bptree_split(bpnode * node);
bpnode * find_insert_place(bpnode * node, int key);

/**
 * 创建一个新的节点
 */
bpnode * bptree_create_node()
{
	bpnode * dummy_root = (bpnode *)malloc(sizeof(bpnode));
	dummy_root -> keynum = 0;
	dummy_root -> isleaf = 1;	//默认叶节点
	for (int i = 0; i < BPLUSTREE_M; ++i)
	{
		dummy_root -> keys[i] = 0;
	}
	for (int i = 0; i < BPLUSTREE_M; ++i)
	{
		dummy_root -> children[i] = null;
	}
	return dummy_root;
}

void bptree_init()
{
	if( bptree_root == null )
	{
		bptree_root = bptree_create_node();
	}
}

void bptree_print(bpnode * root)
{
	if( root == null )
	{
		return;
	}

	if( root -> isleaf )
	{
		printf("[");
		for (int i = 0; i < root -> keynum; ++i)
		{
			if( i == root -> keynum - 1 )
			{
				printf("%d", root -> keys[i]);
			}
			else
			{
				printf("%d,", root -> keys[i]);
			}
		}
		printf("]");
		printf("isleaf:%d,", root -> isleaf);
		printf("pos:%d\n", root -> pos);	//就是因为缺少了这行打印出pos的调试信息，导致一直没有发现bug的问题所在，所以说调试过程中得到关键信息很重要！
	}
	else
	{
		if( root -> keynum > 0 )
		{
			for (int i = 0; i < root -> keynum; ++i)
			{
				bptree_print( root -> children[i] );
			}
		}
	}
}

/**
 * 其实节点的插入有两种
 * 一种是判断到满节点则先分裂再插入
 * 另外一种是判断到满节点，但是先不分裂，而是继续判断这个满节点的子节点中有没有插入的位置，并且这个插入的位置是否已满
 * 如果未满，那么我们完全可以不分裂第一个搜索到的满节点，而是直接插入这个位置，这样子可以避免过多的分裂（而过多的分裂会导致树的高度增加）
 */
void bptree_insert(bpnode * root, int key)
{
	//如果插入节点为空
	if(root == null)
	{
		//则先开辟一个节点
		bptree_init();

		//然后在这个新开辟的节点上插入关键字
		bptree_insert(bptree_root, key);
	}
	else
	{
		/**
		 * 如果节点没满，那么是两种情况：
		 * 1. 叶子节点的话，直接就可以插入了
		 * 2. 非叶子节点的话，要继续向叶子节点搜索
		 */
		if(root -> keynum < BPLUSTREE_M)
		{
			//如果是叶节点
			if(root -> isleaf)
			{
				//所有关键字往后移动
				int i = root -> keynum;
				while( i > 0 )
				{
					root -> keys[ i ] = root -> keys[ i - 1 ];
					i--;

					if( i - 1 >= 0 && key > root -> keys[ i - 1 ])
					{
						break;
					}
				}

				//关键字插入第一个位置
				root -> keys[ i ] = key;
				root -> keynum += 1;
			}
			//非叶子节点的话，则继续向下寻找插入
			else
			{
				//找到应该插入的位置
				int i;
				for (i = 0; i < root -> keynum; ++i)
				{
					if( key < root -> keys[i] )
					{
						//如果找到了适当的位置，则插入
						bptree_insert( root -> children[i], key );
						return;
					}
				}

				//如果都没找到，则插入到最后一个孩子节点当中去
				bptree_insert( root -> children[i], key );
			}
		}
		//如果节点已满
		else
		{
			/**
			 * 这里做一下优化，如果节点已满，如果不是叶节点的话，我们再判断一下其子节点的情况，如果子节点没满，也可以先不分裂此节点，先插入子节点
			 * 这样做的好处就是，有效减少树的高度
			 */

			//如果是叶子节点则不用想了，分裂就好了
			if(root -> isleaf)
			{
				bptree_split(root);
				bptree_insert(bptree_root, key);	//上面一行代码分裂了叶子节点，但是可能导致向父节点的连锁分裂，所以你不知道分裂到哪一个环节，所以还是从根节点插入关键字
			}
			//如果不是叶子节点的话，我们先看一下他应该插入的位置可不可以插入（也就是有没有满）
			else
			{
				//找到应该插入的位置
				bpnode * finder = find_insert_place(root, key);

				//如果找到应该插入的位置还没有满，则可以插入这里
				if(finder -> keynum < BPLUSTREE_M)
				{
					bptree_insert(finder, key);
				}
				//如果这个位置已经满了，则直接分裂当前节点然后再插入
				else
				{
					bptree_split(root);
					bptree_insert(bptree_root, key);	//上面一行代码分裂了叶子节点，但是可能导致向父节点的连锁分裂，所以你不知道分裂到哪一个环节，所以还是从根节点插入关键字
				}
			}
		}
	}
}

/**
 * 找到适合插入的位置节点
 */
bpnode * find_insert_place(bpnode * node, int key)
{
	if( node -> isleaf )
	{
		return node;
	}

	int i;
	for (i = 0; i < node -> keynum; ++i)
	{
		if( key < node -> keys[i] )
		{
			return find_insert_place( node -> children[i], key );
		}
	}

	return find_insert_place( node -> children[i], key );
}

/**
 * 节点分裂
 */
void bptree_split(bpnode * node)
{

	// 1. 新开辟节点并移动数据

	//如果要分裂一个节点，则首先要开辟一个新节点
	bpnode * right = bptree_create_node();

	//将左边节点的右边的数据全部转移到新开辟的节点上
	for (int i = T, j = 0; i < BPLUSTREE_M; ++i, ++j)
	{
		//赋值到右节点
		right -> keys[j] = node -> keys[i];
		right -> keynum += 1;
		right -> children[j] = node -> children[i];

		//赋值给right节点之后随即清空
		node -> keys[i] = 0;
		node -> keynum -= 1;
		node -> children[i] = null;

		//重置right子节点的父指针
		if(right -> children[j] != null)
		{
			right -> isleaf = 0;
			right -> children[j] -> parent = right;
			right -> children[j] -> pos = j;	//子节点的pos也要更新,这里注释是因为忘记了设置子节点的pos导致最后插入节点18的时候导致了重大bug
		}
	}

	// 2. 分析各种情况

	//如果当前被分裂节点是根节点
	if(node == bptree_root)
	{
		//则新开辟一个根节点
		bpnode * new_root = bptree_create_node();

		//此新的根节点后面会有孩子存在，所以为非叶子节点
		new_root -> isleaf = 0;

		//此节点左孩子为当前被分裂节点
		new_root -> children[0] = node;
		new_root -> children[1] = right;

		//给新的根节点赋予关键字
		new_root -> keys[0] = node -> keys[ node -> keynum - 1 ];
		new_root -> keys[1] = right -> keys[ right -> keynum - 1 ];
		new_root -> keynum += 2;

		//修改孩子的父指针
		node -> parent = new_root;
		right -> parent = new_root;

		//设定孩子的位置
		node -> pos = 0;
		right -> pos = 1;

		//将新开辟的节点作为新的根节点
		bptree_root = new_root;
	}
	else
	{
		//如果被分裂节点不是根节点，则必定存在父节点

		//将父节点的所有元素都后移
		int k = node -> parent -> keynum;
		while( k > node -> pos )
		{
			node -> parent -> keys[ k ] = node -> parent -> keys[ k - 1 ];
			node -> parent -> children[ k ] = node -> parent -> children[ k - 1 ];
			node -> parent -> children[ k ] -> pos = k;
			k--;
		}

		//将新节点插入被让出来的位置
		
		//调整孩子节点的位置
		node -> parent -> children[ k ] = node;
		node -> parent -> children[ k + 1 ] = right;

		node -> pos = k;
		right -> pos = k + 1;

		node -> parent -> keys[ k ] = node -> keys[ node -> keynum - 1 ];
		node -> parent -> keynum += 1;

		//新加入的右节点的父节点指向被分裂节点的父节点
		right -> parent = node -> parent;
	}
}


int main(int argc, char * argv[]){
	// int data[] = {20,8,5,3,1,10,2,7,9,4,6,18,11,12,15};
	int data[] = {20,8,5,3,1,10,2,7,9,4,6,18};
	int len;
	GET_ARRAY_LEN(data, len);

	bptree_init();

	//插入节点
	for (int i = 0; i < len; i++)
	{
		bptree_insert(bptree_root, data[i]);
	}

	bptree_print(bptree_root);

	return 1;
}