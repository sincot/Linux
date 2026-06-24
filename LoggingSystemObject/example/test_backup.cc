// #include "Util.hpp"
// #include "LogLevel.hpp"
// #include "LogMessage.hpp"
// #include "Format.hpp"
// #include "LogSink.hpp"
// #include "Logger.hpp"
// #include "LogBuffer.hpp"

#include "ObjectLoggingSystem/LoggingSystem.hpp"

#include <unistd.h>

void test_log()
{
    // Log::Logger::ptr logger = Log::LoggerManager::GetInstance().GetLogger("async_logger");
    // logger->Debug(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Info(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Warn(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Error(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Fatal(__FILE__, __LINE__, "%s", "测试日志");

    // logger->Debug( "%s", "测试日志");
    // logger->Info("%s", "测试日志");
    // logger->Warn("%s", "测试日志");
    // logger->Error("%s", "测试日志");
    // logger->Fatal("%s", "测试日志");

    // size_t count = 0;
    // while(count < 500000)
    // {
    //     logger->Fatal("测试日志-%d", count++);
    // }

    DEBUG( "%s", "测试日志");
    INFO("%s", "测试日志");
    WARN("%s", "测试日志");
    ERROR("%s", "测试日志");
    FATAL("%s", "测试日志");

    size_t count = 0;
    while(count < 500000)
    {
        FATAL("测试日志-%d", count++);
    }
}

