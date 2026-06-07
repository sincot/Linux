#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <cstdio>
using namespace std;

//class Data
//{
//public:
//	//日期类的构造函数
//	Data(int year = 1, int month = 1, int day = 1)
//	{
//		m_year = year;
//		m_month = month;
//		m_day = day;
//		cout << "Data(int year = 1, int month = 1, int day = 1)" << endl;
//	}
//
//	void DataPrint()
//	{
//		std::cout << m_year << "/" << m_month
//			<< "/" << m_day << std::endl;
//	}
//
//	Data()
//	{
//		cout << "Data()" << endl;
//	}
//
//private:
//	int m_year;
//	int m_month;
//	int m_day;
//};


//int main()
//{
//	/* 列表初始化 */
//	//// 内置类型
//	//int num1 = { 10 };
//	//double d1 = { 3.14 };
//	//// 还可以省略 = 
//	//int num2{ 10 };
//	//double d2{ 3.14 };
//
//	//// 自定义类型
//	//Data data1 = { 2026, 4, 12 };
//	//Data data2{ 2026, 4, 12 };
//	
//	//// 多参数的类型转换
//	//const Data& data1 = { 2026, 4, 12 };
//	//const Data& data2{ 2026, 4, 12 };
//
//	//// 单参数的类型转换
//	//Data data3 = { 2026 };
//	//Data data4 = 2026;
//	//Data data5{ 2026 };
//
//	//// 最好用的一点，直接使用隐式类型转换
//	//vector<Data> v;
//	//v.push_back({ 2026,4,12 });
//	//v.push_back({ 2026, 4,12 });
//
//	//// 这是 initializer_list 
//	//vector<int> v = { 1,2,3,4,5 };
//	//// 这是 { } 初始化
//	//Data data = { 2025,4,12 };
//
//	/*auto il = { 1,2,3,4,5,6,7,8,9 };
//	cout << sizeof(il) << endl;
//	cout << typeid(il).name() << endl;*/
//
//	//// pair对象的初始化和map的initializer_list构造结合在一起使用
//	//map<string, string> dict = { {"map", "地图"}, {"string", "字符串"} };
//
//	return 0;
//}


//int main()
//{
//	// 左值：可以取地址
//	/*int* p = new int(10);
//	int num = 1;
//	const int n = num;
//	*p = 10;
//	string s("string");
//	s[0] = 'x';
//
//	int* &r1 = p;
//	int& r2 = num;
//	const int& r3 = n;
//	int& r4 = *p;
//	string& r5 = s;
//	char& r6 = s[0];*/
//
//	/*cout << &p << endl;
//	cout << &num << endl;
//	cout << &n << endl;
//	cout << &*p << endl;
//	cout << &s << endl;
//	cout << (void*)&s[0] << endl;*/
//
//	//int x = 10, y = 20;
//	//// 右值：不可以取地址
//	//int&& rr1 = 10;
//	//int&& rr2 = x + y;
//	//string&& rr3 = string("string");
//
//	/*cout << &(x + y) << endl;
//	cout << &10 << endl;
//	cout << &string("string") << endl;*/
//	
//	//int x = 10, y = 20;
//	//// 左值引用不能直接引用右值
//	//int& r1 = 10;
//	//int& r3 = x + y;
//	//// const左值引用可以引用右值
//	//const int& r2 = 10;
//	//const int& r4 = x + y;
//
//
//	//int* p = new int(10);
//	//string s("string");
//	//// 右值引用不能直接引用左值
//	//int*&& rr1 = p;
//	//string&& rr2 = s;
//	//// 右值引用可以引用move(左值)
//	//int*&& rr3 = move(p);
//	//string&& r3 = move(s);
//
//	return 0;
//}

//void Func(int& x)
//{
//	cout << "左值引用：void Func(int& x)" << endl;
//}
//
//void Func(const int& x)
//{
//	cout << "const 左值引用：void Func(const int& x)" << endl;
//}
//
//void Func(int&& x)
//{
//	cout << "右值引用：void Func(int&& x)" << endl;
//}
//
//int main()
//{
//	int num1 = 1;
//	Func(num1);			// 调用 Func(int& x)
//	Func(move(num1));	// 调用 Func(int&& x)
//	const int num2 = 1;
//	Func(num2);			// 调用 Func(const int& x)
//	int&& num3 = 1;
//	Func(num3);			// 调用 Func(int& x)
//	Func(move(num3));	// 调用 Func(int&& x)
//
//	return 0;
//}

namespace AY
{
	class string
	{
	public:
		typedef char* iterator;
		typedef const char* const_iterator;

		iterator begin()
		{
			return _str;
		}

		iterator end()
		{
			return _str + _size;
		}

		const_iterator begin() const
		{
			return _str;
		}

		const_iterator end() const
		{
			return _str + _size;
		}

