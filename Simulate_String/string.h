#pragma once

#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

using namespace std;

namespace AY
{
	class string
	{
	public:
		//// 两个构造函数
		//string();
		//string(const char* s);

		// 全缺省构造函数
		string(const char* s = "");

		// 析构函数
		~string();

		// 拷贝构造函数
		string(const string& str);

		// 赋值运算符重载
		string& operator= (const string& str);

		// c_str 函数
		const char* c_str() const;

		// size 函数
		size_t size() const;

		// operator[] 函数 —— 普通版本
		char& operator[] (size_t pos);
		// operator[] 函数 —— const 版本
		const char& operator[] (size_t pos) const;

		typedef char* iterator;
		// begin 函数
		iterator begin();
		// end 函数
		iterator end();

		typedef const char* const_iterator;
		// const 版本的 begin 函数
		const_iterator begin() const;
		// const 版本的 end 函数
		const_iterator end() const;

		// reserve 函数
		void reserve(size_t n = 0);
		
		// push_back 函数
		void push_back(char c);

		// append 函数
		string& append(const char* s);

		// operator+= 函数 —— += 字符
		string& operator+= (char c);
		// operator+= 函数 —— += 字符串
		string& operator+= (const char* s);
		
		// insert 函数 —— 插入字符
		string& insert(size_t pos, char c);
		// insert 函数 —— 插入字符串
		string& insert(size_t pos, const char* s);

		// erase 函数
		string& erase(size_t pos = 0, size_t len = npos);

		// pop_back 函数
		void pop_back();

		// find 函数 —— 查找字符
		size_t find(char c, size_t pos = 0) const;
		// find 函数 —— 查找字符串
		size_t find(const char* s, size_t pos = 0) const;

		// substr 函数
		string substr(size_t pos = 0, size_t len = npos) const;

		// 六种比较函数
		bool operator< (const string& str);
		bool operator<= (const string& str);
		bool operator> (const string& str);
		bool operator>= (const string& str);
		bool operator== (const string& str);
		bool operator!= (const string& str);

		// clear 函数
		void clear();

		// swap 函数
		void swap(string& str);

	private:
		char* m_str;
		size_t m_size;
		size_t m_capacity;
	public:
		// 声明
		static const size_t npos;
	};

	// 流插入运算符 << 函数
	ostream& operator<< (ostream& os, const string& str);
	// 流提取运算符 >> 函数
	istream& operator>> (istream& is, string& str);
	// getline 函数
	istream& getline(istream& is, string& str, char delim = '\n');
}
