#include "string.h"

namespace AY
{
	// 定义
	const size_t string::npos = -1;
	/*
	// 无参的构造函数
	string::string()
		: m_str(new char[1]{""})
		, m_size(0)
		, m_capacity(0)
	{}

	// 带参的构造函数
	string::string(const char* s)
		: m_size(strlen(s))
	{
		m_str = new char[m_size + 1];
		m_capacity = m_size;
		// 记得引用头文件 <string.h>
		memcpy(m_str, s, strlen(s) + 1); 
	}
	*/

	// 全缺省的构造函数
	string::string(const char* s)
		: m_size(strlen(s))
	{
		m_str = new char[m_size + 1];
		m_capacity = m_size;
		// 记得引用头文件 <string.h>
		memcpy(m_str, s, strlen(s) + 1);
	}

	// 析构函数
	string::~string()
	{
		delete[] m_str;
		m_str = nullptr;
		m_size = m_capacity = 0;
	}

	// c_str 函数
	const char* string::c_str() const
	{
		return m_str;
	}

	// size 函数
	size_t string::size() const
	{
		return m_size;
	}

	// operator[] 函数 —— 普通版本
	char& string::operator[] (size_t pos)
	{
		// 记得引用头文件<assert.h>
		assert(pos < m_size);

		return m_str[pos];
	}

	// operator[] 函数 —— const 版本
	const char& string::operator[] (size_t pos) const
	{
		// 记得引用头文件<assert.h>
		assert(pos < m_size);

		return m_str[pos];
	}

	// begin 函数
	string::iterator string::begin()
	{
		return m_str;
	}

	// end 函数
	string::iterator string::end()
	{
		return m_str + m_size;
	}

	// const 版本的 begin 函数
	string::const_iterator string::begin() const
	{
		return m_str;
	}

	// const 版本的 end 函数
	string::const_iterator string::end() const
	{
		return m_str + m_size;
	}

	// reserve 函数 —— 扩容，异地扩容
	void string::reserve(size_t n)
	{
		// 判断是否需要扩容
		if (n > m_capacity)
		{
			// 开辟新空间 —— 为'\0'预留一块空间
			char* str = new char[n + 1];
			// 拷贝旧数据
			memcpy(str, m_str, m_size + 1);
			// 释放旧空间
			delete[] m_str;
			// 更新变量的数据
			m_str = str;
			m_capacity = n;
		}
	}

	// push_back 函数
	void string::push_back(char c)
	{
		// 判断是否需要扩容
		if (m_size >= m_capacity)
		{
			size_t newcapacity = m_capacity == 0 ? 4 : 2 * m_capacity;
			reserve(newcapacity);
		}

		// 插入数据
		m_str[m_size] = c;
		// 有效数据个数 +1
		m_size++;
		// 手动加上 '\0'
		m_str[m_size] = '\0';
	}

	// append 函数
	string& string::append(const char* s)
	{
		//先求出待插入的字符串的长度
		size_t len = strlen(s);

		// 判断是否需要扩容
		if (m_size + len >= m_capacity)
		{
			size_t newcapacity = m_size + len > 2 * m_capacity
				? m_size + len : 2 * m_capacity;
			reserve(newcapacity);
		}

		// 插入数据 —— +1 是算上了'\0'
		memcpy(m_str + m_size, s, len + 1);
		// 有效数据个数 + len
		m_size += len;

		return *this;
	}

	// operator+= 函数 —— += 字符
	string& string::operator+= (char c)
	{
		push_back(c);

		return *this;
	}
	
	// operator+= 函数 —— += 字符串
	string& string::operator+= (const char* s)
	{
		append(s);

		return *this;
	}

	//// 流插入运算符 << 函数
	//ostream& operator<< (ostream& os, const string& str)
	//{
	//	os << str.c_str() << endl;

	//	return os;
	//}

	// 流插入运算符 << 函数
	ostream& operator<< (ostream& os, const string& str)
	{
		// 依次输出
		for (size_t i = 0; i < str.size(); i++)
		{
			os << str[i];
		}

		return os;
	}

	// insert 函数 —— 插入字符
	string& string::insert(size_t pos, char c)
	{
		// 判断 pos 位置是否合法
		assert(pos < m_size);
		// 判断是否需要扩容
		if (m_size >= m_capacity)
		{
			size_t newcapacity = m_capacity == 0 ? 4 : 2 * m_capacity;
			reserve(newcapacity);
		}

		// 挪动数据
		size_t end = m_size + 1;
		while (end > pos)
		{
			m_str[end] = m_str[end - 1];
			end--;
		}

		// 插入数据
		m_str[pos] = c;
		m_size++;

		return *this;
	}

	// insert 函数 —— 插入字符串
	string& string::insert(size_t pos, const char* s)
	{
		// 判断 pos 位置是否合法
		assert(pos < m_size);

		//先求出待插入的字符串的长度
		size_t len = strlen(s);

		// 判断是否需要扩容
		if (m_size + len >= m_capacity)
		{
			size_t newcapacity = m_size + len > 2 * m_capacity
				? m_size + len : 2 * m_capacity;
			reserve(newcapacity);
		}

		// 挪动数据
		size_t end = m_size + len;
		while (end > pos + len - 1)
		{
			m_str[end] = m_str[end - len];
			end--;
		}

		// 插入数据
		for (size_t i = 0; i < len; i++)
		{
			m_str[pos + i] = s[i];
		}

		// m_size 的大小增加 len
		m_size += len;

		return *this;
	}

	string& string::erase(size_t pos, size_t len)
	{
		// 判断 pos 位置是否合法
		assert(pos < m_size);

		if (len >= m_size - pos || pos >= npos)
		{
			m_size = pos;
			m_str[m_size] = '\0';
		}
		else
		{
			// 挪动数据
			for (size_t i = pos; i < pos + len; i++)
			{
				m_str[i] = m_str[i + len];
			}

			// m_size 的大小 - len
			m_size -= len;
			m_str[m_size] = '\0';
		}

		return *this;
	}
    
