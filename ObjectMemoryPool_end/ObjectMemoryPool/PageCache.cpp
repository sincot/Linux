#include "PageCache.h"

// 必须在 .cpp 中定义，否则链接时会报"未定义引用"错误
PageCache PageCache::_sInst;    // 定义并初始化 PageCache 的静态单例实例

Span* PageCache::GetSpans(size_t k)		// 申请一个包含 k 个连续物理页的 Span
{
	// assert(k > 0 && k < NPAGES);	// 确保请求的页数在有效范围内
	assert(k > 0);		// 确保请求的页数在有效范围内

	// 如果请求的页数超过 128 页（1MB），直接向操作系统申请，不经过内存池管理
	if (k > NPAGES - 1)		// 超大对象处理（k > 128）
	{
		void* ptr = SystemAlloc(k);
		// Span* span = new Span();
		Span* span = _spanPool.New();
		span->s_pageId = (PAGE_ID)ptr >> PAGE_SHIFT;
		span->s_n = k;

		//for (PAGE_ID i = 0; i < span->s_n; i++)
		//{
		//	_idSpanMap[span->s_pageId + i] = span;  // 每一页都映射
		//}

		//// 空闲的 span，将它的首尾页号与 span 的映射关系存储起来
		//_idSpanMap[span->s_pageId] = span;
		//_idSpanMap[span->s_pageId + span->s_n - 1] = span;

		// 大内存：记录所有页映射，确保能正确回收
		for (PAGE_ID i = 0; i < span->s_n; i++)
		{
			// _idSpanMap[span->s_pageId + i] = span;
			_idSpanMap.set(span->s_pageId + i, span);
		}

		return span;
	}

	// 检查页数为 k 的桶中是否有空闲 Span；如果有，直接取出返回（头删法）
	if (!_pageLists[k].Empty())		// 第 k 页不为空
	{
		Span* kSpan = _pageLists[k].PopFront();
		// 切出 k 页的 span，建立页号与 span 的映射关系，便于 Central Cache 回收小块内存时，查找对应的 span
		for (PAGE_ID i = 0; i < kSpan->s_n; i++)
		{
			// _idSpanMap[kSpan->s_pageId + i] = kSpan;
			_idSpanMap.set(kSpan->s_pageId + i, kSpan);
		}
		kSpan->s_isUse = true;  // 标记为使用中

		return kSpan;
		// return _pageLists[k].PopFront();	// 从第 k 个桶取出 Span
	}

	// 检查后面的桶有没有 span，如果有可以将其切分
	for (size_t i = k + 1; i < NPAGES; i++)
	{
		if (!_pageLists[i].Empty())		// 找到第一个非空的桶
		{
			Span* nSpan = _pageLists[i].PopFront();		// 取出该桶的一个 Span（称为 nSpan，大小为 i 页）
			// 将 i 页切分成 k页的span 和 i-k 页的span
			// Span* kSpan = new Span;
			Span* kSpan = _spanPool.New();
			// 在 nSpan 的头部切一个 k页
			kSpan->s_pageId = nSpan->s_pageId;		// nSpan 的页号给 kSpan，页号与原 Span 相同（从头部切）
			kSpan->s_n = k;							// 页数为 k
			kSpan->s_isUse = true;  // 标记为使用中

			nSpan->s_pageId = nSpan->s_pageId + k;	// nSpan 的页号发生改变，分出去了 k页，页号向后移动 k 页
			nSpan->s_n = nSpan->s_n - k;			// nSpan 的页数量发生改变，分出了 k页，页数减去 k

			// i-k页的 span 挂载到 i-k页，nSpan 目前的页数量就是 i-k
			_pageLists[nSpan->s_n].PushFront(nSpan);

			// 存储 nSpan 的首尾页号与 span 的映射关系，便于 page cache 回收内存时，进行合并查找
			// _idSpanMap[nSpan->s_pageId] = nSpan;
			_idSpanMap.set(nSpan->s_pageId, nSpan);
			// _idSpanMap[nSpan->s_pageId + nSpan->s_n - 1] = nSpan;
			_idSpanMap.set(nSpan->s_pageId + nSpan->s_n - 1, nSpan);


			// 切出 k 页的 span，建立页号与 span 的映射关系，便于 Central Cache 回收小块内存时，查找对应的 span
			for (PAGE_ID i = 0; i < kSpan->s_n; i++)
			{
				// _idSpanMap[kSpan->s_pageId + i] = kSpan;
				_idSpanMap.set(kSpan->s_pageId + i, kSpan);
			}

			return kSpan;		// 返回切分出来的 k 页 Span
		}
	}

	// 后面的桶一个 span 都没有，向系统申请 128页的 span
	// Span* bigSpan = new Span;
	Span* bigSpan = _spanPool.New();
	void* ptr = SystemAlloc(NPAGES - 1);	// 向操作系统申请连续内存
	// 由指针转换成页号，每页的大小是固定的，那么 页号 = ptr / 每页大小
	bigSpan->s_pageId = (PAGE_ID)ptr >> PAGE_SHIFT;
	bigSpan->s_n = NPAGES - 1;		// 页数量为 128
	bigSpan->s_isUse = false;		// 标记为未使用
	// 获取到 128页的 span，依旧需要切分，将 bigSpan 插入到对应的桶位置，再使用时，调用 GetSpans 切分
	_pageLists[bigSpan->s_n].PushFront(bigSpan);	// 将新申请的大 Span 挂到对应大小的桶中

	// 记录首尾映射
	// _idSpanMap[bigSpan->s_pageId] = bigSpan;
	_idSpanMap.set(bigSpan->s_pageId, bigSpan);
	// _idSpanMap[bigSpan->s_pageId + bigSpan->s_n - 1] = bigSpan;
	_idSpanMap.set(bigSpan->s_pageId + bigSpan->s_n - 1, bigSpan);

	// 递归调用自己，重新开始查找。因为大 Span 已经放入桶中，再次调用会从第一步开始，最终会找到或切分出合适的 Span
	return GetSpans(k);
}

