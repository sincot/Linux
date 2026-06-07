#pragma once

#include <iostream>
using namespace std;

// 枚举颜色
enum Color
{
	RED,   // 红色
	BLACK  // 黑色
};

// 结点的结构
template<class K, class V>
struct RBTreeNode
{
	// 红黑树控制平衡也需要parent指针
	pair<K, V> _kv;
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
	Color _col;

	// 构造函数
	RBTreeNode(const pair<K, V>& kv)
		:_kv(kv)
		, _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
	{}
};

// 树的结构
template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;

public:
	// 插入
	bool Insert(const pair<K, V>& kv)
	{
		// 若树为空，则直接新增结点，赋值给root指针
		if (_root == nullptr)
		{
			_root = new Node(kv);
			// 根结点的颜色是黑色的
			_root->_col = BLACK;
			return true;
		}
		// 若树不为空
		// 定义一个指针，指向 cur 的父节点
		Node* parent = nullptr; // 初始值为空
		// 定义一个指针找空位置，插入新节点
		Node* cur = _root;

		while (cur) // 找空位置，跳出循环说明找到空位置了
		{
			//插入值比当前结点大往右走
			if (kv.first > cur->_kv.first)
			{
				// 更新 parent 的指向
				parent = cur;
				cur = cur->_right;
			}
			//插入值比当前结点小往左⾛
			else if (kv.first < cur->_kv.first)
			{
				// 更新 parent 的指向
				parent = cur;
				cur = cur->_left;
			}
			// 插入值等于当前结点的值，返回false
			else
			{
				return false;
			}
		}

		// 跳出循环，说明cur当前为空，也表明找到了空位置
		cur = new Node(kv);
		// 链接 —— 需要判断链接在 parent 的左边还是右边
		if (kv.first > parent->_kv.first)
		{
			// 插入的值大于 parent 的值，链接在右边
			parent->_right = cur;
		}
		else if (kv.first < parent->_kv.first)
		{
			// 插入的值小于 parent 的值，链接在左边
			parent->_left = cur;
		}
		else
		{
			// 插入的值等于 parent 的值，返回 false
			return false;
		}

		// 更新cur的parent指针的指向
		cur->_parent = parent;

		// 父节点存在,且父节点的颜色为RED
		// 如果父节点不存在，新插入的结点就是根结点,也就不需要调整了
		// 如果父节点存在且父节点的颜色为BLACK,也不需要调整
		while (parent && parent->_col == RED)
		{
			// cur 结点就是新增结点c,parent就是cur的父节点
			// 找g结点,p结点的parent指针指向g结点
			Node* grandfather = parent->_parent;
			// 找u结点
			// 如果p结点是g结点的左子树,那么u结点就是g结点的右子树
			if (grandfather->_left == parent)
			{
				Node* uncle = grandfather->_right;

				// u存在,且u结点的颜色为RED
				if (uncle && uncle->_col == RED)
				{
					// p结点和u结点变成黑色
					parent->_col = uncle->_col = BLACK;
					// g 结点变成红色
					grandfather->_col = RED;

					// 继续向上调整
					cur = grandfather;
					parent = cur->_parent;
				}
				// u不存在,或者u存在且u结点的颜色为BLACK
				// 需要旋转+变色
				else
				{
					//       g
					//    p		u
					// c
					// 左边高,右旋
					// 此时c为p的左节点
					if (cur == parent->_left)
					{
						// 右旋,以g结点为旋转点
						RotateR(grandfather);
						// p变成黑色,g变成红色
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					//		g
					//	 p     u
					//     c
					// 左右双旋——右边高,左单旋;左边高,右单旋
					// 此时c为p的右节点
					else
					{
						// 左单旋，以p结点为旋转点
						RotateL(parent);
						// 右单旋，以g结点为旋转点
						RotateR(grandfather);

						// 此时c成为新的根结点,根结点的颜色变成黑色
						cur->_col = BLACK;
						// g 变成红色
						grandfather->_col = RED;
					}

					// 如果旋转的是子树
					// 此时p为根结点,根结点为黑色,不需要再向上调整
					// 可以跳出循环
					break;
				}
			}
			// 如果p结点是g结点的右子树,那么u结点就是g结点的左子树
			else
			{
				Node* uncle = grandfather->_left;

				// u存在,且u结点的颜色为RED
				if (uncle && uncle->_col == RED)
				{
					// p结点和u结点变成黑色
					parent->_col = uncle->_col = BLACK;
					// g 结点变成红色
					grandfather->_col = RED;

					// 继续向上调整
					cur = grandfather;
					parent = cur->_parent;
				}
				// u不存在,或者u存在且u结点的颜色为BLACK
				// 需要旋转+变色
				else
				{
					//       g
					//    u		p
					//		       c
					// 右边高,左旋
					// 此时c为p的右节点
					if (cur == parent->_right)
					{
						// 左旋,以g结点为旋转点
						RotateL(grandfather);
						// p变成黑色,g变成红色
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					//		g
					//	 u     p
					//       c
					// 右左双旋——左边高，右单旋;右边高，左单旋
					// 此时c为p的右节点
					else
					{
						// 右单旋，以p结点为旋转点
						RotateR(parent);
						// 左单旋，以g结点为旋转点
						RotateL(grandfather);

						// 此时c成为新的根结点,根结点的颜色变成黑色
						cur->_col = BLACK;
						// g 变成红色
						grandfather->_col = RED;
					}

					// 如果旋转的是子树
					// 此时p为根结点,根结点为黑色,不需要再向上调整
					// 可以跳出循环
					break;
				}
			}
		}

		// 无论什么情况，循环结束/跳出循环后，根结点的颜色都得是黑色
		_root->_col = BLACK;

		return true;
	}

	// 中序遍历
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	//检查红黑树是否平衡---检查不满足的情况
	bool IsBalance()
	{
		// 如果树为空,一定平衡
		if (_root == nullptr)
		{
			return true;
		}
		// 如果树的根结点为红色,则说明树不平衡
		if (_root->_col == RED)
		{
			return false;
		}

		// 以最左路径的黑色结点数量为参考值
		Node* leftmost = _root;
		int standard = 0;
		while (leftmost)
		{
			if (leftmost->_col == BLACK)
			{
				++standard;
			}

			leftmost = leftmost->_left;
		}
		return Check(_root, 0, standard);
	}
private:
	// 右单旋
	void RotateR(Node* parent)
	{
		//        8
		//   3         h    
		// h   h+1    
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		// subLR 变成parent的左子树
		parent->_left = subLR;
		// 更新结点的parent指针的指向
		// subLR 的父节点变成了 parent
		if (subLR != nullptr)
		{
			subLR->_parent = parent;
		}

		// 记录根结点的父节点
		Node* parentParent = parent->_parent;
		// parent 变成subL的右子树
		subL->_right = parent;
		// 在更新根结点之前，更新结点的parent指针的指向
		// parent 的父节点变成了 subL
		parent->_parent = subL;

		// 在更新结点之前,如果 parent 为 _root,即parentParent等于空
		// 更新根结点
		if (parentParent == nullptr)
		{
			_root = subL;
			// 根结点的parent指针置为空
			subL->_parent = nullptr;
		}
		// 更新结点之前,如果 parent 不为 _root,即parentParent不为空
		// 表明我们旋转的树是局部子树
		// 这时又需要分情况讨论，这个子树是左子树还是右子树
		else
		{
			// 左子树
			if (parentParent->_left == parent)
			{
				// parentParent的左子树的根结点更新了
				// 更新parentParent的左指针指向
				parentParent->_left = subL;
			}
			// 右子树
			else
			{
				// parentParent的右子树的根结点更新了
				// 更新parentParent的右指针指向
				parentParent->_right = subL;
			}

			// 更新子树根结点的parent指针的指向
			subL->_parent = parentParent;
		}
	}

	// 左单旋
	void RotateL(Node* parent)
	{
		//      8
		//  h        10
		//        h      h+1
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		// subRL 变成parent的右子树
		parent->_right = subRL;
		// 更新subRL的parent指针的指向
		if (subRL != nullptr)
		{
			subRL->_parent = parent;
		}
		// 在更新根结点之前，记录根结点的父节点
		Node* parentParent = parent->_parent;
		// parent变成subR的左子树
		subR->_left = parent;
		// 更新parent的parent指针的指向
		parent->_parent = subR;

		// 在更新结点之前,如果 parent 为 _root,即parentParent为空
		// 更新根结点
		if (parentParent == nullptr)
		{
			_root = subR;
			// 根结点的parent指针置为空
			subR->_parent = nullptr;
		}
		// 在更新结点之前,如果parent 不为 _root,即parentParent不为空
		// 表明我们旋转的树是局部子树
		// 这时又需要分情况讨论，这个子树是左子树还是右子树
		else
		{
			// 左子树
			if (parentParent->_left == parent)
			{
				// parentParent的左子树的根结点更新了
				// 更新parentParent的左指针指向
				parentParent->_left = subRL;
			}
			// 右子树
			else
			{
				// parentParent的右子树的根结点更新了
				// 更新parentParent的右指针指向
				parentParent->_right = subR;
			}

			// 更新子树根结点的parent指针的指向
			subR->_parent = parentParent;
		}
	}

	// 遍历二叉搜索树 —— 中序遍历
	void _InOrder(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		// 遍历左子树
		_InOrder(root->_left);
		// 打印值
		cout << root->_kv.first << ":" << root->_kv.second;
		cout << endl;
		// 遍历右子树
		_InOrder(root->_right);
	}

	// 检查 --- blackNum 用于统计路径上黑色结点的个数
	bool Check(Node* cur, int blackNum, const int standard)
	{
		// 走到空结点,说明遍历路径的黑色结点的个数已经计算出来了
		// 与参考值相比较,相等说明红黑树平衡,不相等说明红黑树不平衡
		if (cur == nullptr)
		{
			if (blackNum != standard)
			{
				// 打印报错信息
				cout << "任意路径的黑色结点数量不相等" << endl;
				return false;
			}

			return true;
		}
		// 如果结点的颜色为红色,检查其父节点是否存在/存在是否为红色
		// 若为红色,违反了规则3,红黑树不平衡
		if (cur->_col == RED && cur->_parent && cur->_parent->_col == RED)
		{
			// 打印报错信息
			cout << cur->_kv.first << "->" << "连续的红色结点" << endl;
			return false;
		}

		if (cur->_col == BLACK)
		{
			// 计算当前遍历结点的黑节点个数
			++blackNum;
		}

		// 再检查当前结点的左子树和右子树
		return Check(cur->_left, blackNum, standard) 
			&& Check(cur->_right, blackNum, standard);
	}

private:
	Node* _root = nullptr;
};
