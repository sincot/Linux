/* 使用示例 */

#include "../ObjectLoggingSystem/LoggingSystem.hpp"

#include <unistd.h>

void test_log(const std::string &name)
{
    INFO("%s", "测试开始");
    Log::Logger::ptr logger = Log::LoggerManager::GetInstance().GetLogger(name);
    logger->Debug("%s", "测试日志");
    logger->Info("%s", "测试日志");
    logger->Warn("%s", "测试日志");
    logger->Error("%s", "测试日志");
    logger->Fatal("%s", "测试日志");
    INFO("%s", "测试完毕");
}

void test_Size()
{
    /*   滚动文件以文件大小进行滚动   */
    std::unique_ptr<Log::LoggerBuilder> builder(new Log::GlobalLoggerBuilder());    // 全局日志器
    // builder->BuildLoggerName("sync_logger");                         // 同步
    builder->BuildLoggerName("async_logger");                           // 异步
    builder->BuildLoggerLevel(Log::LogLevel::value::WARN);
    builder->BuildFormatter("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n");
    builder->BuildLoggerType(Log::LoggerType::LOGGER_ASYNC);         // 异步日志器
    // builder->BuildLoggerType(Log::LoggerType::LOGGER_SYNC);             // 同步日志器
    // builder->BuildLoggerSink<Log::FileSink>("./logfile/sync.log");      // 同步
    builder->BuildLoggerSink<Log::FileSink>("./logfile/async.log");     // 异步
    builder->BuildLoggerSink<Log::StdoutSink>();
    builder->BuildLoggerSink<Log::RollBySizeSink>("./logfile/roll-sync-by-size", 1024 * 1024);     // 一个文件 1MB
    builder->Build();

    // test_log("sync_logger");                                         // 同步
    test_log("async_logger");                                           // 异步
    /*   滚动文件以文件大小进行滚动   */
}

void test_Time()
{
    /*   滚动文件按照时间进行滚动   */
    std::unique_ptr<Log::LoggerBuilder> builder(new Log::GlobalLoggerBuilder());    // 全局日志器
    builder->BuildLoggerName("sync_logger");                        // 同步
    // builder->BuildLoggerName("async_logger");                    // 异步
    builder->BuildLoggerLevel(Log::LogLevel::value::WARN);
    builder->BuildFormatter("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n");
    // builder->BuildLoggerType(Log::LoggerType::LOGGER_ASYNC);     // 异步日志器
    builder->BuildLoggerType(Log::LoggerType::LOGGER_SYNC);         // 同步日志器
    // builder->BuildLoggerSink<Log::FileSink>("./logfile/async.log");  // 异步
    builder->BuildLoggerSink<Log::FileSink>("./logfile/sync.log");  // 同步
    builder->BuildLoggerSink<Log::StdoutSink>();
    builder->BuildLoggerSink<Log::RollByTimeSink>("./logfile/roll-sync-by-time", Log::TimeGap::GAP_SECOND);     // 一个文件 1MB
    Log::Logger::ptr logger = builder->Build();

    time_t old = Log::util::Date::GetTime();
    while(Log::util::Date::GetTime() < old + 5)    // 写 5 秒的数据
    {
        logger->Fatal("这是一条测试日志（按照时间进行滚动）");
    }

    test_log("sync_logger");                                        // 同步
    // test_log("async_logger");                                    // 异步
    /*   滚动文件按照时间进行滚动   */
}

int main()
{
    test_Time();
    // test_Size();

    return 0;
}
