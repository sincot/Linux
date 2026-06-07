#pragma once

#include <iostream>
#include <assert.h>
#include <math.h>

using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	// 需要parent指针，方便更新平衡因子
	pair<K, V> _kv;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	int _bf; // 平衡因子(balance factor) bf

	AVLTreeNode(const pair<K, V>& kv)
		: _kv(kv)
		, _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _bf(0)
	{}
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;

public:

	// 插入
	bool Insert(const pair<K, V>& kv)
	{
		// 若树为空，则直接新增结点，赋值给root指针
		if (_root == nullptr)
		{
			_root = new Node(kv);

			return true;
		}
		// 若树不为空
		// 定义一个指针找空位置，插入新节点
		Node* cur = _root;
		// 定义一个指针，指向 cur 的父节点
		Node* parent = nullptr; // 初始值为空

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

		// 新插入结点的parent指针指向它的父节点,即parent结点
		cur->_parent = parent;

		// 更新平衡因子 --- 循环条件,结点parent不为空
		while (parent)
		{
			// 新增结点在parent的左子树，parent平衡因子 --
			if (cur == parent->_left)
			{
				parent->_bf--;
			}
			// 新增结点在parent的右子树，parent平衡因子 ++
			else
			{
				parent->_bf++;
			}

			// 判断是否需要继续向上更新
			// 1. 更新后 parent 的平衡因子等于0,结束更新
			if (parent->_bf == 0)
			{
				break;
			}
			// 2. 更新后 parent 的平衡因子等于1或-1,继续向上更新
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				cur = parent;
				parent = parent->_parent;
			}
			// 3. 更新后 parent 的平衡因子等于2或-2,旋转
			else if (parent->_bf == -2 || parent->_bf == 2)
			{
				// 旋转
				// 右单旋 --- parent->_bf == -2 && cur->_bf == -1
				if (parent->_bf == -2 && cur->_bf == -1)
				{
					// 右单旋
					// 旋转点 parent
					// 插入数据时,快速判断哪处旋转出现了问题
					//cout << "执行右单旋" << endl;
					RotateR(parent);
				}
				// 左单旋 --- parent->_bf == 2 && cur->_bf == 1
				else if (parent->_bf == 2 && cur->_bf == 1)
				{
					// 左单旋
					// 旋转点parent
					// 插入数据时,快速判断哪处旋转出现了问题
					//cout << "执行左单旋" << endl; 
					RotateL(parent);
				}
				// 左右双旋 --- parent->_bf == -2 && cur->_bf == 1
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					// 左右单旋
					// 旋转点parent
					// 插入数据时,快速判断哪处旋转出现了问题
					//cout << "执行左右双旋" << endl;
					RotateLR(parent);
				}
				// 右左双旋 --- parent->_bf == 2 && cur->_bf == -1
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					// 右左单旋
					// 旋转点parent
					// 插入数据时,快速判断哪处旋转出现了问题
					//cout << "执行右左双旋" << endl;
					RotateRL(parent);
				}
				// 不用旋转,跳出循环
				break;
			}
			// 4. 更新后 parent 的平衡因子为其它,说明上述的更新过程中存在错误
			else
			{
				assert(false);
			}
		}
		return true;
	}

	// 中序遍历
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	// 检查
	void IsBalanceTree()
	{
		_IsBalanceTree(_root);
		cout << endl;
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

		// 旋转之后，还需更新subL和parent结点的平衡因子
		// 它们的平衡因子都变成了0，上图展示过
		subL->_bf = parent->_bf = 0;
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

		// 旋转之后，还需更新subR和parent结点的平衡因子
		// 它们的平衡因子都变成了0，上图展示过
		subR->_bf = parent->_bf = 0;
	}

	// 左右双旋
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		// 插入子树的不同会改变最终结点的平衡因子
		// 但是可以通过 subLR的平衡因子进行判断
		// 然而单旋会改变结点的平衡因子
		// 所以需要提前存储结点的 subLR 平衡因子
		int bf = subLR->_bf;
		// 左单旋
		// 旋转点subL
		RotateL(subL);

		// 右单旋
		// 旋转点parent
		RotateR(parent);

		// 如果subLR原来的平衡因子为0，说明满足我所说的特殊情况
		if (bf == 0)
		{
			// 平衡因子均为0
			parent->_bf = 0;
			subL->_bf = 0;
			subLR->_bf = 0;
		}
		// 如果subLR原来的平衡因子为-1，说明插入的是d子树
		else if (bf == -1)
		{
			// parent 的平衡因子最终为1
			parent->_bf = 1;
			// subL 的平衡因子最终为0
			subL->_bf = 0;
			// subLR 的最终平衡因子为0
			subLR->_bf = 0;
		}
		// 如果subLR原来的平衡因子为1，说明插入的是e子树
		else if(bf == 1)
		{
			// parent 的平衡因子最终为0
			parent->_bf = 0;
			// subL 的最终平衡因子为-1
			subL->_bf = -1;
			// subLR 的最终平衡因子为0
			subLR->_bf = 0;
		}
		// 如果subLR原来的平衡因子为其它，说明存在错误
		else
		{
			assert(false);
		}
	}

	// 右左单旋
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		// 插入子树的不同会改变最终结点的平衡因子
		// 但是可以通过 subRL 的平衡因子进行判断
		// 然而单旋会改变结点的平衡因子
		// 所以需要提前存储结点的 subRL 平衡因子
		int bf = subRL->_bf;
		// 右单旋
		// 旋转点subR
		RotateR(subR);

		// 左单旋
		// 旋转点parent
		RotateL(parent);

		// 如果subLR原来的平衡因子为0，说明满足我所说的特殊情况
		if (bf == 0)
		{
			// 平衡因子均为0
			parent->_bf = 0;
			subR->_bf = 0;
			subRL->_bf = 0;
		}
		// 如果subRL原来的平衡因子为-1，说明插入的是d子树
		else if (bf == -1)
		{
			// parent 的平衡因子最终为0
			parent->_bf = 0;
			// subR 的平衡因子最终为1
			subR->_bf = 1;
			// subRL 的最终平衡因子为0
			subRL->_bf = 0;
		}
		// 如果subRL原来的平衡因子为1，说明插入的是e子树
		else if (bf == 1)
		{
			// parent 的平衡因子最终为-1
			parent->_bf = -1;
			// subR 的最终平衡因子为0
			subR->_bf = 0;
			// subRL 的最终平衡因子为0
			subRL->_bf = 0;
		}
		// 如果subRL原来的平衡因子为其它，说明存在错误
		else
		{
			assert(false);
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

	// 获取树的高度
	int _Height(Node* root)
	{
		if (root == nullptr)
		{
			return 0;
		}

		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}

	// 检查AVL树是否平衡
	bool _IsBalanceTree(Node* root)
	{
		// 空树也是AVL树
		if (nullptr == root)
				return true;
		// 计算root结点的平衡因子：即root左右子树的高度差
		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		int diff = rightHeight - leftHeight;

		// 如果计算出的平衡因子与root的平衡因子不相等
		// 或者 root 平衡因子的绝对值超过1，则一定不是AVL树
		if (abs(diff) >= 2)
		{
			cout << root->_kv.first << "高度差异常" << endl;
			return false;
		}
		if (root->_bf != diff)
		{
			cout << root->_kv.first << "平衡因子异常" << endl;
			return false;
		}
		// 如果root的左和右都是AVL树，则该树一定是AVL树
		return _IsBalanceTree(root->_left) && _IsBalanceTree(root->_right);
	}

private:
	Node* _root = nullptr;
};
