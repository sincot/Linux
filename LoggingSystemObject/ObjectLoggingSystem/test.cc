#include "Util.hpp"
#include "LogLevel.hpp"
#include "Format.hpp"
#include "LogSink.hpp"
#include "Logger.hpp"

/************************ 测试 Util.hpp ************************/
// int main()
// {
//     std::cout << Log::util::Date::GetTime() << std::endl;       // 获取当前时间戳
//     std::string pathname = "./abc/bcd/a.txt";
//     // 调用 File 类中的 GetPath 函数，获取当前文件路径
//     // 调用 File 类中的 CreateDirectory 函数，根据获取到的路径创建目录
//     Log::util::File::CreateDirectory(Log::util::File::GetPath(pathname));

//     return 0;
// }
/************************ 测试 Util.hpp ************************/

/************************ 测试 LogLevel.hpp ************************/
// int main()
// {
//     std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::DEBUG) << std::endl;
//     std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::UNKNOW) << std::endl;
//     std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::INFO) << std::endl;
// }
/************************ 测试 Util.hpp ************************/

/************************ 测试 Format.hpp ************************/
// int main()
// {
//     /*
//         LogLevel::value level, 
//         std::string file, 
//         size_t line,  
//         std::string logger,
//         std::string payload
//     */
//     Log::LogMessage msg(Log::LogLevel::value::INFO, __FILE__, __LINE__, "root", "格式化 Format 测试");
//     Log::Formatter fmt("abc%%de[%d{%H:%M:%S}]%m%n");
//     std::string str = fmt.format(msg);  // 返回字符串
//     std::cout << str << std::endl;

//     return 0;
// }
/************************ 测试 Format.hpp ************************/

/************************ 测试 LogSink.hpp ************************/
// int main()
// {
//     Log::LogMessage msg(Log::LogLevel::value::INFO, __FILE__, __LINE__, "root", "格式化 Format 测试");
//     Log::Formatter fmt;
//     std::string str = fmt.format(msg);  // 返回字符串

//     Log::LogSink::ptr stdout_sink = Log::LogSinkFactory::Create<Log::StdoutSink>();
//     Log::LogSink::ptr file_sink = Log::LogSinkFactory::Create<Log::FileSink>("./logfile/file.log");
//     Log::LogSink::ptr roll_sink = Log::LogSinkFactory::Create<Log::RollBySizeSink>("./logfile/roll.log", 1024*1024);  // 1MB

//     stdout_sink->Sink(str.c_str(), str.length());       // 输出到显示器
//     file_sink->Sink(str.c_str(), str.length());         // 输出到指定文件
//     size_t cursize = 0;
//     while(cursize < 1024 * 1024 * 10)   // 到达 10MB 就切换文件
//     {
//         roll_sink->Sink(str.c_str(), str.length());
//         cursize += str.length();
//     }

//     return 0;
// }
/************************ 测试 LogSink.hpp ************************/

/************************ 测试 RollByTimeSink 类 ************************/
// int main()
// {
//     Log::LogMessage msg(Log::LogLevel::value::INFO, __FILE__, __LINE__, "root", "格式化 Format 测试");
//     Log::Formatter fmt;
//     std::string str = fmt.format(msg);  // 返回字符串

//     Log::LogSink::ptr time_sink = Log::LogSinkFactory::Create<Log::RollByTimeSink>("./logfile/time", Log::TimeGap::GAP_SECOND);
//     time_t old = Log::util::Date::GetTime();    // 获取当前系统时间
//     while(Log::util::Date::GetTime() < old + 5)     // 生成 5 个文件
//     {
//         time_sink->Sink(str.c_str(), str.length());
//         usleep(1000);
//     }

//     return 0;
// }
/************************ 测试 RollByTimeSink 类 ************************/

/************************ 测试同步日志器 ************************/
// int main()
// {
//     /*
//         SyncLogger(const std::string &logger_name,
//                 LogLevel::value level, 
//                 const Formatter::ptr &formatter, 
//                 const std::vector<LogSink::ptr> &sinks)
//     */
//     std::string logger_name = "sync_logger";
//     Log::LogLevel::value limit_level = Log::LogLevel::value::ERROR;
//     Log::Formatter::ptr formatter = std::make_shared<Log::Formatter>();       // 使用默认的格式化格式

//     Log::LogSink::ptr stdout_sink = Log::LogSinkFactory::Create<Log::StdoutSink>();
//     Log::LogSink::ptr file_sink = Log::LogSinkFactory::Create<Log::FileSink>("./logfile/file.log");
//     Log::LogSink::ptr roll_sink = Log::LogSinkFactory::Create<Log::RollBySizeSink>("./logfile/roll.log", 1024*1024);  // 1MB
//     std::vector<Log::LogSink::ptr> sinks = { stdout_sink, file_sink, roll_sink };

//     Log::Logger::ptr logger(new Log::SyncLogger(logger_name, limit_level, formatter, sinks));   // 实例化 Logger 对象

//     Log::LogMessage msg(Log::LogLevel::value::INFO, __FILE__, __LINE__, "root", "测试同步日志 SyncLogger");
//     Log::Formatter fmt;
//     std::string str = fmt.format(msg);  // 返回字符串

//     size_t cursize = 0;
//     while(cursize < 1024 * 1024 * 10)
//     {
//         logger->Fatal(__FILE__, __LINE__, "%s", str.c_str());
//         cursize += str.length();
//     }

//     return 0;
// }
/************************ 测试同步日志器 ************************/

/************************ 测试局部日志器建造者类 ************************/
int main()
{
    /*
        SyncLogger(const std::string &logger_name,
                LogLevel::value level, 
                const Formatter::ptr &formatter, 
                const std::vector<LogSink::ptr> &sinks)
    */
    std::unique_ptr<Log::LoggerBuilder> builder(new Log::LocalLoggerBuilder());
    builder->BuildLoggerType(Log::LoggerType::LOGGER_SYNC);     // 同步日志器
    builder->BuildLoggerName("sync_logger");                    // 日志器名称
    /* Formatter 日志格式化形式使用默认的 */
    builder->BuildLoggerLevel(Log::LogLevel::value::ERROR);     // 日志限制输出等级
    builder->BuildLoggerSink<Log::FileSink>("./logfile/file.txt");      // 落地器为文件
    builder->BuildLoggerSink<Log::RollBySizeSink>("./logfile/roll", 1024 * 1024);   // 落地器为滚动文件

    Log::Logger::ptr logger = builder->Build();

    Log::LogMessage msg(Log::LogLevel::value::INFO, __FILE__, __LINE__, "root", "测试局部建造者类 LocalLoggerBuilder");
    Log::Formatter fmt;
    std::string str = fmt.format(msg);  // 返回字符串

    size_t cursize = 0;
    while(cursize < 1024 * 1024 * 10)
    {
        logger->Fatal(__FILE__, __LINE__, "%s", str.c_str());
        cursize += str.length();
    }

    return 0;
}
/************************ 测试局部日志器建造者类 ************************/

