/**
 * 	B-Tree，B树，1970 年由 R. Bayer 和 E. McCreight 发明
 *
 * 	M阶B树的定义 - balanced tree of order m
 *	1.任意非叶子节点最多只有M个儿子，且M>2
 *	2.根节点的儿子数为[2,M]
 *	3.除根节点以外的非叶子节点的儿子数为[ceil(M/2),M]（也就是除终端节点之外的节点的孩子数最少为ceil(M/2)）
 *	4.每个节点存放至少ceil(M/2)-1和至多M-1个关键字；（但是最少2个关键字）
 *	5.非叶子节点的关键字个数 = 指向儿子的指针个数 - 1
 *	6.非叶子节点的关键字k[1],k[2], ... k[M-1];并且k[i] < k[i+1]
 *	7.非叶节点的指针：P[1],P[2], ... , P[M]；其中P[1]指向关键字小于K[1]的子树，P[M]指向关键字大于K[M-1]的子树，其他P[i]指向关键字属于（K[i-1],K[i]）的子树
 *	8.所有叶子节点位于同一层
 *
 *	如果你能根据上面的定义明白B树到底是什么，那真是万幸！你很可能天赋秉异！
 *	但是如果你看完之后还是搞不懂B树到底是什么，那我更能理解你（因为我也看不懂），因为网上到处都是这种脱离人类语言，晦涩难懂又绕来绕去的学术定义，一般人根本搞不懂，所以我们一步一步来解释清楚：
 *
 *	首先我们要搞清楚，B树阶的定义是不统一的（看见了吧，不是我们糊涂，而是本身B树的定义都不同，所以你可能一会儿看到这种定义，一会儿看到另一种定义，不糊涂才怪呢），看下面：
 *	Unfortunately, the literature on B-trees is not uniform in its terminology (Folk & Zoellick 1992, p. 362). 
 *	Bayer & McCreight (1972), Comer (1979), and others define the order of B-tree as the minimum number of keys in a non-root node. 
 *	Folk & Zoellick (1992) points out that terminology is ambiguous because the maximum number of keys is not clear. An order 3 B-tree might hold a maximum of 6 keys or a maximum of 7 keys. 
 *	Knuth (1998, p. 483) avoids the problem by defining the order to be maximum number of children (which is one more than the maximum number of keys). 
 *
 *	综上所述，因为B树中阶的定义乱七八糟，所以最后Knuth看不下去了，为了避免混淆，把order（也就是阶）定义为最大的孩子数（也就是最大的子节点数，最大的指针域）
 *	（其实我们应该明白一件事情，那就是，B树怎么被定义，阶被定义成什么都不重要，关键是我们要实现这么一种树，这种树就是这个样子的，嗯~~）
 *	Ok，搞清楚B树中阶的定义之后，我们可以开始接着搞明白m阶B树的定义：
 *	
 *	1.所有节点的数据域（也就是他们说的关键字）都比指针域（也就是他们说的子节点树，孩子数）少一个。
 *	2.根节点最少有两个子节点，至多有m个子节点。
 *	3.其他非叶节点最少有ceil(m/2)个子节点，最多有m个子节点；数据域最少有ceil(m/2)-1个，最多有m-1个
 *
 *	怎么样，上面三条就解释清楚的事情，他们非要用七八条定义把你搞糊涂才行...（虽然上面的解释不够严谨，但是指出了B树的几个关键特征，能让你大概了解B树到底是怎么一回儿事）。
 *
 *	我们再具体一点，我们来看一下四阶树的定义：
 *	1.所有节点的指针都比数据多一个，也就是这样的：[指针1,数据1,指针2,数据2,指针3]
 *	2.根节点最少有2个子节点，最多有4个。
 *	3.其他非叶节点最少有个2个子节点，最多有4个子节点；数据区最少有1个，最多有3个。
 *
 *	这么描述，是否能明白？
 *
 *	我们再用简短的语言描述一下M阶B树：B树是一棵多路搜索树，根据其可以最多叉的数目M称为M阶树
 *
 *	也看到网上有稍微简短清晰一点的定义：
 *	1.树中每个节点最多含有M棵子树
 *	2.若根节点不是叶子节点，则至少有2棵子树；
 *	3.除根节点之外的所有非叶子节点至少有ceil(M/2)棵子树
 *	4.每个非叶节点中包含信息keynum, ptr[0],key[0], ... ptr[keynum-1],key[keynum-1],ptr[keynum]
 *
 *	最后说下整个B树的编写经过：
 *		整个编写一个B树花费了两天半的时候，是的，从周三下午到周四，到今天周五下班，都在编写B树......你可以知道我有多笨了吧@_@
 *		前一天半，我是参考B树的定义以及网上给出的例程在理解B树以及其如何实现，但是整个周四我都在调试网上给出的例程，当在插入最后一个节点6的时候，就出错了
 *		我始终找不到是哪里的问题，一直到周五上午
 *		周五上午我分析应该是在插入节点6的时候，这时候根节点是[2,5,8]，是一个满节点，这时候根据B树的规则，根节点要进行分裂，
 *		所以这时候我已经大体知道问题是出在根节点进行分裂的时候，没有正确处理根节点中四个子节点，导致最后Segmentation fault: 11的错误
 *		但是我调试多次仍然是无法找到问题症结，因为这代码实在晦涩难懂，我心想如果继续在网上例程的沼泽里调试下去，还不知道要调试到什么时候，干脆不如重写算了！
 *
 *		于是周五整个下午，我都在实现B树，在这个过程中，我是根据节点插入的步骤一个一个来处理的，你会发现，如果你一步一步来实现B树，其实逻辑就渐渐地捋清楚了
 *		1.首先在插入根节点为空的时候，直接赋值，这个时候我们就有了根节点
 *		2.然后向根节点继续插入节点的时候，我们有了insert_nonfull函数来处理插入未满节点的函数
 *		3.根节点在插入[3,5,8]的时候，先满了，这时候在插入节点1的时候，根节点第一次需要使用到分裂，这个时候我们编写分裂函数
 *		4.分裂函数的主要过程是建立一个临时节点，然后将被分裂节点的右半部分成员key转移给这个临时节点（这个时候我还没有转移右半部分的孩子成员children，因为现在他们还是NULL，我还没察觉到他们）
 *		5.然后在插入9的时候，需要分裂右子节点，这个时候涉及到了提取右子节点的中间元素到父节点当中的问题，因为这个时候父节点并不是空的，需要对提升到父节点的中间元素进行排序
 *		6.然后在插入4的时候，跟上面第五条也是一样的道理
 *		7.最后重头戏在于插入节点6的时候，因为这个时候根节点是[2,5,8]，判断到根节点已满之后，就需要先对根节点进行分裂操作，这个时候，设计到了分裂节点的孩子节点的处理，这个时候我才注意到了分裂节点也有孩子节点需要处理
 *		8.正确处理好（或者说分配好）被分裂节点的孩子节点之后，我们就可以插入最后一个元素6了。
 *
 *	当处理完最后根节点的分裂，并且插入节点6并调试成功之后，正好18:00整~~~，哇...呵呵，可以下班了~~
 *
 *
 *	****************************************** 2016-06-22 ******************************************
 *  编写B树的删除，猜猜花费了多长时间？一天？两天？一个周？
 *  都不对，实际是，花费了整整两个周！！！
 *  编写B树的删除逻辑，其中涉及到了节点的合并，与添加节点时的分裂相呼应，但是这个合并的逻辑，我整整花费了两个星期的时间......
 *  而且中间非常吃力，对于调试C代码经验也非常不足(中间调试测试代码花费了非常多的时间)......所以又再一次证明了我不是一个聪明的人
 *  但是也想通过这个事情说明，哪怕像我这么笨的人，通过两个星期的时间，也可以实现B树的删除，所以，没有什么算法是不能实现的，只要你肯去做。
 *  这次合并的逻辑，并不完善，其中可能有一些地方可以使用分裂之后再合并的方法可能更好，但是我没有去实现；
 *  按层级打印B树的功能也没有实现好，目前只是通过加入了一个右兄弟指针的方法来实现的，属于投机取巧的方法（而且还有bug）；
 *  另外这里为了实现一个B树，竟然用了七八百行代码，说明肯定有冗余的部分，
 *  所有这些问题以后有时间有机会一定要继续优化~~~
 *  
 *  
 *  
 *	@Description: 	Description
 *	@Datetime: 		2016-04-20 17:33:30
 *	@Author: 		linjunjie
 */

