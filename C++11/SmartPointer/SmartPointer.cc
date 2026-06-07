#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
// #include <memory>
#include <functional>

using namespace std;

//double Divide(int a, int b)
//{
//	// 当 b == 0时抛出异常
//	if (b == 0)
//	{
//		throw "Divide by zero condition!";
//	}
//	else
//	{
//		return (double)a / (double)b;
//	}
//}
//
//void Func()
//{
//	int* array1 = new int[10];
//	int* array2 = new int[10];   // 抛异常呢
//
//	try
//	{
//		int len, time;
//		cin >> len >> time;
//		cout << Divide(len, time) << endl;
//	}
//	catch (...)
//	{
//		cout << "delete []" << array1 << endl;
//		cout << "delete []" << array2 << endl;
//		delete[] array1;
//		delete[] array2;
//		throw; // 异常重新抛出，捕获到什么抛出什么
//	}
//	cout << "delete []" << array1 << endl;
//	delete[] array1;
//	cout << "delete []" << array2 << endl;
//	delete[] array2;
//}
//
//int main()
//{
//	try
//	{
//		Func();
//	}
//	catch (const char* errmsg)
//	{
//		cout << errmsg << endl;
//	}
//	catch (const exception& e)
//	{
//		cout << e.what() << endl;
//	}
//	catch (...)
//	{
//		cout << "未知异常" << endl;
//	}
//
//	return 0;
//}


//template <typename T>
//class SmartPointer
//{
//public:
//	SmartPointer(T* ptr) : m_ptr(ptr) {}
//	~SmartPointer()
//	{ 
//		cout << "delete [] " << m_ptr << endl;
//		delete[] m_ptr; 
//	}
//
//	T& operator*() { return *m_ptr; }
//	T* operator->() { return m_ptr; }
//	T& operator[](size_t index) { return m_ptr[index]; }
//
//private:
//	T* m_ptr;
//};

//double Divide(int a, int b)
//{
//	// 当 b == 0时抛出异常
//	if (b == 0) { throw "Divide by zero condition!"; }
//	else { return (double)a / (double)b; }
//}
//
//void Func()
//{
//	//int* array1 = new int[10];
//	//int* array2 = new int[10];   // 抛异常呢
//	SmartPointer<int> array1(new int[10]);
//	SmartPointer<int> array2(new int[10]);
//
//	try
//	{
//		int len, time;
//		cin >> len >> time;
//		cout << Divide(len, time) << endl;
//	}
//	catch (...)
//	{
//		throw; // 异常重新抛出，捕获到什么抛出什么
//	}
//}
//
//int main()
//{
//	try
//	{
//		Func();
//	}
//	catch (const char* errmsg)
//	{
//		cout << errmsg << endl;
//	}
//	catch (const exception& e)
//	{
//		cout << e.what() << endl;
//	}
//	catch (...)
//	{
//		cout << "未知异常" << endl;
//	}
//
//	return 0;
//}


//int main()
//{
//	//int* p = new int[10];
//	//SmartPointer<int> sp(p);	// 将 p 传递给智能指针，智能指针将负责释放内存
//
//	SmartPointer<int> sp = new int[10];	// 将 p 传递给智能指针，智能指针将负责释放内存
//
//	return 0;
//}

//int main()
//{
//	SmartPointer<int> sp1 = new int[10];
//	SmartPointer<int> sp2 = new int[10];
//	SmartPointer<int> sp3(sp2);	// 错误：智能指针不允许复制构造
//
//	return 0;
//}

struct Date
{
	int _year;
	int _month;
	int _day;

	Date(int year = 1, int month = 1, int day = 1)
		:_year(year)
		, _month(month)
		, _day(day)
	{}

	~Date() { cout << "~Date()" << endl; }
};