	// pop_back 函数
	void string::pop_back()
	{
		assert(m_size != 0);

		m_size--;
		m_str[m_size] = '\0';
	}

	// find 函数 —— 查找字符
	size_t string::find(char c, size_t pos) const
	{
		for (size_t i = pos; i < m_size; i++)
		{
			if (m_str[i] == c)
			{
				return i;
			}
		}

		return npos;
	}

	// find 函数 —— 查找字符串
	size_t string::find(const char* s, size_t pos) const
	{
		char* ret = strstr(m_str + pos, s);
		if (ret != nullptr)
		{
			return ret - m_str;
		}
		else
		{
			return npos;
		}
	}

	// substr 函数
	string string::substr(size_t pos, size_t len) const
	{
		// 判断 pos 的位置是否合法
		assert(pos < m_size);

		// 调整 len 的大小
		if (len >= m_size - pos || len >= npos)
		{
			len = m_size - pos;
		}

		// 先创建一个 string 类对象用来返回
		string tmp;
		// 提前预留好空间
		tmp.reserve(len);
		for (size_t i = 0; i < len; i++)
		{
			tmp += m_str[pos + i];
		}

		return tmp;
	}

	// 六种比较函数
	bool string::operator< (const string& str)
	{
		size_t i1 = 0;
		size_t i2 = 0;

		// 遍历两个字符串
		while (i1 < m_size && i2 < str.m_size)
		{
			if (m_str[i1] < str[i2])
			{
				return true;
			}
			else if (m_str[i1] > str[i2])
			{
				return false;
			}
			else
			{
				i1++;
				i2++;
			}
		}

		return i2 < str.m_size;
	}

	bool string::operator== (const string& str)
	{
		size_t i1 = 0;
		size_t i2 = 0;

		// 遍历两个字符串
		while (i1 < m_size && i2 < str.m_size)
		{
			if (m_str[i1] != str[i2])
			{
				return false;
			}

			i1++;
			i2++;
		}

		return i1 == m_size && i2 == str.m_size;
	}


	bool string::operator<= (const string& str)
	{
		return (*this < str) || (*this == str);
	}

	bool string::operator> (const string& str)
	{
		return !(*this < str || *this == str);
	}

	bool string::operator>= (const string& str)
	{
		return !(*this < str);
	}

	bool string::operator!= (const string& str)
	{
		return !(*this == str);
	}

	// clear 函数
	void string::clear()
	{
		m_size = 0;
		m_str[m_size] = '\0';
	}

	
	// 流提取运算符 >> 函数
	/*istream& operator>> (istream& is, string& str)
	{
		// 清除 str 中的数据
		str.clear();
		char c;
		
		// 依次输入字符
		while (is.get(c))
		{
			if (c == ' ' || c == '\n')
			{
				break;
			}

			str += c;
		}

		return is;
	}*/

	// 流提取运算符 >> 函数
	istream& operator>> (istream& is, string& str)
	{
		// 清除 str 中的数据
		str.clear();
		char c;
		
		size_t i = 0;
		char buff[128] = "";

		// 依次输入字符
		while (is.get(c))
		{
			if (c == ' ' || c == '\n')
			{
				break;
			}

			if (i == 127)
			{
				buff[i] = '\0';
				str += buff;
				i = 0;
			}

			buff[i++] = c;
		}

		if (i > 0)
		{
			buff[i] = '\0';
			str += buff;
		}

		return is;
	}

	// getline 函数
	istream& getline(istream& is, string& str, char delim)
	{
		// 清除 str 中的数据
		str.clear();
		char c;

		size_t i = 0;
		char buff[128] = "";

		// 依次输入字符
		while (is.get(c))
		{
			if (c == delim)
			{
				break;
			}

			if (i == 127)
			{
				buff[i] = '\0';
				str += buff;
				i = 0;
			}

			buff[i++] = c;
		}

		if (i > 0)
		{
			buff[i] = '\0';
			str += buff;
		}

		return is;
	}

	// swap 函数
	void string::swap(string& str)
	{
		// 调用 std 库中的 swap 函数
		std::swap(m_str, str.m_str);
		std::swap(m_size, str.m_size);
		std::swap(m_capacity, str.m_capacity);
	}

	// 拷贝构造函数 —— 第一种实现方法
	/*string::string(const string& str)
	{
		// 开辟新空间
		m_str = new char[str.m_capacity + 1];
		// 拷贝旧数据
		memcpy(m_str, str.m_str, str.m_size + 1);
		// 更新变量的数据
		m_size = str.m_size;
		m_capacity = str.m_capacity;
	}*/

	// 拷贝构造函数 —— 第二种实现方法
	string::string(const string& str)
	{
		string tmp(str.m_str);
		// 这里的 swap 函数是 this 在调用
		// 实际上是这样的 this->swap(tmp)
		swap(tmp);
	}

	// 赋值运算符重载
	string& string::operator= (const string& str)
	{  
		if (*this != str)
		{
			// 开辟新空间
			char* tmp = new char[str.m_capacity + 1];
			// 拷贝旧数据
			memcpy(tmp, str.m_str, str.m_size + 1);
			// 释放旧空间
			delete[] m_str;
			// 更新变量的数据
			m_str = tmp;
			m_size = str.m_size;
			m_capacity = str.m_capacity;

			return *this;
		}
	}

	// 赋值重载函数
	string& string::operator= (const string& str)
	{
		if (*this != str)
		{
			string tmp(str);
			swap(tmp);
		}
		
		return *this;
	}
}
