#include "ThreadCache.h"
#include "CentralCache.h"

/*********************** 申请与释放内存对象 ***********************/
void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAX_BYTES);	// 确保请求的大小不超过最大限制（256KB）

	// 将原始大小对齐到对应规则的大小
	size_t alignedSize = SizeClass::RoundUp(size); 
	// 根据原始大小（不是对齐后的大小）计算大小类索引
	size_t index = SizeClass::Index(size);

	// 检查对应大小类的自由链表是否为空
	if (_freeLists[index].Empty() == false)  // 如果对应大小类的自由链表不为空，就从自由链表中取出一个内存块
	{
		// 不为空：直接从链表头部取出一个内存块返回
		return _freeLists[index].Pop();   // 从自由链表中取出一个内存块(头)并返回
	}
	else    // 如果对应大小类的自由链表为空，就从中央缓存中获取更多的内存块
	{
		// 为空：需要从 CentralCache 批量获取
		// 自由链表为空时，调用 FetchFromCentralCache 从 CentralCache 批量获取内存块
		return FetchFromCentralCache(index, alignedSize);
	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
	// 确保释放的指针有效，大小不超过限制
	assert(ptr != nullptr);     // 确保释放的内存块不为空
	assert(size <= MAX_BYTES);	// 确保释放的内存大小不超过最大值

	// 找到对应的映射的自由链表桶，将对象插入对应桶的自由链表中
	size_t index = SizeClass::Index(size);  // 根据大小计算大小类索引，找到对应的自由链表桶
	_freeLists[index].Push(ptr);    // 将释放的内存块头插到对应自由链表中

	// 如果现在链表的长度超过向 Central Cache 一次批量申请的长度，就释放给 Central Cache
	if (_freeLists[index].GetSize() >= _freeLists[index].GetMaxSize())
	{
		ListTooLong(_freeLists[index], size);
	}
}
/******************************************************************/

// 当 ThreadCache 的 freeList 为空时，向 CentralCache 批量申请内存 —— 获取 size 大小类的内存块
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	// 慢开始反馈调节算法
	// 1. 最开始不会一次向 central cache 一批次要太多，因为要太多了可能使用不完
	// 2. 若对于 size 大小内存的需求很大，那么 batchNum 会不断增长，直到达到上限 [2, 512]
	// 核心机制：
	// 1. _freeLists[index].GetMaxSize()：当前批量获取数量的上限（初始为 1）
	// 2. SizeClass::NumMoveSize(size)：系统允许的最大批量值
	// 3. 取两者的最小值作为本次期望获取的数量
	size_t batchNum = min(_freeLists[index].GetMaxSize(), SizeClass::NumMoveSize(size));

	// 如果本次实际申请数量达到了当前上限，说明需求旺盛，将上限 + 1 慢增长
	// 这样设计可以：避免一开始就批量太大造成浪费；根据实际需求动态调整，达到最佳性能
	if (batchNum == _freeLists[index].GetMaxSize())	_freeLists[index].GetMaxSize() += 1;	// 慢增长

	// 从 CentralCache 获取内存，调用 CentralCache 的 FetchRangeObj，返回值：实际获取到的对象数量（可能小于 batchNum）
	void* start = nullptr, * end = nullptr;	
	size_t actualNum = CentralCache::GetInstance().FetchRangeObj(start, end, batchNum, size);		// 实际申请到的 size 大小内存对象的个数
	assert(actualNum > 0);		// 实际获得的 size 大小内存对象至少有 1 个

	// 如果只获取到 1 个对象，直接返回这个对象（不需要维护链表）
	if (actualNum == 1) { assert(start == end);	 return start; }
	else    // 如果获取到多个对象
	{
		_freeLists[index].PushRange(*(void**)start, end, actualNum-1);	// 将除了第一个对象外的其余对象挂到 ThreadCache 的 freeList 上
		return start;	// 返回 start（第一个对象）给应用程序
	}

	return nullptr;
}

// 释放对象，链表过长，回收内存到 Central Cache
void ThreadCache::ListTooLong(FreeList& list, size_t size)
{
	// 取一批内存
	void* start = nullptr, * end = nullptr;
	list.PopRange(start, end, list.GetMaxSize());

	// 取出来的还给 Central Cache
	CentralCache::GetInstance().ReleaseListToSpans(start, size);
}