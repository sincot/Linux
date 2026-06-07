#pragma once

#include "Common.h"

class ThreadCache
{
public:
	// 申请与释放内存对象
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);

	// 从 Central Cache 中获取内存块 —— 获取 size 大小类的内存块
	void* FetchFromCentralCache(size_t index, size_t size);

	// 释放对象，链表过长，回收内存到 Central Cache
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