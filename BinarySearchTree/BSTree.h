#pragma once
#include <iostream>

using namespace std;

// 结点的结构 —— struct 结点的结构是公有的
template <class K>
struct BSTNode
{
public:
	// 构造函数
	BSTNode(const K& key)
		: _key(key)
		, _left(nullptr)
		, _right(nullptr)
	{}

	K _key;
	BSTNode<K>* _left;
	BSTNode<K>* _right;
};

// 树的结构
template<class K>
class BSTree
{
	// 私有的
	typedef BSTNode<K> Node;

public:
	// 使用编译器默认生成的构造函数

	// 插入
	bool Insert(const K& key)
	{
		// 若树为空，则直接新增结点，赋值给root指针
		if (_root == nullptr)
		{
			_root = new Node(key);
		}
		// 若树不为空
		// 定义一个指针找空位置，插入新节点
		Node* cur = _root;
		// 定义一个指针，指向 cur 的父节点
		Node* parent = nullptr; // 初始值为空

		while (cur) // 找空位置，跳出循环说明找到空位置了
		{
			//插入值比当前结点大往右走
			if (key > cur->_key)
			{
				// 更新 parent 的指向
				parent = cur;
				cur = cur->_right;
			}
			//插入值比当前结点小往左⾛
			else if (key < cur->_key)
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
		cur = new Node(key);
		// 链接 —— 需要判断链接在 parent 的左边还是右边
		if (key > parent->_key)
		{
			// 插入的值大于 parent 的值，链接在右边
			parent->_right = cur;
		}
		else if(key < parent->_key)
		{
			// 插入的值小于 parent 的值，链接在左边
			parent->_left = cur;
		}
		else
		{
			// 插入的值等于 parent 的值，返回 false
			return false;
		}

		return true;
	}

	// 中序遍历
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	// 查找
	bool Find(const K& key)
	{
		Node* cur = _root;

		while (cur) // 循环查找
		{
			// key比当前结点的值大，则往右边查找
			if (key > cur->_key)
			{
				cur = cur->_right;
			}
			// key比当前结点的值小，则往左边查找
			else if (key < cur->_key)
			{
				cur = cur->_left;
			}
			// key等于当前结点的值，说明找到，返回 true
			else
			{
				return true;
			}
		}

		// 跳出循环，cur都走到空了，说明找不到
		return false;
	}

	// 删除
	bool Erase(const K& key)
	{
		// 查找
		Node* cur = _root;
		Node* parent = nullptr; // 初始值设为空
		while (cur) // 循环查找
		{
			// key比当前结点的值大，则往右边查找
			if (key > cur->_key)
			{
				// 更新 parent 的指向
				parent = cur;
				cur = cur->_right;
			}
			// key比当前结点的值小，则往左边查找
			else if (key < cur->_key)
			{
				// 更新 parent 的指向
				parent = cur;
				cur = cur->_left;
			}
			// key等于当前结点的值，说明找到
			else
			{
				// 准备删除
					// cur 的左孩子为空，右孩子不为空
				if (cur->_left == nullptr)
				{
					// 若 cur 有父节点
					if (cur != _root)
					{
						// 把 parent 对应的孩子指针指向 cur 的右孩子
					// 需要判断 cur 是 parent 的左孩子还是右孩子

					// 若 cur 是 parent 的左孩子
						if (cur == parent->_left)
						{
							// 把 parent 的左指针指向 cur 的右孩子
							parent->_left = cur->_right;
						}
						// 若 cur 是 parent 的右孩子
						else
						{
							// 把 parent 的右指针指向 cur 的右孩子
							parent->_right = cur->_right;
						}
					}
					// 若 cur 没有父节点，即 cur 是根结点
					else
					{
						// 更新根结点
						_root = cur->_right;
					}

					delete cur;
				}
				// cur 的左孩子不为空，右孩子为空
				else if (cur->_right == nullptr)
				{
					// 若 cur 有根结点
					if (cur != _root)
					{
						// 把 parent 对应的孩子指针指向 cur 的左孩子
					// 需要判断 cur 是 parent 的左孩子还是右孩子

					// 若 cur 是 parent 的左孩子
						if (cur == parent->_left)
						{
							// 把 parent 的左指针指向 cur 的左孩子
							parent->_left = cur->_left;
						}
						// 若 cur 是 parent 的右孩子
						else
						{
							// 把 parent 的右指针指向 cur 的左孩子
							parent->_right = cur->_left;
						}
					}
					// 若 cur 没有根结点
					else
					{
						// 更新根结点
						_root = cur->_left;
					}

					delete cur;
				}
				// cur 的左孩子不为空，右孩子也不为空
				else
				{
					// 找子树中适合的结点替代 --- 找右子树的最小结点
					Node* minRight = cur->_right;
					//// 指向 minRight 的父亲节点，初始值设为空
					//Node* minRightParent = nullptr; 

					// 指向 minRight 的父亲节点，初始值设为 cur
					Node* minRightParent = cur;

					// 开始找
					// 右子树的最小结点在最左边，所以只需结点的left指针
					while (minRight->_left)
					{
						// 更新 minRightParent 的指向
						minRightParent = minRight;
						// 向下查找
						minRight = minRight->_left;
					}

					// 跳出循环，minRight 的 left 指针为空
					// 表明 minRight 所指向的就是右子树的最小结点
					
					// 让 cur 和 minRight 的 key 值交换
					swap(cur->_key, minRight->_key);

					// 需要处理 minRightParent 的对应指针的指向 minRight 的右孩子
					// 如果 minRight 为 Parent 的左孩子
					if (minRight == minRightParent->_left)
					{
						minRightParent->_left = minRight->_right;
					}
					// 如果 minRight 为 Parent 的右孩子
					else
					{
						minRightParent->_right = minRight->_right;
					}
					
					//minRightParent->_left = minRight->_right;
					// 删除 minRight 指针所指向的结点
					delete minRight;
				}

				return true;
			}
		}

		// 找不到
		return false;
	}

private:
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
		cout << root->_key << " ";
		// 遍历右子树
		_InOrder(root->_right);
	}

private:
	// 与数据结构树的结构的成员类似
	Node* _root = nullptr;
};


