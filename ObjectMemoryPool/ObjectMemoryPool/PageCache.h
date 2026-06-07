#pragma once

#include "Common.h"

// 单例模式的 PageCache 类，负责管理内存页的分配和回收
class PageCache
{
public:
	// static：可以通过类名直接调用，无需对象 —— PageCache::GetInstance()
	// 返回引用而不是指针，避免空指针问题；内联函数，无调用开销
	static PageCache& GetInstance() { return _sInst;  } // 获取单例对象的引用

	// 返回引用，避免拷贝 std::mutex（mutex 不可拷贝），使用方式：PageCache::GetInstance().GetMutex().lock()
	Span* GetSpans(size_t k);		// 申请一个包含 k 个连续页的 Span，请求的页数（1 ~ NPAGES-1）

	std::mutex& GetMutex() { return _mutex; }

	Span* MapObjectToSpan(void* obj);	// 获取页号与 Span 的映射关系

	// 释放空闲的 span 回到 PageCache，并合并相邻的 span
	void ReleaseSpanToPageCache(Span* span);

private:
	SpanList _pageLists[NPAGES];   // 每个页数对应一个链表，链表中存储了所有页数为 i 的 Span
	std::mutex _mutex;             // 全局锁，保护整个 PageCache 的访问，保证线程安全

	// 为什么 PageCache 使用全局锁？
	// PageCache 操作本身需要全局一致性：PageCache 负责管理整个内存页的分配与回收，涉及多个链表的操作，必须原子完成，否则会出现数据不一致
	// 1. 低频访问：仅 CentralCache 短缺时才访问，锁竞争少
	// 2. 操作需要全局一致：查找、合并可能跨多个桶，桶锁难以实现
	// 3. 避免死锁：跨桶操作时多锁容易死锁
	// 4. 实现简单：一个锁保护所有数据，代码简单可靠
	// 5. 性能足够：全局锁开销远小于 PageCache 操作本身
	// 桶锁无法解决跨桶操作问题：容易造成死锁
	// 设计原则：高频路径 + 独立数据 → 细粒度锁（CentralCache 桶锁）；低频路径 + 共享数据 → 粗粒度锁（PageCache 全局锁）

	std::unordered_map<PAGE_ID, Span*> _idSpanMap;		// 页号与 Span 的映射关系

private:
	// 私有化默认构造函数，外部直接创建实例，只能通过 GetInstance() 访问
	PageCache() = default;							 // 私有构造函数，禁止外部实例化
	PageCache(const PageCache&) = delete;            // 禁止拷贝构造，确保单例的唯一性
	static PageCache _sInst;       // 单例对象，static —— 属于类本身，不是某个对象
};

// 为什么最大的页是 128？最大页数限制为 128 页（128 × 8KB = 1MB）是基于多方面的权衡考虑
// 1. 避免单次分配过大，防止浪费：大块内存被切分成小块后，只要有一小块在使用，整个大 Span 就无法归还给系统；造成内存滞留，无法及时释放
// 2. 控制碎片化程度：页数越大，释放时越难找到连续的相邻空闲页；128 页是一个平衡点：既能满足大块内存需求，又不会造成严重碎片
// 3. 配合对齐策略的整数倍：最大对象 256KB 对应 32 页，而 PageCache 的 128 页限制是它的 4 倍，留有余量。