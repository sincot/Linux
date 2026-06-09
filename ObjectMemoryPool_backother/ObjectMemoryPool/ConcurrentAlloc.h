#pragma once

#include "Common.h"
#include "ThreadCache.h"
#include "PageCache.h"

// 实现成全局的静态函数：
// 保持它们在当前文件内可见，避免命名冲突；同时也方便在其它文件中调用这些函数，而不需要创建对象实例
static void* ConcurrentAlloc(size_t size)		// 申请内存对象
{
	// 大对象不走 ThreadCache，直接从 PageCache 分配; 避免大对象占用 ThreadCache 资源
	if (size > MAX_BYTES)	// 大对象处理（size > 256KB）
	{
		size_t alignSize = SizeClass::RoundUp(size);	// 将大小对齐到对应的对齐数
		size_t kpage = alignSize >> PAGE_SHIFT;			// 除以 8KB，计算出需要多少页

		PageCache::GetInstance().GetMutex().lock();
		Span* span = PageCache::GetInstance().GetSpans(kpage);
		span->s_objSize = size;
		PageCache::GetInstance().GetMutex().unlock();

		void* ptr = (void*)(span->s_pageId << PAGE_SHIFT);		// 将页号转换为实际内存地址，返回给用户

		return ptr;
	}
	else
	{
		// 使用安全的 TLS 获取
		/*ThreadCache* tc = GetThreadCache();
		return tc->Allocate(size);*/

		// 使用安全的 TLS 获取
		if (pTLSThreadCache == nullptr)   // 如果当前线程还有 Thread Cache 实例，就创建一个新的 Thread Cache 实例
		{
			static ObjectPool<ThreadCache> tcPool;
			pTLSThreadCache = tcPool.New();
			// pTLSThreadCache = new ThreadCache;
		}

		return pTLSThreadCache->Allocate(size);    // 调用当前线程的 Thread Cache 实例的 Allocate 方法来申请内存对象
	}
}

static void ConcurrentDealloc(void* ptr)		// 释放内存对象
{
	assert(ptr != nullptr);

	// 通过内存地址找到对应的 Span，MapObjectToSpan 内部通过页号在哈希表中查找
	Span* span = PageCache::GetInstance().MapObjectToSpan(ptr);
	size_t size = span->s_objSize;		// 内存对象的大小
	if (size > MAX_BYTES)	// 大对象释放（size > 256KB）
	{	
		PageCache::GetInstance().GetMutex().lock();		// 加上全局锁
		PageCache::GetInstance().ReleaseSpanToPageCache(span);			// 将 span 还给 PageCache
		PageCache::GetInstance().GetMutex().unlock();	// 解锁
	}
	else
	{
		assert(pTLSThreadCache != nullptr);   // 确保当前线程有 Thread Cache 实例
		pTLSThreadCache->Deallocate(ptr, size);    // 调用当前线程的 Thread Cache 实例的 Deallocate 方法来释放内存对象
	}
}

	
