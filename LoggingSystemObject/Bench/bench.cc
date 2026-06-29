#include "../ObjectLoggingSystem/LoggingSystem.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>   // 计时

// logger_name: 日志器名称  thread_count: 线程数量  msg_count: 总日志数量  msg_len: 消息长度
void Bench(const std::string &logger_name, size_t thread_count, size_t msg_count, size_t msg_len)
{
    // 1. 获取日志器
    Log::Logger::ptr logger = Log::GetLogger(logger_name);
    if(logger.get() == nullptr)     // 没有日志器
    {
        return;
    }    

    std::cout << "测试日志: " << msg_count << " 条日志数量, 总大小: " << (msg_count * msg_len) / 1024 << "KB" << std::endl;
    // 2. 组织指定长度的日志消息
    std::string msg(msg_len - 1, 'x');      // 少一字节是为了给末尾添加换行符  

    // 3. 创建指定数量的线程
    std::vector<std::thread> threads;
    std::vector<double> cost_array(thread_count);
    size_t thread_log_count = msg_count / thread_count;    // 总日志数量 / 线程数量 = 每个线程要输出的日志数量
    for(int i = 0; i < thread_count; i++)       // 创建 thread_count 个线程
    {
        threads.emplace_back([&, i](){
            // 4. 线程函数内部开始计时
            auto start = std::chrono::high_resolution_clock::now();     // 起始时间
            // 5. 开始循环写日志
            for(int j = 0; j < thread_log_count; j++)
            {
                logger->Fatal("%s", msg.c_str());    // 以最高等级输出
            }
            // 6. 线程函数内部结束计时
            auto end = std::chrono::high_resolution_clock::now();       // 结束时间
            std::chrono::duration<double> cost = end - start;
            cost_array[i] = cost.count();
            std::cout << "线程" << i << "\t输出日志数量: " << thread_log_count << "\t耗时: " << cost.count() << "s" << std::endl; 
        });
    }

    for(int i = 0; i < thread_count; i++)   { threads[i].join(); }

    // 7. 计算总耗时 在多线程中，每个线程都会耗费时间，但是线程是并发处理的，因此耗时最高的就是总时间
    double max_cost = cost_array[0];
    for(int i = 0; i < thread_count; i++)  { max_cost = (max_cost < cost_array[i] ? cost_array[i] : max_cost); }
    size_t msg_per_sec = msg_count / max_cost;        // 线程每秒钟所输出的数量
    size_t size_per_sec = (msg_count * msg_len) / (max_cost * 1024);

    // 8. 进行输出打印
    std::cout << "总耗时: " << max_cost << "s" << std::endl;
    std::cout << "每秒输出的日志数量: " << msg_per_sec << "条" << std::endl;
    std::cout << "每秒输出的日志大小: " << size_per_sec << "KB" << std::endl; 
}

void sync_bench()       // 同步
{
    std::unique_ptr<Log::LoggerBuilder> builder(new Log::GlobalLoggerBuilder());    // 全局日志器
    builder->BuildLoggerName("sync_logger");                        // 同步
    builder->BuildFormatter("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n");
    builder->BuildLoggerType(Log::LoggerType::LOGGER_SYNC);         // 同步日志器
    builder->BuildLoggerSink<Log::FileSink>("./logfile/sync.log");  // 同步
    builder->Build();

    // logger_name: sync_logger, thread_count: 1, msg_count: 1000000, msg_len: 100
    // Bench("sync_logger", 1, 1000000, 100);
    Bench("async_logger", 3, 1000000, 100);
}

void async_bench()      // 异步
{
    std::unique_ptr<Log::LoggerBuilder> builder(new Log::GlobalLoggerBuilder());    // 全局日志器
    builder->BuildLoggerName("async_logger");                           // 异步
    builder->BuildLoggerLevel(Log::LogLevel::value::WARN);
    builder->BuildFormatter("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n");
    builder->BuildLoggerType(Log::LoggerType::LOGGER_ASYNC);         // 异步日志器
    builder->BuildUnSafeAsync();    // 异步，开启非安全模式 -- 主要是为了将实际落地时间排除在外
    builder->BuildLoggerSink<Log::FileSink>("./logfile/async.log");     // 异步
    builder->Build();

    // logger_name: async_logger, thread_count: 1, msg_count: 1000000, msg_len: 100
    // Bench("async_logger", 1, 1000000, 100);
    Bench("async_logger", 3, 1000000, 100);
    // Bench("async_logger", 3, 2000000, 100);
}

int main()
{
    std::cout << "/*************** 同步 ***************/" << std::endl;
    sync_bench();
    // std::cout << "/*************** 异步 ***************/" << std::endl;
    // async_bench();

    return 0;
}