		string(const char* str = "")
			:_size(strlen(str))
			, _capacity(_size)
		{
			cout << "string(char* str)构造" << endl;
			_str = new char[_capacity + 1];
			strcpy(_str, str);
		}

		string(const string& s) :_str(nullptr)
		{
			cout << "string(const string& s) —— 拷贝构造" << endl;
			reserve(s._capacity);
			for (auto ch : s)
			{
				push_back(ch);
			}
		}

		void swap(string& s)
		{
			::swap(_str, s._str);
			::swap(_size, s._size);
			::swap(_capacity, s._capacity);
		}

		// 移动构造
		string(string&& s)
		{
			cout << "string(string&& s) -- 移动构造" << endl;
			swap(s);
		}

		string& operator=(const string& s)
		{
			cout << "string& operator=(const string& s) -- 拷贝赋值" << endl;
			if (this != &s)
			{
				_str[0] = '\0';
				_size = 0;
				reserve(s._capacity);
				for (auto ch : s)
				{
					push_back(ch);
				}
			}
			return *this;
		}

		// 移动赋值
		string& operator=(string&& s)
		{
			cout << "string& operator=(string&& s) -- 移动赋值" << endl;
			swap(s);
			return *this;
		}

		/*~string()
		{
			cout << "~string() -- 析构" << endl;
			delete[] _str;
			_str = nullptr;
		}*/

		void reserve(size_t n)
		{
			if (n > _capacity)
			{
				char* tmp = new char[n + 1];
				if (_str)
				{
					strcpy(tmp, _str);
					delete[] _str;
				}
				_str = tmp;
				_capacity = n;
			}
		}

		void push_back(char ch)
		{
			if (_size >= _capacity)
			{
				size_t newcapacity = _capacity == 0 ? 4 : _capacity * 2;
				reserve(newcapacity);
			}
			_str[_size] = ch;
			++_size;
			_str[_size] = '\0';
		}

		char& operator[](size_t pos)
		{
			// assert(pos < _size);
			return _str[pos];
		}

		string& operator+=(char ch)
		{
			push_back(ch);
			return *this;
		}
		const char* c_str() const
		{
			return _str;
		}
		size_t size() const
		{
			return _size;
		}

	private:
		char* _str = nullptr;
		size_t _size = 0;
		size_t _capacity = 0;
	};

	string addStrings(string num1, string num2)
	{
		string str;
		// cout << "str 的地址：" << &str << endl;
		int end1 = num1.size() - 1, end2 = num2.size() - 1;
		int next = 0;
		while (end1 >= 0 || end2 >= 0)
		{
			int val1 = end1 >= 0 ? num1[end1--] - '0' : 0;
			int val2 = end2 >= 0 ? num2[end2--] - '0' : 0;
			int ret = val1 + val2 + next;
			next = ret / 10;
			ret = ret % 10;
			str += ('0' + ret);
		}
		if (next == 1)	str += '1';
		reverse(str.begin(), str.end());
		cout << "******************************" << endl;
		return str;
	}
}

// #include <list>
#include "list.h"
using namespace XAY;

int main()
{
	list<AY::string> lt;
	AY::string str("string");
	lt.push_back(str);		// 左值
	cout << "******************************" << endl;

	lt.push_back(AY::string("string"));		// 右值
	cout << "******************************" << endl;

	lt.push_back("string");		// 右值
	cout << "******************************" << endl;

	lt.push_back(move(str));	// 右值
	cout << "******************************" << endl;

	return 0;
}


//int main()
//{
//	// 场景1：
//	/*AY::string ret = AY::addStrings("1111", "2222");
//	cout << ret.c_str() << endl;
//	cout << "ret的地址：" << &ret << endl;*/
//
//	// 场景2
//	/*AY::string ret;
//	ret = AY::addStrings("11111", "2222");
//	cout << ret.c_str() << endl;*/
//	// cout << "ret的地址：" << &ret << endl;
//
//	return 0;
//}

//int main()
//{
//	AY::string s1("string");
//	cout << "******************************" << endl;
//	AY::string s2(s1);	// 拷贝构造
//	cout << "******************************" << endl;
//	AY::string s3 = AY::string("string");	// 构造 + 移动构造，优化后直接构造
//	cout << "******************************" << endl;
//	AY::string s4 = move(s1);	// 移动构造
//	cout << endl;
//
//	cout << "************* 赋值 *****************" << endl;
//	s3 = s1;
//	cout << "******************************" << endl;
//	s3 = move(s1);
//
//	return 0;
//}

//int main()
//{
//	int num = 1;
//	int& r1 = num;
//	cout << typeid(r1).name() << endl;
//	int& r2 = r1;
//	int&& rr1 = 10;
//	cout << typeid(rr1).name() << endl;
//	// int&& rr2 = rr1;		// 无法将左值绑定到右值引用
//	int&& rr2 = move(rr1);
//	cout << typeid(rr2).name() << endl;
//
//	return 0;
//}

