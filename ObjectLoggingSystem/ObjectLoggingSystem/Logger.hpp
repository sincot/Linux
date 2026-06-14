/*
日志器模块
    1. 抽象日志器基类
        管理的成员：
        1. 格式化模块对象
        2. 落地模块对象[数组]（一个日志器可能会向多个位置进行日志输出）
        3. 默认的日志输出限制等级（大于等级限制等级的日志才能输出）
        4. 互斥锁（互斥输出，保证日志输出是线程安全的，不会出现交叉日志）
        5. 日志器名称（日志器的唯一标识，以便于查找）
    2. 派生出不同的子类（同步日志器类 & 异步日志器类）
*/

#ifndef MY_LOGGER_H
#define MY_LOGGER_H

#include "Util.hpp"
#include "LogLevel.hpp"
#include "Format.hpp"
#include "LogSink.hpp"
#include "LogLooper.hpp"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <cstdio>
#include <atomic>
#include <vector>
#include <mutex>
#include <cstdarg>
#include <functional>
#include <memory>
#include <unordered_map>

namespace Log
{
    class Logger
    {
    public:
        using ptr = std::shared_ptr<Logger>;

        Logger(const std::string &logger_name, LogLevel::value level, 
            const Formatter::ptr &formatter, const std::vector<LogSink::ptr> &sinks)
            : _logger_name(logger_name), _limit_level(level), _formatter(formatter), _sinks(sinks.begin(), sinks.end()) 
        {}

        /* 完成构造日志消息对象过程并进行格式化，得到格式化后的日志消息字符串，然后进行输出 */
        /* 通过传入的参数，构造出一个日志消息对象 LogMessage */
        // 1. 判断当前的日志是否达到了输出等级
        // 2. 对 fmt 格式化字符串和不定参数进行字符串组织，得到日志消息的字符串
        // 3. 构造 LogMsg 对象
        // 4. 通过格式化工具对 LogMsg 进行格式化，得到格式化后的字符串
        // 5. 进行日志落地
        void Debug(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            /* 1. 判断当前的日志是否达到了输出等级 */
            if(LogLevel::value::DEBUG < _limit_level) { return; }   

            /* 2. 对 fmt 格式化字符串和不定参数进行字符串组织，得到日志消息的字符串 */
            // 运行到这，说明可以格式化输出 调用 va_start
            va_list ap;
            va_start(ap, fmt);
            char* strp;
            // 专门用来使用不定参参数的函数: int vasprintf(char **strp, const char *fmt, va_list ap);
            int ret = vasprintf(&strp, fmt.c_str(), ap);    // 必须使用 free 释放 strp    成功返回组织后的字符串长度失败返回 -1
            if(ret == -1)   
            {
                std::cerr << "vasprintf failed!";
                return;
            }
            va_end(ap);     // 将 ap 置空

            MessageSerialize(LogLevel::value::DEBUG, file, line, strp);

            free(strp);
        }

        void Info(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            /* 1. 判断当前的日志是否达到了输出等级 */
            if(LogLevel::value::INFO < _limit_level) { return; }   

            /* 2. 对 fmt 格式化字符串和不定参数进行字符串组织，得到日志消息的字符串 */
            // 运行到这，说明可以格式化输出 调用 va_start
            va_list ap;
            va_start(ap, fmt);
            char* strp;
            // 专门用来使用不定参参数的函数: int vasprintf(char **strp, const char *fmt, va_list ap);
            int ret = vasprintf(&strp, fmt.c_str(), ap);    // 必须使用 free 释放 strp    成功返回组织后的字符串长度失败返回 -1
            if(ret == -1)   
            {
                std::cerr << "vasprintf failed!";
                return;
            }
            va_end(ap);     // 将 ap 置空

            MessageSerialize(LogLevel::value::INFO, file, line, strp);

            free(strp);
        }

        void Warn(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            /* 1. 判断当前的日志是否达到了输出等级 */
            if(LogLevel::value::WARN < _limit_level) { return; }   

            /* 2. 对 fmt 格式化字符串和不定参数进行字符串组织，得到日志消息的字符串 */
            // 运行到这，说明可以格式化输出 调用 va_start
            va_list ap;
            va_start(ap, fmt);
            char* strp;
            // 专门用来使用不定参参数的函数: int vasprintf(char **strp, const char *fmt, va_list ap);
            int ret = vasprintf(&strp, fmt.c_str(), ap);    // 必须使用 free 释放 strp    成功返回组织后的字符串长度失败返回 -1
            if(ret == -1)   
            {
                std::cerr << "vasprintf failed!";
                return;
            }
            va_end(ap);     // 将 ap 置空

            MessageSerialize(LogLevel::value::WARN, file, line, strp);

            free(strp);
        }

