#pragma once

#include <assert.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <unordered_map>
#include <memory>

using std::cout;
using std::endl;

static const size_t MAX_BYTES = 256 * 1024;		// 256 KB，定义内存池能处理的最大对象大小
static const size_t MAX_INDEX = 208;            // 208 个大小类（桶），定义大小类的总数
static const size_t NPAGES = 129;				// 最大页数，定义 PageCache 管理的最大页数
static const size_t PAGE_SHIFT = 13;			// 2……13 = 8 * 1024（一页的大小 8KB），页大小的位移数，用于快速计算：地址 → 页号（右移 PAGE_SHIFT）


// 绕过 malloc，直接向操作系统申请内存，避免与用户态内存分配器冲突
// windows 下的是 VirtualAlloc
#ifdef _WIN32	// Windows：Windows.h 提供 VirtualAlloc / VirtualFree
#include <Windows.h>	
#else			// Linux/Unix：sys/mman.h 提供 mmap/munmap，unistd.h 提供 sysconf
#include <sys/mman.h>
#include <unistd.h>
#endif

inline static void* SystemAlloc(size_t kpage)		// 向操作系统申请 kpage 页的连续内存
{
/**************** 获取操作系统页大小（通常为 4KB） ****************/

//#ifdef _WIN32
//	SYSTEM_INFO si;
//	GetSystemInfo(&si);
//	size_t pageSize = si.dwPageSize;
//#else
//	size_t pageSize = sysconf(_SC_PAGESIZE);
//#endif

/**************** 获取操作系统页大小（通常为 4KB） ****************/

/**************** 获取操作系统页大小（通常为 8KB） ****************/

	size_t pageSize = (1 << PAGE_SHIFT);  // 8KB

	size_t size = kpage * pageSize;			// 计算实际需要分配的字节数
	// size_t size = kpage * pageSize;		// 计算实际需要分配的字节数

/**************** 调用系统 API 分配内存 ****************/
#ifdef _WIN32
	void* ptr = VirtualAlloc(
		nullptr,						// 系统决定地址
		size,							// 分配大小
		MEM_COMMIT | MEM_RESERVE,		// 提交并预留
		PAGE_READWRITE					// 可读可写
	);
#else	// Linux
	void* ptr = mmap(
		nullptr,						// 系统选择地址
		size,							// 分配大小
		PROT_READ | PROT_WRITE,			// 可读可写
		MAP_PRIVATE | MAP_ANONYMOUS,	// 私有匿名映射
		-1,								// 无文件描述符
		0								// 偏移量0
	);

	if (ptr == MAP_FAILED) ptr = nullptr;
#endif
/**************** 调用系统 API 分配内存 ****************/

	// 检查分配是否成功，失败则抛出异常
	if (ptr == nullptr)	throw std::bad_alloc();

	return ptr;
}

// 释放系统分配的内存
inline static void SystemFree(void* ptr, size_t size)		// size —— 额外记录分配的大小
{
	if (ptr == nullptr) return;

#ifdef _WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#elif defined(__linux__) || defined(__APPLE__)
	// Linux/Mac 使用 munmap 释放 mmap 分配的内存
	// 注意：munmap 需要知道内存大小，通常需要额外记录
	// 这里假设 ptr 是从 mmap 分配的，且大小已记录在其他地方
	munmap(ptr, size);  // 实际使用中需要传递正确的大小
#else
	free(ptr);
#endif
}

// 自由链表
class FreeList
{
public:
	void Push(void* obj)		// 将内存块 obj 加入自由链表
	{
		assert(obj);		// 确保 obj 不为空
		// 将 obj 的前4/8字节存储当前链表头的地址（头插）
		*(void**)obj = _freeList;	// 将 obj 的前4/8字节存储当前链表的头指针
		_freeList = obj;	// 更新链表的头指针为 obj
		_size++;			// 插入内存块 obj，节点数量 +1
	}

	void* Pop()			// 从自由链表中取出一个内存块
	{
		assert(_freeList != nullptr);	// 确保链表不为空

		void* obj = _freeList;		// 获取链表头的内存块
		void* next = *(void**)_freeList;	// 获取链表头的下一个内存块的地址
		_freeList = next;		// 更新链表头为下一个内存块

		_size--;			// 取出内存块 obj，节点数量 -1
		return obj;			// 返回链表头的内存块
	}

