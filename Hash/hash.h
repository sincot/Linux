#pragma once
#include <iostream>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;


namespace HashTables
{
	// 设置哈希表中的位置状态
	enum Status
	{
		EMPTY,  // 空
		EXIST,  // 存在
		DELETE  // 删除
	};

	template<class K, class V>
	struct HashData
	{
		pair<K, V> _kv;
		Status _status = EMPTY;
	};

	// 仿函数
	template<class K>
	struct HashFunc
	{
		size_t operator()(const K& key)
		{
			// 强制类型转化为无符号的整型,为了解决key为负数的情况
			return (size_t)key;
		}
	};

	// 特化
	template<>
	struct HashFunc<string>
	{
		size_t operator()(const string& str)
		{
			// 将字符串中所有的ASCII都相加
			// 但是不同的字符串的字符相加之后的值相等
			// 这里我们使用 BKDR 哈希的思路，用上次的计算结果去乘以一个质数
			// 这个质数一般取 31, 131 等数值效果会比较好
			// 这里选取 31
			size_t hash = 0;
			for (auto e : str)
			{
				hash += e;
				hash *= 31;
			}

			return hash;
		}
	};

	// 添加仿函数
	template<class K, class V, class Hash = HashFunc<K>>
	class HashTable
	{
	public:
		// 构造函数
		HashTable()
			: _ht(__stl_next_prime(0))
			, _n(0)
		{}

		inline unsigned long __stl_next_prime(unsigned long n)
		{
			// Note: assumes long is at least 32 bits.
			static const int __stl_num_primes = 28;
			static const unsigned long __stl_prime_list[__stl_num_primes] =
			{
				  53,         97,         193,       389,       769,
				  1543,       3079,       6151,      12289,     24593,
				  49157,      98317,      196613,    393241,    786433,
				  1572869,    3145739,    6291469,   12582917,  25165843,
				  50331653,   100663319,  201326611, 402653189, 805306457,
				  1610612741, 3221225473, 4294967291
			};

			const unsigned long* first = __stl_prime_list;
			const unsigned long* last = __stl_prime_list + __stl_num_primes;
			const unsigned long* pos = lower_bound(first, last, n);
			return pos == last ? *(last - 1) : *pos;
		}

		// 插入
		bool Insert(const pair<K, V>& kv)
		{
			/*
			// 判断是否需要扩容
			// 当负载因子大于等于0.7时，需要扩容
			// 负载因子的计算公式：N/M
			//N -- 哈希表中存储的数据个数，M -- 哈希表的大小
			if ((double)_n / _ht.size() >= 0.7)
			{
				// 创建一个新的哈希表，在创建的同时开辟好空间
				vector<hashData<K, V>> newhashTable(_ht.size() * 2);
				// 将旧表中的数据重新映射到新表中
				for (auto& data : _ht)
				{
					// 如果hash表的位置状态为EXIST,就映射在新hash表中
					if (data._status == EXIST)
					{
						// 计算第一个元素存储在hash表中的下标位置
						size_t hash0 = kv.first % _ht.size();
						// 从第一个元素存储的下标位置开始探测
						size_t hashi = hash0;
						size_t i = 0;
						// 线性探测 --- 循环条件探测的位置的状态为空
						while (_ht[hashi]._status == EMPTY)
						{
							// hc(key,i) = hashi = (hash0+i) % M
							hashi = (hash0 + i) % _ht.size();
							++i;
						}

						// 将值存储到hash表中
						_ht[hashi]._kv = kv;
						// 更改 hashi 位置的状态
						_ht[hashi]._status = EXIST;
						// hash表中的有效数据个数加1
						++_n;
					}
				}

				// 新旧两表的引用进行交换
				_ht.swap(newhashTable);
			}
			*/

			// 去冗余
			if (Find(kv.first))
			{
				return false;
			}
			// 判断是否需要扩容
			// 当负载因子大于等于0.7时，需要扩容
			// 负载因子的计算公式：N/M
			// N -- 哈希表中存储的数据个数，M -- 哈希表的大小
			if ((double)_n / _ht.size() >= 0.7)
			{
				// 新建一个hash表
				HashTable<K, V, Hash> newHT;
				// 开辟两倍大小空间
				// 获取素数表中的下一个素数
				// 假设ht.size()的结果是53，53+1=54
				// 在素数表中获取大于等于54的素数，也就是97,这样就达到扩容的目的了
				newHT._ht.resize(__stl_next_prime(_ht.size() + 1));
				// 遍历旧表，将所有的值重新映射到新表
				for (auto& data : _ht)
				{
					// 如果hash表的位置状态为EXIST,就映射在新hash表中
					if (data._status == EXIST)
					{
						// 内部逻辑：线性探测解决冲突问题
						// 与Insert函数中的逻辑一致,直接调用表中的insert函数
						newHT.Insert(data._kv);
					}

					// 新旧两表进行交换
					_ht.swap(newHT._ht);
				}
			}
			// h(key) = hash0 = key % M
			// M 是 hash 表的大小
			// 计算第一个元素存储在hash表中的下标位置
			Hash hs;
			size_t hash0 = hs(kv.first) % _ht.size();
			// 从第一个元素存储的下标位置开始探测
			size_t hashi = hash0;
			size_t i = 0;
			// 线性探测 --- 循环条件探测的位置的状态为空
			while (_ht[hashi]._status == EMPTY)
			{
				// hc(key,i) = hashi = (hash0+i) % M
				hashi = hs(hash0 + i) % _ht.size();
				++i;
			}

			// 将值存储到hash表中
			_ht[hashi]._kv = kv;
			// 更改 hashi 位置的状态
			_ht[hashi]._status = EXIST;
			// hash表中的有效数据个数加1
			++_n;

			return true;
		}