        void Error(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            /* 1. 判断当前的日志是否达到了输出等级 */
            if(LogLevel::value::ERROR < _limit_level) { return; }   

            /* 2. 对 fmt 格式化字符串和不定参数进行字符串组织，得到日志消息的字符串 */
            // 运行到这，说明可以格式化输出 调用 va_start
            va_list ap;
            va_start(ap, fmt);
            char* strp;
            // 专门用来使用不定参参数的函数: int vasprintf(char **strp, const char *fmt, va_list ap);
            int ret = vasprintf(&strp, fmt.c_str(), ap);    // 必须使用 free 释放 strp    成功返回组织后的字符串长度失败返回 -1
            if(ret == -1)   
            {
                std::cerr << "vasprintf failed!";
                return;
            }
            va_end(ap);     // 将 ap 置空

            MessageSerialize(LogLevel::value::ERROR, file, line, strp);

            free(strp);
        }

        void Fatal(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            /* 1. 判断当前的日志是否达到了输出等级 */
            if(LogLevel::value::FATAL < _limit_level) { return; }   

            /* 2. 对 fmt 格式化字符串和不定参数进行字符串组织，得到日志消息的字符串 */
            // 运行到这，说明可以格式化输出 调用 va_start
            va_list ap;
            va_start(ap, fmt);
            char* strp;
            // 专门用来使用不定参参数的函数: int vasprintf(char **strp, const char *fmt, va_list ap);
            int ret = vasprintf(&strp, fmt.c_str(), ap);    // 必须使用 free 释放 strp    成功返回组织后的字符串长度失败返回 -1
            if(ret == -1)   
            {
                std::cerr << "vasprintf failed!";
                return;
            }
            va_end(ap);     // 将 ap 置空

            MessageSerialize(LogLevel::value::FATAL, file, line, strp);

            free(strp);
        }

        const std::string& GetLoggerName()    { return _logger_name; }        // 获取日志器的名称（防止外界修改）

    protected:
        virtual void SinkOutput(const char *data, size_t len) = 0;   /* 完成实际的落地输出--不同的日志器会有不同的实际落地方式 */
        
        void MessageSerialize(LogLevel::value level, const std::string &file, size_t line, char* strp)     // 对日志消息进行序列化
        {
            // 3. 构造 LogMsg 对象
            // LogMessage(Log::LogLevel::value level, std::string file, size_t line, std::string logger, std::string payload)
            LogMessage msg(level, file, line, _logger_name, strp);

            // 4. 通过格式化工具对 LogMsg 进行格式化，得到格式化后的字符串
            std::stringstream ss;
            // void format(std::ostream &out, const Log::LogMessage &msg)
            _formatter->format(ss, msg);

            // 5. 进行日志落地
            SinkOutput(ss.str().c_str(), ss.str().length());
        }
    // private:
    protected:      // 让子类也能访问
        std::string _logger_name;       // 日志器名称
        std::atomic<LogLevel::value> _limit_level;   // 默认的日志输出限制等级   它会被频繁的访问，将它设置成原子的
        Formatter::ptr _formatter;          // 格式化模块对象
        std::vector<LogSink::ptr> _sinks;   // 落地模块对象数组
        std::mutex _mutex;                  // 互斥锁 
    };

    /* 同步日志器，是将日志直接通过落地模块句柄进行日志落地 */
    class SyncLogger : public Logger        // 同步日志器
    {
    public:
        SyncLogger(const std::string &logger_name, LogLevel::value level, 
            const Formatter::ptr &formatter, const std::vector<LogSink::ptr> &sinks)
            : Logger(logger_name, level, formatter, sinks)
        {}