#include "algorithm.h"

//4阶树，也就是2-3-4树，最简单的B树是2-3树，也即3阶树
#define BTREE_M 4 	//四阶树，也就是有四个子孩子，三个元素的B树

//为方便编程，有的地方也称为出度，这里四阶B树的度即为2
//这里关于B树的度和其高度H还有一个关系，也就是B树的高度H的上限为log以T为底(关键字个数N+1)/2的对数
//     		( N + 1 )/2
// H <= log 
//     		T
// 所以据此我们也得出了一个很直观的结论，也就是B树查找节点的渐进复杂度为
// 		   N
// O ( log   )
// 		   T
// 也就是以度T为底，结点个数N的对数
// 可以看出B树是一个非常有效率的数据结构
// 因为通常现实情况下出度T非常大，例如100甚至1000，那么得到的对数则会非常小，通常<=3
// 当然查找到相应节点之后还要在节点关键字上进行二分查找等消耗
#define T BTREE_M/2 	

#define true 1
#define false 0
#define null NULL

typedef struct btree_node{
	int keynum;		//顾名思义，保存的数据的数量，也就是key的数量
	int isleaf;
	int keys[2*T - 1];	//保存的数据域
	struct btree_node * children[2*T];	//保存的孩子节点
	struct btree_node * right_sibling;
	struct btree_node * parent;	//父节点
	int p_pos;	//当前节点在父节点的位置
} bnode;

