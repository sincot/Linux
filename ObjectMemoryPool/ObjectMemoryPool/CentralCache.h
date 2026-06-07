#pragma once

#include "Common.h"

// Central Cache 被所有的线程共享 —— 设计成单例模式（饿汉模式）
class CentralCache
{
public:
	// 实例化单例对象
	static CentralCache& GetInstance() { return _sInst; }

	// 从中心缓存获取一定数量(n)的对象(大小为bytes)给 thread cache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t bytes);

	// 获取一个非空的 Span
	// list —— 对应大小类的 Span 链表	bytes —— 每个小块内存的大小
	Span* GetOneSpan(SpanList& list, size_t bytes);

	void ReleaseListToSpans(void* start, size_t bytes);		// 将一定数量的对象释放到 span
private:
	SpanList _spanLists[MAX_INDEX];		// 有多少个桶，就有多少个桶链表

private:
	// 私有化构造函数与拷贝构造
	CentralCache() {}
	CentralCache(const CentralCache&) = delete;

	static CentralCache _sInst;		// 单例对象
};