    protected:
        void SinkOutput(const char *data, size_t len)    /* 完成实际的落地输出 */
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if(_sinks.empty())  { return; }     // 没有落地模块对象
            for(auto& sink : _sinks)    { sink->Sink(data, len); }
        }
    };

    /* 异步日志器 */
    class AsyncLogger : public Logger
    {
    public:
        AsyncLogger(const std::string &logger_name, LogLevel::value level, 
            const Formatter::ptr &formatter, const std::vector<LogSink::ptr> &sinks, AsyncType looper_type)
            : Logger(logger_name, level, formatter, sinks) 
            , _looper(std::make_shared<AsyncLooper>(std::bind(&AsyncLogger::ActualSinkOutput, this, std::placeholders::_1), looper_type))
        {}

        void ActualSinkOutput(LogBuffer &buffer)        // 实际的落地操作
        {
            std::unique_lock<std::mutex> lock(_mutex);  // 使用基类的 _mutex
            if(_sinks.empty())  { return; }     // sinks 为空，没有落地模块对象，直接返回
            for(auto &sink : _sinks)
            {
                sink->Sink(buffer.begin(), buffer.ReadAbleLength());
            }
        }
    protected:
        void SinkOutput(const char *data, size_t len)
        {
            _looper->Push(data, len);
        }
    private:
        AsyncLooper::ptr _looper;
    };

    /* 使用建造者模式建造日志器，而不是让用户直接去构造日志器，简化用户的使用复杂度 */
    // 1. 抽象一个日志器建造者类（完成日志器对象所需零部件的构建 & 日志器的构建）
    //      1. 设置日志器类型
    //      2. 将不同类型日志器的创建放到同一个日志器建造者类中完成    
    // 2. 派生出具体的建造者类 -- 局部日志器的建造者 & 全局的日志器建造者（后面添加了全局单例管理器之后，将日志器添加到全局单例管理器中）
    enum class LoggerType
    {
        LOGGER_SYNC,        // 同步日志器
        LOGGER_ASYNC        // 异步日志器
    };

    class LoggerBuilder
    {
    public:
        // 默认是同步日志器
        LoggerBuilder():_logger_type(LoggerType::LOGGER_SYNC), _limit_level(LogLevel::value::DEBUG), _looper_type(AsyncType::ASYNC_SAFE){}

        void BuildLoggerType(LoggerType type) { _logger_type = type; }   // 通过日志器的类型进行建造
        void BuildLoggerName(const std::string &name) { _logger_name = name; }
        void BuildLoggerLevel(LogLevel::value level) { _limit_level = level; }
        void BuildFormatter(const std::string &pattern) { _formatter = std::make_shared<Formatter>(pattern); }    // 构造一个日志格式化器

        void BuildUnSafeAsync() { _looper_type = AsyncType::ASYNC_UNSAFE; }     // 启动它变成非安全状态

        // 自己提供日志器类型，LoggerBuilder 来创建日志器，便于扩展
        template <class SinkType, class ...Args>
        void BuildLoggerSink(Args &&...args)
        {
            LogSink::ptr psink = LogSinkFactory::Create<SinkType>(std::forward<Args>(args)...);     // 对参数包完美转发
            _sinks.emplace_back(psink);
        }

        virtual Logger::ptr Build() = 0;       // 用于建造日志器

    protected:
        // 日志器的各个零部件
        LoggerType _logger_type;        // 日志器类型
        std::string _logger_name;       // 日志器名称
        LogLevel::value _limit_level;   // 默认的日志输出限制等级   它会被频繁的访问，将它设置成原子的
        Formatter::ptr _formatter;          // 格式化模块对象
        std::vector<LogSink::ptr> _sinks;   // 落地模块对象数组 

        AsyncType _looper_type;           // 异步日志器的工作模式
    };
    
    // 派生出具体的建造者类 —— 局部日志器建造者
    class LocalLoggerBuilder : public LoggerBuilder
    {
    public:
        Logger::ptr Build() override
        {
            assert(_logger_name.empty() == false);       // 必须要有日志器名称 必须不为空
            if(_formatter.get() == nullptr)
            {
                _formatter = std::make_shared<Formatter>();
            }
            if(_sinks.empty())      // _sinks 数组为空
            {
                BuildLoggerSink<StdoutSink>();
            }

            // 部件都有了，开始创造对象 区分不同的日志器
            if(_logger_type == LoggerType::LOGGER_ASYNC)     // 异步日志器
            {
                return std::make_shared<AsyncLogger>(_logger_name, _limit_level, _formatter, _sinks, _looper_type);
            }

            // 同步日志器
            return std::make_shared<SyncLogger>(_logger_name, _limit_level, _formatter, _sinks);
        }
    };

    /* 并不是很需要指挥者，指挥者主要是用于指挥一个对象各个部件如何构造，按照什么顺序进行构造
       这里没有顺序要求，只要构造出来即可 因此直接省略了指挥者这一步，直接使用建造者建造 */


    /* 日志器管理器 
        管理的成员：
            1. 默认日志器
            2. 所管理的日志器数组
            3. 互斥锁
    */
    class LoggerManager
    {
    public:
        static LoggerManager& GetInstance()         // 获取单例句柄（懒汉）
        {
            // 在 C++11 之后，针对静态局部变量，编译器在编译的层面实现了线程安全
            // 当静态局部变量在没有构造完成之前，其它的线程进入就会阻塞
            static LoggerManager eton;
            return eton;
        }

        void AddLogger(Logger::ptr &logger)         // 添加日志器
        {
            // std::unique_lock<std::mutex> lock(_mutex);
            // 添加之前，先检查 logger 日志器是否存在，不存在就添加
            if(HasLogger(logger->GetLoggerName())) { return; }
            // if(_loggers.find(logger->GetLoggerName()) != _loggers.end())
            // { 
            //     return; 
            // }       
            _loggers.insert(std::make_pair(logger->GetLoggerName(), logger));   // 添加日志器名称与日志器的映射关系
        }

        bool HasLogger(const std::string &name)     // 有没有指定的 Logger
        {
            // 查找指定的 Logger，使用 vector 很麻烦，因此日志器使用哈希来管理
            std::unique_lock<std::mutex> lock(_mutex);

            return _loggers.find(name) != _loggers.end();
        }

        Logger::ptr GetLogger(const std::string &name)      // 通过日志器的名称获取一个日志器
        {
            // 先查找指定的 Logger，看看是否存在
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _loggers.find(name);
            if(it == _loggers.end())    { return Logger::ptr(); }

            return it->second;
        }

        Logger::ptr GetRootLogger()        // 获取默认的日志器
        {
            // 可能在其他线程修改时访问，返回 shared_ptr 的拷贝是线程安全的
            std::unique_lock<std::mutex> lock(_mutex);
            return _root_logger;
        }

    private:
        LoggerManager()         // 构造函数
        {
            // 注意 LocalLoggerBuilder 不要改成 GlobalLoggerBuilder
            std::unique_ptr<Log::LoggerBuilder> builder(new Log::LocalLoggerBuilder());
            builder->BuildLoggerName("root");
            _root_logger = builder->Build();
            _loggers.insert(std::make_pair("root", _root_logger));
        }     
    private:
        std::mutex _mutex;
        Logger::ptr _root_logger;   // 默认日志器
        // std::vector<Logger::ptr> _loggers;      // 日志器数组
        std::unordered_map<std::string, Logger::ptr> _loggers;      // 日志器名称与日志器的映射关系
    };

    // 设计一个全局日志器的建造者 -- 在局部日志器建造者的基础上增加了一个功能：将日志器添加到单例对象中
    class GlobalLoggerBuilder : public LoggerBuilder         // 派生出具体建造者类 -- 全局日志器建造者
    {
    public:
        Logger::ptr Build() override
        {
            assert(_logger_name.empty() == false);       // 必须要有日志器名称 必须不为空
            if(_formatter.get() == nullptr)
            {
                _formatter = std::make_shared<Formatter>();
            }
            if(_sinks.empty())      // _sinks 数组为空
            {
                BuildLoggerSink<StdoutSink>();
            }

            Logger::ptr logger;
            // 部件都有了，开始创造对象 区分不同的日志器
            if(_logger_type == LoggerType::LOGGER_ASYNC)     // 异步日志器
            {
                logger = std::make_shared<AsyncLogger>(_logger_name, _limit_level, _formatter, _sinks, _looper_type);
            }
            else
            {
                logger = std::make_shared<SyncLogger>(_logger_name, _limit_level, _formatter, _sinks);
            }

            LoggerManager::GetInstance().AddLogger(logger);     // 将日志器添加到管理器中

            return logger;
        }
    };
}

#endif