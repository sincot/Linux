#pragma once

#include "Common.h"

// Central Cache 被所有的线程共享 —— 设计成单例模式（饿汉模式）
// 饿汉模式：程序启动时自动初始化，无需考虑线程安全问题
class CentralCache
{
public:
	// 实例化单例对象
	// static：通过类名直接调用（CentralCache::GetInstance()）
	// 返回引用而非指针，避免空指针问题；函数体直接返回静态成员 _sInst，无调用开销（编译器可内联）
	static CentralCache& GetInstance() { return _sInst; }

	// 从中心缓存获取一定数量(n)的对象(大小为bytes)给 thread cache
	// start/end：输出参数，返回内存块链表的头尾指针；n：期望获取的对象数量；bytes：每个对象的大小（字节）
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t bytes);

	// 获取一个非空的 Span		如果当前链表中没有可用 Span，会向 PageCache 申请新的 Span
	// list —— 对应大小类的 Span 链表；bytes —— 每个小块内存的大小，用于计算需要向 PageCache 申请多少页
	Span* GetOneSpan(SpanList& list, size_t bytes);

	// ThreadCache 链表过长时调用
	// 功能：将链表中的每个内存块归还给对应的 Span，当 Span 的 usecount 降为 0 时，将整个 Span 释放回 PageCache
	// start：要释放的内存块链表的起始地址；bytes：每个对象的大小，用于定位对应的 Span
	void ReleaseListToSpans(void* start, size_t bytes);		// 将一定数量的对象释放到 span 

private:
	SpanList _spanLists[MAX_INDEX];		// 有多少个桶，就有多少个桶链表

private:
	// 私有化构造函数与拷贝构造
	CentralCache() {}
	CentralCache(const CentralCache&) = delete;

	static CentralCache _sInst;		// 单例对象		_sInst 声明在类内，定义在 .cpp 文件中
};