Span* PageCache::MapObjectToSpan(void* obj)		// 获取页号与 Span 的映射关系
{
	assert(obj != nullptr);
	PAGE_ID pageId = (PAGE_ID)obj >> PAGE_SHIFT;	// 地址右移 PAGE_SHIFT（13 位）得到页号

	//// std::unique_lock<std::mutex> lock(PageCache::GetInstance().GetMutex());		// 使用 raii 锁
	//// 必须加锁保护，因为 _idSpanMap 可能被其他线程修改
	//std::unique_lock<std::mutex> lock(_mutex);		// 使用 raii 锁
	//
	//auto ret = _idSpanMap.find(pageId);				// 在 idSpanMap 中查找 pageId 对应的 span
	//if (ret != _idSpanMap.end()) { return ret->second;  }	// 返回 span
	//else
	//{ 
	//	assert(false);	
	//	return nullptr; 
	//}		// 不可能找不到		断言确保一定能找到（因为每个页都被映射过）

	auto ret = (Span*)_idSpanMap.get(pageId);
	/*if (ret == nullptr) {
		printf("ERROR: MapObjectToSpan failed! obj=%p, pageId=%llu\n",
			obj, (unsigned long long)pageId);
		assert(false && "MapObjectToSpan: span not found!");
	}*/
	assert(ret != nullptr);

	return ret;
}

// 释放一个完全空闲的 Span 回 PageCache，并尝试与相邻的空闲 Span 合并，减少内存碎片
void PageCache::ReleaseSpanToPageCache(Span* span)
{
	assert(span != nullptr);
	// 清理该 Span 的所有页映射（避免脏数据）
	for (PAGE_ID i = 0; i < span->s_n; i++)
	{
		_idSpanMap.erase(span->s_pageId + i);
	}

	// 如果 Span 超过 128 页（直接向系统申请的），直接归还给操作系统
	if (span->s_n > NPAGES - 1)
	{
		void* ptr = (void*)(span->s_pageId << PAGE_SHIFT);
		// 直接将其释放掉，调用 SystemFree
		SystemFree(ptr, span->s_n << PAGE_SHIFT);
		// delete span;
		_spanPool.Delete(span);

		return;
	}

	// 对 span 前后的页，进行合并，缓解内存碎片问题

	while (1)		// 合并前面的页
	{
		PAGE_ID prevId = span->s_pageId - 1;	// 前一个页的页号
		//auto pret = _idSpanMap.find(prevId);
		//if (pret == _idSpanMap.end())	break;
		//Span* prevSpan = pret->second;

		auto pret = (Span*)_idSpanMap.get(prevId);
		if (pret == nullptr)	break;
		Span* prevSpan = pret;
		if (prevSpan->s_isUse == true)	break;	// 前一个 Span 正在使用，停止合并
		if (prevSpan->s_n + span->s_n > NPAGES - 1)	break;	// 合并出超过 128 页的 span，没办法管理，也不能继续合并

		// 清理 prevSpan 的映射
		_idSpanMap.erase(prevSpan->s_pageId);
		_idSpanMap.erase(prevSpan->s_pageId + prevSpan->s_n - 1);

		// 更新当前 Span 的起始页号和页数
		span->s_pageId = prevSpan->s_pageId;	// 更新 span 的起始页号
		span->s_n += prevSpan->s_n;				// 合并后的页数量

		// 从桶中删除前一个 Span 并释放内存
		_pageLists[prevSpan->s_n].Erase(prevSpan);		// 从桶中删除
		// delete prevSpan;
		_spanPool.Delete(prevSpan);
	}

	while (1)		// 合并后面的页
	{
		PAGE_ID nextId = span->s_pageId + span->s_n;
		/*auto nret = _idSpanMap.find(nextId);
		if (nret == _idSpanMap.end())	break;*/
		//Span* nextSpan = nret->second;

		auto nret = (Span*)_idSpanMap.get(nextId);
		if (nret == nullptr)	break;
		Span* nextSpan = nret;
		if (nextSpan->s_isUse == true)	break;
		if (nextSpan->s_n + span->s_n > NPAGES - 1)	break;

		// 清理 nextSpan 的映射
		_idSpanMap.erase(nextSpan->s_pageId);
		_idSpanMap.erase(nextSpan->s_pageId + nextSpan->s_n - 1);

		span->s_n += nextSpan->s_n;			// 合并后的页数量

		_pageLists[nextSpan->s_n].Erase(nextSpan);		// 从桶中删除
		// delete nextSpan;
		_spanPool.Delete(nextSpan);
	}

	// 将最终合并完成的 Span 放入对应大小的桶中，标记为未使用
	_pageLists[span->s_n].PushFront(span);
	span->s_isUse = false;		// 没被使用
	// 合并后 Span 的起始页号和结束页号都映射到自身，便于后续再次合并时查找
	// _idSpanMap[span->s_pageId] = span, _idSpanMap[span->s_pageId + span->s_n - 1] = span;
	_idSpanMap.set(span->s_pageId, span);
	_idSpanMap.set(span->s_pageId + span->s_n - 1, span);
}


// 1. 只有在 ReleaseSpabToPageCache 和 GetSpans 函数中，回建立 pageid 和 span 的映射，也就是说会去写
// 2. 基数树在写之前就会提前开好空间，写数据过程中，不会动结构
// 3. 读写是分离的。线程1对一个位置读写的时候，线程2不可能对这个位置读写（因为先前加了全局锁）