//class Solution {
//public:
//	// 传值返回需要拷⻉
//	string addStrings(string num1, string num2) {
//		string str;
//		int end1 = num1.size() - 1, end2 = num2.size() - 1;
//		// 进位
//		int next = 0;
//		while (end1 >= 0 || end2 >= 0)
//		{
//			int val1 = end1 >= 0 ? num1[end1--] - '0' : 0;
//			int val2 = end2 >= 0 ? num2[end2--] - '0' : 0;
//			int ret = val1 + val2 + next;
//			next = ret / 10;
//			ret = ret % 10;
//			str += ('0' + ret);
//		}
//		if (next == 1)
//			str += '1';
//		reverse(str.begin(), str.end());
//
//		return str;
//	}
//
//	// 这里的传值返回拷贝代价就太大了
//	vector<vector<int>> generate(int numRows) {
//		vector<vector<int>> vv(numRows);
//		for (int i = 0; i < numRows; ++i)
//		{
//			vv[i].resize(i + 1, 1);
//		}
//		for (int i = 2; i < numRows; ++i)
//		{
//			for (int j = 1; j < i; ++j)
//			{
//				vv[i][j] = vv[i - 1][j] + vv[i - 1][j - 1];
//			}
//		}
//		return vv;
//	}
//};

//int main()
//{
//	string s1 = "string ";
//	// const的左值引用能延长临时对象生命周期
//	const string& r1 = s1 + s1;
//	// 但是不能修改
//	// r1 += "ret";
//	
//	// 右值引用可用于为临时对象延长生命周期
//	string&& rr1 = s1 + s1;
//	// 可以修改
//	rr1 += "ret";
//
//	return 0;
//}

//// 由于引用折叠限定，Func1 实例化以后总是⼀个左值引用
//template<class T>
//void Func1(T& x)
//{}
//
//// 由于引用折叠限定，Func2 实例化后可以是左值引用，也可以是右值引用
//template<class T>
//void Func2(T&& x)
//{}


// 引用折叠
//int main()
//{
//	// 不能直接定义引用的引用
//	// int&&& rr1 = 1;
//
//	//typedef int& lref;
//	//typedef int&& rref;
//
//	//// 引用的引用
//	//int n = 0;
//	//lref& r1 = n;	// 左值引用 左值引用 —— 左值引用
//	//lref&& r2 = n;	// 左值引用 右值引用 —— 左值引用
//	//rref& rr1 = n;	// 右值引用 左值引用 —— 左值引用
//	//rref&& rr2 = 1;	// 右值引用 右值引用 —— 右值引用
//
//	int n = 0;
//	// 折叠->实例化为void Func1(int& x)
//	Func1<int&>(n);
//	// Func1<int&>(0); // 报错
//
//	// 折叠->实例化为 void Func1(int& x)
//	Func1<int&&>(n);
//	// Func1<int&&>(0); // 报错
//
//	// 折叠->实例化为void Func1(const int& x)
//	Func1<const int&>(n);
//	Func1<const int&>(0);
//
//	// 折叠->实例化为void Func1(const int& x)
//	Func1<const int&&>(n);
//	Func1<const int&&>(0);
//
//	// 没有折叠->实例化为void Func2(int&& x)
//	// Func2<int>(n);   // 报错
//	Func2<int>(0);
//
//	// 折叠->实例化为void Func2(int& x)
//	Func2<int&>(n);
//	// Func2<int&>(0);  // 报错
//
//	// 折叠->实例化为void Func2(int&& x)
//	// Func2<int&&>(n); // 报错
//	Func2<int&&>(0);
//
//	return 0;
//}

//template <class T>
//void Function(T&& t)
//{
//	int a = t;
//	T x = a;
//	// a++;
//	cout << &a << endl;
//	cout << &x << endl << endl;
//}
//
//int main()
//{
//	// 10 为右值，推导出 T 为 int，模板实例化为 void Function(int&& t)
//	Function(10);	// 右值
//
//	int a;
//	// a 为左值，推导出 T 为 int&，引用折叠，模板实例化为 void Function(int& t)
//	Function(a);	// 左值
//
//	// a 为左值，move(a) 是右值，推导出 T 为 int，模板实例化为 void Function(int&& t)
//	Function(move(a));	// 右值
//
//	const int b = 0;
//	// b 为左值，推导出 T 为 const int&，引用折叠，模板实例化为 void Function(const int& t)
//	Function(b);	// 左值
//	// b 为左值，move(b) 是右值,推导出 T 为 const int，模板实例化为 void Function(const int&& t)
//	Function(move(b));	// 右值
//
//	return 0;
//}