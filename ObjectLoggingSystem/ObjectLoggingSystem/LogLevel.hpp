/*
实现两种功能：
    1. 定义枚举列，枚举出日志等级
    2. 提供转换接口，将枚举转换成对应字符串 
*/

#ifndef MY_LOGLEVEL_H   // 避免头文件重复包含
#define MY_LOGLEVEL_H
namespace Log
{
    class LogLevel
    {
    public:
        enum class value    // 定义枚举类型
        {
            UNKNOW = 0,     // 从 0 开始
            DEBUG,          // 调试信息
            INFO,           // 用户信息
            WARN,           // 警告信息
            ERROR,          // 错误信息
            FATAL,          // 致命错误信息
            OFF             // 关闭所有日志输出
        };

        // 使用 static 修饰 可以直接使用类名来访问 LevelToString 函数
        static const char* LevelToString(LogLevel::value level)    // 将日志等级转换成字符串
        {
            switch(level)
            {
            case LogLevel::value::DEBUG:     return "DEBUG";
            case LogLevel::value::INFO:      return "INFO";
            case LogLevel::value::WARN:      return "WARN";
            case LogLevel::value::ERROR:     return "ERROR";
            case LogLevel::value::FATAL:     return "FATAL";
            }

            return "UNKOWN";    // 啥都不是就返回 UNKOWN
        }
    };
}
#endif