bnode * bt_root;

void init_btree();
void print_btree(bnode * root);
void hierarchy_traversal(bnode * node);
void hierarchy_traversal_recurse(bnode * root, int level);
void insert_nonfull(bnode * node, int data);
void split_node(bnode * split_node_parent, int index);
void insert(bnode * * root, int key);
void remove_key(bnode * * root, int key);
bnode * find(bnode * root, int key, int * pos);
bnode * find_max(bnode * root, int * pos, int * key);
bnode * find_min(bnode * root, int * pos, int * key);
void merge_sibling(bnode * theone);
void merge_sibling_recursive(bnode * theone);

void init_btree()
{
	bt_root = NULL;
}

//插入到一个非空节点
void insert_nonfull(bnode * node, int data)
{
	//如果是叶子节点，则直接插入
	if(node -> isleaf == true)
	{
		//得到节点的key的数组的最大索引值
		int i = node -> keynum - 1;

		/**
		 *	开始比较要插入的data
		 *	如果data小于当前结点的key，则所有的key往后移：i+1 <= i
		 *	最后吧data放到最终确定的位置，也就是把data放到node中最小的key的位置
		 */
		while( i >= 0 && data < node -> keys[i])
		{
			node -> keys[i+1] = node -> keys[i];
			i--;
		}
		node -> keynum++;
		node -> keys[i+1] = data;
	}
	//如果不是叶子节点，则肯定不是插在这个节点，而是需要比较大小，插入到他的子节点中去
	else
	{
		///得到节点的key的数组的最大索引值
		int i = node -> keynum - 1;

		//寻找，通过比较大小，看看被插入节点应该落在哪个位置
		while( i >= 0 && data < node -> keys[i] )
		{
			i--;
		}

		//往前进一步
		i++;

		//如果这个子节点已经满了，那么简单，先分裂此节点
		if(node -> children[i] -> keynum == 2*T - 1)
		{
			//分裂这个已满的i节点
			split_node(node, i);

			//注意，分裂之后的节点，肯定有一个节点被提上来了，并且这个被提上来的节点位置为i，而且这个被提上来的节点可能小于也可能大于之前确定好的i，所以我们这里要比较一下
			//如果被提上来的节点比我们要插入的节点要小的话，那么我们之前确定的位置i就要变更了，具体就是+1，往后挪一位
			if(data > node -> keys[i])
			{
				i++;
			}
		}

		//插入到这个（如果满了就是被分裂）（如果没满就是未满节点）中去
		insert_nonfull(node -> children[i], data);
	}
}

