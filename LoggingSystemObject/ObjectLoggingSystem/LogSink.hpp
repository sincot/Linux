/*
日志落地模块
作用：将格式化完成后的日志消息字符串，输出到指定位置。支持同时将日志落地到不同的位置
位置分类：
    1. 标准输出
    2. 指定文件（事后进行日志分析）
    3. 滚动文件（文件按照时间/大小进行滚动切换）
    支持落地方向的扩展 用户可以自己编写一个新的落地模块，将日志进行其它方向的落地

实现思想：
    1. 抽象出落地模块基类
    2. 不同落地方向从基类中派生
    3. 使用工厂模式实现创建-表示的分离
*/

#ifndef MY_LOGSINK_H
#define MY_LOGSINK_H

#include "Util.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>
#include <iomanip>      // setw 函数所需

namespace Log
{
    class LogSink      // 抽象落地基类
    {
    public:
        virtual ~LogSink() = default;       // 提供默认实现
        virtual void Sink(const char* data, size_t len) = 0;    // 日志落地

        using ptr = std::shared_ptr<LogSink>;   // 管理基类
    };

    // 根据不同的落地方向派生出不同的子类
    
    // 落地方向：标准输出
    class StdoutSink : public LogSink
    {
    public:
        void Sink(const char* data, size_t len)     // 将日志消息写入到标准输出
        {
            std::cout.write(data, len);     // 从 data 位置开始，写入 len 长度的数据
        }    
    };

    // 落地方向：指定文件
    class FileSink : public LogSink
    {
    public:
        FileSink(const std::string& pathname) :_filename(pathname)      // 构造时传入文件名，并打开文件，将操作句柄管理起来
        {
            // 1. 创建日志文件所在的目录
            util::File::CreateDirectory(util::File::GetPath(pathname));

            // 2. 创建并打开日志文件
            _ofs.open(_filename, std::ios::binary | std::ios::app);     // 以写的方式打开文件，将日志信息追加到文件末尾
            assert(_ofs.is_open());     // 文件是否被打开 没有被打开，程序退出
        }

        void Sink(const char* data, size_t len)
        {
            _ofs.write(data, len);
            assert(_ofs.good());    // 文件操作句柄是否是正常的
        }    // 将日志消息写入到指定文件
    private:
        std::string _filename;   // 文件名
        std::ofstream _ofs;      // 文件操作句柄
    };

    // 落地方向：滚动文件（按照大小进行滚动）
    class RollBySizeSink : public LogSink
    {
    public:
        // 构造时传入文件名，并打开文件，将操作句柄管理起来
        RollBySizeSink(const std::string &basename, size_t max_size)    // 用户自己设计一个文件的大小
            : _basename(basename), _max_fsize(max_size), _cur_fsize(0), _name_count(0)
        {
            // 1. 根据基础文件名生成实际文件名
            std::string pathname = CreateNewFile();

            // 2. 创建日志文件所在的目录
            util::File::CreateDirectoy(util::File::GetPath(pathname));

            // 3. 创建并打开日志文件
            _ofs.open(pathname, std::ios::binary | std::ios::app);     // 以写的方式打开文件，将日志信息追加到文件末尾
            assert(_ofs.is_open());     // 文件是否被打开 没有被打开，程序退出
        }

        // 将日志消息写入到标准输出，写入前判断文件大小，超过了最大大小就要切换文件
        void Sink(const char* data, size_t len)
        {
            if(_cur_fsize >= _max_fsize)
            {
                _ofs.close();   // 关闭原来的已经打开的文件 不然会造成资源泄漏

                std::string pathname = CreateNewFile();     // 创建新的文件名
                // 打开新的文件
                _ofs.open(pathname, std::ios::binary | std::ios::app);     // 以写的方式打开文件，将日志信息追加到文件末尾
                assert(_ofs.is_open());     // 文件是否被打开 没有被打开，程序退出
                _cur_fsize = 0;     // 清 0
            }
            _ofs.write(data, len);
            assert(_ofs.good());
            _cur_fsize += len;
        }

    private:
        // 基于 max_fsize 和 cur_fsize 的大小来判断 cur_fsize 超过 max_fsize 则创建新文件
        std::string CreateNewFile()
        {
            // 获取系统时间，以时间来构造文件的扩展名
            time_t time = util::Date::GetTime();

            /*
            struct tm {
               int tm_sec;    // Seconds (0-60)
               int tm_min;    // Minutes (0-59)
               int tm_hour;   // Hours (0-23)
               int tm_mday;   // Day of the month (1-31)
               int tm_mon;    // Month (0-11)
               int tm_year;   // Year - 1900
               int tm_wday;   // Day of the week (0-6, Sunday = 0)
               int tm_yday;   // Day in the year (0-365, 1 Jan = 0)
               int tm_isdst;  // Daylight saving time 
            };
            */
            struct tm s_tm = Log::SafeLocalTime(time);      // 接收转化后的时间结构
            std::stringstream filename;

            filename << _basename << "-"
                    << std::setfill('0')  // 设置填充字符为 '0'
                    << std::setw(4) << (s_tm.tm_year + 1900)   // 年份4位
                    << std::setw(2) << (s_tm.tm_mon + 1)       // 月份2位
                    << std::setw(2) << s_tm.tm_mday            // 日期2位
                    << std::setw(2) << s_tm.tm_hour            // 小时2位
                    << std::setw(2) << s_tm.tm_min             // 分钟2位
                    << std::setw(2) << s_tm.tm_sec             // 秒数2位
                    << ".log"
                    << _name_count++;

            return filename.str();
        }

