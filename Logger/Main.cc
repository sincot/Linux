#include "Logger.hpp"

#include <unistd.h>

using namespace LOGMOUDLE;

int main()
{
    ENABLE_FILE_LOG_STRATEGY();      // 向文件打印
    
    LOG(LogLevel::DEBUG) << "hello world" << " 3_20";
    LOG(LogLevel::ERROR) << "hello world" << " 3_20";
    LOG(LogLevel::FATAL) << "hello world" << " 3_20";
    LOG(LogLevel::INFO) << "hello world" << " 3_20";
    LOG(LogLevel::WARNING) << "hello world" << " 3_20";

    // ENABLE_CONSOLE_LOG_STRATEGY();      // 向显示器打印
    
    // LOG(LogLevel::DEBUG) << "hello world" << " 3_20";
    // LOG(LogLevel::ERROR) << "hello world" << " 3_20";
    // LOG(LogLevel::FATAL) << "hello world" << " 3_20";
    // LOG(LogLevel::INFO) << "hello world" << " 3_20";
    // LOG(LogLevel::WARNING) << "hello world" << " 3_20";


    // Log 类中重载了 ()
    // __FILE__， __LINE__ 获取文件名和当前代码所处的行号
    // glog 是前面定义的全局对象
    // glog(LogLevel::DEBUG, __FILE__, __LINE__) << "hello world" << " 3_20";
    // glog(LogLevel::INFO, __FILE__, __LINE__) << "hello world" << " 3_20";
    // glog(LogLevel::WARNING, __FILE__, __LINE__) << "hello world" << " 3_20";
    // glog(LogLevel::ERROR, __FILE__, __LINE__) << "hello world" << " 3_20";
    // glog(LogLevel::FATAL, __FILE__, __LINE__) << "hello world" << " 3_20";


    // ENABLE_CONSOLE_LOG_STRATEGY();      // 向显示器打印
    // glog.DeBug("console strategy_1\n");
    // glog.DeBug("console strategy_2\n");
    // glog.DeBug("console strategy_3\n");
    // glog.DeBug("console strategy_4\n");
    // glog.DeBug("console strategy_5\n");

    // ENABLE_FILE_LOG_STRATEGY();       // 向文件打印
    // glog.DeBug("file strategy_1\n");
    // glog.DeBug("file strategy_2\n");
    // glog.DeBug("file strategy_3\n");
    // glog.DeBug("file strategy_4\n");
    // glog.DeBug("file strategy_5\n");

    // ENABLE_CONSOLE_LOG_STRATEGY();      // 向显示器打印
    // glog.DeBug("console strategy_6\n");
    // glog.DeBug("console strategy_7\n");
    // glog.DeBug("console strategy_8\n");
    // glog.DeBug("console strategy_9\n");
    // glog.DeBug("console strategy_0\n");


    return 0;
}

