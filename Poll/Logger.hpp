#pragma once

#include "Mutex.hpp"

#include <iostream>
#include <string>
#include <ctime>
#include <sys/time.h>
#include <filesystem> // C++17的新特性
#include <fstream>    // C++ 的文件操作
#include <memory>     // 使用智能指针需要包含的头文件
#include <unistd.h>
#include <sstream>


namespace LOGMOUDLE
{
    // 日志等级
    enum class LogLevel
    {
        INFO,    // 常规日志
        WARNING, // 告警信息
        ERROR,   // 错误信息
        FATAL,   // 致命错误信息
        DEBUG
    };

    // 将日志等级转换成字符串
    std::string LogLevelToString(LogLevel level)
    {
        switch(level)
        {
            case LogLevel::INFO:    
                return "Info";
            case LogLevel::WARNING:
                return "Warning";
            case LogLevel::ERROR:
                return "Error";
            case LogLevel::FATAL:
                return "Fatal";
            case LogLevel::DEBUG:
                return "Debug";
            default:
                return "Unknown";
        }
    }

    std::string GetCurrentTime()
    {
        struct timeval current_time; // 获取时间戳
        // int gettimeofday(struct timeval *restrict tv, struct timezone *_Nullable restrict tz)
        gettimeofday(&current_time, nullptr);
        // current_time 中就存储着获取到的时间戳, time_t tv_sec 和 suseconds_t tv_usec
        // 调用 localtime_r 函数，将时间戳转换成年月日时分秒
        // struct tm *localtime_r(const time_t *restrict timep, struct tm *restrict result)
        struct tm struct_time;
        localtime_r(&(current_time.tv_sec), &struct_time);
        // struct tm 中的成员：
        //     struct tm {
        //        int         tm_sec;    /* Seconds          [0, 60] */
        //        int         tm_min;    /* Minutes          [0, 59] */
        //        int         tm_hour;   /* Hour             [0, 23] */
        //        int         tm_mday;   /* Day of the month [1, 31] */
        //        int         tm_mon;    /* Month            [0, 11]  (January = 0) */
        //        int         tm_year;   /* Year minus 1900 */
        //        int         tm_wday;   /* Day of the week  [0, 6]   (Sunday = 0) */
        //        int         tm_yday;   /* Day of the year  [0, 365] (Jan/01 = 0) */
        //        int         tm_isdst;  /* Daylight savings flag */

        //        long        tm_gmtoff; /* Seconds East of UTC */
        //        const char *tm_zone;   /* Timezone abbreviation */
        //    };

        // 存储年月日时分秒
        char strtime[128];
        snprintf(strtime, sizeof(strtime), "%04d-%02d-%02d %02d:%02d:%02d:%ld",
                 struct_time.tm_year + 1900, struct_time.tm_mon + 1, struct_time.tm_mday,
                 struct_time.tm_hour, struct_time.tm_min, struct_time.tm_sec, current_time.tv_usec);

        return strtime;
    }

    // 实现策略接口 —— 纯虚类
    class LogStrategy
    {
    public:
        virtual ~LogStrategy() = default;
        // 日志的刷新方法，不实现，由使用它的人实现
        virtual void SycnLog(const std::string &message) = 0;
    };

    // 控制台日志刷新策略 —— 向显示器打印，继承 LogStrategy 类
    class ConsoleStrategy : public LogStrategy
    {
    public:
        void SycnLog(const std::string &message) override // 重写
        {
            LockGuard lockguard(_mutex); // 以线程安全的方式向显示器打印
            std::cerr << message << std::endl;
        }

        ~ConsoleStrategy() {}

    private:
        Mutex _mutex;
    };

    const std::string defaultpath = "./log";       // 当前目录下的 log
    const std::string defaultfilename = "log.txt"; // 默认文件名为 log.txt

    class FileStrategy : public LogStrategy
    {
    public:
        FileStrategy(const std::string &path = defaultpath, const std::string &name = defaultfilename)
            : _logpath(path), _logfilename(name)
        {
            LockGuard lockguard(_mutex);
            // 判断保存日志的路径是否存在
            // 在 C++17 中，存在文件系统，对各种文件操作接口做了封装
            // exists 函数 —— 判断路径是否存在，返回值为 bool 类型
            if (std::filesystem::exists(_logpath)) // 存在，就不做构造路径
            {
                return;
            }
            try
            {
                // 不存在，就创建这个路径
                std::filesystem::create_directories(_logpath);
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                std::cerr << e.what() << std::endl; // 输出异常
            }
        }

