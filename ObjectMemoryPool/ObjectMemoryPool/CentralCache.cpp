#include "CentralCache.h"
#include "PageCache.h"

CentralCache CentralCache::_sInst;		// 单例对象

// 从 Central Cache获取 n 个大小为 bytes 的内存对象给 thread cache，返回实际获取的数量
// start —— 输出参数，指向获取的内存块链表头；end —— 输出参数，指向获取的内存块链表尾
// n —— 期望获取的对象数量；bytes —— 每个对象的大小（字节）
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t bytes)
{
	size_t index = SizeClass::Index(bytes);		// 计算大小类索引

	_spanLists[index].GetMutex().lock();	// 加桶锁（CentralCache是共享资源）

	Span* span = GetOneSpan(_spanLists[index], bytes);	// 获取一个有空闲块的Span
	assert(span);
	assert(span->s_freeList);

	// 遍历获取到的合适的 span，获取 n个大小为 bytes 的内存空间。若没有 n 个，有多少获取多少
	start = span->s_freeList;		// start 指向第一个节点
	end = start;					// end 初始指向第一个节点
	size_t i = 0, actualNum = 1;	// 实际获取到的内存对象个数
	// end 向后走 n - 1 步，让 end 指向第 n 个节点
	while (i < n - 1 && *(void**)end != nullptr)
	{
		end = *(void**)end;		// 向后遍历
		i++;
		actualNum++;
	}

	// 更新 Span 的空闲链表头
	span->s_freeList = *(void**)end;	// 指向自由链表剩余部分
	*(void**)end = nullptr;				// 切断取出部分的链表
	span->s_usecount += actualNum;		// 分配给 thread cache 的计数增加 actualNum

	_spanLists[index].GetMutex().unlock();	// 解锁

	return actualNum;	// 从中心缓存获取到的对象的个数
}

// 从 Span 链表中获取一个有空闲块的 Span，如果没有则向 PageCache 申请新的 Span
Span* CentralCache::GetOneSpan(SpanList& list, size_t bytes)
{
	/*****  1. 调用时已经持有桶锁（FetchRangeObj 中加的锁）此时桶锁已被锁住 *****/

	
	// 查看当前的 SpanList 中是否存在还未分配对象的 span
	Span* it = list.Begin();
	while (it != list.End())
	{
		if (it->s_freeList != nullptr)	return it;
		it = it->s_next;
	}

	// 没有空闲的 Span，只能从 Page Cache 获取

	/***** 2. 查看链表，发现没有空闲 Span *****/


	/********** 3. 解锁桶锁 **********/
	// 先将 Central Cache 的桶锁解掉，这样就不会阻塞住其它线程释放内存对象
	list.GetMutex().unlock();

	/********** 4. 加 PageCache 全局锁 **********/
	PageCache::GetInstance().GetMutex().lock();		// 加上全局锁
	// 计算需要多少页（根据bytes大小决定）
	Span* span = PageCache::GetInstance().GetSpans(SizeClass::NumMovePage(bytes));
	span->s_isUse = true;	// span 被使用
	// assert(span != nullptr);	// span 不能为空
	PageCache::GetInstance().GetMutex().unlock();		// 解开全局锁


	/********** 5. 切割 Span **********/
	// 不需要再加上桶锁。什么时候需要加锁？多个线程竞争时
	// 这里并不会有多个线程竞争，对获取的 span 进行切分，其它的线程拿不到/访问不到 span，因为它没有被挂载到 span

	// 将大块内存切割成小对象
	// 计算 span 的地址，只知道它的页号，那么 span 的起始地址 = 页号 * 一页的大小（8KB）
	char* start = (char*)(span->s_pageId << PAGE_SHIFT);
	// 计算 span 大块内存的总大小
	size_t size = span->s_n << PAGE_SHIFT;
	char* end = start + size;		// 结束地址


	// 将大块内存切成自由链表连接起来 —— 采用尾插
	// 先切一块作为头节点，方便尾插
	span->s_freeList = start;
	start += bytes;
	void* tail = span->s_freeList;
	while (start < end)
	{
		*(void**)tail = start;
		tail = *(void**)tail;		// 继续向后
		start += bytes;				// 跳过每一小块内存大小
	}
	
	/********** 6. 重新加桶锁 **********/
	list.GetMutex().lock();		// 加锁（要访问桶）
	// 将新申请的 Span（已被切割成小块）挂到 CentralCache 对应的 SpanList 链表头部
	list.PushFront(span);

	return span;	// 返回获取到的空闲的 span
}

// 将一定数量的对象释放到 span
void CentralCache::ReleaseListToSpans(void* start, size_t bytes)
{
	size_t index = SizeClass::Index(bytes);

	_spanLists[index].GetMutex().lock();		// 加锁

	// 内存块对象属于哪一个页？地址 / 8KB = 页号
	while (start)
	{
		void* next = *(void**)start;

		Span* span = PageCache::GetInstance().MapObjectToSpan(start);	// start 内存对象对应的 span
		
		*(void**)start = span->s_freeList;
		span->s_freeList = start;
		span->s_usecount--;
		
		// span 的 usecount 为 0，说明 span 切分出去的所有内存都归还回来了
		if (span->s_usecount == 0)
		{
			_spanLists[index].Erase(span);
			span->s_freeList = nullptr;
			span->s_prev = span->s_next = nullptr;

			// 释放 span 给 page cache 时，使用 page cache 的锁，暂时解开桶锁
			_spanLists[index].GetMutex().unlock();

			// 保证 Central Cache 归还 span 给 Page Cache 操作是原子的
			PageCache::GetInstance().GetMutex().lock();			// 加锁
			PageCache::GetInstance().ReleaseSpanToPageCache(span);
			PageCache::GetInstance().GetMutex().unlock();		// 解锁
		}

		start = next;
	}

	_spanLists[index].GetMutex().unlock();		// 解锁
}