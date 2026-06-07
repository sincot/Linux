#pragma once
#include "Common.h"

#include <iostream>
#include <vector>
#include <ctime>

#define MALLOC_SIZE (128 * 1024)	// 128 KB


// 定长内存池
template<class T>
class ObjectPool
{
public:
	T* New()	// 获取内存池中的内存空间
	{
		T* obj = nullptr;

		// 归还回来的内存块，可以再次被使用，优先使用归还的内存块
		if (_freeList != nullptr)
		{
			// 头删
			void* next = *((void**)_freeList);		// 获取头结点的下一个节点的地址，它也就存放在头结点的前4/8字节中
			obj = (T*)_freeList;	// 使用自由链表中头内存块
			_freeList = next;	// 新的头结点
		}
		else
		{
			// 解决3：为了处理内存块被归还时，链接到自由链表中，也就是排除内存块的大小 < 4/8byte的情况
			// 如果要使用内存池空间的对象的大小 < 指针的大小的，就给指针大小的空间使用
			// 确保每块从内存池切出的内存至少有一个指针大小，足够安全地作为链表节点使用。
			size_t objSize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);

			// 解决1：内存池剩余空间的大小小于一个对象的大小，再开一大块空间，未使用的空间就直接丢弃
			if (_remainBytes < objSize)
			{
				// _memory = (char*)malloc(MALLOC_SIZE);
				_memory = (char*)SystemAlloc(MALLOC_SIZE >> 13);	// 优化
				if (_memory == nullptr)	// 申请失败，抛异常
				{
					throw std::bad_alloc();	// 抛出 std::bad_alloc 异常
				}
				_remainBytes = MALLOC_SIZE;
			}

			obj = (T*)_memory;		// 因为 _memory 要移动，存储 memory 作为返回值

			// 如果最后内存池中可用的内存只有 T 类型大小的空间，memory += sizeof(T)，之后就没有空间了，就不能再使用内存池
			// 问题2：需要注意的是，内存池并不为空，只是空间被使用完毕了，怎么知道内存池中的空间被使用完了？需要新定义成员，来标记内存池剩余空间大小
			// _memory += sizeof(T);

			// 解决3：为了处理内存块被归还时，链接到自由链表中，也就是排除内存块的大小 < 4/8byte的情况
			// 如果要使用内存池空间的对象的大小 < 指针的大小的，就给指针大小的空间使用

			// 这串代码并不能放在这，因为前面有判断内存池剩余空间大小小于一个对象大小
			// 确保每块从内存池切出的内存至少有一个指针大小，足够安全地作为链表节点使用
			/*size_t objSize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);*/
			_memory += objSize;

			/*// 如果 MALLOC_SIZE / sizeof(T) 是整除的，那么正好可以被切割成完整的对象
			// 问题1：如果没有被整除，切割到最后的，剩余的内存空间不够用，此时，应当再次申请大块内存空间
			_remainBytes -= sizeof(T);*/
			_remainBytes -= objSize;
		}

		// 对于已经有的空间可以直接初始化，调用构造函数初始化
		// 定位 new，显示调用T的构造函数初始化
		new(obj)T;	// obj 为已经分配好的内存地址（指针），T 为要构造的类型

		return obj;
	}

	// 现在还回来了一块内存块，应该怎么处理？使用 _freeList 进行链接
	// 存在两种情况
	// freeList 为空，让 freeList 存储归还回来的内存块的地址
	// 但是内存块作为链表上的一个节点，如何标识链表的结束？让内存块的头4/8byte的空间存储
	// 问题3：内存块的大小不一定就是就是4/8byte，可能大于/小于，怎么处理
	void Delete(T* object)
	{
		/*if (_freeList == nullptr)
		{
			_freeList = object;		// freeList 指向归还的内存块的地址

			// 让 object 的头4/8byte字节存储空，将 object 强转为 int* 类型，再解引用使用这块内存空间
			// 我们需要知道的是，这种方法似乎只在 32 位机器上，才没有问题。64位机器呢？指针是8byte，需要使用前8byte的空间
			// 也许有人会想，判断当前机器是32/64位，如果是32位，使用int*；如果是64位，使用long long*。这种方法也可以，但是还有更高明的办法
			// *((int*)object) = nullptr;
			// 既然在32位/64位机器下，指针的大小不同，那么使用指针来存储nullptr不就行了吗？ *((void**)object) = nullptr
			// 之前对 int* 解引用，使用的 int 大小；对 int** 解引用，使用的就是 int* 大小
			// 然而，在 32/64 位机器下，指针大小不一样，这恰好满足了我们针对不同的机器，使用头部4/8字节的空间
			*((void**)object) = nullptr;	// 这里的类型是什么，并不重要，重要的是它要是一个二级指针
		}

		// 以上解决的都是第一次链接问题，还有其它次呢？之后的内存块要尾插进自由链表吗？当然不是，时间复杂度O(N)，采用头插的方法
		else
		{
			// 当前内存块中的头4/8byte空间存放自由链表的头结点的地址，即freeList
			*((void**)object) = _freeList;
			_freeList = object;		// 当前内存块称为头结点
		}*/

		object->~T();	// 显示调用析构函数清理

		// 我们发现 else 中的代码块似乎也能满足 freeList 为空的情况
		*((void**)object) = _freeList;
		_freeList = object;		// 当前内存块称为头结点
	}


private:
	char* _memory = nullptr;		    // 指向申请的大块内存空间
	void* _freeList = nullptr;		    // 归还过程链接的自由链表的头指针
	size_t _remainBytes = 0;	        // 内存池剩余空间的大小	// 解决2
};