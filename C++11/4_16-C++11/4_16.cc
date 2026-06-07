#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iomanip>		// 将科学计数法转换成正常点数

using namespace std;

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

/*********************  lambda 表达式  *************************/
//int main()
//{
//	auto add = [](int x, int y) { return x + y; };
//
//	cout << add(1, 2) << endl;
//
//	return 0;
//}

//struct Goods
//{
//	string _name;  // 名字
//	double _price; // 价格
//	int _evaluate; // 评价
//
//	Goods(const char* str, double price, int evaluate)
//		:_name(str) , _price(price) , _evaluate(evaluate)
//	{}
//};
//
//// 降序
//struct ComparePriceLess
//{
//	bool operator()(const Goods& gl, const Goods& gr)
//	{
//		return gl._price < gr._price;
//	}
//};
//
//// 升序
//struct ComparePriceGreater
//{
//	bool operator()(const Goods& gl, const Goods& gr)
//	{
//		return gl._price > gr._price;
//	}
//};

//int main()
//{
//	vector<Goods> v = { { "苹果 ", 2.1, 5 }, { "香蕉", 3, 4 }, 
//						{ "橙子", 2.2, 3 }, { "菠萝", 1.5, 4 } };
//	/*sort(v.begin(), v.end(), ComparePriceLess());
//	sort(v.begin(), v.end(), ComparePriceGreater());*/
//
//	// lamada 表达式作为函数对象，传给 sort 来作为比较器进行比较
//	sort(v.begin(), v.end(), [](const Goods& gl, const Goods& gr) {
//		return gl._price < gr._price;
//		});
//
//	sort(v.begin(), v.end(), [](const Goods& gl, const Goods& gr) {
//		return gl._price > gr._price;
//		});
//
//	sort(v.begin(), v.end(), [](const Goods& gl, const Goods& gr) {
//		return gl._evaluate < gr._evaluate;
//		});
//
//	sort(v.begin(), v.end(), [](const Goods& gl, const Goods& gr) {
//		return gl._evaluate > gr._evaluate;
//		});
//
//	return 0;
//}


//int main()
//{
//	int x = 10, y = 20, z = 30, num = 0;
//
//	auto add = [](int x, int y) {
//		return x + y;
//		};
//
//	// 函数对象
//	add(10, 20);
//
//	/*auto add = [=]() mutable {
//		x++, y++, z++, num++;
//		};
//	cout << x << " " << y << " " << z << " " << num << endl;*/
//
//	//// 混合捕捉 —— = 混合捕捉时，后⾯的捕捉变量必须是引用捕捉
//	//auto add = [=, x, y]() {
//	//	return x + y + z + num;
//	//	};
//
//	//auto add = [=, &x, &y]() {
//	//	// x,y 是引用捕捉，z和num是值捕捉
//	//	x++, y++;
//	//	return x + y + z + num;
//	//	};
//
//	//// 混合捕捉 —— & 混合捕捉时，后面的捕捉变量必须是值捕捉
//	//auto add = [&, &x, &y]() {
//	//	return x + y + z + num;
//	//	};
//	//
//	//// 混合捕捉 —— & 混合捕捉时，后面的捕捉变量必须是值捕捉
//	//auto add = [&, x, y]() {
//	//	// x,y 是值捕捉，z和num是引用捕捉
//	//	z++, num++;
//	//	return x + y + z + num;
//	//	};
//
//	//// 隐式的值捕捉
//	//auto add = [=]() {
//	//	return x + y + z + num;
//	//	};
//
//	//// 隐式的引用捕捉
//	//auto add = [&]() {
//	//	x++, y++, z++, num++;
//	//	return x + y + z + num;
//	//	};
//
//
//	//auto add = [x, y, &z]() {
//	//	// x += 10;	// 值捕捉不能被修改
//	//	z += 10;	// 引用捕捉可以被修改
//	//	// num++;		// 不能使用没有捕捉的
//	//	return x + y + z;
//	//	};
//
//	return 0;
//}

#include <functional>

//int f(int a, int b) { return a + b; }
//
//struct Functor
//{
//public:
//	int operator() (int a, int b)
//	{
//		return a + b;
//	}
//};
//
//class Plus
//{
//public:
//	Plus(int n = 10) :_n(n) { }
//	static int plusi(int a, int b) { return a + b; }
//	double plusd(double a, double b) { return (a + b) * _n; }
//
//private:
//	int _n;
//};


