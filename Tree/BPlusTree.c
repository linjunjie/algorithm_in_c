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
 *  2016-07-04：
 *	B+树的删除仍然比其他操作都复杂，但是竟然用了一个下午的时间就实现了......并且中间没有调试，一气呵成！然后几乎是一次调试就通过了。。。真的假的。。。连我自己都佩服我自己了......
 * 	因为删除操作中包括合并节点，借关键字，更新索引等等操作，东西还是蛮多的，没想到能一次通过。。。。。。佩服自己！
 *
 *	2016-07-05：
 *	我还是太乐观了，在我昨天就以为已经搞定删除的时候，没想到在删除合并到最后根节点的地方又有问题，然后才发现自己删除节点的时候，自始至终的节点合并都有问题，
 *	在合并完当前节点之后，应该一直递归遍历往祖先节点合并的，另外在合并到祖先节点的时候也有bug。。。
 *	就这样在经历了一整天无数次的痛苦调试之后，才终于调通删除逻辑。。。啊。。。真的好辛苦啊。。。
 *	每当觉得基本告一段落的时候，就发现其实自己高兴太早了。。。会有无数个bug不知道从哪里蹦出来。。。
 *	还是水平不够吧@_@
 *	
 *	
 * 
 *	Q & A
 *  Q1:能不能有二阶B+树？
 *  A1:一般B+树种的阶M>2，所以二阶B+树没有意义
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

