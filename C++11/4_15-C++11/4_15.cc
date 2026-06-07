#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cstring>

using namespace std;

//void Fun(int& x) { cout << "左值引用" << endl; }
//void Fun(const int& x) { cout << "const 左值引用" << endl; }
//void Fun(int&& x) { cout << "右值引用" << endl; }
//void Fun(const int&& x) { cout << "const 右值引用" << endl; }
//
//template<class T>
//void Function(T&& t)
//{
//	// Fun(t);
//	Fun(forward<T>(t));
//}


//int main()
//{
//	int num = 10;
//
//	// 10 是右值，推导出 T 为 int，模板实例化为 void Function<int&& t>
//	Function(10);	// 右值
//
//	// num 是左值，推导出 T 为 int&，引用折叠，模板实例化为 void Function<int& t>
//	Function(num);	// 左值
//
//	// move(num) 是右值，推导出 T 为 int，模板实例化为 void Function<int&& t>
//	Function(move(num));	// 右值
//
//	const int cnum = 10;
//	// cnum 是左值，推导出 T 为 int&，引用折叠，模板实例化为 void Function<const int& t>
//	Function(cnum);	// const 左值
//
//	// move(num) 是右值，推导出 T 为 int，模板实例化为 void Function<const int&& t>
//	Function(move(cnum));	// const 右值
//
//	return 0;
//}


//int main()
//{
//	string s1("string");
//	string s2 = "string";
//	string s3 = { "string" };
//	string s4{ "string" };
//	string s5(10, 'x');		// 拷贝构造
//	string s6 = { 10, 'x' };	// 多参数的隐式类型转换
//
//	int num1 = 0;
//	int num2 = {};
//	int num3{};
//	cout << num1 << endl;
//	cout << num2 << endl;
//	cout << num3 << endl;
//
//	return 0;
//}

//template<class ...Args>
//void Print(Args&&... args)
//{
//	cout << sizeof...(args) << endl;
//}
//
//int main()
//{
//	int num = 10;
//	Print();								// 包里有0个参数
//	Print(10);								// 包里有1个参数
//	Print(10, string("string"));			// 包里有2个参数
//	Print(10, string("string"), num);		// 包里有3个参数
//
//	return 0;
//}


//void Recurs()
//{
//	cout << endl;
//	return;
//}
//
//template <class T, class ...Args>
//void Recurs(T x, Args... args)
//{
//	cout << x << " ";
//	if (sizeof...(args) == 0) return;
//	// 剩下的参数继续向下推导
//	Recurs(args...);
//}
//
//template <class ...Args>
//void Print(Args ...args)
//{
//	// 参数往下传，先推导出一个参数
//	Recurs(args...);
//}
//
//int main()
//{
//	int num = 10;
//	Print();								// 包里有0个参数
//	Print(10);								// 包里有1个参数
//	Print(10, string("string"));			// 包里有2个参数
//	Print(10, string("string"), num);		// 包里有3个参数
//
//	return 0;
//}


#include <list>

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

		string(size_t n, char ch)
		{
			cout << "string(size_t n, char ch) —— 构造" << endl;
			_str = new char[n + 1];
			for (size_t i = 0; i < n; i++)
			{
				_str += ch;
			}
			_str += '\0';
			_size = n;
			_capacity = n;
		}

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


class Data
{
public:
	//日期类的构造函数
	Data(int year = 1, int month = 1, int day = 1)
	{
		m_year = year;
		m_month = month;
		m_day = day;
		cout << "Data(int year = 1, int month = 1, int day = 1) 构造函数" << endl;
	}

	// 日期类的拷贝构造函数
	Data(const Data& other)
		: m_year(other.m_year), m_month(other.m_month), m_day(other.m_day)
	{
		cout << "Data(const Data& other) 拷贝构造" << endl;
	}

	void DataPrint()
	{
		std::cout << m_year << "/" << m_month
			<< "/" << m_day << std::endl;
	}

	Data()
	{
		cout << "Data()" << endl;
	}

private:
	int m_year;
	int m_month;
	int m_day;
};

#include "list.h"

/*int main()
{
	//XAY::list<AY::string> lt;
	//AY::string str("string");
	//// 传左值，与push_back一样，调用拷贝构造
	//lt.emplace_back(str);
	//cout << "******************************" << endl;

	//// 传右值，与push_back一样，调用移动构造
	//lt.emplace_back(move(str));
	//cout << "******************************" << endl;

	//lt.push_back("string");
	//cout << "******************************" << endl;
	//lt.emplace_back("string");

	//list<Data> lt;
	//lt.push_back({ 2026, 4, 14 });	// 构造 + 拷贝构造
	//cout << "******************************" << endl;

	//lt.emplace_back(2026, 4, 14);	// 构造
	//cout << "******************************" << endl;

	// lt.emplace_back(10, 'x');

	//list<pair<string, string>> lt;
	//lt.emplace_back("string", "字符串");
	//// lt.push_back("string", "字符串");			// 不可以这样写
	//// 不可以这样写,它是参数包，但是使用的是花括号初始化
	//// 推导出来的就是 initializer_list 类型，属于不同类型，且没有相应的拷贝构造
	//// lt.emplace_back({ "string", "字符串" });	
	//// 它会隐式类型转换成pair，先构造pair对象，再调用拷贝构造
	//lt.push_back({ "string", "字符串" });

	return 0;
}*/

//class Person
//{
//public:
//	Person(const char* name = "", int age = 0)
//		:_name(name)
//		, _age(age)
//	{}
//	
//
//	/*~Person() {}
//
//	Person(Person&& rrp) = default;
//	Person(const Person& rrp) = default;
//	Person& operator=(const Person& rp) = default;
//	Person& operator=(Person&& rrp) = default;*/
//
//private:
//	AY::string _name;
//	int _age;
//};
//
//int main()
//{
//	Person per1;				// 左值 构造
//	Person per2 = per1;			// 左值 拷贝构造
//	Person per3 = move(per1);	// 右值 移动构造
//	Person per4;				// 左值 构造
//	per4 = move(per2);			// 右值 移动赋值
//
//	return 0;
//}


//class Example
//{
//public:
//	Example(int x, int y) :_x(x), _y(y)
//	{
//		cout << "目标构造函数" << endl;
//	}
//
//	Example(int x) :Example(x, 0)
//	{
//		cout << "委托构造函数" << endl;
//	}
//
//	int _x;
//	int _y;
//};
//
//int main()
//{
//	Example(1, 2);
//	Example(1);
//
//	return 0;
//}

class Base
{
public:
	Base(int x, double y) :_x(x), _y(y) { cout << "Base(int x, double y)" << endl; }
	Base(int x) :_x(x) { cout << "Base(int x)" << endl; }
	Base(double y) :_y(y) { cout << "Base(double y)" << endl; }

	int _x;
	double _y;
};

//// 传统派生类实现构造
//class Derive : Base
//{
//public:
//	Derive(int x) : Base(x) {}
//	Derive(double y) : Base(y) {}
//	Derive(int x, double y) : Base(x, y) {}
//};

// C++ 继承基类的所有构造函数
class Derive : Base
{
	using Base::Base;
};

int main()
{
	Derive(1);
	Derive(1.1);
	Derive(1, 1.1);

	return 0;
}