//int main()
//{
//	// 包装各种可调用对象，相当于统一了类型 —— function<int(int, int)>
//	// function<int(int, int)>: 参数为 int,int, 返回值为 int
//	function<int(int, int)> f1 = f;
//	function<int(int, int)> f2 = Functor();
//	function<int(int, int)> f3 = [](int a, int b) {return a + b; };
//
//	cout << f1(1, 1) << endl;
//	cout << f2(1, 1) << endl;
//	cout << f3(1, 1) << endl;
//
//	vector < function<int(int, int)>> vf = { f, Functor(), [](int a, int b) {return a + b; } };
//	for (auto& f : vf)
//	{
//		cout << f(1, 1) << endl;
//	}
//
//	// 包装静态成员函数
//	// 成员函数要指定类域并且前面加 & 才能获取地址
//	function<int(int, int)> f4 = &Plus::plusi;
//	cout << f4(1, 1) << endl;
//
//	// 包装普通成员函数
//	// 普通成员函数还有一个隐含的this指针参数，所以绑定时传对象或者对象的指针过去都可以
//	function<double(Plus*, double, double)> f5 = &Plus::plusd;
//	Plus ps;
//	cout << f5(&ps, 1.1, 1.1) << endl;
//
//	function<double(Plus, double, double)> f6 = &Plus::plusd;
//	cout << f6(Plus(), 1.1, 1.1) << endl;
//
//	function<double(Plus&, double, double)> f7 = &Plus::plusd;
//	cout << f7(ps, 1.1, 1.1) << endl;
//
//	function<double(Plus&&, double, double)> f8 = &Plus::plusd;
//	cout << f8(Plus(), 1.1, 1.1) << endl;
//
//	return 0;
//}


/*********************  bind  *************************/
// 展开占位符
using placeholders::_1;
using placeholders::_2;
using placeholders::_3;

int Sub(int a, int b)
{
	return (a - b) * 10;
}

int main()
{
	// cout << Sub(10, 5) << endl;

	//// 调整参数顺序
	//auto newSub1 = bind(Sub, _1, _2);	
	//cout << "bind(Sub, _1, _2) —— " << newSub1(10, 5) << endl;	// 这个仍然是原顺序
	//auto newSub2 = bind(Sub, _2, _1);	
	//cout << "bind(Sub, _2, _1) —— " << newSub2(10, 5) << endl;	// _2传给a, _1传给b

	//// 调整参数个数
	//auto newSub1 = bind(Sub, _1, 10);	// 绑死第二个参数，就不需要用户来传参了
	//cout << "bind(Sub, _1, 10) —— " << newSub1(5) << endl;
	//auto newSub2 = bind(Sub, 5, _1);	// 绑死第一个参数，就不需要用户来传参了
	//cout << "bind(Sub, 5, _1) —— " << newSub2(10) << endl;

	// 计算复利的lambda
	auto func1 = [](double rate, double money, int year)->double {
		double ret = money;
		for (int i = 0; i < year; i++)
		{
			ret += ret * rate;
		}
		return ret - money;
	};

	// 绑死⼀些参数，实现出支持不同年华利率，不同金额和不同年份计算出复利的结算利息
	function<double(double)> func3_1_5 = bind(func1, 0.015, _1, 3);		
	function<double(double)> func5_1_5 = bind(func1, 0.015, _1, 5);
	function<double(double)> func10_2_5 = bind(func1, 0.025, _1, 10);
	function<double(double)> func20_3_5 = bind(func1, 0.035, _1, 30);

	// 本金一百万
	cout << "bind(func1, 0.015, _1, 3) —— " << fixed << func3_1_5(1000000) << endl;	// 3 年后的结算利息
	cout << "bind(func1, 0.015, _1, 5) —— " << fixed << func5_1_5(1000000) << endl;	// 5 年后的结算利息
	cout << "bind(func1, 0.025, _1, 10) —— " << fixed << func10_2_5(1000000) << endl;	// 10 年后的结算利息
	cout << "bind(func1, 0.035, _1, 30) —— " << fixed << func20_3_5(1000000) << endl;	// 30 年后的结算利息

	return 0;
}