#include "ObjectPool.h"
#include "ConcurrentAlloc.h"
#include "BenchMark.h"

void Alloc1()
{
	for (size_t i = 0; i < 5; i++)
	{
		void* ptr = ConcurrentAlloc(6);
	}
}

void Alloc2()
{
	for (size_t i = 0; i < 5; i++)
	{
		void* ptr = ConcurrentAlloc(9);
	}
}

void TestObjectPool()
{
	std::thread t1(Alloc1);
	std::thread t2(Alloc2);
	
	t1.join();
	t2.join();
}

void TLSTest()
{
	std::thread t1(Alloc1);
	t1.join();

	std::thread t2(Alloc2);
	t2.join();
}

void TestConcurrentAlloc()
{
	void* p1 = ConcurrentAlloc(6);
	void* p2 = ConcurrentAlloc(8);
	void* p3 = ConcurrentAlloc(1);
	void* p4 = ConcurrentAlloc(7);
	void* p5 = ConcurrentAlloc(3);

	cout << p1 << endl;
	cout << p2 << endl;
	cout << p3 << endl;
	cout << p4 << endl;
	cout << p5 << endl;
}

void TestAddressShift()
{
	PAGE_ID id1 = 2000;
	PAGE_ID id2 = 2001;

	char* p1 = (char*)(id1 << PAGE_SHIFT);
	char* p2 = (char*)(id2 << PAGE_SHIFT);

	cout << (void*)p1 << endl;
	cout << (void*)p2 << endl;

	while (p1 < p2)
	{
		cout << (void*)p1 << " " << ((PAGE_ID)p1 << PAGE_SHIFT) << endl;
		p1 += 8;
	}
}

void TestBigAlloc()
{
	/*void* p1 = ConcurrentAlloc(257 * 1024);
	ConcurrentDealloc(p1, 257 * 1024);
	void* p2 = ConcurrentAlloc(129 * 8 * 1024);
	ConcurrentDealloc(p2, 129 * 8 * 1024);*/

	void* p1 = ConcurrentAlloc(257 * 1024);
	ConcurrentDealloc(p1);
	void* p2 = ConcurrentAlloc(129 * 8 * 1024);
	ConcurrentDealloc(p2);
}

int main()
{
	// TestObjectPool();
	// TestConcurrentAlloc();	// 单线程分配测试
	// TestAddressShift();
	// TestBigAlloc();

	// 参数说明：
	// 参数1: 每个线程每轮次申请/释放的次数
	// 参数2: 并发线程数
	// 参数3: 重复执行的轮次

	// 示例1：轻量测试（快）
	// BenchmarkMalloc(1000, 4, 10);
	// BenchmarkConcurrentMalloc(1000, 4, 10);

	// 示例2：中等压力测试
	// BenchmarkMalloc(10000, 8, 20);
	//BenchmarkConcurrentMalloc(10000, 8, 20);

	// 示例3：高压力测试（耗时较长）
	//BenchmarkMalloc(100000, 16, 10);
	//BenchmarkConcurrentMalloc(100000, 16, 10);

	// TestConcurrentAlloc();  // 单线程分配测试

	return 0;
}