//这里分裂的其实是待分裂节点的父节点
void split_node(bnode * split_node_parent, int index)
{
	//开辟一个新节点保存被实际分裂节点右边的节点
	bnode * right_node = (bnode *)malloc(sizeof(bnode));
	right_node -> isleaf = split_node_parent -> children[index] -> isleaf;
	right_node -> keynum = 0;
	for(int i=0; i < 2*T - 1; i++)
	{
		right_node -> keys[i] = 0;
	}
	for(int i=0; i < 2*T; i++)
	{
		right_node -> children[i] = NULL;
	}

	//把当前节点右半边的节点赋值给right_node
	for(int i=T,j=0; i < 2*T - 1; i++, j++)
	{
		//把被分裂节点右边的值全部赋值给right_node
		right_node -> keys[j] = split_node_parent -> children[index] -> keys[i];
		right_node -> keynum ++;

		//赋值之后右边值清空
		split_node_parent -> children[index] -> keys[i] = 0;
		split_node_parent -> children[index] -> keynum --;
	}

	//把右半边的孩子也赋值给right_node
	for(int i=T,j=0; i < 2*T; i++,j++)
	{
		right_node -> children[j] = split_node_parent -> children[index] -> children[i];
		split_node_parent -> children[index] -> children[i] = NULL;
	}

	//把右半边的元素数赋给父节点的右孩子
	int m = split_node_parent -> keynum;

	//所有孩子也右移，是为了容纳分裂之后产生的新的右孩子
	while(m >= index + 1)
	{
		split_node_parent -> children[m + 1] = split_node_parent -> children[m];
		m--;
	}
	split_node_parent -> children[index+1] = right_node;


	//把分裂节点的第T-1个key（也就是中间key）提到父节点上来
	//先拿到中间节点
	int mid_key = split_node_parent -> children[index] -> keys[T - 1];

	//拿到父节点现在的数组最大索引值
	int i = split_node_parent -> keynum - 1;

	//如果比提上来的节点小，则全部往后移，目的是给移上来的节点腾出合适的地方
	while(i >= index){
		split_node_parent -> keys[ i + 1 ] = split_node_parent -> keys[ i ];
		i--;
	}
	i++;	//找到腾出来的位置，要+1的

	split_node_parent -> keys[ i ] = mid_key;
	split_node_parent -> children[index] -> keynum --;
	split_node_parent -> keynum ++;

	//更新一些指针和节点信息
	for (int i = 0; i < split_node_parent -> keynum; i++)
	{
		//最后这个是建立起节点之间右兄弟节点的联系（用来日后层级遍历B树）
		split_node_parent -> children[i] -> right_sibling = split_node_parent -> children[i+1];
	}

	//注意这里处理子节点的数目应该是keynum
	for (int i = 0; i <= split_node_parent -> keynum; i++)
	{
		//各个子节点指向父节点
		split_node_parent -> children[i] -> parent = split_node_parent;

		//子节点在父节点的位置
		split_node_parent -> children[i] -> p_pos = i;
	}

}

void insert(bnode * * root, int key)
{
	//根节点为空，则直接放入根节点
	if(*root == NULL)
	{
		bnode * new_root = (bnode *)malloc(sizeof(bnode));
		new_root -> parent = NULL;
		new_root -> isleaf = true;
		new_root -> keynum = 1;
		for(int i=0; i < 2*T - 1; i++)
		{
			new_root -> keys[i] = 0;
		}
		for(int i=0; i < 2*T; i++)
		{
			new_root -> children[i] = NULL;
		}
		new_root -> keys[0] = key;
		*root = new_root;
		return;
	}

	//根节点满了
	if((* root) -> keynum == 2*T - 1)
	{
		//新开辟一个节点，孩子节点指向根节点
		bnode * tmp = (bnode *)malloc(sizeof(bnode));
		tmp -> keynum = 0;
		for(int i=0; i < 2*T - 1; i++)
		{
			tmp -> keys[i] = 0;
		}
		for(int i=0; i < 2*T; i++)
		{
			tmp -> children[i] = NULL;
		}
		tmp -> children[0] = *root;

		//分裂这个子节点所指向当前根节点的新节点
		split_node(tmp, 0);

		//将分裂后的节点设置为根节点
		*root = tmp;

		insert_nonfull(*root, key);
	}
	else
	{
		insert_nonfull(*root, key);
	}
}

