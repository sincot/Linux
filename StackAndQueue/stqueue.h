#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <deque>

using namespace std;

template<class T>
class stack
{
public:
	//入栈
	void push(const T& val)
	{
		// 尾插
		_v.push_back(val);
	}

	// 出栈
	void pop()
	{
		// 尾删
		_v.pop_back();
	}

	// 取栈顶数据
	T& top()
	{
		// 取 vector 中最后一个元素
		return  _v.back();
	}

	// 获取栈中有效数据个数
	size_t size() const
	{
		return _v.size();
	}

	// 判断栈是否为空
	bool empty() const
	{
		return _v.empty();
	}

private:
	vector<T> _v;
};

template<class T>
class queue
{
public:
	// 构造函数
	queue()
	{}

	// 检测队列是否为空
	bool empty() const
	{
		return _lt.empty();
	}

	// 获取队列中有效元素个数
	size_t size() const
	{
		return _lt.size();
	}

	// 返回队头元素的引用
	T& front()
	{
		return _lt.front();
	}

	// 返回队尾元素的引用
	T& back()
	{
		return _lt.back();
	}

	// 入队列(队尾)
	void push(const T& val)
	{
		_lt.push_back(val);
	}

	// 出队列(队头)
	void pop()
	{
		_lt.pop_front();
	}

private:
	list<T> _lt;
};

// 容器适配器
template<class T, class Container = deque<T>>
class stack
{
public:
	// 入栈 -- push 函数
	void push(const T& val)
	{
		_cont.push_back(val);
	}

	// 出栈 --- pop 函数
	void pop()
	{
		_cont.pop_back();
	}

	// 取栈顶数据 --- top 函数
	T& top()
	{
		// 所有的线性容器都实现了front/back接口
		_cont.back();
	}

	// 获取栈中有效数据个数 --- size 函数
	size_t size()
	{
		_cont.size();
	}

	// 判断栈是否为空
	bool empty()
	{
		_cont.empty();
	}

private:
	Container _cont;
};

// 容器适配器
template<class T, class Container = deque<T>>
class queue
{
public:
	// 构造函数 --- queue
	queue() {}

	// 检测队列是否为空 --- empty函数
	bool empty()
	{
		return _cont.empty();
	}

	// 获取队列中有效元素个数 --- size 函数
	size_t size()
	{
		return _cont.size();
	}

	// 返回队头元素的引用 --- front 函数
	T& front()
	{
		return _cont.front();
	}
	 
	// 返回队尾元素的引用 --- back 函数
	T& back()
	{
		return _cont.back();
	}

	// 入队列(队尾) --- push 函数
	void push(const T& val)
	{
		_cont.push_back(val);
	}

	// 出队列(队头) --- pop 函数
	void pop()
	{
		_cont.pop_front();
	}

private:
	Container _cont;
};



template <class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator {
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;  // 模板参数
	typedef Ptr pointer;   // 指针
	typedef Ref reference; // 引用
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T** map_pointer;

	typedef __deque_iterator self;
	__deque_iterator(T* x, map_pointer y)
		: cur(x), first(*y), last(*y + buffer_size()), node(y) {
	}

	reference operator*() const { return *cur; }

	difference_type operator-(const self& x) const {
		return difference_type(buffer_size()) * (node - x.node - 1) +
			(cur - first) + (x.last - x.cur);
	}

	self& operator++() {
		++cur;
		// 当前buff走完了
		if (cur == last)  
		{
			// 迭代器要指向下一个buff的第一个数据
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--() {
		if (cur == first) {
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		--*this;
		return tmp;
	}

	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size()))
			cur += n;
		else {
			difference_type node_offset =
				offset > 0 ? offset / difference_type(buffer_size())
				: -difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}

	self operator+(difference_type n) const {
		self tmp = *this;
		return tmp += n;
	}

	self& operator-=(difference_type n) { return *this += -n; }

	self operator-(difference_type n) const {
		self tmp = *this;
		return tmp -= n;
	}

	reference operator[](difference_type n) const { return *(*this + n); }

	void set_node(map_pointer new_node) {
		// 更新迭代器的指向
		node = new_node;   // node 指向下一个 node 
		first = *new_node; // first 指针指向下一个buff的开始
		// last 指针指向下一个buff的末尾
		last = first + difference_type(buffer_size()); 
	}
};

//reference operator[](size_type n) { return start[difference_type(n)]; }
//reference operator[](difference_type n) const {
//	return *(*this + n);
//self operator+(difference_type n) const {
//		self tmp = *this;
//		return tmp += n;
//}
//
//self& operator+=(difference_type n) {
//	difference_type offset = n + (cur - first);
//	if (offset >= 0 && offset < difference_type(buffer_size()))
//		cur += n;
//	else {
//		difference_type node_offset =
//			offset > 0 ? offset / difference_type(buffer_size())
//			: -difference_type((-offset - 1) / buffer_size()) - 1;
//		set_node(node + node_offset);
//		cur = first + (offset - node_offset * difference_type(buffer_size()));
//	}
//	return *this;
//}
