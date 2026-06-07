#pragma once

#include "Common.h"
#include "ThreadCache.h"

// 实现成全局的静态函数：
// 保持它们在当前文件内可见，避免命名冲突；同时也方便在其它文件中调用这些函数，而不需要创建对象实例
static void* ConcurrentAlloc(size_t size)		// 申请内存对象
{
	if (pTLSThreadCache == nullptr)   // 如果当前线程还有 Thread Cache 实例，就创建一个新的 Thread Cache 实例
	{
		pTLSThreadCache = new ThreadCache;
	}

	return pTLSThreadCache->Allocate(size);    // 调用当前线程的 Thread Cache 实例的 Allocate 方法来申请内存对象
}

static void ConcurrentDealloc(void* ptr, size_t size)		// 释放内存对象
{
	assert(pTLSThreadCache != nullptr);   // 确保当前线程有 Thread Cache 实例

	pTLSThreadCache->Deallocate(ptr, size);    // 调用当前线程的 Thread Cache 实例的 Deallocate 方法来释放内存对象
}
