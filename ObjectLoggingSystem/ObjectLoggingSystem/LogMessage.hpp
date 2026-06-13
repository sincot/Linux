/*
定义日志消息类，进行日志中间信息的存储
    1. 日志的输出时间（用于过滤日志输出时间）
    2. 日志等级（用于进行日志过滤分析）
    3. 源文件名称
    4. 源代码行号（源文件名称和源代码行号 用于定位出现错误的代码位置）
    5. 线程ID（用于过量出错的线程）
    6. 日志主体消息
    7. 日志器名称（当前支持多日志器的同时使用）
*/

#ifndef MY_LOGMESSAGE_H
#define MY_LOGMESSAGE_H

#include "Util.hpp"
#include "LogLevel.hpp"

#include <ctime>
#include <thread>
#include <iostream>
#include <string>

namespace Log
{
    class LogMessage
    {
    public:
        LogMessage(LogLevel::value level, 
               std::string file, 
               size_t line,  
               std::string logger,
               std::string payload):
        _ctime(util::Date::GetTime()),      // 获取系统时间戳
        _level(level),
        _file(file),
        _line(line),
        _tid(std::this_thread::get_id()),   // 获取线程 ID
        _logger(logger),
        _payload(payload){}

    public:
        time_t _ctime;                  // 日志产生的时时间戳
        LogLevel::value _level;         // 日志等级
        std::string _file;              // 源文件名称
        size_t _line;                   // 源代码行号
        std::thread::id _tid;           // 线程 ID
        std::string _logger;            // 日志器名称
        std::string _payload;           // 日志消息中的有效载荷
    };
}
#endif