//int main()
//{
//	//auto_ptr<Date> ap1(new Date);
//	//auto_ptr<Date> ap2(ap1);	// ap1 的所有权转移给 ap2，ap1 不再拥有 Date 对象
//	//ap1->_year = 2024;		// 错误：ap1 不再拥有 Date 对象，访问 ap1 是未定义行为
//	//ap2->_year = 2025;		// 正确：ap2 拥有 Date 对象，可以访问和修改它
//
//	//unique_ptr<Date> up1(new Date);
//	//// unique_ptr<Date> up2(up1);			// 错误：unique_ptr 不允许拷贝
//	//// unique_ptr<Date> up2 = up1;			// 错误：unique_ptr 不允许拷贝
//	//unique_ptr<Date> up3 = move(up1);	// 将 up1 的所有权转移给 up3，up1 不再拥有 Date 对象
//	////up1->_year = 2024;				// 错误：up1 不再拥有 Date 对象，访问 up1 是未定义行为
//	//up3->_year = 2025;					// 正确：up3 拥有 Date 对象，可以访问和修改它
//
//	//shared_ptr<Date> sp1(new Date);
//	//shared_ptr<Date> sp2(sp1);			// sp1 和 sp2 共享 Date 对象的所有权
//	//shared_ptr<Date> sp3 = sp1;			// sp1 和 sp3 也共享 Date 对象的所有权
//	//sp1->_year = 2024;					// 正确：sp1 拥有 Date 对象，可以访问和修改它
//	//sp2->_year = 2025;					// 正确：sp2 也拥有 Date 对象，可以访问和修改它
//	//sp3->_year = 2026;					// 正确：sp3 也拥有 Date 对象，可以访问和修改它
//	//shared_ptr<Date> sp4 = move(sp1);	// 将 sp1 的所有权转移给 sp4，sp1 不再拥有 Date 对象
//	//cout << "sp1.use_count() = " << sp1.use_count() << endl;	// 输出 0，sp1 不再拥有 Date 对象
//	//cout << "sp2.use_count() = " << sp2.use_count() << endl;	// 输出 3，sp2 仍然拥有 Date 对象
//	//cout << "sp3.use_count() = " << sp3.use_count() << endl;	// 输出 3，sp3 仍然拥有 Date 对象
//	//cout << "sp4.use_count() = " << sp4.use_count() << endl;	// 输出 3，sp4 现在拥有 Date 对象
//
//	//shared_ptr<Date> sp1(new Date);
//	//shared_ptr<Date> sp2(sp1);			// sp1 和 sp2 共享 Date 对象的所有权
//	//shared_ptr<Date> sp3 = sp1;			// sp1 和 sp3 也共享 Date 对象的所有权
//	//if (sp1)	// 检查 sp1 是否为空
//	//{
//	//	cout << "sp1.use_count() = " << sp1.use_count() << endl;
//	//	sp1.reset();	// sp1 释放对 Date 对象的所有权，sp2 和 sp3 仍然拥有 Date 对象
//	//	cout << "sp1.use_count() = " << sp1.use_count() << endl;
//	//}
//	//if (sp2)	// 检查 sp2 是否为空
//	//{
//	//	cout << "sp2.use_count() = " << sp2.use_count() << endl;
//	//	sp2.reset();	// sp2 释放对 Date 对象的所有权，sp3 仍然拥有 Date 对象
//	//	cout << "sp2.use_count() = " << sp2.use_count() << endl;
//	//}
//	//if (sp3)	// 检查 sp3 是否为空
//	//{
//	//	cout << "sp3.use_count() = " << sp3.use_count() << endl;
//	//	sp3.reset();	// sp3 释放对 Date 对象的所有权，Date 对象被销毁
//	//	cout << "sp3.use_count() = " << sp3.use_count() << endl;
//	//}
//
//	//shared_ptr<Date> sp1(new Date);
//	//shared_ptr<Date> sp2(sp1);			// sp1 和 sp2 共享 Date 对象的所有权
//	//shared_ptr<Date> sp3 = sp1;			// sp1 和 sp3 也共享 Date 对象的所有权
//	//cout << sp1.get() << endl;	// 获取 sp1 底层的指针，输出 Date 对象的地址
//	//cout << sp2.get() << endl;	// 获取 sp2 底层的指针，输出 Date 对象的地址，与 sp1 相同
//	//cout << sp3.get() << endl;	// 获取 sp3 底层的指针，输出 Date 对象的地址，与 sp1 相同
//
//	//int* p = new int(10);
//	//shared_ptr<int> a(new int(20));
//	//shared_ptr<int> b(a, p);	// b 共享 a 的所有权，但管理一个不同的指针 p
//
//	//cout << p << endl;	// 输出 p 的地址
//	//cout << a.get() << endl;	// 输出 a 底层的指针，输出 p 的地址
//	//cout << b.get() << endl;	// 输出 b 底层的指针，输出 p 的地址
//
//	/*cout << "a.use_count() = " << a.use_count() << endl;	
//	cout << "b.use_count() = " << b.use_count() << endl;	*/
//
//	//unique_ptr<Date> up1(new Date[10]);		// 会报错
//	//shared_ptr<Date> sp1(new Date[10]);		// 会报错
//
//	//unique_ptr<Date[]> up1(new Date[3]);		// 不会报错
//	//shared_ptr<Date[]> sp1(new Date[3]);		// 不会报错
//
//	return 0;
//}

