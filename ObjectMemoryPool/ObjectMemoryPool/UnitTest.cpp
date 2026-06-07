#include "ObjectPool.h"
#include "ConcurrentAlloc.h"


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

int main()
{
	// TestObjectPool();
	// TestConcurrentAlloc();
	TestAddressShift();

	return 0;
}