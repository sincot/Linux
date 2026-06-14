/* 供外界使用的接口头文件 */

#ifndef MY_LOGGINGSYSTEM_H
#define MY_LOGGINGSYSTEM_H

#include "Logger.hpp"

namespace Log
{
    /* 1. 提供获取指定日志器的全局接口（避免用户自己操作单例对象） */
    Logger::ptr GetLogger(const std::string &name)          // 获取日志器
    {
        return Log::LoggerManager::GetInstance().GetLogger(name);
    }
    Logger::ptr GetRootLogger()      // 获取默认日志器
    {
        return Log::LoggerManager::GetInstance().GetRootLogger();   
    }

    /* 2. 使用宏函数对日志器的接口进行代理（代理模式） */
    #define Debug(fmt, ...) Debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define Info(fmt, ...) Info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define Warn(fmt, ...) Warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define Error(fmt, ...) Error(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define Fatal(fmt, ...) Fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

    /* 3. 提供宏函数，直接通过默认日志器进行日志的标准输出打印（不用获取日志器） */ // LoggerManager::
    #define DEBUG(fmt, ...) Log::GetRootLogger()->Debug(fmt, ##__VA_ARGS__)
    #define INFO(fmt, ...) Log::GetRootLogger()->Info(fmt, ##__VA_ARGS__)
    #define WARN(fmt, ...) Log::GetRootLogger()->Warn(fmt, ##__VA_ARGS__)
    #define ERROR(fmt, ...) Log::GetRootLogger()->Error(fmt, ##__VA_ARGS__)
    #define FATAL(fmt, ...) Log::GetRootLogger()->Fatal(fmt, ##__VA_ARGS__)
}

#endif