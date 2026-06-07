#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>


using namespace std;

//double Function(int x, int y)
//{
//	try
//	{
//		if (y == 0)	// 当 y == 0，抛出异常
//		{
//			string s("Diride by zero condition!");
//			throw s;
//		}
//		else
//		{
//			return ((double)x / (double)y);
//		}
//	}
//	catch (int err)
//	{
//		cout << "Function err" << endl;
//	}
//	catch (const string& errmsg)
//	{
//		cout << "Function errmsg" << endl;
//	}
//}
//
//void Func()
//{
//	int left, right = 0;
//	cin >> left >> right;
//	Function(left, right);
//}
//
//int main()
//{
//	try
//	{
//		Func();
//	}
//	catch (int err)
//	{
//		cout << "man err" << endl;
//	}
//	catch(const string& errmsg)
//	{
//		cout << "main errmsg" << endl;
//	}
//
//	return 0;
//}

class Exception
{
public:
    Exception(const string& errmsg, int id)
        :_errmsg(errmsg)
        , _id(id)
    {}
    virtual string what() const { return _errmsg; }
    int getid() const { return _id; }

protected:
    string _errmsg;
    int _id;
};

class SqlException : public Exception
{
public:
    SqlException(const string& errmsg, int id, const string& sql)
        : Exception(errmsg, id), _sql(sql)
    {}

    virtual string what() const { return "SqlException: " + _errmsg + " -> " + _sql; }

private:
    const string _sql;
};

class CacheException : public Exception
{
public:
    CacheException(const string& errmsg, int id, const string& cache)
        :Exception(errmsg, id), _cache(cache)
    {}
    virtual string what() const { return "CacheException: " + _errmsg + " -> " + _cache; }

private:
	const string _cache;
};


void SqlFunc()
{
    if (rand() % 10 < 5)
    {
        throw SqlException("sql error", 1001, "select * from user");
    }
    else if (rand() % 10 < 8)
    {
        throw SqlException("sql success", 1003, "select * from user");
    }
    else
    {
        throw Exception("unknown error", 1000);
    }
}

void CacheFunc()
{
    if (rand() % 10 < 5)
    {
        throw CacheException("cache error", 1002, "user cache");
    }
    else if (rand() % 10 < 8)
    {
        throw CacheException("cache success", 1004, "user cache");
    }
    else
    {
        throw Exception("unknown error", 1000);
    }
}

//int main()
//{
//	srand((unsigned)time(nullptr));
//
//    while(true)
//    {
//        try
//        {
//            SqlFunc();
//            CacheFunc();
//        }
//		catch (const Exception& ex)     // 捕获基类异常，处理所有派生类异常
//        {
//            cout << ex.what() << " id: " << ex.getid() << endl;
//        }
//        catch(...)	 // 捕获所有未知异常
//        {
//            cout << "main —— unknown exception" << endl;
//		}
//	}
//
//    return 0;
//}


//void _SeedMsg(const string& s) noexcept
//{
//    if (rand() % 2 == 0)
//    {
//        throw SqlException("⽹络不稳定，发送失败", 102, "put");
//    }
//    else if (rand() % 7 == 0)
//    {
//        throw SqlException("你已经不是对象的好友，发送失败", 103, "put");
//    }
//    else
//    {
//        cout << "发送成功" << endl;
//    }
//}
//
//void SendMsg(const string& s)
//{ 
//	// 当发送消息失败时，重试三次，如果是网络不稳定导致的失败，则继续重试；
//    // 如果是其他未知原因导致的失败，则提示用户发送失败
//    for(int i = 0; i < 4; ++i)
//    {
//        try
//        {
//            _SeedMsg(s);
//            break;
//        }
//        catch (const SqlException& ex)
//        {
//            if (ex.getid() == 102)
//            {
//                if (i == 3) { throw; }
//                cout << "开始第 " << i + 1 << " 次重试" << endl;
//                continue;
//            }
//            else
//            {
//				throw;   // 重新抛出异常，交由外层处理
//            }
//        }
//        catch (...)
//        {
//            cout << "未知异常，发送失败" << endl;
//            break;
//        }
//	}
//}
//
//int main()
//{
//    srand(time(0));
//    string str;
//    while (cin >> str)
//    {
//        try
//        {
//            SendMsg(str);
//        }
//        catch (const Exception& ex)
//        {
//            cout << ex.what() << endl << endl;
//        }
//        catch (...)
//        {
//            cout << "Unkown Exception" << endl;
//        }
//    }
//	return 0;
//}


//double Function(int x, int y)
//{
//    if (y == 0)	// 当 y == 0，抛出异常
//    {
//        string s("Diride by zero condition!");
//        throw s;
//    }
//    else
//    {
//        return ((double)x / (double)y);
//    }
//}
//
//void Func()
//{
//	int left, right = 0;
//	cin >> left >> right;
//	Function(left, right);
//}
//
//int main()
//{
//	/*try
//	{
//		Func();
//	}
//	catch(const string& errmsg)
//	{
//		cout << "errmsg: " << errmsg << endl;
//	}*/
//
//    int num = 10;
//    cout << noexcept(Function(1, 2)) << endl;
//    cout << noexcept(Function(1, 0)) << endl;
//	cout << noexcept(num++) << endl;
//
//	return 0;
//}