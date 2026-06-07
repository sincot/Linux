#pragma once

#include <iostream>
#include <assert.h>
#include <string.h>
#include <string>

using namespace std;

namespace AY
{
	template<class T>
	class vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		// 无参的构造函数
		vector()
			: m_start(nullptr)
			, m_finish(nullptr)
			, m_end_of_storage(nullptr)
		{}

		// 析构函数
		~vector()
		{
			if (m_start != nullptr)
			{
				delete[] m_start;
				m_finish = m_end_of_storage = nullptr;
			}
		}

		// reserve 函数
		void reserve(size_t n = 0)
		{
			if (n > capacity())
			{
				// 提前存储 size 函数的返回值
				size_t old_size = size();
				// 开辟新空间
				T* tmp = new T[n];

				// 拷贝旧数据
				// 检验数组的数据是否为0
				if (m_start != NULL)
				{
					//memcpy(tmp, m_start, sizeof(T) * old_size);
					for (size_t i = 0; i < old_size; i++)
					{
						tmp[i] = m_start[i];
					}
					// 释放旧空间
					delete[] m_start;
				}

				// 更新变量的数据
				m_start = tmp;
				m_finish = m_start + old_size;
				m_end_of_storage = m_start + n;
			}
		}

		// size 函数
		size_t size() const
		{
			return m_finish - m_start;
		}

		// capacity 函数
		size_t capacity() const
		{
			return m_end_of_storage - m_start;
		}

		// push_back 函数
		void push_back(const T& x)
		{
			if (m_finish == m_end_of_storage)
			{
				size_t newcapacity = (capacity() == 0 ? 4 : 2 * capacity());
				reserve(newcapacity);
			}

			// finish 指向的是有效字符的末尾
			*m_finish = x;
			++m_finish;
		}

		// operator[] 函数 —— 普通版本
		T& operator[](size_t pos)
		{
			// 检验 pos 是否合法
			assert(pos < size());

			return m_start[pos];
		}

		// operator[] 函数 —— const 版本
		const T& operator[](size_t pos) const
		{
			// 检验 pos 是否合法
			assert(pos < size());

			return m_start[pos];
		}

		// begin 函数
		iterator begin()
		{
			return m_start;
		}

		// end 函数
		iterator end()
		{
			return m_finish;
		}

		// begin 函数 —— const 版本
		const_iterator begin() const
		{
			return m_start;
		}

		// end 函数 —— const 版本
		const_iterator end() const
		{
			return m_finish;
		}

		// empty 函数
		bool empty() const
		{
			return m_start == m_finish;
		}

		// pop_back 函数
		void pop_back()
		{
			assert(!empty());

			--m_finish;
		}

		// insert 函数
		iterator insert(iterator pos, const T& val)
		{
			// 检验 pos 迭代器的合法性
			assert(pos >= m_start && pos <= m_finish);

			// 判断是否需要扩容
			if (m_finish == m_end_of_storage)
			{
				size_t len = pos - m_start;
				size_t newcapacity = capacity() == 0 ? 4 : 2 * capacity();
				reserve(newcapacity);
				// 更新 pos 的指向
				pos = m_start + len;
			}

			iterator end = m_finish;
			while (end > pos)
			{
				*end = *(end - 1);
				--end;
			}
			*pos = val;
			++m_finish;

			// 指向插入数据的位置的迭代器，在这里也就是 pos
			return pos;
		}

		// erase 函数
		iterator erase(iterator pos)
		{
			// 检查 pos 迭代器的合法性
			assert(pos >= m_start && pos <= m_finish);

			// 挪动数据
			iterator it = pos;
			while (it != m_finish)
			{
				*it = *(it + 1);
				it++;
			}

			// 有效数据个数 - 1
			--m_finish;

			// 返回被删除数据的下个数据的位置的迭代器，在这里也就是 pos
			return pos;
		}

		// clear 函数
		void clear()
		{
			m_finish = m_start;
		}

		// resize 函数
		void resize(size_t n, const T& val = T())
		{
			// n 大于 有效数据个数 —— 就扩容
			if (n > size())
			{
				reserve(n);
				while (m_finish != m_start + n)
				{
					*m_finish = val;
					++m_finish;
				}
			}
			// n 小于等于 有效数据个数，减少有效数据个数
			else
			{
				m_finish = m_start + n;
			}
		}

		// swap 函数
		void swap(vector<T>& x)
		{
			std::swap(m_start, x.m_start);
			std::swap(m_finish, x.m_finish);
			std::swap(m_end_of_storage, x.m_end_of_storage);
		}

		// 拷贝构造函数
		vector(const vector<int>& v)
			: m_start(nullptr)
			, m_finish(nullptr)
			, m_end_of_storage(nullptr)
		{
			reserve(v.capacity());
			for (auto& e : v)
			{
				push_back(e);
			}
		}

		// 支持花括号初始化容器
		vector(initializer_list<T> il)
		{
			reserve(il.size());
			for (auto& e : il)
			{
				push_back(e);
			}
		}
        
		// 支持迭代器区间初始化
		/*vector(iterator begin, iterator end)
		{
			// 把迭代器区间的值插入到容器中
			while (begin != end)
			{
				push_back(*begin);
				++begin;
			}
		}*/

		// 支持迭代器区间初始化
		template <class InputIterator, 
			      enable_if_t<_Is_iterator_v<InputIterator>, int> = 0>
		vector(InputIterator first, InputIterator last)
		{
			// 把迭代器区间的值插入到容器中
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}

		// 支持 n 个 val 值初始化
		vector(size_t n, const T& val = T())
		{
			resize(n, val);
		}

		// 赋值重载函数
		vector<T>& operator=(const vector<T>& x)
		{
			swap(x);

			return *this;
		}

    private:
		iterator m_start = nullptr;
		iterator m_finish = nullptr;
		iterator m_end_of_storage = nullptr;
	};
}
