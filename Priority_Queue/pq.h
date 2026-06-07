#pragma once
#include<vector>
#include <iostream>
#include <queue>

// 仿函数
template<class T>
class Less
{
public:
	bool operator()(const T& x, const T& y)
	{
		return x < y;
	}
};

template<class T>
class Greater
{
public:
	bool operator()(const T& x, const T& y)
	{
		return x > y;
	}
};

namespace AY
{
	// 优先级队列是一个容器适配器
	// priority_queue的底层是堆
	// 堆的底层是数组，使用vector作为优先队列的底层数据结构
	// 大堆 --- Less  小堆 --- Greater
	template<class T, class Container = std::vector<T>, class Compare = Less<T>>
	class priority_queue
	{
		// 默认是大堆
	private:
		// 交换函数
		template<class T>
		void swap(T& r1, T& r2)
		{
			T tmp = r1;
			r1 = r2;
			r2 = tmp;
		}
		// 向上调整建堆算法
		// 向上指的是插入的数据向上
		void AdjustUp(int child)
		{
			Compare cmp;
			// 循环调整
			// 调整到根结点就停止调整,即 child = 0
			while (child > 0)
			{
				int parent = (child - 1) / 2;
				// 大的往上放，小的往下放
				//if (_cont[child] > _cont[parent])
				// 大堆 --- Less  小堆 --- Greater
				if(cmp(_cont[parent], _cont[child]))
				{
					swap(_cont[parent], _cont[child]);
					// 移动 child 的位置
					child = parent;
				}
				// 小于等于，就结束调整
				else
				{
					break;
				}
			}
		}
		// 向下调整建堆算法
		// 向下指的是根结点向下
		void AdjustDown(int parent)
		{
			int child = parent * 2 + 1; // 左孩子
			Compare cmp;
			// 让根结点与左右孩子中大的那个数进行比较
			// 让大的往上放，小的往下放
			// 孩子下标不能越界
			while (child < _cont.size())
			{
				// 找左右孩子中大的那个孩子
				// child + 1 < _cont.size() 是为了判断右孩子是否存在
				if (child + 1 < _cont.size() && _cont[child + 1] > _cont[child])
				{
					child++;
				}
				
				// 孩子结点大于根结点，交换
				//if (_cont[child] > _cont[parent])
				// 大堆 --- Less  小堆 --- Greater
				if (cmp(_cont[parent], _cont[child]))
				{
					swap(_cont[child], _cont[parent]);
					// child 和 parnet 向下走
					parent = child;
					child = parent * 2 + 1;
				}
				// 孩子结点小于等于根结点，建堆完毕，跳出循环
				else
				{
					break;
				}
			}
		}

	public:
		// 强制编译器默认生成构造函数
		// 对底层的容器初始化
		priority_queue() = default;

		// 迭代器区间初始化
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last)
			: _cont(first, last)
		{
			// 使用向下调整算法建堆,向下调整建堆的算法的效率更高
			for (int i = (_cont.size() - 1 - 1) / 2; i >= 0; i--)
			{
				AdjustDown(i);
			}
		}
		// 入堆
		void push(const T& val)
		{
			// 尾插
			_cont.push_back(val);
			
			// 向上调整建堆 --- 从堆尾开始调整
			AdjustUp(_cont.size() - 1);
		}

		// 出堆顶数据
		void pop()
		{
			// 将堆顶数据与堆尾数据交换
			swap(_cont[0], _cont[_cont.size() - 1]);
			// 再删除堆尾元素
			_cont.pop_back();
			// 向下调整建堆 --- 从堆头开始
			AdjustDown(0);
		}

		// 取堆顶数据
		T& top()
		{
			// 取优先级队列第一个元素
			return _cont[0];
		}

		// 判断堆是否为空
		bool empty()
		{
			return _cont.empty();
		}

		// 获取堆中有效元素个数
		size_t size()
		{
			return _cont.size();
		}
	private:
		Container _cont;
	};
}