/**
 * 合并兄弟节点
 * 
 * 当对接点进行了变更之后，则调用此方法判断节点是否可以进行合并
 */
void merge_sibling(bnode * theone)
{
	if( theone -> keynum <= T - 1)
	{
		bnode * parent = theone -> parent;

		int sibling_pos = abs(theone -> p_pos - 1);	//需要待合并的节点位置

		if(sibling_pos > theone -> p_pos)	//如果兄弟节点在当前节点右边，则直接使用当前节点合并
		{
			merge_sibling_recursive(theone);
		}
		else	//如果兄弟节点在当前节点左边，则使用兄弟节点进行合并
		{
			merge_sibling_recursive(parent -> children[ sibling_pos ]);
		}
	}
}

/**
 * 递归合并节点
 *
 * 这里进来的元素，在merge_sibling方法已经被矫正过一次，所以进来的节点都是左子节点，
 * 哪怕本来这个节点是右边的节点需要合并，现在也变成了左边的节点，
 * 就像下面这样
 *
 *                父节点
 * 当前节点   <-------------  右子节点
 */
void merge_sibling_recursive(bnode * theone)
{
	if( theone -> keynum <= T - 1)
	{
		bnode * parent = theone -> parent;

		int sibling_pos = theone -> p_pos + 1;

		if(parent -> children[ sibling_pos ] -> keynum <= T - 1)	//如果待合并的节点的兄弟节点确实满足合并的条件
		{
			//父节点元素下移
			theone -> keys[ theone -> keynum ] = parent -> keys[ theone -> p_pos ];
			theone -> keynum ++;

			//把兄弟节点的孩子也赋值给theone
			int j = 0;
			while( j < parent -> children[ sibling_pos ] -> keynum )
			{
				theone -> keys[ theone -> keynum + j ] = parent -> children[ sibling_pos ] -> keys[ j ];
				theone -> children[ theone -> keynum + j ] = parent -> children[ sibling_pos ] -> children[ j ];
				
				if(theone -> children[ theone -> keynum + j ] != NULL)
				{	
					theone -> children[ theone -> keynum + j ] -> parent = theone;
					theone -> children[ theone -> keynum + j ] -> p_pos = theone -> keynum + j;
				}
				j++;
			}
			//这里处理最后一个节点孩子
			theone -> children[ theone -> keynum + j ] = parent -> children[ sibling_pos ] -> children[ j ];
			if(theone -> children[ theone -> keynum + j ] != NULL)
			{
				theone -> children[ theone -> keynum + j ] -> parent = theone;
				theone -> children[ theone -> keynum + j ] -> p_pos = theone -> keynum + j;
			}
			theone -> keynum += j;

			//父节点下移之后，也要调整父节点相应关键字和子节点的位置，也就是向左移动
			int k = theone -> p_pos;
			while( k < parent -> keynum )
			{
				parent -> keys[ k ] = parent -> keys[ k + 1 ];
				parent -> children[ k + 1 ] = parent -> children[ k + 2 ];
				if(parent -> children[ k + 1 ] != NULL)
				{
					parent -> children[ k + 1 ] -> p_pos = k + 1;
				}
				k++;
			}
			parent -> children[ k + 1 ] = parent -> children[ k + 2 ];
			if(parent -> children[ k + 1 ] != NULL)
			{
				parent -> children[ k + 1 ] -> p_pos = k + 1;
			}

			//调整完父节点之后，将父节点的关键字数目减1
			parent -> keynum --;

			if(parent -> keynum > 0)
			{
				//建立右兄弟的关系
				parent -> children[ theone -> p_pos ] -> right_sibling = parent -> children[ theone -> p_pos + 1 ];
			}

			//如果不是父节点（父节点需要特殊处理）
			if(parent != bt_root)
			{
				int parent_sibling_pos = abs(parent -> p_pos - 1);	//需要待合并的节点位置
				if(parent_sibling_pos > parent -> p_pos)	//如果兄弟节点在当前节点右边，则直接使用当前节点合并
				{
					merge_sibling_recursive(parent);
				}
				else	//如果兄弟节点在当前节点左边，则使用兄弟节点进行合并
				{
					merge_sibling_recursive(parent -> parent -> children[ parent_sibling_pos ]);
				}
			}
			else
			{
				//如果父节点元素已经为空，则销毁父节点，并重新建立父节点
				if(parent -> keynum == 0)
				{
					free(parent);
					bt_root = theone;
					bt_root -> right_sibling = null;
				}
			}
		}
		else
		{
			//如果兄弟节点为空的话
			if( theone -> keynum == 0 )
			{

				//父节点下移到左子节点
				theone -> keys[0] = parent -> keys[ theone -> p_pos ];
				theone -> keynum = 1;

				//右子节点最左边关键字上移到父节点
				parent -> keys[ theone -> p_pos ] = parent -> children[ sibling_pos ] -> keys[ 0 ];
				int j = 0;
				while( j < parent -> children[ sibling_pos ] -> keynum - 1 )
				{
					parent -> children[ sibling_pos ] -> keys[ j ] = parent -> children[ sibling_pos ] -> keys[ j + 1 ];
					j ++;
				}

				//右子节点关键字数目减一
				parent -> children[ sibling_pos ] -> keynum --;

				//递归继续查看节点是否满足合并条件
				merge_sibling_recursive(theone);
			}
		}
	}
	//如果当前节点满足B树的特性，那么则判断其兄弟节点是否不满足B树特性，如果不满足则可以进行合并
	else
	{
		bnode * parent = theone -> parent;
		int sibling_pos = theone -> p_pos + 1;

		//如果兄弟节点为空的话
		if( parent -> children[ sibling_pos ] -> keynum <= T - 1 )
		{
			//父节点下移到右子节点的最左边
			//首先所有孩子往右移
			int j = parent -> children[ sibling_pos ] -> keynum;
			while(j > 0)
			{
				parent -> children[ sibling_pos ] -> keys[ j ] = parent -> children[ sibling_pos ] -> keys[ j - 1 ];
				j --;
			}
			parent -> children[ sibling_pos ] -> keys[ 0 ] = parent -> keys[ theone -> p_pos ];
			parent -> children[ sibling_pos ] -> keynum += 1;

			//左子节点最右边关键字上移到父节点
			parent -> keys[ theone -> p_pos ] = theone -> keys[ theone -> keynum - 1 ];

			//左子节点关键字数目减一
			theone -> keynum --;

			//继续递归判断
			merge_sibling_recursive(theone);

		}
	}
}