	void PushRange(void* start, void* end, size_t n)		// 插入一段链表
	{
		assert(n > 0);
		*(void**)end = _freeList;
		_freeList = start;

		_size += n;			// 插入一段链表，节点数量 +n
	}

	void PopRange(void*& start, void*& end, size_t n)		// 取出一段链表
	{
		assert(n <= _size);		// 取出的链表的长度至少要大于等于自由链表的长度
		// assert(n >= _size);		// 取出的链表的长度至少要大于等于自由链表的长度			

		/*if (n == 0) {
			start = end = nullptr;
			return;
		}*/

		start = _freeList, end = start;
		for (size_t i = 0; i < n - 1; i++)
		{
			end = *(void**)end;	// 向后移动
			assert(end != nullptr);		// 确保链表足够长
		}
		_freeList = *(void**)end;
		*(void**)end = nullptr;

		_size -= n;			// 取出一段链表，节点数量 -n
	}
	
	bool Empty() const   // 判断自由链表是否为空
	{
		return _freeList == nullptr;    // 如果链表头指针为 nullptr，说明链表为空
	}

	size_t& GetMaxSize() { return _maxSize; }		// 使用引用用于改变 _maxSize 的值
	size_t GetSize() { return _size; }				// 获取当前自由链表节点的个数
private:
	void* _freeList = nullptr;		// 自由链表的头指针
	size_t _maxSize = 1;			// Thread Cache 一次向 Central Cache 批量获取数量的上限（初始为 1）
	size_t _size = 0;				// 记录自由链表中节点的个数
};


// 对象大小类，负责计算对象大小对齐后的大小和对象大小对应的大小类索引
class SizeClass
{
public:
	// 整体控制在 10% 左右的内碎片浪费（一共 208 个大小类）
	// [1,128]						8byte对齐			freelist[0,16)		16个大小类
	// [128+1, 1024]				16byte对齐			freelist[16,72)		56个大小类
	// [1024+1, 8*1024]				128byte对齐			freelist[72,128)	56个大小类
	// [8*1024+1, 64*1024]			1024byte对齐		freelist[128,184)	56个大小类
	// [64*1024+1, 256*1024]		8*1024byte对齐		freelist[184,208)	24个大小类
	// 如果都以 8byte 对齐，那么 256KB 就有 32768 个大小类，内碎片浪费就很大了，所以不同范围的对象使用不同的对齐方式

	/*********************** 计算对象大小对齐后的大小 ***********************/ 
	// _RoundUp 函数是静态的原因
	// 1. 该函数是 SizeClass 类的一个工具函数，它不依赖于 SizeClass 类的任何成员变量，逻辑上属于类本身的行为，而不是某个具体对象的行为，因此可以将其定义为静态函数。
	// 2. 将 _RoundUp 定义为静态函数可以避免在每个 SizeClass 对象中都存储一个函数指针，从而节省内存空间。因为 _RoundUp 的实现是固定的，不需要为每个对象单独存储一份。
	// 3. static 使函数具有内部链接，如果多个 .cpp 文件都包含这个头文件，不会产生重复定义错误，每个编译单元有自己的函数副本，链接时不会冲突
	// 总结：1. 不依赖对象，可通过类名调用；2. 避免多重定义（内部链接）；3. 明确表示这是类的工具函数
	// _RounUp 函数是 inline 的原因
	// 1. 消除函数调用开销：_RoundUp 是一个非常简单的函数（只有一行位运算），调用开销（压栈、跳转、返回）可能远大于函数本身执行时间，inline 建议编译器将函数体直接展开到调用处
	// 2. 编译器优化：inline 函数可以让编译器更好地优化代码，因为编译器在看到函数调用时可以直接替换为函数体，从而进行更多的优化
	// 总结：1. 消除函数调用开销；2. 允许在头文件中定义； 3. 建议编译器展开函数体
	// 组合使用：头文件中定义小工具函数的标准写法
	static inline size_t _RoundUp(size_t bytes, size_t align)  // 将 bytes 对齐到 align 的整数倍
	{
		// 对齐后大小 = ((实际大小 + 对齐数 - 1) / 对齐数) * 对齐数
		// return ((bytes + align - 1) / align) * align;
		
		// 对齐后大小 = (实际大小 + 对齐数 - 1) & ~(对齐数 - 1)
		// &~(对齐数 - 1) 的作用是将 bytes + align - 1 的二进制表示中，最后 log2(align) 位清零，从而实现对齐
		return ((bytes + align - 1) & ~(align - 1));
	}