		// 查找
		HashData<K, V>* Find(const K& key)
		{
			Hash hs;
			// h(key) = hash0 = key % M
			// M 是 hash 表的大小
			// 计算第一个元素存储在hash表中的下标位置
			size_t hash0 = hs(key) % _ht.size();
			// 从第一个元素存储的下标位置开始查找
			size_t hashi = hash0;
			// 从0位置开始查起
			size_t i = 0;
			// 循环查找 --- 循环条件为查找的位置的状态不为空
			while (_ht[hashi]._status != EMPTY)
			{
				// 如果状态为EXIST,且key值相等,则说明找到了
				if (_ht[hashi]._status == EXIST && _ht[hashi]._kv.first == key)
				{
					return &_ht[hashi];
				}
				// hc(key,i) = hashi = (hash0+i) % M
				hashi = hs(hash0 + i) % _ht.size();
				++i;
			}

			return nullptr;
		}

		// 删除
		bool Erase(const K& key)
		{
			// 查找要删除的数据
			auto* ptr = Find(key);

			// 如果找到了,将待删除的数据所在的位置状态置为DELETE
			if (ptr)
			{
				ptr->_status = DELETE;
				// 表的有效数据减1
				--_n;

				return true;
			}

			return false;
		}

	private:
		// 底层是vector
		vector<HashData<K, V>> _ht;
		size_t _n = 0; // 哈希表中的有效数据个数
	};
}


namespace HashBucket
{
	template<class K, class V>
	// 链表的结点的结构
	struct HashNode
	{
		pair<K, V> _kv;
		HashNode<K, V>* _next;

		// 构造函数
		HashNode(const pair<K, V>& kv)
			: _kv(kv)
			, _next(nullptr)
		{}
	};

	// 仿函数
	template<class K>
	struct HashFunc
	{
		size_t operator()(const K& key)
		{
			// 强制类型转化为无符号的整型,为了解决key为负数的情况
			return (size_t)key;
		}
	};

	// 特化
	template<>
	struct HashFunc<string>
	{
		size_t operator()(const string& str)
		{
			// 将字符串中所有的ASCII都相加
			// 但是不同的字符串的字符相加之后的值相等
			// 这里我们使用 BKDR 哈希的思路，用上次的计算结果去乘以一个质数
			// 这个质数一般取 31, 131 等数值效果会比较好
			// 这里选取 31
			size_t hash = 0;
			for (auto e : str)
			{
				hash += e;
				hash *= 31;
			}

			return hash;
		}
	};

	template<class K, class V, class Hash = HashFunc<K>>
	class HashBucket
	{
		typedef HashNode<K, V> Node;
	public:
		// 构造函数
		HashBucket()
			: _hb(__stl_next_prime(0), nullptr)
			, _n(0)
		{}
		