int main()
{
    // std::cout << Log::util::Date::GetTime() << std::endl;       // 获取当前时间戳
    // std::string pathname = "./abc/bcd/a.txt";
    // // 调用 File 类中的 GetPath 函数，获取当前文件路径
    // // 调用 File 类中的 CreateDirectory 函数，根据获取到的路径创建目录
    // Log::util::File::CreateDirectory(Log::util::File::GetPath(pathname));

    // std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::DEBUG) << std::endl;
    // std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::UNKNOW) << std::endl;
    // std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::INFO) << std::endl;
    
    /*
    LogMessage(LogLevel::value level, 
               std::string file, 
               size_t line,  
               std::string logger,
               std::string payload):
    */
    // Log::LogMessage msg(Log::LogLevel::value::INFO, "main.c", 53, "root", "格式化功能测试");
    // Log::Formatter fmt;
    // std::string str = fmt.format(msg);
    // std::cout << str << std::endl;

    // Log::LogMessage msg(Log::LogLevel::value::INFO, "main.c", 53, "root", "格式化功能测试");
    // Log::Formatter fmt("abc%%abc[%d{%H:%M:%S}] %m%n");
    // Log::Formatter fmt("abc%%abc[%d{%H:%M:%S}] %m%n{");
    // Log::Formatter fmt("abc%%abc[%d{%H:%M:%S}] %m%");
    // Log::Formatter fmt("abc%%abc[%d{%H:%M:%S}] %m%n%g");
    // Log::Formatter fmt("abc%%abc[%d{%H:%M:%S}] %m%n");
    // Log::Formatter fmt;
    // std::string str = fmt.format(msg);
    // std::cout << str << std::endl;

    // Log::LogSink::ptr stdout_lsp = Log::LogSinkFactory::Create<Log::StdoutSink>();
    // Log::LogSink::ptr file_lsp = Log::LogSinkFactory::Create<Log::FileSink>("./logfile/file.log");
    // Log::LogSink::ptr roll_lsp = Log::LogSinkFactory::Create<Log::RollBySizeSink>("./logfile/roll", 1024*1024);

    // stdout_lsp->Sink(str.c_str(), str.length());
    // file_lsp->Sink(str.c_str(), str.length());
    // size_t curfsize = 0;
    // size_t count = 0;
    // while(curfsize < 1024 * 1024 * 10)
    // {
    //     std::string tmp = str + std::to_string(count++);    // 检查插入的数据是否是连贯的
    //     roll_lsp->Sink(tmp.c_str(), tmp.length());
    //     curfsize += tmp.length();
    // }
    
    // Log::LogMessage msg(Log::LogLevel::value::INFO, "main.c", 53, "root", "格式化功能测试");
    // Log::Formatter fmt;
    // std::string str = fmt.format(msg);

    // Log::LogSink::ptr time_lsp = Log::LogSinkFactory::Create<Log::RollByTimeSink>("./logfile/roll", Log::TimeGap::GAP_SECOND);
    // time_t old = Log::util::Date::GetTime();
    // while(Log::util::Date::GetTime() < old + 5)    // 写 5 秒的数据
    // {
    //     time_lsp->Sink(str.c_str(), str.length());
    //     usleep(1000);
    // }

    // std::string logger_name = "sync_logger";
    // Log::LogLevel::value limit_level = Log::LogLevel::value::ERROR;
    // Log::Formatter::ptr fmt = std::make_shared<Log::Formatter>("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n");

    // Log::LogSink::ptr stdout_lsp = Log::LogSinkFactory::Create<Log::StdoutSink>();
    // Log::LogSink::ptr file_lsp = Log::LogSinkFactory::Create<Log::FileSink>("./logfile/file.log");
    // Log::LogSink::ptr roll_lsp = Log::LogSinkFactory::Create<Log::RollBySizeSink>("./logfile/roll", 1024*1024);
    // std::vector<Log::LogSink::ptr> sinks = { stdout_lsp, file_lsp, roll_lsp };

    // Log::Logger::ptr logger(new Log::SyncLogger(logger_name, limit_level, fmt, sinks));

    // // 由于默认日志输出等级是 ERROR，所以最终只有日志等级大于等于 ERROR 的日志信息被输出
    // logger->Debug(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Info(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Warn(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Error(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Fatal(__FILE__, __LINE__, "%s", "测试日志");
    // size_t curfsize = 0, count = 0;
    // std::string str = "测试日志";
    // while(curfsize < 1024 * 1024 * 10)
    // {
    //     // std::string tmp = str + std::to_string(count++);
    //     // logger->Fatal(__FILE__, __LINE__, "测试日志-%s", tmp.c_str());
    //     // curfsize += tmp.length();

    //     logger->Fatal(__FILE__, __LINE__, "测试日志-%d", count++);
    //     curfsize += 20;
    // }

    // std::unique_ptr<Log::LoggerBuilder> builder(new Log::LocalLoggerBuilder());
    // builder->BuildLoggerName("sync_logger");
    // builder->BuildLoggerLevel(Log::LogLevel::value::ERROR);
    // builder->BuildFormatter("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n");
    // builder->BulidLoggerType(Log::LoggerType::LOGGER_SYNC);     // 同步日志器
    // builder->BuildLoggerSink<Log::FileSink>("./logfile/test.log");
    // builder->BuildLoggerSink<Log::RollBySizeSink>("./logfile/roll", 1024 * 1024);
    // Log::Logger::ptr logger = builder->Build();

    // // 由于默认日志输出等级是 ERROR，所以最终只有日志等级大于等于 ERROR 的日志信息被输出
    // logger->Debug(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Info(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Warn(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Error(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Fatal(__FILE__, __LINE__, "%s", "测试日志");
    // size_t curfsize = 0, count = 0;
    // std::string str = "测试日志";
    // while(curfsize < 1024 * 1024 * 10)
    // {
    //     // std::string tmp = str + std::to_string(count++);
    //     // logger->Fatal(__FILE__, __LINE__, "测试日志-%s", tmp.c_str());
    //     // curfsize += tmp.length();

    //     logger->Fatal(__FILE__, __LINE__, "测试日志-%d", count++);
    //     curfsize += 20;
    // }


    // 读取文件数据，一点一点的写入缓冲区，最终将缓冲区数据写入文件，判断生成的新文件与源文件是否一致
    // std::ifstream ifs("./logfile/test.log", std::ios::binary);      // 以二进制的方式读取文件
    // if(ifs.is_open() == false)  { return -1; }      // 没有被打开

    // ifs.seekg(0, std::ios::end);    // 读写位置跳转到文件末尾
    // size_t fsize = ifs.tellg();     // 获取当前读写位置相对于起始位置的偏移量
    // ifs.seekg(0, std::ios::beg);    // 重新跳转到文件的起始位置

    // std::string body;
    // body.resize(fsize);
    // ifs.read(&body[0], fsize);  // 不能使用 body.c_str() 因为它返回的 const char* 它是没有办法被修改的
    // if(ifs.good() == false)     { std::cerr << "read error"; return -1; }
    // ifs.close();

    // Log::LogBuffer buffer;
    // for(int i = 0; i < body.length(); i++)
    // {
    //     buffer.Push(&body[0], 1);
    // }
    // std::ofstream ofs("./logfile/tmp.log", std::ios::binary);
    // size_t rsize = buffer.ReadAbleLength();     // 可读的长度
    // for(int i = 0; i < rsize; i++)    // 小于可读文件的大小
    // {
    //     ofs.write(buffer.begin(), 1);
    //     buffer.MoveReader(1);   // 读位置向后偏移 1 个字符
    // }
    // ofs.close(); 

    // std::unique_ptr<Log::LoggerBuilder> builder(new Log::LocalLoggerBuilder());
    // builder->BuildLoggerName("async_logger");
    // builder->BuildLoggerLevel(Log::LogLevel::value::ERROR);
    // builder->BuildFormatter("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n");
    // builder->BuildLoggerType(Log::LoggerType::LOGGER_ASYNC);     // 异步日志器
    // builder->BuildUnSafeAsync();                                 // 启动非安全模式
    // builder->BuildLoggerSink<Log::FileSink>("./logfile/async.log");
    // builder->BuildLoggerSink<Log::StdoutSink>();
    // Log::Logger::ptr logger = builder->Build();

    // // 由于默认日志输出等级是 ERROR，所以最终只有日志等级大于等于 ERROR 的日志信息被输出
    // logger->Debug(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Info(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Warn(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Error(__FILE__, __LINE__, "%s", "测试日志");
    // logger->Fatal(__FILE__, __LINE__, "%s", "测试日志");
    // size_t curfsize = 0, count = 0;
    // // while(curfsize < 1024 * 1024 * 10)
    // // {
    // //     logger->Fatal(__FILE__, __LINE__, "测试日志-%d", count++);
    // //     curfsize += 20;
    // // }
    // while(count < 500000)
    // {
    //     logger->Fatal(__FILE__, __LINE__, "测试日志-%d", count++);
    // }

    std::unique_ptr<Log::LoggerBuilder> builder(new Log::GlobalLoggerBuilder());    // 全局日志器
    builder->BuildLoggerName("async_logger");
    builder->BuildLoggerLevel(Log::LogLevel::value::ERROR);
    builder->BuildFormatter("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n");
    builder->BuildLoggerType(Log::LoggerType::LOGGER_ASYNC);     // 异步日志器
    builder->BuildUnSafeAsync();                                 // 启动非安全模式
    builder->BuildLoggerSink<Log::FileSink>("./logfile/async.log");
    builder->BuildLoggerSink<Log::StdoutSink>();
    builder->Build();

    test_log();

    return 0;
}
