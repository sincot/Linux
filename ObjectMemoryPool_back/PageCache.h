#pragma once

#include "Common.h"
#include "ObjectPool.h"


// 单例模式的 PageCache 类，负责管理内存页的分配和回收
class PageCache
{
public:
	// static：可以通过类名直接调用，无需对象 —— PageCache::GetInstance()
	// 返回引用而不是指针，避免空指针问题；内联函数，无调用开销；_sInst 是静态成员变量，在类外定义
	static PageCache& GetInstance() { return _sInst; } // 获取单例对象的引用

	// 返回引用，避免拷贝 std::mutex（mutex 不可拷贝），使用方式：PageCache::GetInstance().GetMutex().lock()
	Span* GetSpans(size_t k);		// 申请一个包含 k 个连续页的 Span，请求的页数（1 ~ NPAGES-1）

	std::mutex& GetMutex() { return _mutex; }

	// obj：内存块地址（ThreadCache 释放时传入）
	// 通过地址计算页号，再从 _idSpanMap 哈希表中查找对应的 Span，用于 ReleaseListToSpans 中确定内存块属于哪个 Span
	Span* MapObjectToSpan(void* obj);	// 获取页号与 Span 的映射关系

	// 释放空闲的 span 回到 PageCache，并合并相邻的 span
	void ReleaseSpanToPageCache(Span* span);	// CentralCache 调用此接口归还完全空闲的 Span

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


	// 键：页号（起始页号）		值：指向 Span 的指针
	// 用于 MapObjectToSpan：给定任意地址，右移 PAGE_SHIFT 得到页号，然后查表找到 Span；也用于 Span 合并时判断相邻页的 Span 是否存在
	// 映射表的策略：
	// 1. Span 类型：正在使用的 Span	映射记录方式：每个页都映射	目的：CentralCache 快速查找
	// 2. Span 类型：空闲的 Span	映射记录方式：只记录首尾页	目的：PageCache 合并时查找相邻 Span
	std::unordered_map<PAGE_ID, Span*> _idSpanMap;		// 页号与 Span 的映射关系

	ObjectPool<Span> _spanPool;		// 使用定长数组优化 new

	// std::unordered_map<PAGE_ID, size_t> _idSizeMap;		// 页号与 Span的Size 的映射关系

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