#pragma once

#include <iostream>
#include <vector>
#include <ctime>

#define MALLOC_SIZE (128 * 1024)	// 128 KB

// 优化：不调用 malloc 函数，直接调用系统调用
// windows 下的是 VirtualAlloc
#ifdef _WIN32               // 如果是 Windows 平台
	#include <Windows.h>    // 包含 VirtualAlloc 函数所需头文件
#else                       // 是 Linux 平台
	#include <sys/mman.h>   // 包含 mmap 函数
	#include <unistd.h>     // 包含 sysconf 函数
#endif

inline static void* SystemAlloc(size_t kpage)   // 直接向操作系统申请内存，不经过 malloc
{
	// 获取系统页大小
#ifdef _WIN32
	SYSTEM_INFO si;                     // 声明系统信息结构体
	GetSystemInfo(&si);                 // 获取系统信息（页大小、CPU 信息等）
	size_t pageSize = si.dwPageSize;    // 获取 Windows 页大小（通常 4KB）
#else
	size_t pageSize = sysconf(_SC_PAGESIZE);    // Linux 获取页大小（通常 4096 字节）
#endif

	size_t size = kpage * pageSize;             // 计算需要分配的字节数：页数 × 页大小

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

	if (ptr == MAP_FAILED) ptr = nullptr;   // mmap 失败返回 MAP_FAILED，转为 nullptr
#endif
	if (ptr == nullptr)	throw std::bad_alloc();     // 分配失败抛出异常

	return ptr;                         // 返回分配的内存地址
}

// 定长内存池
template<class T>       // 模板类，T 是要分配的对象类型
class ObjectPool
{
public:
	T* New()	// 从内存池获取一个对象的内存
	{
		T* obj = nullptr;	

		// 归还回来的内存块，可以再次被使用，优先使用归还的内存块
		if (_freeList != nullptr)
		{
			// 头删
			void* next = *((void**)_freeList);		// 获取头结点的下一个节点的地址，它也就存放在头结点的前4/8字节中
			obj = (T*)_freeList;	// 使用头节点作为分配的内存
			_freeList = next;	    // 头指针向后移动
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
				_memory = (char*)SystemAlloc(MALLOC_SIZE);	// 优化：向系统申请内存
				if (_memory == nullptr)	// 申请失败，抛异常
				{
					throw std::bad_alloc();	// 抛出 std::bad_alloc 异常
				}
				_remainBytes = MALLOC_SIZE; // 更新内存对象的剩余字节数
			}

			obj = (T*)_memory;		// 因为 _memory 要移动，存储 memory 作为返回值      从内存池头部切出一块

			// 如果最后内存池中可用的内存只有 T 类型大小的空间，memory += sizeof(T)，之后就没有空间了，就不能再使用内存池
			// 问题2：需要注意的是，内存池并不为空，只是空间被使用完毕了，怎么知道内存池中的空间被使用完了？需要新定义成员，来标记内存池剩余空间大小
			// _memory += sizeof(T);
			
			// 解决3：为了处理内存块被归还时，链接到自由链表中，也就是排除内存块的大小 < 4/8byte的情况
			// 如果要使用内存池空间的对象的大小 < 指针的大小的，就给指针大小的空间使用

			// 这串代码并不能放在这，因为前面有判断内存池剩余空间大小小于一个对象大小
			// 确保每块从内存池切出的内存至少有一个指针大小，足够安全地作为链表节点使用
			/*size_t objSize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);*/
			_memory += objSize;     // 内存指针向后移动

			/*// 如果 MALLOC_SIZE / sizeof(T) 是整除的，那么正好可以被切割成完整的对象
			// 问题1：如果没有被整除，切割到最后的，剩余的内存空间不够用，此时，应当再次申请大块内存空间
			_remainBytes -= sizeof(T);*/
			_remainBytes -= objSize;    // 剩余空间减少
		}

		// 对于已经有的空间可以直接初始化，调用构造函数初始化
		// 定位 new，显示调用T的构造函数初始化
		new(obj)T;	// obj 为已经分配好的内存地址（指针），T 为要构造的类型

        /******************* 关于定位new的相关问题 *******************/
        // 为什么要初始化 obj 对象？obj 指向一块原始内存，里面是随机值
        // 为什么要使用定位 new 来初始化 obj 对象？
        // 1. 普通 new 会重新分配内存，obj = new T，会重新 malloc 内存，完全违背了内存池的意义！
        // 2. 分离内存分配和对象构造
        // 传统方式：分配+构造 耦合在一起   T* obj = new T;  // 1. malloc 分配内存 2. 调用构造函数
        // 内存池方式：分离     T* obj = (T*)_memory;  // 1. 从池子拿内存（不构造），new(obj)T;     // 2. 只调用构造函数（不分配内存）