namespace AY
{
	// key/value 二叉搜索树
	// 结点的结构 —— struct 结点的结构是公有的
	template <class K, class V>
	struct BSTNode
	{
		// 构造函数
		BSTNode(const K& key, const V& value)
			: _key(key)
			, _value(value)
			, _left(nullptr)
			, _right(nullptr)
		{}

		K _key;
		V _value;
		BSTNode<K, V>* _left;
		BSTNode<K, V>* _right;
	};

	// 树的结构
	template<class K, class V>
	class BSTree
	{
		// 私有的
		typedef BSTNode<K, V> Node;

	public:
		// 使用编译器默认生成的构造函数

		// 插入
		bool Insert(const K& key, const V& value)
		{
			// 若树为空，则直接新增结点，赋值给root指针
			if (_root == nullptr)
			{
				_root = new Node(key, value);
			}
			// 若树不为空
			// 定义一个指针找空位置，插入新节点
			Node* cur = _root;
			// 定义一个指针，指向 cur 的父节点
			Node* parent = nullptr; // 初始值为空

			while (cur) // 找空位置，跳出循环说明找到空位置了
			{
				//插入值比当前结点大往右走
				if (key > cur->_key)
				{
					// 更新 parent 的指向
					parent = cur;
					cur = cur->_right;
				}
				//插入值比当前结点小往左⾛
				else if (key < cur->_key)
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
			cur = new Node(key, value);
			// 链接 —— 需要判断链接在左边还是右边
			if (key > parent->_key)
			{
				// 插入的值大于 parent 的值，链接在右边
				parent->_right = cur;
			}
			else if (key < parent->_key)
			{
				// 插入的值小于 parent 的值，链接在左边
				parent->_left = cur;
			}
			else
			{
				// 插入的值等于 parent 的值，返回 false
				return false;
			}

			return true;
		}

		// 中序遍历
		void InOrder()
		{
			_InOrder(_root);
			cout << endl;
		}

		// 查找
		Node* Find(const K& key)
		{
			Node* cur = _root;

			while (cur) // 循环查找
			{
				// key比当前结点的值大，则往右边查找
				if (key > cur->_key)
				{
					cur = cur->_right;
				}
				// key比当前结点的值小，则往左边查找
				else if (key < cur->_key)
				{
					cur = cur->_left;
				}
				// key等于当前结点的值，说明找到，返回结点的指针
				else
				{
					return cur;
				}
			}

			// 跳出循环，cur都走到空了，说明找不到
			return nullptr;
		}

		// 删除
		bool Erase(const K& key)
		{
			// 查找
			Node* cur = _root;
			Node* parent = nullptr; // 初始值设为空
			while (cur) // 循环查找
			{
				// key比当前结点的值大，则往右边查找
				if (key > cur->_key)
				{
					// 更新 parent 的指向
					parent = cur;
					cur = cur->_right;
				}
				// key比当前结点的值小，则往左边查找
				else if (key < cur->_key)
				{
					// 更新 parent 的指向
					parent = cur;
					cur = cur->_left;
				}
				// key等于当前结点的值，说明找到
				else
				{
					// 准备删除
						// cur 的左孩子为空，右孩子不为空
					if (cur->_left == nullptr)
					{
						// 若 cur 有父节点
						if (cur != _root)
						{
							// 把 parent 对应的孩子指针指向 cur 的右孩子
						// 需要判断 cur 是 parent 的左孩子还是右孩子

						// 若 cur 是 parent 的左孩子
							if (cur == parent->_left)
							{
								// 把 parent 的左指针指向 cur 的右孩子
								parent->_left = cur->_right;
							}
							// 若 cur 是 parent 的右孩子
							else
							{
								// 把 parent 的右指针指向 cur 的右孩子
								parent->_right = cur->_right;
							}
						}
						// 若 cur 没有父节点，即 cur 是根结点
						else
						{
							// 更新根结点
							_root = cur->_right;
						}

						delete cur;
					}
					// cur 的左孩子不为空，右孩子为空
					else if (cur->_right == nullptr)
					{
						// 若 cur 有根结点
						if (cur != _root)
						{
							// 把 parent 对应的孩子指针指向 cur 的左孩子
						// 需要判断 cur 是 parent 的左孩子还是右孩子

						// 若 cur 是 parent 的左孩子
							if (cur == parent->_left)
							{
								// 把 parent 的左指针指向 cur 的左孩子
								parent->_left = cur->_left;
							}
							// 若 cur 是 parent 的右孩子
							else
							{
								// 把 parent 的右指针指向 cur 的左孩子
								parent->_right = cur->_left;
							}
						}
						// 若 cur 没有根结点
						else
						{
							// 更新根结点
							_root = cur->_left;
						}

						delete cur;
					}
					// cur 的左孩子不为空，右孩子也不为空
					else
					{
						// 找子树中适合的结点替代 --- 找右子树的最小结点
						Node* minRight = cur->_right;
						//// 指向 minRight 的父亲节点，初始值设为空
						//Node* minRightParent = nullptr; 

						// 指向 minRight 的父亲节点，初始值设为 cur
						Node* minRightParent = cur;

						// 开始找
						// 右子树的最小结点在最左边，所以只需结点的left指针
						while (minRight->_left)
						{
							// 更新 minRightParent 的指向
							minRightParent = minRight;
							// 向下查找
							minRight = minRight->_left;
						}

						// 跳出循环，minRight 的 left 指针为空
						// 表明 minRight 所指向的就是右子树的最小结点

						// 让 cur 和 minRight 的 key 值交换
						swap(cur->_key, minRight->_key);

						// 需要处理 minRight 的父节点的对应指针的指向 minRight 的右孩子
						// 如果 minRight 为 Parent 的左孩子
						if (minRight == minRightParent->_left)
						{
							minRightParent->_left = minRight->_right;
						}
						// 如果 minRight 为 Parent 的右孩子
						else
						{
							minRightParent->_right = minRight->_right;
						}

						//minRightParent->_left = minRight->_right;
						// 删除 minRight 指针所指向的结点
						delete minRight;
					}

					return true;
				}
			}

			// 找不到
			return false;
		}

	private:
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
			cout << root->_key << ":" << root->_value << "\t";
			// 遍历右子树
			_InOrder(root->_right);
		}

	private:
		// 与数据结构树的结构的成员类似
		Node* _root = nullptr;
	};
}