/**
 *
 * 删除B树种的关键字
 *
 * 
 * B树的特性：
 * 
 * 除根节点之外的节点必须满足：
 * ceil(m/2) - 1 <= n <= m-1
 * n表示关键字个数
 * 
 * 对于四阶树来说，也就是关键字满足：
 * 1 <= n <= 3
 *
 */
void remove_key(bnode * * root, int key)
{
	if(root == null)
	{
		return;
	}

	bnode * node = * root;
	int pos = -1;

	bnode * theone = find(node, key, &pos);

	if(theone == NULL)
	{
		return;
	}
	else
	{
		//如果节点是叶节点，则直接删除元素
		if(theone -> isleaf == true)
		{
			//则将此元素后面的元素向前移来删除此元素
			int i = pos;
			while(i < theone -> keynum)
			{
				theone -> keys[i] = theone -> keys[i + 1];
				i++;
			}

			//节点元素数减1
			theone -> keynum --;

			//判断当前节点在删除关键字后还是否满足B树的节点关键字特性
			merge_sibling(theone);
		}
		/**
		 * 如果被删除的不是叶子节点，则有三种情况：
		 * 1.左边能借
		 * 2.右边能借
		 * 3.左右都不能借
		 */
		else
		{
			//如果节点左孩子满足富余节点的特性
			if( theone -> children[pos] -> keynum > T - 1 )
			{
				// printf("%s\n", "left");
				int pos_left_max, key_left_max= -1;
				bnode * findnode = find_max( theone -> children[pos], &pos_left_max, &key_left_max);
				if(findnode != null)
				{
					remove_key(&findnode, key_left_max);
				}
				theone -> keys[pos] = key_left_max;
			}
			//如果节点右孩子满足富余节点的特性
			else if(theone -> children[pos+1] -> keynum > T - 1 )
			{
				// printf("%s\n", "right");
				int pos_right_min,key_right_min= -1;
				bnode * findnode = find_min( theone -> children[pos+1], &pos_right_min, &key_right_min);
				if(findnode != null)
				{
					remove_key(&findnode, key_right_min);
				}
				theone -> keys[pos] = key_right_min;
			}
			//如果左右子节点关键字都不够用，那么需要先把左子节点，当前节点，右子节点进行合并
			else if(
				theone -> children[pos] -> keynum ==  T - 1
				&& 
				theone -> children[pos+1] -> keynum == T - 1)
			{
				// printf("%s\n", "mid");
				//父节点下移到左子节点
				theone -> children[ pos ] -> keys[ theone -> children[ pos ] -> keynum ] = theone -> keys[ pos ];
				theone -> children[ pos ] -> keynum ++;

				//右子节点关键字并入左子节点
				int i = 0;
				while( i < theone -> children[ pos + 1 ] -> keynum)
				{
					theone -> children[ pos ] -> keys[ theone -> children[ pos ] -> keynum + i ] = theone -> children[ pos + 1] -> keys[ i ];
					theone -> children[ pos ] -> children[ theone -> children[ pos ] -> keynum + i ] = theone -> children[ pos + 1] -> children[ i ];
					theone -> children[ pos ] -> children[ theone -> children[ pos ] -> keynum + i ] -> p_pos = theone -> children[ pos ] -> keynum + i;
					theone -> children[ pos ] -> keynum ++;
					i++;
				}
				//不要忘了处理最后一个孩子
				theone -> children[ pos ] -> children[ theone -> children[ pos ] -> keynum + i ] = theone -> children[ pos + 1] -> children[ i ];
				theone -> children[ pos ] -> children[ theone -> children[ pos ] -> keynum + i ] -> p_pos = theone -> children[ pos ] -> keynum + i;
				theone -> children[ pos ] -> keynum ++;

				//父节点全部孩子左移
				int j = pos;
				while(j < theone -> keynum)
				{
					theone -> keys[j] = theone -> keys[j + 1];
					theone -> children[j + 1] = theone -> children[j + 2];
					j++;
				}
				theone -> keynum --;

				//不要忘记更新此节点指向的右兄弟节点（为了层级遍历）
				theone -> children[ pos ] -> right_sibling = theone -> children[ pos + 1 ];

				//在子节点中继续删除元素
				remove_key( & (theone -> children[pos]), key );
			}

		}
	}
}