		// 析构函数
		~HashBucket()
		{
			// 遍历哈希桶
			for (size_t i = 0; i < _hb.size(); i++)
			{
				Node* cur = _hb[i];
				// 将链表的结点逐一释放
				while (cur)
				{
					// 提前存储下一个结点的地址
					Node* next = cur->_next;
					// 释放当前结点
					delete cur;
					cur = next;
				}

				// 再将桶位置置为空
				_hb[i] = nullptr;
			}
		}

		// 返回在数组中大于等于n的第一个数
		inline unsigned long __stl_next_prime(unsigned long n)
		{
			// Note: assumes long is at least 32 bits.
			static const int __stl_num_primes = 28;
			static const unsigned long __stl_prime_list[__stl_num_primes] =
			{
				  53,         97,         193,       389,       769,
				  1543,       3079,       6151,      12289,     24593,
				  49157,      98317,      196613,    393241,    786433,
				  1572869,    3145739,    6291469,   12582917,  25165843,
				  50331653,   100663319,  201326611, 402653189, 805306457,
				  1610612741, 3221225473, 4294967291
			};

			const unsigned long* first = __stl_prime_list;
			const unsigned long* last = __stl_prime_list + __stl_num_primes;
			const unsigned long* pos = lower_bound(first, last, n);
			return pos == last ? *(last - 1) : *pos;
		}

		// 插入
		bool Insert(const pair<K, V>& kv)
		{
			Hash ht;
			// 插入结点前,需要判断是否需要扩容
			// 当负载因子等于1时就需要扩容
			if (_n == _hb.size())
			{
				// 创建新表
				vector<Node*> newbucket(__stl_next_prime(_hb.size() + 1));
				// 遍历旧表 --- 建议使用下标访问
				for (size_t i = 0; i < _hb.size(); i++)
				{
					// 遍历旧表,将旧表的数据插入到新表中
					Node* cur = _hb[i];
					// 遍历旧表中位置存储的链表
					while (cur)
					{
						// 保存链表中下一个结点的地址
						Node* next = cur->_next;
						// 计算出数据插入新表的位置
						size_t hashi = ht(cur->_kv.first) % newbucket.size();
						// 头结点的指针就在哈希表中,新的结点的next指针指向头结点
						cur->_next = newbucket[hashi];
						// 插入的结点成为新的头结点,指针存在hash表中
						newbucket[hashi] = cur;
						// 继续向后遍历链表
						cur = next;
					}
				}

				// 新旧两表交换
				_hb.swap(newbucket);
			}

			// 计算出数据插入的位置
			size_t hashi = ht(kv.first) % _hb.size();

			// 插入时,怎么插？头插还是尾插？
			// 这里的链表是我们自己实现的,是单向链表
			// 尾插还需要找尾，建议头插
			Node* newnode = new Node(kv);
			// 头结点的指针就在哈希表中,新的结点的next指针指向头结点
			newnode->_next = _hb[hashi];
			// 插入的结点成为新的头结点,指针存在hash表中
			_hb[hashi] = newnode;

			return true;
		}

		// 查找
		Node* Find(const K& key)
		{
			Hash ht;
			// 计算查找数据的位置
			size_t hashi = ht(key) % _hb.size();
			// 在表中位于hashi位置的链表中找
			Node* cur = _hb[hashi];
			// 遍历链表
			while (cur)
			{
				if (cur->_kv.first == key)
				{
					return cur;
				}

				// 继续往后查找
				cur = cur->_next;
			}

			// 找不到返回空指针
			return nullptr;
		}

		// 删除
		bool Erase(const K& key)
		{
			Hash ht;
			// 计算删除数据的位置
			size_t hashi = ht(key) % _hb.size();
			// 在表中位于hashi位置的链表中找
			Node* cur = _hb[hashi];
			// 找 cur 结点的前一个结点
			Node* prev = nullptr;
			// 遍历链表
			while (cur)
			{
				if (cur->_kv.first == key)
				{
					// 如果cur就是头结点
					if (prev == nullptr)
					{
						// 将头结点的下一个结点的地址给表头
						_hb[hashi] = cur->_next;
					}
					else
					{
						prev->_next = cur->_next;
					}

					delete cur;
					return true;
				}

				// 继续往后查找
				prev = cur;
				cur = cur->_next;
			}

			return false;
		}

	private:
		// 哈希桶的底层结构不能这样,这个方式代码实现起来有些复杂
		//vector < list<pair<K, V>> _hb;
		// 推荐使用这种方式
		vector<Node*> _hb;
		// 桶中的有效数据个数
		size_t _n;
	};
}