	static size_t RoundUp(size_t bytes)		// 根据大小范围，使用不同的对齐数
	{
		if (bytes <= 128)				return _RoundUp(bytes, 8);	// 8byte对齐
		else if (bytes <= 1024)			return _RoundUp(bytes, 16); // 16byte对齐
		else if (bytes <= 8 * 1024)		return _RoundUp(bytes, 128); // 128byte对齐
		else if (bytes <= 64 * 1024)	return _RoundUp(bytes, 1024); // 1024byte对齐
		else if (bytes <= 256 * 1024)	return _RoundUp(bytes, 8 * 1024); // 8*1024byte对齐
		// else { assert(false);	return -1; }	// 超过最大值，抛异常
		else { return _RoundUp(bytes, 1 << PAGE_SHIFT);  }
	}
	/************************************************************************/

	/*********************** 计算对象大小对应的大小类索引 ***********************/
	static inline size_t _Index(size_t bytes, size_t align)	 // 计算 bytes 在当前对齐方式下的索引
	{
		// - 1 是因为大小类索引从 0 开始，而对齐后的大小是从 align 开始的，所以需要减去 1 来调整索引
		return ((bytes + align - 1) / align - 1);  // 对齐后大小 / 对齐数 - 1就是大小类索引
	}

	static size_t Index(size_t bytes)
	{
		assert(bytes <= MAX_BYTES);  // 确保对象大小不超过最大值

		static int indexArray[4] = { 16, 56, 56, 24 };	// 每个对齐方式对应的大小类数量
		if (bytes <= 128)
		{
			return _Index(bytes, 8);  // 8byte对齐
		}
		else if (bytes <= 1024)
		{
			// 16byte对齐，前面有 indexArray[0] 个大小类
			return _Index(bytes - 128, 16) + indexArray[0];  
		}
		else if (bytes <= 8 * 1024)
		{
			// 128byte对齐，前面有 indexArray[0] + indexArray[1] 个大小类
			return _Index(bytes - 1024, 128) + indexArray[0] + indexArray[1];  
		}
		else if (bytes <= 64 * 1024)
		{
			// 8*1024byte对齐，前面有 indexArray[0] + indexArray[1] + indexArray[2] 个大小类
			return _Index(bytes - 8 * 1024, 128) + indexArray[0] + indexArray[1] + indexArray[2];
		}
		else if (bytes <= 256 * 1024)
		{
			// 64*1024byte对齐，前面有 indexArray[0] + indexArray[1] + indexArray[2] + indexArray[3] 个大小类
			return _Index(bytes - 64 * 1024, 128) + indexArray[0] + indexArray[1] + indexArray[2] + indexArray[3];
		}
		else
		{
			assert(false);  // 超过最大值，抛异常
			return -2;
		}
	}

	// 为什么 RoundUp 和 Index 函数设置成 static 静态成员函数？
	// 因为这两个函数都是 SizeClass 类的工具函数，它们不依赖于 SizeClass 类的任何成员函数，逻辑上属于类本身的行为，而不是某个具体对象的行为，因此可以将它们定义为静态函数

	/****************************************************************************/


	// thread cache 一次 从中心缓存获取的对象个数
	static size_t NumMoveSize(size_t bytes)
	{
		if (bytes == 0)	return 0;

		// [2, 512] 一次批量移动多少个对象的慢启动上限值
		// 小对象一次批量上限高，大对象一次批量上限低
		size_t num = MAX_BYTES / bytes;
		if (num < 2) num = 2;
		if (num > 512) num = 512;

		return num;
	}
	
	// 计算一次向 PageCache 申请多少页
	static size_t NumMovePage(size_t size)
	{
		size_t num = NumMoveSize(size);		// 一次 thread cache 从中心缓存获取的对象个数
		// size_t npage = num * size;			// 多少字节	
		// npage >>= PAGE_SHIFT;

		size_t npage = (num * size + (1 << PAGE_SHIFT) - 1) >> PAGE_SHIFT;
		if (npage == 0)	npage = 1;

		return npage;
	}
};
/**************************************************************************************************/