    private:
        /* 通过基础文件名 + 扩展文件名组成一个实际的当前输出文件名 */
        std::string _basename;      // 文件的基础名称   ./logs/base -> ./logs/base-20260613112134.log
        std::ofstream _ofs;         // 文件操作句柄
        size_t _max_fsize;          // 记录最大大小，当前文件超过该值就要切换文件
        size_t _cur_fsize;          // 记录当前文件已经写入的数据大小

        size_t _name_count;         // 日志文件的名称数量
    };


    /*
    扩展一个以时间作为入职文件滚动切换类型的日志落地模块
        1. 以时间进行文件滚动，实际上是以时间段进行滚动     
        实现思想：以当前系统时间，取模时间段大小，可以得到当前时间段是第几个时间段
        time(nullptr) % gap   time(nullptr) % 60  以分钟进行滚动 当前就是第 n 个60s
    */

    enum class TimeGap
    {
        GAP_SECOND,     // 秒
        GAP_MINUTE,     // 分
        GAP_HOUR,       // 时
        GAP_MDAY        // 天
    };

    // 落地方向：滚动文件（按照时间进行滚动）
    class RollByTimeSink : public LogSink
    {
    public:
        // 构造时传入文件名，并打开文件，将操作句柄管理起来
        RollByTimeSink(const std::string &basename, TimeGap gap_type)    // 用户自己设计一个时间段的大小
            : _basename(basename)
        {
            switch(gap_type)
            {
                case TimeGap::GAP_SECOND: _gap_size = 1; break;
                case TimeGap::GAP_MINUTE: _gap_size = 60; break;
                case TimeGap::GAP_HOUR:  _gap_size = 60*60; break;
                case TimeGap::GAP_MDAY: _gap_size = 24*60*60; break;
            }
            
            // 如果 _gap_size 为 1，任何数取模1都为0，因此需要做特殊的处理
            _cur_gap = _gap_size == 1 ? util::Date::GetTime() : util::Date::GetTime() % _gap_size;   // 当前是第几个时间段
            // 1. 根据基础文件名生成实际文件名
            std::string pathname = CreateNewFile();

            // 2. 创建日志文件所在的目录
            util::File::CreateDirectoy(util::File::GetPath(pathname));

            // 3. 创建并打开日志文件
            _ofs.open(pathname, std::ios::binary | std::ios::app);     // 以写的方式打开文件，将日志信息追加到文件末尾
            assert(_ofs.is_open());     // 文件是否被打开 没有被打开，程序退出
        }

        // 将日志消息写入到标准输出，判断当前时间是否是当前文件的时间段，不是则切换文件
        void Sink(const char* data, size_t len)
        {
            // 获取当前的系统时间
            time_t curtime =  util::Date::GetTime();
            // 判断当前时间是否是当前文件的时间段
            if((curtime % _gap_size) != _cur_gap)
            {
                _ofs.close();       // 关闭原来的文件
                // 根据基础文件名生成实际文件名
                std::string pathname = CreateNewFile();
                _ofs.open(pathname, std::ios::binary | std::ios::app);     // 以写的方式打开文件，将日志信息追加到文件末尾
                assert(_ofs.is_open());     // 文件是否被打开 没有被打开，程序退出
            }

            _ofs.write(data, len);
            assert(_ofs.good());
        }

    private:
        std::string CreateNewFile()
        {
            // 获取系统时间，以时间来构造文件的扩展名
            time_t time = util::Date::GetTime();

            struct tm s_tm;     // 接收转化后的时间结构
            /*
            struct tm {
               int tm_sec;    // Seconds (0-60)
               int tm_min;    // Minutes (0-59)
               int tm_hour;   // Hours (0-23)
               int tm_mday;   // Day of the month (1-31)
               int tm_mon;    // Month (0-11)
               int tm_year;   // Year - 1900
               int tm_wday;   // Day of the week (0-6, Sunday = 0)
               int tm_yday;   // Day in the year (0-365, 1 Jan = 0)
               int tm_isdst;  // Daylight saving time 
            };
            */
            localtime_r(&time, &s_tm);      // 将 time 时间戳转换成时间结构
            std::stringstream filename;

            filename << _basename << "-"
                    << std::setfill('0')  // 设置填充字符为 '0'
                    << std::setw(4) << (s_tm.tm_year + 1900)   // 年份4位
                    << std::setw(2) << (s_tm.tm_mon + 1)       // 月份2位
                    << std::setw(2) << s_tm.tm_mday            // 日期2位
                    << std::setw(2) << s_tm.tm_hour            // 小时2位
                    << std::setw(2) << s_tm.tm_min             // 分钟2位
                    << std::setw(2) << s_tm.tm_sec             // 秒数2位
                    << ".log";

            return filename.str();
        }

    private:
        std::string _basename;
        std::ofstream _ofs;
        size_t _cur_gap;    // 当前是第几个时间段
        size_t _gap_size;   // 时间段的大小 s
    };

    class LogSinkFactory    // 工厂模式
    {
    public:
        // static LogSink::ptr Create(int type);   // 类型通过参数设置，扩展性差
        template <class SinkType, class ...Args>   // 日志落地类的类型 不定参是函数的不定参
        static LogSink::ptr Create(Args &&...args)
        {
            return std::make_shared<SinkType>(std::forward<Args>(args)...);
        }
    };
}

#endif