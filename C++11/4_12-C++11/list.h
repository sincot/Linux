#pragma once

#include <iostream>
#include <algorithm>

using namespace std;

namespace XAY
{
	template<class T>
	struct list_node
	{
		T s_data;
		list_node<T>* s_prev;
		list_node<T>* s_next;

		list_node(const T& val)
			: s_data(val)
			, s_prev(nullptr)
			, s_next(nullptr)
		{
		}

		// template <class X>
		list_node(T&& val = T())
			: s_data(move(val))
			, s_prev(nullptr)
			, s_next(nullptr)
		{
		}
	};

	template <class T, class Ref, class Ptr>
	struct _list_iterator
	{
		typedef list_node<T> Node;
		typedef _list_iterator<T, Ref, Ptr> Self;
		// 创建一个指针
		Node* s_node;

		// 初始化指针
		_list_iterator(Node* node)
			: s_node(node)
		{
		}

		// 重载 * 运算符 --- operator*
		Ref& operator*()
		{
			return s_node->s_data;
		}

		// 重载 -> 运算符
		Ptr operator->()
		{
			return &(s_node->s_data);
		}

		// 重载前置++ 运算符 --- 不改变对象的内容
		Self& operator++()
		{
			s_node = s_node->s_next;

			return *this;
		}

		// 重载前置-- 运算符 --- 不改变对象的内容
		Self& operator--()
		{
			s_node = s_node->s_prev;

			return *this;
		}

		// 重载后置++ 运算符
		Self& operator++(int)
		{
			Self tmp(*this);
			s_node = s_node->s_next;

			return tmp;
		}

		// 重载后置-- 运算符
		Self& operator--(int)
		{
			Self tmp(*this);
			s_node = s_node->s_prev;

			return tmp;
		}

		// 重载 == 运算符
		bool operator==(const Self& it) const
		{
			return s_node == it.s_node;
		}

		// 重载 != 运算符
		bool operator!=(const Self& it) const
		{
			return s_node != it.s_node;
		}
	};

	template<class T>
	class list
	{
		typedef list_node<T> Node;

	public:
		typedef _list_iterator<T, T&, T*> iterator;
		typedef _list_iterator<T, const T&, const T*> const_iterator;

		iterator begin()
		{
			return iterator(m_head->s_next);
		}

		iterator end()
		{
			return iterator(m_head);
		}

		const_iterator begin() const
		{
			return iterator(m_head->s_next);
		}

		const_iterator end() const
		{
			return iterator(m_head);
		}

	public:
		list()
		{
			m_head = new Node;
			m_head->s_prev = m_head;
			m_head->s_next = m_head;
		}

		// 拷贝构造函数
		list(const list<T>& lt)
		{
			//创建新节点
			m_head = new Node;
			m_head->s_prev = m_head;
			m_head->s_next = m_head;

			// 拷贝数据
			for (const auto& e : lt)
			{
				push_back(e);
			}
		}

		// 花括号初始化
		list(initializer_list<T> il)
		{
			//创建新节点
			m_head = new Node;
			m_head->s_prev = m_head;
			m_head->s_next = m_head;

			// 拷贝数据
			for (const auto& e : il)
			{
				push_back(e);
			}
		}

		// 赋值重载函数 --- 第一种写法
		list<T>& operator=(const list<T>& lt)
		{
			// 避免自己赋值自己
			if (*this != lt)
			{
				// 清空 this 对象中的数据
				clear();
				// 拷贝数据
				for (const auto& e : lt)
				{
					push_back(e);
				}
			}

			return *this;
		}

		// swap 函数
		void swap(list<T>& lt)
		{
			std::swap(m_head, lt.m_head);
		}

		// 赋值重载函数 --- 第二种写法
		list<T>& operator=(list<T>& lt)
		{
			swap(lt);
		}

		// clear 函数
		void clear()
		{
			// 删除所有结点
			auto it = begin();
			while (it != end())
			{
				// 迭代器失效 --- 更新迭代器
				it = erase(it);
			}
		}

		// 析构函数
		~list()
		{
			clear();
			// 删除哨兵结点
			delete m_head;
			m_head = nullptr;
		}

		//// insert 函数（左值引用）
		//iterator insert(iterator pos, const T& val)
		//{
		//	// 创建新节点
		//	Node* newnode = new Node(val);
		//	Node* cur = pos.s_node;
		//	Node* prev = cur->s_prev;

		//	// prev newnode cur
		//	prev->s_next = newnode;
		//	newnode->s_prev = prev;
		//	newnode->s_next = cur;
		//	cur->s_prev = newnode;

		//	// 返回指向第一个插入元素的迭代器 --- nenwode
		//	return newnode;
		//	// return iterator(newnode);
		//}

		// insert 函数（右值引用） —— move
		//template <class X>
		//iterator insert(iterator pos, X&& val)
		//{
		//	// 创建新节点
		//	Node* newnode = new Node(move(val));
		//	Node* cur = pos.s_node;
		//	Node* prev = cur->s_prev;

		//	// prev newnode cur
		//	prev->s_next = newnode;
		//	newnode->s_prev = prev;
		//	newnode->s_next = cur;
		//	cur->s_prev = newnode;

		//	// 返回指向第一个插入元素的迭代器 --- nenwode
		//	return newnode;
		//	// return iterator(newnode);
		//}

		// insert 函数（右值引用） —— 完美转发
		template <class X>
		iterator insert(iterator pos, X&& val)
		{
			// 创建新节点
			Node* newnode = new Node(forward<X>(val));
			Node* cur = pos.s_node;
			Node* prev = cur->s_prev;

			// prev newnode cur
			prev->s_next = newnode;
			newnode->s_prev = prev;
			newnode->s_next = cur;
			cur->s_prev = newnode;

			// 返回指向第一个插入元素的迭代器 --- nenwode
			return newnode;
			// return iterator(newnode);
		}

		// erase 函数
		iterator erase(iterator pos)
		{
			Node* cur = pos.s_node;
			Node* prev = cur->s_prev;
			Node* next = cur->s_next;

			prev->s_next = next;
			next->s_prev = prev;
			delete cur;

			// 返回删除元素的下一个元素的迭代器 --- next
			return next;
			// return iterator(next);
		}

		// push_front 函数 -- 头插
		void push_front(const T& val)
		{
			insert(begin(), val);
		}

		//// push_back 函数 --- 尾插（左值引用）
		//void push_back(const T& val)
		//{
		//	insert(end(), val);
		//}

		// push_back 函数 --- 尾插（右值引用） —— move
		/*template <class X>
		void push_back(X&& val)
		{
			insert(end(), move(val));
		}*/

		// push_back 函数 --- 尾插（右值引用） —— 完美转发
		template <class X>
		void push_back(X&& val)
		{
			insert(end(), forward<X>(val));
		}

		// pop_front 函数 --- 头删
		void pop_front()
		{
			earse(begin());
		}

		// pop_back 函数 --- 尾删
		void pop_back()
		{
			// end 函数的返回值为 m_head，它不能动
			// 删除的是 m_head->s_prev，也就是 --end()
			erase(--end());
		}

		// size 函数
		size_t size()
		{
			size_t count = 0;
			for (auto& e : *this)
			{
				++count;
			}

			return count;
		}

	private:
		Node* m_head;
	};
}