template <typename T>
class DeleteArray
{
public:
	void operator()(T* p) const
	{
		cout << "delete [] " << p << endl;
		delete[] p;
	}
};

//int main()
//{
//	//unique_ptr<Date> up1(new Date[10]);		// 会报错
//	//shared_ptr<Date> sp1(new Date[10]);		// 会报错
//
//	//// 使用自定义删除器来正确删除数组
//	//shared_ptr<Date> sp2(new Date[10], DeleteArray<Date>());	// 使用 DeleteArray 作为自定义删除器
//	//shared_ptr<Date> sp3(new Date[10], [](Date* p) {
//	//	cout << "delete [] " << p << endl;
//	//	delete[] p;
//	//	});		// 使用 lambda 表达式作为自定义删除器
//
//	//unique_ptr<Date, DeleteArray<Date>> up2(new Date[10]);	// unique_ptr 需要指定删除器类型
//	//unique_ptr<Date, void(*)(Date*)> up3(new Date[10], [](Date* p) {
//	//	cout << "delete [] " << p << endl;
//	//	delete[] p;
//	//	});		// unique_ptr 需要指定删除器类型
//
//
//	return 0;
//}

template<class T>
class auto_ptr
{
public:
	auto_ptr(T* ptr = nullptr) : m_ptr(ptr) {}		// 构造函数，接受一个原始指针
	~auto_ptr() { delete m_ptr; }					// 析构函数，释放资源
	auto_ptr(auto_ptr& other) : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }	// 拷贝构造函数，转移所有权
	auto_ptr& operator=(auto_ptr& other)			// 赋值运算符，转移所有权
	{
		if (this != &other)
		{
			if(m_ptr) delete m_ptr;	// 释放当前对象拥有的资源
			m_ptr = other.m_ptr;	// 转移所有权
			other.m_ptr = nullptr;	// 将其他对象的指针置空，防止重复删除
		}
		return *this;
	}
	T& operator*() { return *m_ptr; }
	T* operator->() { return m_ptr; }

private:
	T* m_ptr;
};

template <class T>
class unique_ptr
{
public:
	unique_ptr(T* ptr = nullptr) : m_ptr(ptr) {}		// 构造函数，接受一个原始指针
	~unique_ptr() { delete m_ptr; }						// 析构函数，释放资源
	unique_ptr(const unique_ptr&) = delete;				// 禁止拷贝构造
	unique_ptr& operator=(const unique_ptr&) = delete;	// 禁止赋值运算符
	// 移动构造函数，转移所有权
	unique_ptr(unique_ptr&& other) noexcept : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }
	unique_ptr& operator=(unique_ptr&& other) noexcept	// 移动赋值运算符，转移所有权
	{
		if (this != &other)
		{
			if (m_ptr) delete m_ptr;	// 释放当前对象拥有的资源,前提是当前对象必须拥有资源
			m_ptr = other.m_ptr;		// 转移所有权
			other.m_ptr = nullptr;		// 将其他对象的指针置空，防止重复删除
		}
		return *this;
	}
	T& operator*() { return *m_ptr; }
	T* operator->() { return m_ptr; }

private:
	T* m_ptr;
};