/**
 * 查找的基本思路是这样的:
 *
 * 1.元素在当前节点中寻找
 * 2.如果元素大于节点中当前元素，则一直往后找
 * 3.否则查看元素和当前元素是否相等，若相等则说明已经找到
 * 4.否则查看当前元素中间是否存在子节点
 * 5.在子节点中重复以上1~4步骤
 */
bnode * find(bnode * root, int key, int * pos)
{
	if(root == NULL)
	{
		return false;
	}

	bnode * tmp = root;
	int i = 0;

	//如果大于当前元素，则一直往后找
	while ( i < tmp -> keynum && key > tmp -> keys[i] )
	{
		i++;
	}

	//若相等，则说明已经找到
	if( i < tmp -> keynum && key == tmp -> keys[i])
	{
		* pos = i;
		return tmp;
	}

	//如果当前元素是叶子节点，说明没找到
	if( tmp -> isleaf )
	{
		return NULL;
	}

	//否则继续向深处寻找
	return find( tmp -> children[i], key, pos);
}

//找到此节点所有子节点下的最大元素
bnode * find_max(bnode * root, int * pos, int * key)
{
	if(root == null)
	{
		return false;
	}

	bnode * tmp = root;

	if( tmp -> isleaf )
	{
		* pos = tmp -> keynum - 1;
		* key = tmp -> keys[ tmp -> keynum - 1 ];
		return tmp;
	}
	else
	{
		return find_max( tmp -> children[ tmp -> keynum ], pos , key);
	}

}