// 平台相关的页编号类型定义，页编号需要足够大，能够存储从指针转换来的整数
// 在 WIN32 配置下，_WIN32 有定义，_WIN64 没有定义
// 在 WIN64 配置下，_WIN32 和 _WIN64 都有定义
#ifdef _WIN64
	// Windows 64位系统，Windows 中 long 在64位下仍然是4字节，必须用 long long
	typedef unsigned long long PAGE_ID;
#elif _WIN32
	typedef size_t PAGE_ID;
#elif __linux__
	// unsigned long 在 32位系统是4字节，64位系统是8字节。完美适配两种架构
	typedef unsigned long PAGE_ID;
#else
	// 其他平台（macOS、FreeBSD、Android 等），使用 uintptr_t 保证跨平台兼容性
	#include <stdint.h>
	typedef uintptr_t PAGE_ID;
#endif 

// 为什么 Span 结构要设计成双向链表？
// 1. 需要从中间删除节点，双向链表的核心优势是 O(1) 时间从任意位置删除节点。
// 2. 需要从头部和尾部高效操作：分配时：从链表头部取一个 Span；回收时：可能插入到头部或尾部
// 3. 需要遍历链表
struct Span		// 一页的大小 8KB
{
	// 页号的作用：通过页号可以快速计算出 Span 管理的内存起始地址；建立地址到 Span 的映射关系
	size_t s_pageId = 0;			// 页号，页号标识了这块连续内存的起始地址
	size_t s_n = 0;					// 页的数量，告诉系统这块 Span 包含了多少个连续的页

	// 通过 s_pageId 和 s_n 的组合，可以在 O(1) 时间内完成：
	// 1. 地址 → Span 的查找；2. Span 的大小计算；3. 相邻 Span 的合并判断；4. 内存范围的快速验证

	void* s_freeList = nullptr;		// 切分后的小块内存链表

	// 双链表结构
	Span* s_prev = nullptr;
	Span* s_next = nullptr;	

	// 如果 central cache 中的 span usecount 为0，说明 span 中所有的小块内存都没有被 thread cache 归还回来，central cache 可以将这个 span 归还给 page cache
	size_t s_usecount = 0;				// Span 切好的小块内存，被分配给 thread cache 的计数

	bool s_isUse = false;				// span 是否被使用，初始默认未被使用

	size_t s_objSize = 0;				// 切好的小内存块的大小
};

class SpanList		// 带头双向循环链表
{	
public:
	SpanList()		// 构造函数，创建哨兵节点
	{
		_head = new Span;
		_head->s_next = _head, _head->s_prev = _head;
	}

	Span* Begin() { return _head->s_next; }
	Span* End() { return _head;  }

	void Insert(Span* pos, Span* newSpan)
	{
		assert(pos);
		assert(newSpan);

		Span* prev = pos->s_prev;
		// prev newSpan pos
		prev->s_next = newSpan;
		newSpan->s_prev = prev;
		newSpan->s_next = pos;
		pos->s_prev = newSpan;
	}

	void Erase(Span* pos)
	{
		assert(pos);
		assert(pos != _head);	// 不能删除头结点（哨兵节点）

		Span* prev = pos->s_prev, * next = pos->s_next;
		// prev next
		prev->s_next = next, next->s_prev = prev;
	}

	void PushFront(Span* newSpan)	{ Insert(Begin(), newSpan); }
	Span* PopFront() 
	{
		Span* front = _head->s_next;
		Erase(front);
		return front;
	}

	bool Empty() { return _head->s_next == _head; }

	std::mutex& GetMutex() { return _mutex; }

private:
	Span* _head = nullptr;	// 链表的头结点
	std::mutex _mutex;		// 加上桶锁
};

// <= 256KB 的内存空间 —— 三层缓存
// > 256 KB 的内存空间：
// 若内存空间大小在 32*8KB < size < 128*8KB —— 直接找 Page Cache128 * 8KB = 1024KB -> 4*256KB
// 若内存空间大小在 size > 128*8KB —— 直接找堆申请