// 普通版本的 shared_ptr 智能指针
//namespace AY
//{
//	template <class T>
//	class shared_ptr
//	{
//	public:
//		// 构造函数，接受一个原始指针，并初始化引用计数为 1
//		shared_ptr(T* ptr = nullptr) : m_ptr(ptr), m_count(new int(1)) {}
//
//		void release()	// 释放资源，减少引用计数，如果引用计数为 0 则删除资源
//		{
//			if (--(*m_count) == 0)	// 采用前置--，先递减引用计数，再检查是否为 0
//			{
//				cout << "delete " << m_ptr << endl;
//				delete m_ptr;	// 删除资源，前提是 new 出来的对象
//				delete m_count;	// 删除引用计数，前提是 new 出来的对象
//			}
//		}
//
//		// 析构函数，释放资源，如果引用计数为 0 则删除资源
//		~shared_ptr()
//		{
//			release();	// 调用 release 函数来释放资源
//		}
//		// 拷贝构造函数，增加引用计数
//		shared_ptr(const shared_ptr<T>& other) : m_ptr(other.m_ptr), m_count(other.m_count)
//		{
//			++(*m_count);	// 增加引用计数
//		}
//		// 赋值运算符，先释放当前资源，再增加引用计数
//		shared_ptr<T>& operator=(const shared_ptr<T>& other)
//		{
//			// 自赋值检查：sp1 = sp1, sp1 = sp2
//			if (m_ptr != other.m_ptr)			// 检查自赋值
//			{
//				release();				// 判断是否需要释放当前资源
//				m_ptr = other.m_ptr;	// 复制指针和引用计数
//				m_count = other.m_count;
//				++(*m_count);			// 增加引用计数
//			}
//			return *this;
//		}
//
//		T& operator*() { return *m_ptr; }
//		T* operator->() { return m_ptr; }
//		T& operator[](size_t index) { return m_ptr[index]; }
//		int use_count() const { return *m_count; }	// 获取引用计数
//
//	private:
//		T* m_ptr;		// 原始指针，指向共享的资源
//		int* m_count;	// 引用计数，记录有多少个 shared_ptr 实例共享同一个资源
//	};
//}


namespace AY
{
	//template <class T>
	//class shared_ptr
	//{
	//public:
	//	// 构造函数，接受一个原始指针，并初始化引用计数为 1
	//	shared_ptr(T* ptr = nullptr) : m_ptr(ptr), m_count(new int(1)) {}

	//	// 定制删除的构造函数，使用一个成员来保存删除器
	//	template <class D>
	//	shared_ptr(T* ptr, D del) : m_ptr(ptr), m_count(new int(1)), m_del(del) {}


	//	void release()	// 释放资源，减少引用计数，如果引用计数为 0 则删除资源
	//	{
	//		if (--(*m_count) == 0)	// 采用前置--，先递减引用计数，再检查是否为 0
	//		{
	//			cout << "delete " << m_ptr << endl;
	//			m_del(m_ptr);		// 释放资源时，使用删除器去释放，调用operator()
	//			delete m_count;
	//		}
	//	}

	//	// 析构函数，释放资源，如果引用计数为 0 则删除资源
	//	~shared_ptr()
	//	{
	//		release();	// 调用 release 函数来释放资源
	//	}
	//	// 拷贝构造函数，增加引用计数
	//	shared_ptr(const shared_ptr<T>& other)
	//		: m_ptr(other.m_ptr), m_count(other.m_count), m_del(other.m_del)
	//	{
	//		++(*m_count);	// 增加引用计数
	//	}
	//	// 赋值运算符，先释放当前资源，再增加引用计数
	//	shared_ptr<T>& operator=(const shared_ptr<T>& other)
	//	{
	//		// 自赋值检查：sp1 = sp1, sp1 = sp2
	//		if (m_ptr != other.m_ptr)			// 检查自赋值
	//		{
	//			release();				// 判断是否需要释放当前资源
	//			m_ptr = other.m_ptr;	// 复制指针和引用计数
	//			m_count = other.m_count;
	//			++(*m_count);			// 增加引用计数
	//			m_del = other.m_del;	// 复制删除器
	//		}
	//		return *this;
	//	}

	//	T& operator*() { return *m_ptr; }
	//	T* operator->() { return m_ptr; }
	//	T& operator[](size_t index) { return m_ptr[index]; }
	//	int use_count() const { return *m_count; }	// 获取引用计数

	//private:
	//	T* m_ptr;		// 原始指针，指向共享的资源
	//	int* m_count;	// 引用计数，记录有多少个 shared_ptr 实例共享同一个资源
	//	// 为了能够在这里也使用上模板参数 D，可以定义在类模板中
	//	// 或者直接使用 std::function<void(T*)> 来保存删除器，这样就不需要模板参数 D 了
	//	// 默认删除器，使用 lambda 表达式来定义一个默认的删除器，调用 delete 来释放资源
	//	function<void(T*)> m_del = [](T* ptr) { delete ptr; };
	//};

	template <class T>
	class shared_ptr
	{
	public:
		// 构造函数，接受一个原始指针，并初始化引用计数为 1
		shared_ptr(T* ptr = nullptr) : m_ptr(ptr), m_count(new atomic<int>(1)) {}