//找到此节点所有子节点下的最小元素
bnode * find_min(bnode * root, int * pos, int * key)
{
	if(root == null)
	{
		return false;
	}

	bnode * tmp = root;

	if( tmp -> isleaf )
	{
		* pos = tmp -> keynum - 1;
		* key = tmp -> keys[ 0 ];
		return tmp;
	}
	else
	{
		return find_min( tmp -> children[ 0 ], pos , key);
	}

}

//普通打印（打印包括节点信息）
void print_btree(bnode * root)
{
	if(root == NULL)
	{
		return;
	}

	printf("节点元素:");
	printf("是否叶子节点:%d", root -> isleaf);

	//打印根节点
	printf(";成员:[");
	for(int i = 0; i < root -> keynum; i++)
	{
		printf("%d", root -> keys[i]);
		if(root -> keynum - 1 != i)
		{
			printf(",");
		}
	}
	printf("]");
	printf(";在父节点的位置:%d", root -> p_pos);
	printf(";key数量:%d\n", root -> keynum);

	//打印子节点
	for(int i = 0; i <= root -> keynum; i++)
	{
		print_btree(root -> children[i]);
	}
}

//层级遍历打印B树
void hierarchy_traversal(bnode * node)
{
	if(node -> keynum > 0)
	{
		bnode * right = node;
		while(right != null)
		{
			printf("[");
			for (int i = 0; i < right -> keynum; i++)
			{
				printf("%d", right -> keys[i]);
				if(right -> keynum - 1 != i)
				{
					printf(",");
				}
			}
			printf("],");

			right = right -> right_sibling;
		}
	}
	printf("\n");
}

//递归层级遍历打印B树
void hierarchy_traversal_recurse(bnode * root, int level)
{
	printf("第%d层:", level);
	if(root -> keynum > 0)
	{
		hierarchy_traversal(root);
	}

	if(root -> isleaf == false)
	{
		hierarchy_traversal_recurse(root -> children[0], ++level);
	}
}

int main(int argc, char * argv[]){
	// int data[] = {20,8,5,3,1,10,2,7,9,4,6,18,11,12,15};
	// int data[] = {20,8,5,3,1,10,2,7,9,4,6,18,11};
	int data[] = {20,8,5,3,1,10,2,7,9,4,6,18};
	int len;
	GET_ARRAY_LEN(data, len);

	init_btree();

	//插入节点
	for (int i = 0; i < len; i++)
	{
		insert(&bt_root, data[i]);
	}

	//测试查找函数
	// int pos = -1;
	// bnode * want = find(bt_root, 9, &pos);
	// if(pos != -1)
	// {
	//  printf("%d\n", want -> parent -> keys[0]);
	// 	printf("pos :%d\n", pos);
	// 	printf("p_pos :%d\n", want -> p_pos);
	// }
	// else
	// {
	// 	printf("%s\n","没找到元素");
	// }

	//层级打印B树
	printf("层级打印：\n");
	hierarchy_traversal_recurse(bt_root, 1);

	printf("普通打印：\n");
	print_btree(bt_root);


	//测试删除数据 示例 1
	// remove_key( &bt_root, 7 );
	// remove_key( &bt_root, 5 );
	// remove_key( &bt_root, 4 );
	// remove_key( &bt_root, 1 );
	// remove_key( &bt_root, 2 );
	// remove_key( &bt_root, 3 );
	// remove_key( &bt_root, 18 );


	//测试删除数据 示例 2
	// remove_key( &bt_root, 20 );
	// remove_key( &bt_root, 7 );


	//测试find_max函数
	// int pos,key = -1;
	// bnode * want = find_max(bt_root -> children[2], &pos, &key);
	// if(pos != -1)
	// {
	// 	printf("%d,%d\n",pos,want -> keys[pos]);
	// }
	// else
	// {
	// 	printf("%s\n","没找到元素");
	// }

	printf("层级打印：\n");
	hierarchy_traversal_recurse(bt_root, 1);

	printf("普通打印：\n");
	print_btree(bt_root);

	return 1;
}