        // 保证打印过程线程安全，使用锁保护
        void SycnLog(const std::string &message) override // 重写
        {
            {
                LockGuard lockguard(_mutex);

                // 路径不能为空，且路径的最后一个字符不是 /
                if (!_logpath.empty() && _logpath.back() != '/')
                {
                    _logpath += "/";
                }

                // 写入的目标文件
                std::string targetLog = _logpath + _logfilename; // ./log/log.txt

                // 打开目标文件，使用C++的文件操作 out是一个文件流
                std::ofstream out(targetLog, std::ios::app); // 必须以追加的方式打开目标文件
                if (!out.is_open())                          // 判断是否打开,没有打开
                {
                    std::cerr << "open " << targetLog << " failed" << std::endl;
                    return;
                }

                // 直接写入文件流中
                out << message << "\n";
                out.close(); // 关闭文件流
            }
        }

        ~FileStrategy() {}

    private:
        std::string _logpath;     // 日志被保存在哪个路径下
        std::string _logfilename; // 日志保存在哪个文件中
        Mutex _mutex;
    };

    // 不同的日志等级的日志信息分别写入不同的文件中




    // 日志类：1.日志的生成 2. 根据不同的策略进行刷新
    class Log
    {
    public:
        Log()   // 默认使用显示器策略
        {
            UseConsoleStrategy();
        }

        // 日志所选择的策略 —— 显示器策略
        void UseConsoleStrategy()
        {
            _strategy = std::make_unique<ConsoleStrategy>();    // 初始化智能指针
        }

        // 日志所选择的策略 —— 文件策略
        void UseFileStrategy()
        {
            _strategy = std::make_unique<FileStrategy>();    // 初始化智能指针
        }

        // 测试接口，测试使用的文件策略还是显示器策略
        void DeBug(const std::string &message)
        {
            if(_strategy != nullptr)
            {
                _strategy->SycnLog(message);
            }
        }

        // 构建日志字符串 —— 定义一个内部类，表示一条完整的日志信息
        class LogMessage
        {
        public:
            // 日志等级loglevel，日志出于哪个文件filename，日志出于哪行line 需要外部传参
            LogMessage(LogLevel loglevel, std::string &filename, int line, Log &log)
                : _loglevel(loglevel)
                , _curr_time(GetCurrentTime())
                , _pid(getpid())
                , _filename(filename)
                , _line(line)
                , _log(log)
            {
                // 一条完整的日志由两部分构成：左半部固定的部分 + 右半部不固定的部分
                // 先构建日志的左半部分，使用 stringstream 类，进行流式格式化
                std::stringstream ss;
                // ss << "xxxxxx" 字符串是向 ss 写的
                ss << "[" << _curr_time << "] "
                   << "[" << LogLevelToString(_loglevel) << "] "
                   << "[" << _pid << "] "
                   << "[" << _filename << "] "
                   << "[" << _line << "]"
                   << " - ";
                
                // 将文件流格式化的字符串信息写入到 _loginfo 中
                _loginfo = ss.str(); 
            }

            // 处理日志的右半部分，右半部分的信息是可变的，类型不一致，因此使用模板
            template<class T>   // 重载 << 操作符
            LogMessage &operator << (const T &info)
            {
                std::stringstream ss;
                ss << info;     // 转成字符串
                _loginfo += ss.str();   // 将可变部分加入

                return *this;       // 返回当前的 LogMessage 对象，方便下次继续使用 << 
            }

            // LogMessage 以 RAII 风格的方式进行刷新，即 LogMessage 对象释放时，做一次刷新
            ~LogMessage()
            {
                if(_log._strategy)  // 若策略不为空
                {
                    _log._strategy->SycnLog(_loginfo);      // 刷新日志信息
                }
            }

        private:
            LogLevel _loglevel;         // 日志等级
            std::string _curr_time;     // 当前的时间
            pid_t _pid;                 // 哪个进程
            std::string _filename;      // 日志的出于哪个文件
            int _line;                  // 日志出于哪行
            std::string _loginfo;       // 一条完整的日志信息

            Log &_log;                  // 引用外部 Log 类对象
        };

        // 采用拷贝的方式返回，构造出一个临时的 LogMessage 对象
        LogMessage operator()(LogLevel loglevel, std::string filename, int line)   // 重载 ()
        {
            // 构建一条日志左半部分
            return LogMessage(loglevel, filename, line, *this);
        }

        ~Log() {}

    private:
        // 定义一个 LogStrategy 类的智能指针
        std::unique_ptr<LogStrategy> _strategy;     // 刷新策略      
    };

    Log glog;   // 日志对象，全局使用

// 统一以宏的方式使用日志
#define ENABLE_CONSOLE_LOG_STRATEGY() glog.UseConsoleStrategy();
#define ENABLE_FILE_LOG_STRATEGY() glog.UseFileStrategy();

#define LOG(level) glog(level, __FILE__, __LINE__)
}