		// 定制删除的构造函数，使用一个成员来保存删除器
		template <class D>
		shared_ptr(T* ptr, D del) : m_ptr(ptr), m_count(new atomic<int>(1)), m_del(del) {}


		void release()	// 释放资源，减少引用计数，如果引用计数为 0 则删除资源
		{
			if (--(*m_count) == 0)	// 采用前置--，先递减引用计数，再检查是否为 0
			{
				cout << "delete " << m_ptr << endl;
				m_del(m_ptr);		// 释放资源时，使用删除器去释放，调用operator()
				delete m_count;
			}
		}

		// 析构函数，释放资源，如果引用计数为 0 则删除资源
		~shared_ptr()
		{
			release();	// 调用 release 函数来释放资源
		}
		// 拷贝构造函数，增加引用计数
		shared_ptr(const shared_ptr<T>& other)
			: m_ptr(other.m_ptr), m_count(other.m_count), m_del(other.m_del)
		{
			++(*m_count);	// 增加引用计数
		}
		// 赋值运算符，先释放当前资源，再增加引用计数
		shared_ptr<T>& operator=(const shared_ptr<T>& other)
		{
			// 自赋值检查：sp1 = sp1, sp1 = sp2
			if (m_ptr != other.m_ptr)			// 检查自赋值
			{
				release();				// 判断是否需要释放当前资源
				m_ptr = other.m_ptr;	// 复制指针和引用计数
				m_count = other.m_count;
				++(*m_count);			// 增加引用计数
				m_del = other.m_del;	// 复制删除器
			}
			return *this;
		}

		T& operator*() { return *m_ptr; }
		T* operator->() { return m_ptr; }
		T& operator[](size_t index) { return m_ptr[index]; }
		int use_count() const { return *m_count; }	// 获取引用计数

	private:
		T* m_ptr;			// 原始指针，指向共享的资源
		atomic<int*> m_count;	// 引用计数，记录有多少个 shared_ptr 实例共享同一个资源
		// 为了能够在这里也使用上模板参数 D，可以定义在类模板中
		// 或者直接使用 std::function<void(T*)> 来保存删除器，这样就不需要模板参数 D 了
		// 默认删除器，使用 lambda 表达式来定义一个默认的删除器，调用 delete 来释放资源
		function<void(T*)> m_del = [](T* ptr) { delete ptr; };
	};


	template <class T>
	class weak_ptr
	{	
	public:
		weak_ptr() : m_ptr(nullptr) {}
		weak_ptr(const shared_ptr<T>& sp) : m_ptr(sp.m_ptr) {}
		weak_ptr<T>& operator=(const shared_ptr<T>& sp)
		{
			m_ptr = sp.m_ptr;
			return *this;
		}

	private:
		T* m_ptr;		// 原始指针，指向共享的资源
	};
}

//int main()
//{
//	//shared_ptr<Date> sp1(new Date(2026, 4, 17));
//	//// 使用 make_shared 创建 shared_ptr，效率更高，代码更简洁
//	//shared_ptr<Date> sp2 = make_shared<Date>(2026, 4, 17);	
//	//auto sp3 = make_shared<Date>(2026, 4, 17);	// 使用 auto 自动推导类型，代码更简洁
//
//	//// shared_ptr<Date> sp1 = new Date;	// 不支持隐式转换，编译错误：无法将 Date* 转换为 shared_ptr<Date>
//	//shared_ptr<Date> sp2(new Date); 
//
//	return 0;
//}

//int main()
//{
//	//AY::shared_ptr<Date> sp1(new Date(2026, 4, 17));
//	//AY::shared_ptr<Date> sp2(sp1);					// sp1 和 sp2 共享 Date 对象的所有权
//	//cout << "sp1.use_count() = " << sp1.use_count() << endl;	// 输出 2，sp1 和 sp2 共享 Date 对象的所有权
//	//cout << "sp2.use_count() = " << sp2.use_count() << endl;	// 输出 2，sp1 和 sp2 共享 Date 对象的所有权
//
//	//AY::shared_ptr<Date> sp3(new Date);				// sp3 拥有一个新的 Date 对象，与 sp1 和 sp2 不共享
//	//sp1 = sp3;		// sp1 释放对原来 Date 对象的所有权，增加对 sp3 的 Date 对象的所有权，sp2 仍然拥有原来 Date 对象的所有权
//	//cout << "sp1.use_count() = " << sp1.use_count() << endl;	// 输出 2，sp1 和 sp3 共享 Date 对象的所有权	
//	//cout << "sp2.use_count() = " << sp2.use_count() << endl;	// 输出 1，sp2 仍然拥有原来 Date 对象的所有权
//	//cout << "sp3.use_count() = " << sp3.use_count() << endl;	// 输出 2，sp1 和 sp3 共享 Date 对象的所有权
//
//	AY::shared_ptr<Date> sp(new Date[3], [](Date* ptr)
//		{	
//			cout << "delete [] " << ptr << endl;
//			delete[] ptr;
//		});
//		
//	return 0;
//}


