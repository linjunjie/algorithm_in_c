/**
 * 	B-Tree，B树，1970 年由 R. Bayer 和 E. McCreight 发明
 *
 * 	M阶B树的定义 - balanced tree of order m
 *	1.任意非叶子节点最多只有M个儿子，且M>2
 *	2.根节点的儿子数为[2,M]
 *	3.除根节点以外的非叶子节点的儿子数为[ceil(M/2),M]
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
 *	@Description: 	Description
 *	@Datetime: 		2016-04-20 17:33:30
 *	@Author: 		linjunjie
 */

#include "algorithm.h"

//4阶树，也就是2-3-4树，最简单的B树是2-3树，也即3阶树
#define BTREE_M 4
#define T BTREE_M/2 	//方便编程，有的地方也称为度，什么出度，入度，大家可以自己去搜索是什么意思

#define true 1
#define false 0

typedef struct btree_node{
	int keynum;		//顾名思义，保存的数据的数量，也就是key的数量
	int isleaf;
	struct btree_node * parent;
	int keys[2*T - 1];	//保存的数据域
	struct btree_node * children[2*T];	//保存的孩子节点
} bnode;

bnode * bt_root;

void init_btree();
void print_btree(bnode * root);
void insert_nonfull(bnode * node, int data);
void split_node(bnode * split_node_parent, int index);
void insert(bnode * * root, int data);

void init_btree()
{
	bt_root = NULL;
}

void print_btree(bnode * root)
{
	if(root == NULL)
	{
		return;
	}

	printf("%d", root -> isleaf);

	//打印根节点
	printf("[");
	for(int i = 0; i < root -> keynum; i++)
	{
		printf("%d", root -> keys[i]);
		if(root -> keynum - 1 != i)
		{
			printf(" ");
		}
	}
	printf("]\n");

	//打印子节点
	for(int i = 0; i <= root -> keynum; i++)
	{
		print_btree(root -> children[i]);
	}
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

	//把右边的节点赋值给right_node
	for(int i=T,j=0; i < 2*T - 1; i++, j++)
	{
		//把被分裂节点右边的值全部赋值给right_node
		right_node -> keys[j] = split_node_parent -> children[index] -> keys[i];
		right_node -> keynum ++;

		//赋值之后右边值清空
		split_node_parent -> children[index] -> keys[i] = 0;
		split_node_parent -> children[index] -> keynum --;
	}

	//把右边的孩子也赋值给right_node
	for(int i=T,j=0; i < 2*T; i++,j++)
	{
		right_node -> children[j] = split_node_parent -> children[index] -> children[i];
		split_node_parent -> children[index] -> children[i] = NULL;
	}

	//把右边的值赋给父节点的右孩子
	int m = split_node_parent -> keynum;	//所有孩子数
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

	//如果比提上来的节点小，则全部往后移，反正就是给移上来的节点腾出合适的地方
	while(i >= index){
		split_node_parent -> keys[ i + 1 ] = split_node_parent -> keys[ i ];
		i--;
	}
	i++;	//找到腾出来的位置，要+1的

	split_node_parent -> keys[ i ] = mid_key;
	split_node_parent -> children[index] -> keynum --;
	split_node_parent -> keynum ++;
}

void insert(bnode * * root, int data)
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
		new_root -> keys[0] = data;
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

		insert_nonfull(*root, data);
	}
	else
	{
		insert_nonfull(*root, data);
	}

}

int main(int argc, char * argv[]){
	int data[] = {20,8,5,3,1,10,2,7,9,4,6,11,12,15};
	// int data[] = {8,5,3,1,10,2,7,9,4};
	int len;
	GET_ARRAY_LEN(data, len);

	init_btree();

	for (int i = 0; i < len; i++)
	{
		insert(&bt_root, data[i]);
	}
	
	print_btree(bt_root);
	
	return 1;
}