        // 为什么写成 new(obj)T 这样的格式？
        // new(obj)T 实际上是 new (obj) T 的简写，遵循了函数调用的语法：
        // 定位 new 可以理解为调用一个特殊的 operator new：operator new(sizeof(T), obj); —— 返回 obj，在 obj 地址上构造 T 对象
        // 所以语法是：new (参数列表) 类型，一个参数：new (obj) T;，多个参数（少见）：new (obj1, obj2) T;

        // 可能产生歧义的写法：new T(obj);  —— 这是创建新对象并传参？还是定位 new？C++ 标准规定：如果括号里是单个参数且是地址，才是定位 new。
        // 为了清晰，有时会写：new (obj) T(); —— 明确告诉编译器：这是 placement new（定位new）

        // new(obj)T 格式的原因：
        // 1. new 是运算符，不是函数
        // 2. (obj) 是传递给 operator new 的参数
        // 3. T 是要构造的对象类型
        //4. 整体含义：在 obj 地址上构造 T 类型对象
		return obj;
	}

	// 现在还回来了一块内存块，应该怎么处理？使用 _freeList 进行链接
	// 存在两种情况
	// freeList 为空，让 freeList 存储归还回来的内存块的地址
	// 但是内存块作为链表上的一个节点，如何标识链表的结束？让内存块的头4/8byte的空间存储
	// 问题3：内存块的大小不一定就是就是4/8byte，可能大于/小于，怎么处理
	void Delete(T* object)		// 归还对象到内存池（不释放给系统）
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

        /******************* 关于"显示调用析构函数清理"的相关问题 *******************/
        // 在内存池中，内存的分配/释放 与 对象的构造/析构 是分离的。
        // 传统方式：内存和对象生命周期绑定 { T obj;    // 自动构造，栈内存 }       // 自动析构，栈内存释放
        // T* obj = new T(); —— 堆内存分配 + 构造；delete obj; —— 析构 + 堆内存释放
        // 内存池方式：分离管理 T* obj = pool.New(); —— 1. 从池子取内存 2. 构造对象；pool.Delete(obj); —— 1. 析构对象 2. 内存归还池子（不释放！）
        // 为什么必须调用析构函数？如果不调用析构函数，会导致资源泄漏

        // 必须显式调用析构函数的原因：
        // 1. 资源释放：析构函数负责释放对象持有的资源（堆内存、文件句柄、锁等）
        // 2. 内存复用：内存池不释放内存块，只是复用，必须清理旧对象状态
        // 3. 防止泄漏：不调用析构会导致资源永远无法释放
        // 4. 对称性：new(ptr)T 构造，ptr->~T() 析构，保持配对
        // 关键区别：delete ptr = ptr->~T() + free(ptr)（释放内存）     pool.Delete(ptr) = ptr->~T() + 归还到池子（不释放内存）

		// 我们发现 else 中的代码块似乎也能满足 freeList 为空的情况
        // 将对象内存块头4/8字节存储自由链表头指针（头插法）
		*((void**)object) = _freeList;      // 当前对象的头指针指向原来的头节点
		_freeList = object;		// 当前对象成为头结点
	}


private:
	char* _memory = nullptr;		    // 指向申请的大块内存空间
	void* _freeList = nullptr;		    // 归还过程链接的自由链表的头指针
	size_t _remainBytes = 0;	        // 内存池剩余空间的大小	// 解决2
};

// // 测试 ObjectPool 中的 New 与 new 之间的性能对比
// struct TreeNode
// {
// 	int _val;
// 	TreeNode* _left;
// 	TreeNode* _right;

// 	TreeNode()
// 		: _val(0)
// 		, _left(nullptr)
// 		, _right(nullptr)
// 	{}
// };

// void TestObjectPool()
// {
// 	const size_t Rounds = 3;
// 	const size_t N = 100000;

// 	// 测试 new
// 	size_t begin1 = clock();
// 	std::vector<TreeNode*> v1;
// 	v1.reserve(N);
// 	for (size_t j = 0; j < Rounds; ++j)
// 	{
// 		for (size_t i = 0; i < N; ++i)
// 		{
// 			v1.push_back(new TreeNode);
// 		}
// 		for (size_t i = 0; i < N; ++i)
// 		{
// 			delete v1[i];
// 		}
// 		v1.clear();
// 	}
// 	size_t end1 = clock();

// 	// 测试 ObjectPool
// 	ObjectPool<TreeNode> TNPool;
// 	size_t begin2 = clock();
// 	std::vector<TreeNode*> v2;
// 	v2.reserve(N);
// 	for (size_t j = 0; j < Rounds; ++j)
// 	{
// 		for (size_t i = 0; i < N; ++i)
// 		{
// 			v2.push_back(TNPool.New());
// 		}
// 		for (size_t i = 0; i < N; ++i)  // ← 关键修复2：改为 N
// 		{
// 			TNPool.Delete(v2[i]);
// 		}
// 		v2.clear();
// 	}
// 	size_t end2 = clock();

// 	std::cout << "new cost time: " << end1 - begin1 << " ms" << std::endl;
// 	std::cout << "object pool cost time: " << end2 - begin2 << " ms" << std::endl;
// }