/**************************** 循环引用场景 **************************/
#include <memory>

//struct ListNode
//{
//	int _data;
//	std::shared_ptr<ListNode> _next;
//	std::shared_ptr<ListNode> _prev;
//
//	ListNode(int data = 0) :_data(data) , _next(nullptr) , _prev(nullptr) {}
//	~ListNode() { cout << "~ListNode()" << endl; }
//};


struct ListNode
{
	int _data;
	/*std::weak_ptr<ListNode> _next;
	std::weak_ptr<ListNode> _prev;*/

	AY::weak_ptr<ListNode> _next;
	AY::weak_ptr<ListNode> _prev; 

	ListNode(int data = 0) :_data(data) {}
	~ListNode() { cout << "~ListNode()" << endl; }
};

//int main()
//{
//	/*ListNode* node1 = new ListNode{ 1, nullptr, nullptr };
//	ListNode* node2 = new ListNode{ 2, nullptr, nullptr };*/
//
//	// 既然如此，我们可以使用 shared_ptr 来管理 ListNode 对象的生命周期，避免内存泄漏
//	/*std::shared_ptr<ListNode> node1(new ListNode{ 1 });
//	std::shared_ptr<ListNode> node2(new ListNode{ 2 });*/
//
//	AY::shared_ptr<ListNode> node1(new ListNode{ 1 });
//	AY::shared_ptr<ListNode> node2(new ListNode{ 2 });
//
//	node1->_next = node2;	// node1 持有 node2 的原始指针
//	node2->_prev = node1;	// node2 持有 node1 的原始指针
//	cout << "node1.use_count() = " << node1.use_count() << endl;	// 输出1，node1 只有一个 shared_ptr 实例拥有它
//	cout << "node2.use_count() = " << node2.use_count() << endl;	// 输出1，node2 只有一个 shared_ptr 实例拥有它
//
//	return 0;
//}

int main()
{
	std::shared_ptr<string> sp1(new string("string"));
	std::shared_ptr<string> sp2(sp1);	// sp1 和 sp2 共享 string 对象的所有权
	std::weak_ptr<string> wp = sp1;	// wp1 是 sp1 的弱引用，不增加 string 对象的引用计数

	//cout << wp.use_count() << endl;	// 输出 2，wp1 不增加 string 对象的引用计数
	//cout << wp.expired() << endl;		// 输出 0，wp1 没有过期，仍然可以访问 string 对象

	// sp1 释放对原来 string 对象的所有权，增加对新 string 对象的所有权，sp2 仍然拥有原来 string 对象的所有权
	sp1 = make_shared<string>("new string");	
	//cout << wp.use_count() << endl;		// 输出 1，wp1 不增加 string 对象的引用计数
	//cout << wp.expired() << endl;		// 输出 0，wp1 没有过期，仍然可以访问原来 string 对象

	// sp2 释放对原来 string 对象的所有权，增加对新 string 对象的所有权
	sp2 = make_shared<string>("another string");	
	//cout << wp.use_count() << endl;		// 输出 0，wp1 不增加 string 对象的引用计数
	//cout << wp.expired() << endl;		// 输出 1，wp1 已经过期，无法访问原来 string 对象

	wp = sp1;
	// 尝试从 wp 获取一个 shared_ptr，如果 wp 没有过期，则 sp3 将拥有 string 对象的所有权，否则 sp3 将是一个空指针
	// 要访问资源，一定是 lock 出一个新的的 share_ptr 对象
	auto sp3 = wp.lock();
	cout << wp.expired() << endl;
	cout << wp.use_count() << endl;

	*sp3 += " string";
	cout << sp3 << endl;

	return 0;
}