typedef struct bptree_node{
	int keynum;		//关键字数量
	int isleaf;		//是否叶节点
	int pos;		//当前节点在父节点的位置
	int keys[BPLUSTREE_M];	//关键字域
	struct bptree_node * parent;	//父指针
	struct bptree_node * left;		//左兄弟指针，增加这个是为了删除时候合并的方便
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

//删除
bool bptree_remove(int key);
bpnode * find_key(bpnode * node, int key);
void update_index ( bpnode * node );
void bptree_merge( bpnode * current , bpnode * target );
void bptree_merge_recursive( bpnode * node );

/**
 * 创建一个新的节点
 */
bpnode * bptree_create_node()
{
	bpnode * dummy_root = (bpnode *)malloc(sizeof(bpnode));
	dummy_root -> parent = null;
	dummy_root -> left = null;
	dummy_root -> right = null;
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
					if ( key < root -> keys[ i - 1 ] )
					{
						root -> keys[ i ] = root -> keys[ i - 1 ];
						i--;
					}
					else
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
				int i = 0;
				while( i < root -> keynum )
				{
					if ( key < root -> keys[i] )
					{
						bptree_insert( root -> children[i], key );
						return;
					}
					i++;
				}

				//如果都没找到，则说明此关键字是最大的，插入到最后一个孩子节点当中去
				bptree_insert( root -> children[ root -> keynum - 1 ], key );
				update_index( root -> children[ root -> keynum - 1 ] );
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

	//更新右兄弟指针
	right -> right = node -> right;
	node -> right = right;

	//更新左兄弟指针
	if( right -> right != null )
	{
		right -> right -> left = right;
	}

	right -> left = node;
}



/**
 * 删除关键字
 *
 * 基本思路是这样的（主要取决于节点的丰满度）：
 * 1. 如果删除节点之后此节点的丰满度仍然 >= T, 则无需合并操作(只需要更新节点索引即可)
 * 2. 如果删除之后 < T, 则查看兄弟节点能不能借给我们关键字：
 * 		2.1 如果兄弟节点丰满度 > T, 则可以借
 * 		2.2 如果兄弟节点丰满度 = T, 则合并
 */
bool bptree_remove(int key)
{
	bpnode * node = find_key(bptree_root, key);

	if( node == null )	//如果没找到
	{
		return false;
	}
	else
	{
		//找到被删除元素的位置
		bool is_remove_the_max_key = false;
		int i;
		for (i = 0; i < node -> keynum; ++i)
		{
			if( key == node -> keys[i] )
			{
				//删除的是否为最大元素，如果是最大元素，则需要更新其祖先节点们的关键字索引值
				if ( i == node -> keynum - 1 )
				{
					is_remove_the_max_key = true;
				}

				//找到被删除元素则退出，此时的i即为被删除元素的实际位置
				break;
			}
		}

		//如果删除的不是最大元素，则被删除节点之后的元素往前移，覆盖上面被删除的元素
		if( is_remove_the_max_key == false )
		{
			while( i < node -> keynum - 1 )
			{
				node -> keys[ i ] = node -> keys[ i + 1 ];
				i++;
			}
		}
		node -> keynum -= 1;

		//如果删除的是最大关键字，则更新所有祖先节点关键字索引
		if ( is_remove_the_max_key )
		{
			update_index( node );
		}

		bptree_merge_recursive( node );		
	}

	return true;
}

void bptree_merge_recursive( bpnode * node )
{
	if ( node == bptree_root || node -> parent == bptree_root ) 
	{
		return;
	}

	//删除之后丰满度依然符合要求，则我们看他的兄弟节点是否有合并的需要
	if ( node -> keynum >= T )
	{
		if ( node -> keynum == T )
		{
			if ( node -> left != null && node -> left -> keynum < T )
			{
				bptree_merge_recursive( node -> left );
			}
			else if ( node -> right != null && node -> right -> keynum < T )
			{
				bptree_merge_recursive( node -> right );
			}
		}

	}
	/**
	 * 如果删除之后不够丰满了，则有两条路走：
	 * 1. 借：兄弟富裕的话，则向兄弟借孩子
	 * 2. 合：兄弟不富裕的话，则更兄弟合并
	 */
	else
	{
		//如果左孩子存在
		if ( node -> left != null )
		{
			//如果左孩子够丰满，则可以借左孩子关键字
			if( node -> left -> keynum > T )
			{
				//所有关键字右移，腾出第一个位置
				int j = node -> keynum;
				while ( j > 0 )
				{
					node -> keys[ j ] = node -> keys[ j - 1 ];
					node -> children[ j ] = node -> children[ j - 1 ];
					if( node -> children[ j ] != null )
					{
						node -> children[ j ] -> pos = j;
					}
					j--;
				}

				//把左边最大的关键字赋值给当前节点第一个位置
				node -> keys[ 0 ] = node -> left -> keys[ node -> left -> keynum - 1 ];
				node -> children[ 0 ] = node -> left -> children[ node -> left -> keynum - 1 ];
				if ( node -> children[ 0 ] != null )
				{
					node -> children[ 0 ] -> parent = node;
					node -> children[ 0 ] -> pos = 0;
				}
				
				//左边相应-1
				node -> left -> keynum -= 1;
				
				//当前相应+1
				node -> keynum += 1;

				//更新左节点索引
				update_index( node -> left );
			}
			//如果左孩子不够丰满，则合并左孩子
			else if( node -> left -> keynum <= T )
			{
				bptree_merge( node -> left, node );
			}
		}
		//没有左孩子，则判断右孩子
		else
		{
			//借右孩子
			if( node -> right -> keynum > T )
			{
				node -> keys[ node -> keynum ] = node -> right -> keys[ node -> right -> keynum - 1 ];

				//右节点所有关键字往前移
				int j = 0;
				while ( j < node -> right -> keynum )
				{
					node -> right -> keys[ j ] = node -> right -> keys[ j + 1 ];
					node -> right -> children[ j ] = node -> right -> children[ j + 1 ];
					if ( node -> right -> children[ j ] != null )
					{
						node -> right -> children[ j ] -> pos = j;
					}
					j++;
				}

				node -> right -> keynum -= 1;

				node -> keynum += 1;

				update_index( node -> right );
			}
			//合并右孩子
			else if( node -> right -> keynum <= T )
			{
				bptree_merge( node , node -> right );
			}
		}

		if( node -> parent != null )
		{
			bptree_merge_recursive( node -> parent );
		}
	}
}

/**
 * current 合并到 target
 *
 * current 在 target 的左边
 * 
 */
void bptree_merge( bpnode * current , bpnode * target )
{
	//如果是同一个父节点下
	if ( current -> parent == target -> parent )
	{
		//先把target的关键字往右移，给current要进来的关键字腾地方
		int i = target -> keynum - 1;
		while ( i >= 0 )
		{
			target -> keys[ i + current -> keynum ] = target -> keys[ i ];
			target -> children[ i + current -> keynum ] = target -> children[ i ];
			if ( target -> children[ i + current -> keynum ] != null )
			{
				target -> children[ i + current -> keynum ] -> pos = i + current -> keynum;
			}
			i--;
		}
		target -> keynum += current -> keynum;

		//把左边的关键字赋值给target（位置上面已经腾出来了）
		int j = 0;
		while( j < current -> keynum )
		{
			target -> keys[ j ] = current -> keys[ j ];
			target -> children[ j ] = current -> children[ j ];
			if ( target -> children[ j ] != null )
			{
				target -> children[ j ] -> pos = j;
			}
			j++;
		}

		//将parent的关键字和孩子都往左边移动
		j = current -> pos;
		int k = target -> parent -> keynum - 1;
		while ( j < k )
		{
			target -> parent -> keys[ j ] = target -> parent -> keys[ j + 1 ];
			target -> parent -> children[ j ] = target -> parent -> children[ j + 1 ];
			if ( target -> parent -> children[ j ] != null )
			{
				target -> parent -> children[ j ] -> pos = j;
			}
			target -> parent -> keynum -= 1;
			j++;
		}
		
	}
	//如果是不同父节点下
	else
	{
		//先把target的关键字（还有孩子）往右移，给current要进来的关键字（和孩子）腾地方
		int i = 0;
		while ( i < target -> keynum )
		{
			target -> keys[ i + current -> keynum ] = target -> keys[ i ];
			target -> children[ i + current -> keynum ] = target -> children[ i ];
			if ( target -> children[ i + current -> keynum ] != null )
			{
				target -> children[ i + current -> keynum ] -> pos = i + current -> keynum;
			}
			i++;
		}

		//把左边的关键字（还有孩子）赋值给target（位置上面已经腾出来了）
		i = 0;
		while( i < current -> keynum )
		{
			target -> keys[ i ] = current -> keys[ i ];
			target -> children[ i ] = current -> children[ i ];
			if ( target -> children[ i ] != null )
			{
				target -> children[ i ] -> parent = target;
				target -> children[ i ] -> pos = i;
			}
			i++;
		}

		//减掉左边节点对应的树的信息
		target -> keynum += current -> keynum;
		// current -> parent -> keys[ current -> parent -> keynum - 1 ] = 0;
		// current -> parent -> children[ current -> parent -> keynum - 1 ] = null;
		current -> parent -> keynum -= 1;
		update_index( current -> parent );
	}

	//如果已经合并到根节点
	if ( target -> parent == bptree_root )
	{
		//则将target置为根节点即可
		bptree_root = target;
	}
	else
	{
		//更新兄弟指针
		target -> left = current -> left;
		if( current -> left != null )
		{
			current -> left -> right = target;
		}
	}

	//彻底删除current
	free(current);
}

/**
 * 更新节点的索引信息
 */
void update_index ( bpnode * node )
{
	if( node != bptree_root )
	{
		node -> parent -> keys[ node -> pos ] = node -> keys[ node -> keynum - 1 ];
		if( node -> pos == node -> parent -> keynum - 1 )
		{
			return update_index( node -> parent );
		}
	}
	else
	{
		return;
	}
}

/**
 * 找到关键字所在的节点
 */
bpnode * find_key(bpnode * node, int key)
{
	if( node -> isleaf )
	{
		return node;
	}

	int i;
	for (i = 0; i < node -> keynum; ++i)
	{
		if( key <= node -> keys[i] )
		{
			return find_key( node -> children[i], key );
		}
	}

	//否则就是没找到
	return null;
}

int main(int argc, char * argv[]){
	// int data[] = {20,8,5,3,1,10,2,7,9,4,6,18,11,12,15};
	int data[] = {20,8,5,3,1,10,2,7,9,4,6,18};
	// int data[] = {20,8,5,3};
	// int data[] = {1,2,3,6};
	int len;
	GET_ARRAY_LEN(data, len);

	bptree_init();

	//插入节点
	for (int i = 0; i < len; i++)
	{
		bptree_insert(bptree_root, data[i]);
	}

	/**
	 * 	测试节点右兄弟指针
	 * 	test node -> right point for int data[] = {20,8,5,3,1,10,2,7,9,4,6,18};
	 * 	int data[] = {20,8,5,3,1,10,2,7,9,4,6,18};
	 */
	// bpnode * node = bptree_root -> children[0] -> children[0];
	// while( node != null )
	// {
	// 	printf("%d\n", node -> keys[0]);
	// 	node = node -> right;
	// }

	/**
	 * 	测试节点左兄弟指针
	 * 	test node -> left point for int data[] = {20,8,5,3,1,10,2,7,9,4,6,18};
	 * 	int data[] = {20,8,5,3,1,10,2,7,9,4,6,18};
	 */
	// bpnode * node = bptree_root -> children[1] -> children[1];
	// while( node != null )
	// {
	// 	printf("%d\n", node -> keys[0]);
	// 	node = node -> left;
	// }

	//test remove
	bptree_remove(18);
	bptree_remove(20);
	bptree_remove(10);
	bptree_remove(7);
	bptree_remove(9);
	bptree_remove(5);
	bptree_remove(4);
	bptree_remove(8);
	bptree_remove(3);
	bptree_print(bptree_root);

	return 1;
}