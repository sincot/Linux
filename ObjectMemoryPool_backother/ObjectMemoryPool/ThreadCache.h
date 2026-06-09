#pragma once

#include "Common.h"
#include "ObjectPool.h"

class ThreadCache
{
public:
	// 申请与释放内存对象
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);

	// 从中心缓存（Central Cache，线程共享）批量获取多个 size 大小的内存块
	void* FetchFromCentralCache(size_t index, size_t size);

	// 当某个自由链表过长时，回收一部分内存回中心缓存
	// list：过长的自由链表，size：内存块大小，用于计算索引和通知中心缓存
	void ListTooLong(FreeList& list, size_t size);

private:
	// 每个线程的线程缓存
	// 线程缓存中维护一个自由链表数组，每个元素对应一个大小类，链表中的每个节点都是一个内存块
	// 线程缓存中维护一个计数器数组，记录每个大小类的内存块数量
	// 线程缓存中维护一个最大内存块数量，当某个大小类的内存块数量超过这个值时，就从中央缓存中获取更多的内存块
	FreeList _freeLists[MAX_INDEX];	// 自由链表数组

};

// 每个线程都有一个独享的 Thread Cache，Thread Cache 中维护一个自由链表数组，每个元素对应一个大小类，链表中的每个节点都是一个内存块
// 当前线程的 Thread Cache 通过 Thread Local Storage (TLS 线程本地存储) 来实现，即每个线程都有一个独立的 Thread Cache 实例，线程之间互不干扰

// 使用静态的 TLS 来实现每个线程独享的 Thread Cache
// 一开始 pTLSThreadCache 是 nullptr，表示当前线程还没有 Thread Cache 实例
// 当线程第一次调用 Allocate 或 Deallocate 时，如果 pTLSThreadCache 是 nullptr，就创建一个新的 Thread Cache 实例，并将其地址赋值给 pTLSThreadCache，这样当前线程就拥有了一个独享的 Thread Cache 实例

// 定义成 static 的，确保它只在当前文件内可见，避免命名冲突
static _declspec(thread) ThreadCache* pTLSThreadCache = nullptr;   // 每个线程都有一个独享的 Thread Cache，线程之间互不干扰


// ========== 跨平台 TLS 实现 ==========
//#ifdef _WIN32
//	#define TLS_DECLARE(type, name) static _declspec(thread) type name
//#else
//	// Linux/Unix: 使用 __thread 关键字（GCC/Clang）
//	#define TLS_DECLARE(type, name) static __thread type name
//#endif
//
//// 每个线程独立的 ThreadCache 指针
//TLS_DECLARE(ThreadCache*, pTLSThreadCache) = nullptr;
//
//// 获取当前线程的 ThreadCache（线程安全初始化）
//inline ThreadCache* GetThreadCache()
//{
//	if (pTLSThreadCache == nullptr)
//	{
//		// 使用 ObjectPool 创建，避免使用 malloc（避免循环依赖）
//		static ObjectPool<ThreadCache> tcPool;
//		// 注意：这里需要确保 tcPool 的线程安全，或者使用 pthread_once
//		pTLSThreadCache = tcPool.New();
//	}
//	return pTLSThreadCache;
//}